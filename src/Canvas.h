#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>

#include "Shader.h"

using namespace glm;

class Canvas
{
	public:
		int width;
		int height;
		Color* colorBuffer;

		Canvas(const int &w, const int &h, Color* color);
		void drawPoint(const VerToFrag& v2f);
		void drawLine(const VerToFrag& f1, const VerToFrag& f2);
		void drawTriangle(const VerToFrag& f1, const VerToFrag& f2, const VerToFrag& f3);
		void drawTriangleFlat(const VerToFrag& f1, const VerToFrag& f2, const VerToFrag& f0);
		void drawTriangle2(const VerToFrag& f1, const VerToFrag& f2, const VerToFrag& f3);



		void clearColorBuffer();

		~Canvas() = default;

};

static VerToFrag lerp(const VerToFrag& f1, const VerToFrag& f2, const float& scale)
{
	VerToFrag f;
	f.fragPos = (1 - scale) * f1.fragPos + scale * f2.fragPos;
	//f.fragPos.x = (int)f.fragPos.x;
	//f.fragPos.y = (int)f.fragPos.y;

	f.normal = (1 - scale) * f1.normal + scale * f2.normal;
	f.texture = (1 - scale) * f1.texture + scale * f2.texture;

	vec4 f1Color = vec4(f1.color.red / 255.0, f1.color.green / 255.0, f1.color.blue / 255.0, f1.color.alpha / 255.0);
	vec4 f2Color = vec4(f2.color.red / 255.0, f2.color.green / 255.0, f2.color.blue / 255.0, f2.color.alpha / 255.0);
	vec4 fColor = (1 - scale) * f1Color + scale * f2Color;
	f.color = Color(fColor.r * 255.0, fColor.g * 255.0, fColor.b * 255.0, fColor.a * 255.0);

	return f;
}

static VerToFrag barycenterLerp(const VerToFrag& f1, const VerToFrag& f2, const VerToFrag& f3, const vec2& pos)
{
	VerToFrag f;

	vec4 p1 = f1.fragPos;
	vec4 p2 = f2.fragPos;
	vec4 p3 = f3.fragPos;

	float a = (-(pos.x - p2.x) * (p3.y - p2.y) + (pos.y - p2.y) * (p3.x - p2.x)) / (-(p1.x - p2.x) * (p3.y - p2.y) + (p1.y - p2.y) * (p3.x - p2.x));
	float b = (-(pos.x - p3.x) * (p1.y - p3.y) + (pos.y - p3.y) * (p1.x - p3.x)) / (-(p2.x - p3.x) * (p1.y - p3.y) + (p2.y - p3.y) * (p1.x - p3.x));
	float c = 1 - a - b;

	f.fragPos = a * f1.fragPos + b * f2.fragPos + c * f3.fragPos;
	f.fragPos.x = pos.x;
	f.fragPos.y = pos.y;

	f.normal = a * f1.normal + b * f2.normal + c * f3.normal;
	f.texture = a * f1.texture + b * f2.texture + c * f3.texture;

	vec4 f1Color = vec4(f1.color.red / 255.0, f1.color.green / 255.0, f1.color.blue / 255.0, f1.color.alpha / 255.0);
	vec4 f2Color = vec4(f2.color.red / 255.0, f2.color.green / 255.0, f2.color.blue / 255.0, f2.color.alpha / 255.0);
	vec4 f3Color = vec4(f3.color.red / 255.0, f3.color.green / 255.0, f3.color.blue / 255.0, f3.color.alpha / 255.0);
	vec4 fColor = a * f1Color + b * f2Color + c * f3Color;
	f.color = Color(fColor.r * 255.0, fColor.g * 255.0, fColor.b * 255.0, fColor.a * 255.0);

	return f;
}

//ÅÅÐò
static inline bool yOrderSort(const VerToFrag& p1, const VerToFrag& p2) {
	return p1.fragPos.y > p2.fragPos.y;
}

