#include "Clip.h"

//视锥体的六个平面方程，用于视锥剔除
//所得的法向都是指向内部的（面向原点）
void viewFrustumPlanes(vector<vec4>& result, const mat4& vp) {
	//左侧  
	result[0].x = vp[0][3] + vp[0][0];
	result[0].y = vp[1][3] + vp[1][0];
	result[0].z = vp[2][3] + vp[2][0];
	result[0].w = vp[3][3] + vp[3][0];
	//右侧
	result[1].x = vp[0][3] - vp[0][0];
	result[1].y = vp[1][3] - vp[1][0];
	result[1].z = vp[2][3] - vp[2][0];
	result[1].w = vp[3][3] - vp[3][0];
	//上侧
	result[2].x = vp[0][3] - vp[0][1];
	result[2].y = vp[1][3] - vp[1][1];
	result[2].z = vp[2][3] - vp[2][1];
	result[2].w = vp[3][3] - vp[3][1];
	//下侧
	result[3].x = vp[0][3] + vp[0][1];
	result[3].y = vp[1][3] + vp[1][1];
	result[3].z = vp[2][3] + vp[2][1];
	result[3].w = vp[3][3] + vp[3][1];
	//Near
	result[4].x = vp[0][3] + vp[0][2];
	result[4].y = vp[1][3] + vp[1][2];
	result[4].z = vp[2][3] + vp[2][2];
	result[4].w = vp[3][3] + vp[3][2];
	//Far
	result[5].x = vp[0][3] - vp[0][2];
	result[5].y = vp[1][3] - vp[1][2];
	result[5].z = vp[2][3] - vp[2][2];
	result[5].w = vp[3][3] - vp[3][2];
}



//修改：修改至在透视除法之前进行剔除

//判断点是否在直线内部
bool pointInside(const vec4& L, const vec4& p) {
	//return L.x * p.x + L.y * p.y + L.z <= 0;

	//常数D需要将w分量相乘
	return L.x * p.x + L.y * p.y + L.z * p.z + L.w * p.w <= 0;
}

//判断三角形三个顶点是否都在NDC内
bool vertexsInside(const vector<VerToFrag>& v)
{
	for (auto i = 0; i < v.size(); i++)
	{
		float x = v[i].fragPos.x;
		float y = v[i].fragPos.y;
		float z = v[i].fragPos.z;
		float w = v[i].fragPos.w;

		if (x > w || y > w || z > w || x < -w || y < -w || z < -w)
			return false;

	}

	return true;
}

//顶点插值求交点
VerToFrag getIntersect(const VerToFrag& f1, const VerToFrag& f2, const vec4& L)
{
	/*if (L.x == -1)
	{
		float w = (-1 - f1.fragPos.x) / (f2.fragPos.x - f1.fragPos.x);
		return lerp(f1, f2, w);
	}

	else if (L.x == 1)
	{
		float w = (1 - f1.fragPos.x) / (f2.fragPos.x - f1.fragPos.x);
		return lerp(f1, f2, w);
	}

	else if (L.y == -1)
	{
		float w = (-1 - f1.fragPos.y) / (f2.fragPos.y - f1.fragPos.y);
		return lerp(f1, f2, w);
	}

	else
	{
		float w = (1 - f1.fragPos.y) / (f2.fragPos.y - f1.fragPos.y);
		return lerp(f1, f2, w);
	}*/
	vec4 p1 = f1.fragPos;
	vec4 p2 = f2.fragPos;

	float d1 = L.x * p1.x + L.y * p1.y + L.z * p1.z + L.w * p1.w;
	float d2 = L.x * p2.x + L.y * p2.y + L.z * p2.z + L.w * p2.w;

	//通过两点到面的距离插值
	float w = d1 / (d1 - d2);

	return lerp(f1, f2, w);
}

//使用Sutherland–Hodgman多边形裁剪算法
//对三角形进行裁剪，返回一个包含新顶点的数组
vector<VerToFrag> viewPortCull(const VerToFrag& f1, const VerToFrag& f2, const VerToFrag& f3)
{
	vector<VerToFrag> output = { f1, f2, f3 };

	//若全部顶点都在NDC中 直接返回这些顶点
	if (vertexsInside(output))
		return output;

	vector<glm::vec4> clipRect =
	{
		//先进行near平面的裁剪 保证w > 0
		//near
		vec4(0,0,1,1),
		//far
		vec4(0,0,-1,1),
		//left
		vec4(1,0,0,1),
		//right
		vec4(-1,0,0,1),
		//top 
		vec4(0,-1,0,1),
		//bottom
		vec4(0,1,0,1)
	};

	for (auto i = 0; i < clipRect.size(); i++)
	{
		vec4 L = clipRect[i];
		vector<VerToFrag> input(output);
		output.clear();

		for (auto j = 0; j < input.size(); j++)
		{
			VerToFrag vNow = input[j];
			VerToFrag vPre = input[(j + input.size() - 1) % input.size()];

			//当前点在裁剪线内
			if (!pointInside(L, vNow.fragPos))
			{
				//上一点不在裁剪线内
				if (pointInside(L, vPre.fragPos))
				{
					VerToFrag f = getIntersect(vNow, vPre, L);
					output.push_back(f);
				}
				output.push_back(vNow);
			}
			//当前点不在裁剪先内 && 上一点在裁剪线内
			//加入交点
			else if (!pointInside(L, vPre.fragPos))
			{
				VerToFrag f = getIntersect(vNow, vPre, L);
				output.push_back(f);
			}
		}
	}

	return output;
}