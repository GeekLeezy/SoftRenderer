#include "Shader.h"
#include "Material.h"
#include "Light.h"
#include "Camera.h"

//ȫ�ֱ���
extern mat4 modelMatrix;
extern mat4 viewMatrix;
extern mat4 projectMatrix;

extern const vec3 ambient;

extern Camera* camera;
extern Material* curMaterial;
extern Light* dirLight;


//������ɫ��
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

//ƬԪ��ɫ��
vec4 Shader::fragmentShader(const VerToFrag& v2f, const vec4& baseColor)
{
	//vec4 albedo = curMaterial->mainTexture->sampler2D(v2f.texture) * curMaterial->diffuse;
	vec4 albedo = baseColor * curMaterial->diffuse;
	vec3 normal = normalize(v2f.normal);
	vec3 viewDir = normalize(camera->position - vec3(v2f.worldPos));

	vec3 color = ambient * vec3(albedo);

	color += dirLight->calLight(vec3(v2f.worldPos), normal, viewDir, albedo);

	return vec4(color, 1.0f);

	/*vec4 color = curMaterial->mainTexture->sampler2D(v2f.texture);
	return color;*/
}

