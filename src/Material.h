#pragma once

#include "Shader.h"

class Material
{
	public:
		vec4 diffuse;
		vec4 specular;
		int glossy;

		Texture* mainTexture;
		Shader* shader;

		Material() : diffuse(vec4(1.0f)), specular(vec4(1.0f)), glossy(32), mainTexture(nullptr), shader(nullptr){}
		Material(
			const vec4& D, 
			const vec4& S, 
			const int& G
		) : diffuse(D), specular(S), glossy(G), mainTexture(nullptr), shader(nullptr) {}

		void setShader(Shader* s) {
			shader = s;
		}
		void setTexture(Texture* t) {
			mainTexture = t;
		}

		~Material() = default;

};
