#include "Render.h"

extern mat4 viewMatrix;
extern mat4 projectMatrix;
extern mat4 viewPortMatrix;
extern mat4 lightViewMatrix; 
extern mat4 lightOrthoMatrix;
extern Material* curMaterial;

void Render::clearBuffer()
{
	frontBuffer->clearColorBuffer();
	frontBuffer->clearDepthBuffer();
	shadowBuffer->clearDepthBuffer();
}

#pragma region stateMachine
void Render::changeRenderMode() {
	if (renderMod == Fill)
		renderMod = Line;
	else
		renderMod = Fill;
}

void Render::updateViewPlanes() {
	viewFrustumPlanes(viewPlanes, projectMatrix * viewMatrix);
}
#pragma endregion stateMachine

#pragma region pipeLine


//透视除法以及透视插值
void Render::perspectiveDivision(VerToFrag& v2f) {
	v2f.Z = 1.0 / v2f.fragPos.w;

	v2f.fragPos /= v2f.fragPos.w;
	v2f.lightSpacePos /= v2f.lightSpacePos.w;
	v2f.fragPos.w = 1.0f;
	v2f.lightSpacePos.w = 1.0f;
	// Z-Buffer∈[0,1]
	v2f.fragPos.z = (v2f.fragPos.z + 1.0) * 0.5;
	v2f.lightSpacePos.z = (v2f.lightSpacePos.z + 1.0) * 0.5;

	v2f.worldPos *= v2f.Z;
	v2f.texture *= v2f.Z;
	v2f.color *= v2f.Z;
}

/*
 * 绘制网格
 * 根据网格内的索引缓冲将三角形输入到顶点着色器
 * 再对顶点着色器输出的中间体进行透视除法以及剔除
 * 进行视口变换后绘制三角形
 */
void Render::drawObject(Object& obj)
{
	if (obj.mesh.EBO.empty())
		return;

	curMaterial = &obj.material;
	for (auto i = 0; i < obj.mesh.EBO.size(); i += 3)
	{
		Vertex v1, v2, v3;
		v1 = obj.mesh.VBO[obj.mesh.EBO[i]];
		v2 = obj.mesh.VBO[obj.mesh.EBO[i + 1]];
		v3 = obj.mesh.VBO[obj.mesh.EBO[i + 2]];

		VerToFrag f1, f2, f3;
		f1 = curMaterial->shader->vertexShader(v1);
		f2 = curMaterial->shader->vertexShader(v2);
		f3 = curMaterial->shader->vertexShader(v3);

		//视锥剔除
		if (!frustumCull(viewPlanes, f1.worldPos, f2.worldPos, f3.worldPos)) {
			continue;
		}

		//裁剪空间剔除
		if (!clipSpaceCull(f1.fragPos, f2.fragPos, f3.fragPos)) {
			continue;
		}

		vector<VerToFrag> clipVertexs = viewPortCull(f1, f2, f3);

		for (auto j = 0; j < clipVertexs.size(); j++)
		{
			perspectiveDivision(clipVertexs[j]);
		}

		int triCount = clipVertexs.size() - 3 + 1;
		for (auto j = 0; j < triCount; j++)
		{
			VerToFrag vNew1 = clipVertexs[0];
			VerToFrag vNew2 = clipVertexs[j + 1];
			VerToFrag vNew3 = clipVertexs[j + 2];

			vNew1.fragPos = viewPortMatrix * vNew1.fragPos;
			vNew2.fragPos = viewPortMatrix * vNew2.fragPos;
			vNew3.fragPos = viewPortMatrix * vNew3.fragPos;

			vNew1.lightSpacePos = viewPortMatrix * vNew1.lightSpacePos;
			vNew2.lightSpacePos = viewPortMatrix * vNew2.lightSpacePos;
			vNew3.lightSpacePos = viewPortMatrix * vNew3.lightSpacePos;

			if (!faceCull(vNew1.fragPos, vNew2.fragPos, vNew3.fragPos))
			{
				continue;
			}
			if (renderMod == Fill)
			{
				drawTriangle(vNew1, vNew2, vNew3);
			}
			else
			{
				drawLine(vNew1, vNew2);
				drawLine(vNew2, vNew3);
				drawLine(vNew3, vNew1);
			}
			

		}



	}
}

