#pragma once

#include "Texture.h"
#include "VerToFrag.h"

using namespace glm;
class Shader
{
	public:
		Shader() = default;
		VerToFrag vertexShader(const Vertex& v);

		vec4 fragmentShader(const VerToFrag& v2f, const vec4& baseColor);

		~Shader() = default;
};

