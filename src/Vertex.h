#pragma once
#include <glm/glm.hpp>

struct Color
{
	//由于实际显示的是BGRA，所以换一下顺序，让其符合RGBA
	unsigned char blue;
	unsigned char green;
	unsigned char red;
	unsigned char alpha;

	Color(const unsigned char& r = 255, const unsigned char& g = 255, const unsigned char& b = 255, const unsigned char& a = 255) {
		red = r;
		green = g;
		blue = b;
		alpha = a;
	}
};

using namespace glm;

class Vertex
{
	public:
		vec4 position;
		Color color;
		vec3 normal;
		vec2 texture;

		Vertex() = default;

		Vertex(
			const vec4& _pos, 
			const Color& _color,
			const vec3& _normal,
			const vec2& _texture) :position(_pos), color(_color), normal(_normal), texture(_texture) {}

		Vertex(
			const vec3& _pos,
			const Color& _color = Color(255, 255, 255, 255),
			const vec3& _normal = vec3(0.0f, 0.0f, 0.0f),
			const vec2& _texture = vec2(0.0f, 0.0f)) :position(vec4(_pos, 1.0f)), color(_color), normal(_normal), texture(_texture) {}

		~Vertex() = default;
};

class VerToFrag
{
public:
	vec4 wolrdPos;
	vec4 fragPos;
	Color color;
	vec3 normal;
	vec2 texture;

	VerToFrag() = default;

	VerToFrag(
		const vec4& _wolrdPos,
		const vec4& _fragPos,
		const Color& _color,
		const vec3& _normal,
		const vec2& _texture) : wolrdPos(_wolrdPos), fragPos(_fragPos), color(_color), normal(_normal), texture(_texture){}

	VerToFrag(const VerToFrag& v2f) :
		wolrdPos(v2f.wolrdPos), fragPos(v2f.fragPos), color(v2f.color), normal(v2f.normal), texture(v2f.texture) {}

	~VerToFrag() = default;
};


