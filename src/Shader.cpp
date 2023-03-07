#include "Shader.h"

VerToFrag Shader::vertexShader(const Vertex& v)
{
	VerToFrag v2f;
	v2f.wolrdPos = modelMat * v.position;
	v2f.fragPos = projectMat * viewMat * v2f.wolrdPos;
	v2f.normal = mat3(transpose(inverse(modelMat))) * v.normal;
	v2f.color = v.color;
	v2f.texture = v.texture;

	return v2f;
}

//‘› ±œ» ‰≥ˆcolor
Color Shader::fragmentShader(const VerToFrag& v2f)
{
	return v2f.color;
}

