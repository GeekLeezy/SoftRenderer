#pragma once
#include <glm/glm.hpp>

#include "Math.h"

using namespace glm;

class Vertex
{
	public:
		vec4 position;
		vec4 color;
		vec3 normal;
		vec2 texture;

		Vertex() = default;

		Vertex(
			const vec4& _pos, 
			const vec4& _color,
			const vec3& _normal,
			const vec2& _texture) :position(_pos), color(_color), normal(_normal), texture(_texture) {}

		Vertex(
			const vec3& _pos,
			const vec4& _color = vec4(255, 255, 255, 255),
			const vec3& _normal = vec3(0.0f, 0.0f, 0.0f),
			const vec2& _texture = vec2(0.0f, 0.0f)) :position(vec4(_pos, 1.0f)), color(_color), normal(_normal), texture(_texture) {}

		~Vertex() = default;
};


