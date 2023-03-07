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

void Mesh::drawMesh(Shader *shader, const mat4& viewPortMat)
{
	if (EBO.empty())
		return;

	for (auto i = 0; i < EBO.size(); i += 3)
	{
		Vertex v1, v2, v3;
		v1 = VBO[EBO[i]];
		v2 = VBO[EBO[i + 1]];
		v3 = VBO[EBO[i + 2]];

		VerToFrag f1, f2, f3;
		f1 = shader->vertexShader(v1);
		f2 = shader->vertexShader(v2);
		f3 = shader->vertexShader(v3);

		f1.fragPos /= f1.fragPos.w;
		f2.fragPos /= f2.fragPos.w;
		f3.fragPos /= f3.fragPos.w;

		f1.fragPos = viewPortMat * f1.fragPos;
		f2.fragPos = viewPortMat * f2.fragPos;
		f3.fragPos = viewPortMat * f3.fragPos;

		//draw();
	}
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