#pragma once

#include "Global.h"
#include "VerToFrag.h"

using namespace std;

//视锥体的六个平面方程，用于视锥剔除
//所得的法向都是指向内部的（面向原点）
void viewFrustumPlanes(vector<vec4>& result, const mat4& vp);


//修改：修改至在透视除法之前进行剔除

//判断点是否在直线内部
bool pointInside(const vec4& L, const vec4& p);

//判断三角形三个顶点是否都在NDC内
bool vertexsInside(const vector<VerToFrag>& v);

//顶点插值求交点
VerToFrag getIntersect(const VerToFrag& f1, const VerToFrag& f2, const vec4& L);
//使用Sutherland–Hodgman多边形裁剪算法
//对三角形进行裁剪，返回一个包含新顶点的数组
vector<VerToFrag> viewPortCull(const VerToFrag& f1, const VerToFrag& f2, const VerToFrag& f3);