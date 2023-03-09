#pragma once

#include "Global.h"
#include "VerToFrag.h"

using namespace std;

//��׶�������ƽ�淽�̣�������׶�޳�
//���õķ�����ָ���ڲ��ģ�����ԭ�㣩
void viewFrustumPlanes(vector<vec4>& result, const mat4& vp);


//�޸ģ��޸�����͸�ӳ���֮ǰ�����޳�

//�жϵ��Ƿ���ֱ���ڲ�
bool pointInside(const vec4& L, const vec4& p);

//�ж����������������Ƿ���NDC��
bool vertexsInside(const vector<VerToFrag>& v);

//�����ֵ�󽻵�
VerToFrag getIntersect(const VerToFrag& f1, const VerToFrag& f2, const vec4& L);
//ʹ��Sutherland�CHodgman����βü��㷨
//�������ν��вü�������һ�������¶��������
vector<VerToFrag> viewPortCull(const VerToFrag& f1, const VerToFrag& f2, const VerToFrag& f3);