void Render::drawModel(Model& model) {
	for (int i = 0; i < model.objects.size(); i++) {
		drawShadowBuffer(model.objects[i]);
		drawObject(model.objects[i]);
	}
}

#pragma endregion pipeLine

#pragma region Rasterization

bool insideTriangle(const vec2& p1, const vec2& p2, const vec2& p3, const vec2& p)
{


	float r1 = (p.x - p1.x) * (p2.y - p1.y) - (p.y - p1.y) * (p2.x - p1.x);
	float r2 = (p.x - p2.x) * (p3.y - p2.y) - (p.y - p2.y) * (p3.x - p2.x);
	float r3 = (p.x - p3.x) * (p1.y - p3.y) - (p.y - p3.y) * (p1.x - p3.x);

	if (r1 >= 0 && r2 >= 0 && r3 >= 0)
		return true;
	return false;
}

void Render::MSAA(const VerToFrag& f1, const VerToFrag& f2, const VerToFrag& f3, const int& x, const int& y)
{
	//MSAA
	vector<vec2> samplerStep
	{
		vec2(0.25,0.25),
		vec2(0.75,0.25),
		vec2(0.25,0.75),
		vec2(0.75,0.75)
	};

	vec2 p1 = vec2(f1.fragPos);
	vec2 p2 = vec2(f2.fragPos);
	vec2 p3 = vec2(f3.fragPos);

	bool depthTest = false;

	for (auto k = 0; k < 4; k++)
	{
		vec2 p = vec2(x + samplerStep[k].x, y + samplerStep[k].y);
		if (insideTriangle(p1, p2, p3, p))
		{
			VerToFrag f = barycenterLerp(f1, f2, f3, p);
			float curDepth = frontBuffer->getDepth(x, y);

			int index = width * 2 * (y * 2 + k / 2) + (x * 2 + k % 2);
			float superDepth = frontBuffer->superDepthBuffer[index];

			if (superDepth > f.fragPos.z)
			{
				depthTest = true;
				frontBuffer->superDepthBuffer[index] = f.fragPos.z;

				f.texture /= f.Z;
				vec4 color = curMaterial->mainTexture->sampler2D(f.texture);

				float* pos = frontBuffer->superColorBuffer;
				*(pos + index * 4) = color.x;
				*(pos + index * 4 + 1) = color.y;
				*(pos + index * 4 + 2) = color.z;
				*(pos + index * 4 + 3) = color.w;

			}
		}
	}

	if (depthTest)
	{
		VerToFrag f = barycenterLerp(f1, f2, f3, vec2(x, y));
		vector<int> index = { 
			width * 2 * (y * 2) + (x * 2),
			width * 2 * (y * 2) + (x * 2 + 1),
			width * 2 * (y * 2 + 1) + (x * 2),
			width * 2 * (y * 2 + 1) + (x * 2 + 1)
		};


		vec4 color(0.0f);
		for (auto k = 0; k < 4; k++)
		{
			float* pos = frontBuffer->superColorBuffer;

			color.x += *(pos + index[k] * 4) / 4.0f;
			color.y += *(pos + index[k] * 4 + 1) / 4.0f;
			color.z += *(pos + index[k] * 4 + 2) / 4.0f;
			color.w += *(pos + index[k] * 4 + 3) / 4.0f;
		}

		float z = f.Z;
		f.worldPos /= z;
		f.texture /= z;
		f.normal /= z;
		f.color /= z;

		frontBuffer->drawPoint(x, y, curMaterial->shader->fragmentShader(f, color));	
		frontBuffer->writeDepth(x, y, f.fragPos.z);
		}
}

