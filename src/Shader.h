#pragma once
#include <glm/glm.hpp>
#include "Vertex.h"
//#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
class Shader
{
	private:
		mat4 modelMat;
		mat4 viewMat;
		mat4 projectMat;
	public:
		Shader() 
		{
			modelMat = mat4(1.0f);
			viewMat = mat4(1.0f);
			projectMat = mat4(1.0f);
		}

		void setModelMat(const mat4& model)
		{
			modelMat = model;
		}

		void setViewMat(const mat4& view)
		{
			viewMat = view;
		}

		void setProjectMat(const mat4& project)
		{
			projectMat = project;
		}

		VerToFrag vertexShader(const Vertex& v);

		Color fragmentShader(const VerToFrag& v2f);

		~Shader() = default;
};

