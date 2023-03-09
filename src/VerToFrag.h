#pragma once
#include "Vertex.h"

class VerToFrag
{
public:
	vec4 worldPos;
	vec4 fragPos;
	vec4 color;
	vec3 normal;
	vec2 texture;
	float Z;

	VerToFrag() = default;

	VerToFrag(
		const vec4& _wolrdPos,
		const vec4& _fragPos,
		const vec4& _color,
		const vec3& _normal,
		const vec2& _texture) : worldPos(_wolrdPos), fragPos(_fragPos), color(_color), normal(_normal), texture(_texture), Z(1.0f) {}

	VerToFrag(const VerToFrag& v2f) :
		worldPos(v2f.worldPos), fragPos(v2f.fragPos), color(v2f.color), normal(v2f.normal), texture(v2f.texture), Z(v2f.Z) {}

	~VerToFrag() = default;
};

static VerToFrag lerp(const VerToFrag& f1, const VerToFrag& f2, const float& scale)
{
	VerToFrag f;

	f.worldPos = (1 - scale) * f1.worldPos + scale * f2.worldPos;
	f.fragPos = (1 - scale) * f1.fragPos + scale * f2.fragPos;
	//f.fragPos.x = (int)f.fragPos.x;
	//f.fragPos.y = (int)f.fragPos.y;

	f.normal = (1 - scale) * f1.normal + scale * f2.normal;
	f.texture = (1 - scale) * f1.texture + scale * f2.texture;

	f.color = (1 - scale) * f1.color + scale * f2.color;
	f.Z = (1 - scale) * f1.Z + scale * f2.Z;

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

	f.worldPos = a * f1.worldPos + b * f2.worldPos + c * f3.worldPos;
	f.normal = a * f1.normal + b * f2.normal + c * f3.normal;
	f.texture = a * f1.texture + b * f2.texture + c * f3.texture;
	f.color = a * f1.color + b * f2.color + c * f3.color;
	f.Z = a * f1.Z + b * f2.Z + c * f3.Z;

	return f;
}

