#include "Math.h"

extern mat4 lightMatrix;

//视口变换
mat4 getViewPortMat(const int& width, const int& height)
{
    mat4 viewPortMat = mat4(1.0f);
    viewPortMat[0][0] = width / 2.0f;
    viewPortMat[3][0] = width / 2.0f;
    viewPortMat[1][1] = height / 2.0f;
    viewPortMat[3][1] = height / 2.0f;
    return viewPortMat;
}

//观察矩阵
mat4 getViewMat(vec3 pos, vec3 dir, vec3 right, vec3 up)
{
    mat4 viewMat(1.0f);
    vec3 d = normalize(dir);
    vec3 r = normalize(cross(d, up));
    vec3 u = cross(r, d);

    viewMat[0][0] = r.x;
    viewMat[1][0] = r.y;
    viewMat[2][0] = r.z;
    viewMat[0][1] = u.x;
    viewMat[1][1] = u.y;
    viewMat[2][1] = u.z;
    viewMat[0][2] = -d.x;
    viewMat[1][2] = -d.y;
    viewMat[2][2] = -d.z;
    viewMat[3][0] = -dot(r, pos);
    viewMat[3][1] = -dot(u, pos);
    viewMat[3][2] = dot(d, pos);

    return viewMat;

    //return glm::lookAt(position, position + dir, up);
}

//透视矩阵
mat4 getProjectMat(const float& fovY, const float& aspect, const float& zNear, const float& zFar)
{
    mat4 projectMat(0.0f);

    float tanHalfFovY = tan(fovY / 2.0f);

    projectMat[0][0] = 1.0f / (aspect * tanHalfFovY);
    projectMat[1][1] = 1.0f / tanHalfFovY;
    projectMat[2][2] = (zNear + zFar) / (zNear - zFar);
    projectMat[2][3] = -1.0f;
    projectMat[3][2] = 2.0f * zNear * zFar / (zNear - zFar);

    return projectMat;
}

//点到平面距离 d =  Ax + By + Cz + D;
// d < 0 点在平面法向反方向所指的区域
// d > 0 点在平面法向所指的区域
// d = 0 在平面上
// d < 0为 false
bool pointToPlane(const glm::vec3& v, const glm::vec4& p) {
    return p.x * v.x + p.y * v.y + p.z * v.z + p.w >= 0;
}

//约束函数
float saturate(const float& val)
{
    if (val > 1.0f)
    {
        return 1.0f;
    }
    if (val < 0.0f)
    {
        return 0.0f;
    }
    return val;
}

vec3 reflect(const vec3& lightDir, const vec3& normal) {
    return lightDir - 2 * dot(normal, lightDir) * normal;
}

vec4 packDepth(const float& depth) {
    vec4 bitShift = vec4(256.0 * 256.0 * 256.0, 256.0 * 256.0, 256.0, 1.0);
    vec4 bitMask = vec4(0.0, 1.0 / 256.0, 1.0 / 256.0, 1.0 / 256.0);
    vec4 encode = fract(depth * bitShift);

    vec4 temp = vec4(encode.x, vec3(encode));
    encode -= temp * bitMask;
    return encode;
}


float unpackDepth(const vec4& enc) {
    const vec4 bitShift = vec4(1.0 / (256.0 * 256.0 * 256.0), 1.0 / (256.0 * 256.0), 1.0 / 256.0, 1.0);
    float decoded = dot(enc, bitShift);
    return decoded;
}