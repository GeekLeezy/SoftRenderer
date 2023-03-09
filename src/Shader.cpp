#include "Shader.h"

extern mat4 modelMatrix;
extern mat4 viewMatrix;
extern mat4 projectMatrix;

VerToFrag Shader::vertexShader(const Vertex& v)
{
	VerToFrag v2f;
	v2f.worldPos = modelMatrix * v.position;
	v2f.fragPos = projectMatrix * viewMatrix * v2f.worldPos;

	v2f.Z = 1.0;
	v2f.normal = mat3(transpose(inverse(modelMatrix))) * v.normal;
	v2f.color = v.color;
	v2f.texture = v.texture;

	return v2f;
}

vec4 Shader::fragmentShader(const VerToFrag& v2f)
{
	vec4 color = texture->sampler2D(v2f.texture);
	return color;
}

