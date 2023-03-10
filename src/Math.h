#pragma once

#include "Global.h"

//视口变换
mat4 getViewPortMat(const int& width, const int& height);

//观察矩阵
mat4 getViewMat(vec3 pos, vec3 dir, vec3 right, vec3 up);

//透视矩阵
mat4 getProjectMat(const float& fovY, const float& aspect, const float& zNear, const float& zFar);

//点到平面距离 d =  Ax + By + Cz + D;
// d < 0 点在平面法向反方向所指的区域
// d > 0 点在平面法向所指的区域
// d = 0 在平面上
// d < 0为 false
bool pointToPlane(const glm::vec3& v, const glm::vec4& p);

float saturate(const float& val);

vec3 reflect(const vec3& lightDir, const vec3& normal);