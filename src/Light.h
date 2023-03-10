#pragma once
#include "Global.h"

class Light
{
	public:
		vec3 position;
		vec3 kd;
		vec3 ks;
		vec3 direction;
		float intensity;

		Light(
			const vec3& _pos = vec3(0.0f),
			const vec3& _kd = vec3(1.0f),
			const vec3& _ks = vec3(1.0f),
			const vec3& _dir = vec3(0.0f, -1.0f, 0.0f),
			const float& I = 1.0f
		) : position(_pos), kd(_kd), ks(_ks), direction(_dir), intensity(I) {}

		virtual vec3 calLight(const vec3& worldPos, const vec3& normal, const vec3& viewDir, const vec3& aldebo) const = 0;

		virtual ~Light() {}
};

class DirectionLight : public Light
{
	public:
		DirectionLight(
			const vec3& _dir = vec3(0.0f, -1.0f, 0.0f),
			const vec3& _kd = vec3(1.0f),
			const vec3& _ks = vec3(1.0f),
			const float& I = 0.5f
		)
		{
			position = vec3(0.0f);
			direction = _dir;
			kd = _kd;
			ks = _ks;
			intensity = I;
		}

		virtual vec3 calLight(const vec3& worldPos, const vec3& normal, const vec3& viewDir, const vec3& aldebo) const;
};

class PointLight : public Light
{
	public:

		float constant;
		float linear;
		float quad;

		PointLight(
			const vec3& _pos = vec3(0.0f, 2.0f, 0.0f),
			const vec3& _kd = vec3(1.0f),
			const vec3& _ks = vec3(1.0f),
			const float& I = 0.5f,
			const float& _constant = 1.0f,
			const float& _linear = 0.9f,
			const float& _quad = 0.032f
		) : Light(_pos, _kd, _ks, vec3(0.0f, 0.0f, 0.0f), I), 
			constant(_constant), linear(_linear), quad(_quad) {}

		virtual vec3 calLight(const vec3& worldPos, const vec3& normal, const vec3& viewDir, const vec3& aldebo) const;

};