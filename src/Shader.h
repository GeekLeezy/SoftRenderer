#pragma once

#include "Texture.h"
#include "VerToFrag.h"

using namespace glm;
class Shader
{
	private:
		Texture* texture;
	public:
		Shader(Texture *_texture) 
		{
			texture = _texture;
		}
		VerToFrag vertexShader(const Vertex& v);

		vec4 fragmentShader(const VerToFrag& v2f);

		~Shader() = default;
};