void Render::drawTriangle(const VerToFrag& f1, const VerToFrag& f2, const VerToFrag& f3)
{
	vec2 p1 = vec2(f1.fragPos);
	vec2 p2 = vec2(f2.fragPos);
	vec2 p3 = vec2(f3.fragPos);

	//构建包围盒
	int max_x = std::max(p1.x, std::max(p2.x, p3.x));
	int min_x = std::min(p1.x, std::min(p2.x, p3.x));
	int max_y = std::max(p1.y, std::max(p2.y, p3.y));
	int min_y = std::min(p1.y, std::min(p2.y, p3.y));

	

	for (auto i = std::max(0, min_x); i <= std::min(width, max_x); i++)
	{
		for (auto j = std::max(0, min_y); j <= std::min(height, max_y); j++)
		{

			//MSAA(f1, f2, f3, i, j);
			if (insideTriangle(p1, p2, p3, vec2(i + 0.5, j + 0.5)))
			{
				VerToFrag f = barycenterLerp(f1, f2, f3, vec2(i, j));
				float depth = frontBuffer->getDepth(i, j);

				if (depth > f.fragPos.z)
				{
					float z = f.Z;
					f.worldPos /= z;
					f.texture /= z;
					f.normal /= z;
					f.color /= z;

					//计算灯光空间下的坐标
					vec4 shadowPos = lightOrthoMatrix * lightViewMatrix * f.worldPos;
					shadowPos.z = (shadowPos.z + 1.0) * 0.5;
					shadowPos = viewPortMatrix * shadowPos;

					vec4 baseColor;
					if (shadowBuffer->getDepth(shadowPos.x, shadowPos.y) < f.lightSpacePos.z - 0.001)
					{
						baseColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
					}
					else
					{
						baseColor = curMaterial->mainTexture->sampler2D(f.texture);
					}
					
					frontBuffer->drawPoint(i, j, curMaterial->shader->fragmentShader(f, baseColor));
					frontBuffer->writeDepth(i, j, f.fragPos.z);
				}
			}
		}
	}
}

/* 画线函数
 * 使用Bresenham算法进行线段的绘制
 */
void Render::drawLine(const VerToFrag& f1, const VerToFrag& f2)
{


	vec4 p1 = f1.fragPos;
	vec4 p2 = f2.fragPos;

	//计算△x和△y
	int deltaX = abs(p2.x - p1.x);
	int deltaY = abs(p2.y - p1.y);

	//初始化步长
	int stepX = 0;
	int stepY = 0;

	VerToFrag f = f1;
	vec4 p = f.fragPos;

	//判断步进方向
	if (p2.x > p1.x) stepX = 1;
	else stepX = -1;

	if (p2.y > p1.y) stepY = 1;
	else stepY = -1;

	//判断斜率，如果斜率小于1大于0，我们就选x轴为主步进轴
	//如果斜率大于1，我们就选y轴为主步进轴
	int pk = 0;//决策参数
	bool useX = true;
	int sumStep = 0;//需要步进的次数
	//斜率大于1
	if (deltaY > deltaX) {
		useX = false;
		sumStep = deltaY;
		std::swap(deltaX, deltaY);
	}
	else {
		useX = true;
		sumStep = deltaX;
	}
	pk = 2 * deltaY - deltaX;

	for (int i = 0; i <= sumStep; i++) {
		frontBuffer->drawPoint((int)p.x, (int)p.y, f.color);

		//颜色插值
		float scale = 0.0f;

		if (pk > 0) {
			if (useX) {
				p.y += stepY;
			}
			else {
				p.x += stepX;
			}

			//只有pk>0时才有这一项
			pk -= 2 * deltaX;
		}

		//主坐标不管pk是何值都要步进
		if (useX) {
			p.x += stepX;
		}
		else {
			p.y += stepY;
		}

		//无论pk为何值都有这一项
		pk += 2 * deltaY;

		if (useX) {
			if (p2.x == p1.x) {//此时两个点重合
				scale = 0.0f;
			}
			else {
				scale = (p.x - p1.x) / (p2.x - p1.x);
			}
		}
		else {
			if (p2.y == p1.y) {//此时两个点重合
				scale = 0.0f;
			}
			else {
				scale = (p.y - p1.y) / (p2.y - p1.y);
			}
		}
		f.color = lerp(f1, f2, scale).color;
	}

}
#pragma endregion Rasterization

