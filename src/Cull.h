#pragma once

#include "Global.h"
#include "Math.h"
#include "Camera.h"

using namespace std;

extern Camera* camera;

//��׶�޳�
bool frustumCull(const vector<vec4> viewPlanes, const vec4& v1, const vec4& v2, const vec4& v3);

//�ü��ռ��޳�
bool clipSpaceCull(const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3);

//���޳�
bool faceCull(const vec4& v1, const vec4& v2, const vec4& v3);
