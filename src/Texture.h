#pragma once

#include "Global.h"

using namespace glm;

class Texture
{
	public:
		int width;
		int height;
		int channels;
		unsigned char* data;

		Texture() = default;

		Texture(const std::string& path);

		void loadTexture(const std::string& path);

		vec4 sampler2D(const vec2& texture);

		vec4 getColor(const int& x, const int& y);

		~Texture() 
		{
			if (data) free(data);
		}
};