void Render::drawShadowBuffer(Object& obj)
{
	if (obj.mesh.EBO.empty())
		return;

	for (auto i = 0; i < obj.mesh.EBO.size(); i += 3)
	{
		Vertex v1, v2, v3;
		v1 = obj.mesh.VBO[obj.mesh.EBO[i]];
		v2 = obj.mesh.VBO[obj.mesh.EBO[i + 1]];
		v3 = obj.mesh.VBO[obj.mesh.EBO[i + 2]];

		VerToFrag f1, f2, f3;
		f1 = shadowShader->vertexShader(v1);
		f2 = shadowShader->vertexShader(v2);
		f3 = shadowShader->vertexShader(v3);

		//视锥剔除
		/*if (!frustumCull(viewPlanes, f1.worldPos, f2.worldPos, f3.worldPos)) {
			continue;
		}*/

		//裁剪空间剔除
		/*if (!clipSpaceCull(f1.fragPos, f2.fragPos, f3.fragPos)) {
			continue;
		}*/

		vector<VerToFrag> clipVertexs = viewPortCull(f1, f2, f3);

		for (auto j = 0; j < clipVertexs.size(); j++)
		{
			perspectiveDivision(clipVertexs[j]);
		}

		int triCount = clipVertexs.size() - 3 + 1;
		for (auto j = 0; j < triCount; j++)
		{
			VerToFrag vNew1 = clipVertexs[0];
			VerToFrag vNew2 = clipVertexs[j + 1];
			VerToFrag vNew3 = clipVertexs[j + 2];

			vNew1.fragPos = viewPortMatrix * vNew1.fragPos;
			vNew2.fragPos = viewPortMatrix * vNew2.fragPos;
			vNew3.fragPos = viewPortMatrix * vNew3.fragPos;

			if (!faceCull(vNew1.fragPos, vNew2.fragPos, vNew3.fragPos))
			{
				continue;
			}
			//对灯光深度图进行处理
			vec2 p1 = vec2(vNew1.fragPos);
			vec2 p2 = vec2(vNew2.fragPos);
			vec2 p3 = vec2(vNew3.fragPos);

			//构建包围盒
			int max_x = std::max(p1.x, std::max(p2.x, p3.x));
			int min_x = std::min(p1.x, std::min(p2.x, p3.x));
			int max_y = std::max(p1.y, std::max(p2.y, p3.y));
			int min_y = std::min(p1.y, std::min(p2.y, p3.y));



			for (auto i = std::max(0, min_x); i <= std::min(width, max_x); i++)
			{
				for (auto j = std::max(0, min_y); j <= std::min(height, max_y); j++)
				{

					//MSAA(f1, f2, f3, i, j);
					if (insideTriangle(p1, p2, p3, vec2(i + 0.5, j + 0.5)))
					{
						VerToFrag f = barycenterLerp(vNew1, vNew2, vNew3, vec2(i, j));
						float depth = shadowBuffer->getDepth(i, j);

						if (depth > f.fragPos.z)
						{
							float z = f.Z;
							f.worldPos /= z;
							f.texture /= z;
							f.normal /= z;
							f.color /= z;
							
							shadowBuffer->writeDepth(i, j, f.fragPos.z);
							//frontBuffer->drawPoint(i, j, packDepth(f.fragPos.z));
							//frontBuffer->drawPoint(i, j, vec4(f.fragPos.z, f.fragPos.z, f.fragPos.z, 1.0));
						}
					}
				}
			}

		}



	}
}


