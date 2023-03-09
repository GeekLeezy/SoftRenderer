#pragma once
#include "Vertex.h"

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

		Mesh& operator=(const Mesh& mesh);

		Mesh& operator+=(const Mesh& mesh);

		~Mesh() = default;
};

Mesh createTriangle(const vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& normal);
Mesh createPlane(const vector<vec3>& p, const vec3& n);
Mesh createBox(const vec3& center, float r);
