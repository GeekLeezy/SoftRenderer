#include "Texture.h"
#include "stb_image.h"

Texture::Texture(const std::string& path)
{
	data = stbi_load(path.c_str(), &width, &height, &channels, 0);
}

void Texture::loadTexture(const std::string& path)
{
	if (data)
		free(data);
	data = stbi_load(path.c_str(), &width, &height, &channels, 0);
}

vec4 Texture::sampler2D(const vec2& texture)
{
	//REPEAT
	float x = texture.x - (float)floor(texture.x);
	float y = texture.y - (float)floor(texture.y);

	x = x < 0 ? -x : x;
	y = y < 0 ? -y : y;

	return getColor(x * (width - 1), y * (height - 1)) / 255.0f;
}

vec4 Texture::getColor(const int& x, const int& y)
{
	if (x < 0 || x > width || y < 0 || y > height)
		return vec4(0, 0, 0, 0);

	int xy = channels * (y * width + x);

	if(channels == 3)
		return vec4(
			*(data + xy),
			*(data + xy + 1),
			*(data + xy + 2),
			255.0f
		);
	else
		return vec4(
			*(data + xy),
			*(data + xy + 1),
			*(data + xy + 2),
			*(data + xy + 3)
		);
}
