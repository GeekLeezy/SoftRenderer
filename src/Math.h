#pragma once

#include "Global.h"

//�ӿڱ任
mat4 getViewPortMat(const int& width, const int& height);

//�۲����
mat4 getViewMat(vec3 pos, vec3 dir, vec3 right, vec3 up);

//͸�Ӿ���
mat4 getProjectMat(const float& fovY, const float& aspect, const float& zNear, const float& zFar);

//�㵽ƽ����� d =  Ax + By + Cz + D;
// d < 0 ����ƽ�淨�򷴷�����ָ������
// d > 0 ����ƽ�淨����ָ������
// d = 0 ��ƽ����
// d < 0Ϊ false
bool pointToPlane(const glm::vec3& v, const glm::vec4& p);

float saturate(const float& val);

vec3 reflect(const vec3& lightDir, const vec3& normal);