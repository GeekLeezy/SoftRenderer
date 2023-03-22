#include "Light.h"
#include "Material.h"
extern Material* curMaterial;

vec3 DirectionLight::calLight(const vec3& worldPos, const vec3& normal, const vec3& viewDir, const vec3& aldebo) const
{
	float diff = std::max(dot(normal, -direction), 0.0f);

	vec3 half = normalize(viewDir - direction);
	float spec = pow(std::max(0.0f, dot(half, normal)), curMaterial->glossy);
	//float spec = pow(max(0.0f, dot(half, normal)),128);

	vec3 diffuse = kd * diff * aldebo;
	vec3 specular = ks * spec;

	return (diffuse + specular) * intensity;
}

vec3 PointLight::calLight(const vec3& worldPos, const vec3& normal, const vec3& viewDir, const vec3& aldebo) const
{
	float dist = distance(worldPos, position);
	float attenuation = 1.0 / (constant + linear * dist + quad * (dist * dist));

	vec3 lightDir = normalize(vec3(worldPos - position));

	float diff = std::max(dot(normal, -lightDir), 0.0f);
	vec3 half = normalize(viewDir - lightDir);
	float spec = pow(std::max(0.0f, dot(half, normal)), curMaterial->glossy);
	//float spec = pow(max(0.0f, dot(half, normal)),128);

	vec3 diffuse = kd * diff * aldebo;
	vec3 specular = ks * spec;

	return (diffuse + specular) * attenuation * intensity;
}