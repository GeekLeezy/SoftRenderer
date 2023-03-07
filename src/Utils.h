#pragma once

#include <glm/glm.hpp>

using namespace glm;

mat4 getProjectMat(const float& fovY, const float& aspect, const float& zNear, const float& zFar)
{
	mat4 projectMat(0.0f);

	float tanHalfFovY = tan(fovY / 2.0f);

	projectMat[0][0] = 1.0f / (aspect * tanHalfFovY);
	projectMat[1][1] = 1.0f / tanHalfFovY;
	projectMat[2][2] = (zNear + zFar) / (zNear - zFar);
	projectMat[2][3] = -1.0f;
	projectMat[3][2] = 2.0f * zNear * zFar / (zNear - zFar);

	return projectMat;
}

mat4 getViewPortMat(const int& width, const int& height)
{
	mat4 viewPortMat = mat4(1.0f);
	viewPortMat[0][0] = width / 2.0f;
	viewPortMat[3][0] = width / 2.0f;
	viewPortMat[1][1] = height / 2.0f;
	viewPortMat[3][1] = height / 2.0f;
	return viewPortMat;
}