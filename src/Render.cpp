#include "Render.h"

extern mat4 viewMatrix;
extern mat4 projectMatrix;
extern mat4 viewPortMatrix;

void Render::Init()
{
	if (shader)
		delete shader;

	shader = new Shader(texture);
}

void Render::clearBuffer()
{
	frontBuffer->clearColorBuffer();
	frontBuffer->clearDepthBuffer();
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


//͸�ӳ����Լ�͸�Ӳ�ֵ
void Render::perspectiveDivision(VerToFrag& v2f) {
	v2f.Z = 1.0 / v2f.fragPos.w;

	v2f.fragPos /= v2f.fragPos.w;
	v2f.fragPos.w = 1.0f;
	// Z-Buffer��[0,1]
	v2f.fragPos.z = (v2f.fragPos.z + 1.0) * 0.5;

	v2f.worldPos *= v2f.Z;
	v2f.texture *= v2f.Z;
	v2f.color *= v2f.Z;
}

/*
 * ��������
 * ���������ڵ��������彫���������뵽������ɫ��
 * �ٶԶ�����ɫ��������м������͸�ӳ����Լ��޳�
 * �����ӿڱ任�����������
 */
void Render::drawMesh(const Mesh& mesh)
{
	if (mesh.EBO.empty())
		return;

	for (auto i = 0; i < mesh.EBO.size(); i += 3)
	{
		Vertex v1, v2, v3;
		v1 = mesh.VBO[mesh.EBO[i]];
		v2 = mesh.VBO[mesh.EBO[i + 1]];
		v3 = mesh.VBO[mesh.EBO[i + 2]];

		VerToFrag f1, f2, f3;
		f1 = shader->vertexShader(v1);
		f2 = shader->vertexShader(v2);
		f3 = shader->vertexShader(v3);

		//��׶�޳�
		if (!frustumCull(viewPlanes, f1.worldPos, f2.worldPos, f3.worldPos)) {
			continue;
		}

		//�ü��ռ��޳�
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

			/*if (!faceCull(vNew1.fragPos, vNew2.fragPos, vNew3.fragPos))
			{
				continue;
			}*/

			drawTriangle(vNew1, vNew2, vNew3);
			/*canvas->drawLine(vNew1, vNew2);
			canvas->drawLine(vNew2, vNew3);
			canvas->drawLine(vNew3, vNew1);*/

		}



	}
}

#pragma endregion pipeLine

#pragma region Rasterization
void Render::drawTriangle(const VerToFrag& f1, const VerToFrag& f2, const VerToFrag& f3)
{
	vec4 p1 = f1.fragPos;
	vec4 p2 = f2.fragPos;
	vec4 p3 = f3.fragPos;

	//������Χ��
	int max_x = std::max(p1.x, std::max(p2.x, p3.x));
	int min_x = std::min(p1.x, std::min(p2.x, p3.x));
	int max_y = std::max(p1.y, std::max(p2.y, p3.y));
	int min_y = std::min(p1.y, std::min(p2.y, p3.y));

	//����жϵ��Ƿ����������ڲ�
	for (auto i = std::max(0, min_x); i <= std::min(width, max_x); i++)
	{
		for (auto j = std::max(0, min_y); j <= std::min(height, max_y); j++)
		{
			float r1 = (i - p1.x) * (p2.y - p1.y) - (j - p1.y) * (p2.x - p1.x);
			float r2 = (i - p2.x) * (p3.y - p2.y) - (j - p2.y) * (p3.x - p2.x);
			float r3 = (i - p3.x) * (p1.y - p3.y) - (j - p3.y) * (p1.x - p3.x);

			if (r1 >= 0 && r2 >= 0 && r3 >= 0)
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

					frontBuffer->drawPoint(i, j, shader->fragmentShader(f));
					frontBuffer->writeDepth(i, j, f.fragPos.z);
				}
			}
		}
	}
}

/* ���ߺ���
 * ʹ��Bresenham�㷨�����߶εĻ���
 */
void Render::drawLine(const VerToFrag& f1, const VerToFrag& f2)
{


	vec4 p1 = f1.fragPos;
	vec4 p2 = f2.fragPos;

	//�����x�͡�y
	int deltaX = abs(p2.x - p1.x);
	int deltaY = abs(p2.y - p1.y);

	//��ʼ������
	int stepX = 0;
	int stepY = 0;

	VerToFrag f = f1;
	vec4 p = f.fragPos;

	//�жϲ�������
	if (p2.x > p1.x) stepX = 1;
	else stepX = -1;

	if (p2.y > p1.y) stepY = 1;
	else stepY = -1;

	//�ж�б�ʣ����б��С��1����0�����Ǿ�ѡx��Ϊ��������
	//���б�ʴ���1�����Ǿ�ѡy��Ϊ��������
	int pk = 0;//���߲���
	bool useX = true;
	int sumStep = 0;//��Ҫ�����Ĵ���
	//б�ʴ���1
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

		//��ɫ��ֵ
		float scale = 0.0f;

		if (pk > 0) {
			if (useX) {
				p.y += stepY;
			}
			else {
				p.x += stepX;
			}

			//ֻ��pk>0ʱ������һ��
			pk -= 2 * deltaX;
		}

		//�����겻��pk�Ǻ�ֵ��Ҫ����
		if (useX) {
			p.x += stepX;
		}
		else {
			p.y += stepY;
		}

		//����pkΪ��ֵ������һ��
		pk += 2 * deltaY;

		if (useX) {
			if (p2.x == p1.x) {//��ʱ�������غ�
				scale = 0.0f;
			}
			else {
				scale = (p.x - p1.x) / (p2.x - p1.x);
			}
		}
		else {
			if (p2.y == p1.y) {//��ʱ�������غ�
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


