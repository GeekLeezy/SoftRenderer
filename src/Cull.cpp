#include "Cull.h"

//视锥剔除
bool frustumCull(const vector<vec4> viewPlanes, const vec4& v1, const vec4& v2, const vec4& v3)
{
	glm::vec3 minPoint, maxPoint;

	//计算世界下的包围盒
	minPoint.x = std::min(v1.x, std::min(v2.x, v3.x));
	minPoint.y = std::min(v1.y, std::min(v2.y, v3.y));
	minPoint.z = std::min(v1.z, std::min(v2.z, v3.z));
	maxPoint.x = std::max(v1.x, std::max(v2.x, v3.x));
	maxPoint.y = std::max(v1.y, std::max(v2.y, v3.y));
	maxPoint.z = std::max(v1.z, std::max(v2.z, v3.z));

	for (auto i = 0; i < 5; i++)
	{
		if (!pointToPlane(minPoint, viewPlanes[i]) && !pointToPlane(maxPoint, viewPlanes[i])) {
			return false;
		}
	}

	// Far 剔除时只保留完全在内的
	if (!pointToPlane(minPoint, viewPlanes[5]) || !pointToPlane(maxPoint, viewPlanes[5])) {
		return false;
	}

	return true;
}

//裁剪空间剔除
bool clipSpaceCull(const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3) {

	if (v1.w <= camera->n && v2.w <= camera->n && v3.w <= camera->n)
		return false;
	if (v1.w >= camera->f && v2.w <= camera->f && v3.w <= camera->f)
		return false;
	if (v1.x <= v1.w || v1.y <= v1.w || v1.z <= v1.w)
		return true;
	if (v2.x <= v2.w || v2.y <= v2.w || v2.z <= v2.w)
		return true;
	if (v3.x <= v3.w || v3.y <= v3.w || v3.z <= v3.w)
		return true;
	return false;
}

//面剔除
bool faceCull(const vec4& v1, const vec4& v2, const vec4& v3)
{
	vec3 s1 = vec3(v2) - vec3(v1);
	vec3 s2 = vec3(v3) - vec3(v1);

	vec3 normal = normalize(cross(s1, s2));
	vec3 view = vec3(0, 0, 1);

	return dot(normal, view) < 0;
}