#pragma once

#include "VerToFrag.h"
#include "Texture.h"

using namespace glm;
class Shader
{
	public:
		Shader() = default;
		virtual VerToFrag vertexShader(const Vertex& v);

		virtual vec4 fragmentShader(const VerToFrag& v2f, const vec4& baseColor);

		~Shader() = default;
};

class ShadowShader
{
public:

	ShadowShader() = default;

	VerToFrag vertexShader(const Vertex& v);

	vec4 fragmentShader(const VerToFrag& v2f, const vec4& baseColor);

	~ShadowShader() = default;
};

