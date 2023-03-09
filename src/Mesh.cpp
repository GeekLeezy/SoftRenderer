#include "Mesh.h"

void Mesh::addMesh(const Mesh& mesh)
{
	int offset = VBO.size();
	VBO.insert(VBO.end(), mesh.VBO.begin(), mesh.VBO.end());
	//索引缓冲对象中的索引需要考虑未增加之前的顶点数量
	//不能简单地insert
	EBO.reserve(EBO.size() + mesh.EBO.size());

	for (int i = 0; i < mesh.EBO.size(); i++) {
		EBO.push_back(mesh.EBO[i] + offset);
	}
}

void Mesh::addTriangel(const Vertex& v1, const Vertex& v2, const Vertex& v3)
{
	int offset = VBO.size();

	VBO.push_back(v1);
	VBO.push_back(v2);
	VBO.push_back(v3);

	EBO.push_back(offset);
	EBO.push_back(offset + 1);
	EBO.push_back(offset + 2);
}

Mesh& Mesh::operator=(const Mesh& mesh)
{
	//若内存地址相同 返回该Mesh内容
	if (&mesh == this)
	{
		return *this;
	}

	VBO = mesh.VBO;
	EBO = mesh.EBO;

	return *this;
}

Mesh& Mesh::operator+=(const Mesh& mesh)
{
	addMesh(mesh);
	return *this;
}


//存储一些简单结构：三角形、平面以及盒子
Mesh createTriangle(const vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& normal) {
	Mesh result(3, 3);
	//逆时针的三角形
	//顶点顺序 0 1 2
	result.VBO[0].position = vec4(v1, 1.0f);
	result.VBO[0].normal = normal;
	result.VBO[0].color = vec4(255, 0, 0, 255);
	result.VBO[0].texture = vec2(0.0f, 1.0f);
	result.VBO[1].position = vec4(v2, 1.0f);
	result.VBO[1].normal = normal;
	result.VBO[1].color = vec4(0, 255, 0, 255);
	result.VBO[1].texture = vec2(0.0f, 0.0f);
	result.VBO[2].position = vec4(v3, 1.0f);
	result.VBO[2].normal = normal;
	result.VBO[2].color = vec4(0, 0, 255, 255);
	result.VBO[2].texture = vec2(1.0f, 0.0f);
	result.EBO[0] = 0;
	result.EBO[1] = 1;
	result.EBO[2] = 2;

	return result;
}

Mesh createPlane(const vector<vec3>& p, const vec3& n)
{
	if (p.size() != 4)
		return Mesh();

	Mesh plane(4, 6);
	vector<vec4> c =
	{
		vec4(255, 0, 0, 255),
		vec4(0, 255, 0, 255),
		vec4(0, 0, 255, 0),
		vec4(255, 0, 255, 255)
	};

	vector<vec2> t =
	{
		vec2(0.0f, 1.0f),
		vec2(1.0f, 1.0f),
		vec2(1.0f, 0.0f),
		vec2(0.0f, 0.0f)
	};
	for (auto i = 0; i < 4; i++)
	{
		plane.VBO[i].position = vec4(p[i], 1.0f);
		plane.VBO[i].normal = n;
		plane.VBO[i].color = c[0];
		plane.VBO[i].texture = t[i];
	}

	plane.EBO[0] = 0;
	plane.EBO[1] = 2;
	plane.EBO[2] = 1;
	plane.EBO[3] = 0;
	plane.EBO[4] = 3;
	plane.EBO[5] = 2;
	return plane;
}

Mesh createBox(const vec3& center, float r)
{
	Mesh Box;
	vector<vec3> normal =
	{
		vec3(0, 0, 1),
		vec3(-1, 0, 0),
		vec3(1, 0, 0),
		vec3(0, 0, -1),
		vec3(0, 1, 0),
		vec3(0, -1, 0),
	};
	vector<vec3> plane =
	{
		vec3(center + vec3(-r, r, r)),
		vec3(center + vec3(-r, -r, r)),
		vec3(center + vec3(r, -r, r)),
		vec3(center + vec3(r, r, r))
	};
	Mesh front = createPlane(plane, normal[0]);
	Box.addMesh(front);
	plane.clear();

	plane =
	{
		vec3(center + vec3(-r, r, -r)),
		vec3(center + vec3(-r, -r, -r)),
		vec3(center + vec3(-r, -r, r)),
		vec3(center + vec3(-r, r, r))
	};
	Mesh left = createPlane(plane, normal[1]);
	Box.addMesh(left);
	plane.clear();

	plane =
	{
		vec3(center + vec3(r, r, r)),
		vec3(center + vec3(r, -r, r)),
		vec3(center + vec3(r, -r, -r)),
		vec3(center + vec3(r, r, -r))
	};
	Mesh right = createPlane(plane, normal[2]);
	Box.addMesh(right);
	plane.clear();

	plane =
	{
		vec3(center + vec3(r, r, -r)),
		vec3(center + vec3(r, -r, -r)),
		vec3(center + vec3(-r, -r, -r)),
		vec3(center + vec3(-r, r, -r))
	};
	Mesh back = createPlane(plane, normal[3]);
	Box.addMesh(back);
	plane.clear();

	plane =
	{
		vec3(center + vec3(-r, r, -r)),
		vec3(center + vec3(-r, r, r)),
		vec3(center + vec3(r, r, r)),
		vec3(center + vec3(r, r, -r))
	};
	Mesh up = createPlane(plane, normal[4]);
	Box.addMesh(up);
	plane.clear();

	plane =
	{
		vec3(center + vec3(-r, -r, r)),
		vec3(center + vec3(-r, -r, -r)),
		vec3(center + vec3(r, -r, -r)),
		vec3(center + vec3(r, -r, r))
	};
	Mesh down = createPlane(plane, normal[5]);
	Box.addMesh(down);
	plane.clear();

	return Box;
}
