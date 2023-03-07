#pragma once
#include "Shader.h"
#include <vector>

using namespace std;
class Mesh
{
	public:
		vector<Vertex> VBO;
		vector<unsigned int> EBO;

		Mesh() = default;

		Mesh(const int& vNum, const int& iNum)
		{
			VBO.resize(vNum);
			EBO.resize(iNum);
		}

		Mesh(const Mesh& mesh) : VBO(mesh.VBO), EBO(mesh.EBO) {}

		void addMesh(const Mesh& mesh);

		void addTriangel(const Vertex& v1, const Vertex& v2, const Vertex& v3);

		void drawMesh(Shader *shader, const mat4& viewPortMat);

		Mesh& operator=(const Mesh& mesh);

		Mesh& operator+=(const Mesh& mesh);

		~Mesh() = default;
};

