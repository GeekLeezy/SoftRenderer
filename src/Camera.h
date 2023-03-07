#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

const float YAW = -90.0f;
const float PITCH = 0.0f;

class Camera
{
    private:
        void updateCameraVectors()
        {
            // ����ŷ���Ǽ���dir
            vec3 direction;
            direction.x = cos(radians(yaw)) * cos(radians(pitch));
            direction.y = sin(radians(pitch));
            direction.z = sin(radians(yaw)) * cos(radians(pitch));
            dir = normalize(direction);
            // ���¼���right��up����
            // ��һ����ֹ�˶��ٶȵĲ�һ��
            right = normalize(cross(dir, worldUp)); 
            up = normalize(cross(right, dir));
        }
	public:
        // �����������
        vec3 position;
        vec3 dir;
        vec3 up;
        vec3 right;
        vec3 worldUp;

        //ŷ����
        float yaw;
        float pitch;

        Camera(
            const vec3& _pos = vec3(0.0f, 0.0f, 0.0f),
            const vec3& _up = vec3(0.0f, 1.0f, 0.0f),
            const float& _yaw = YAW,
            const float& _pitch = PITCH) : dir(vec3(0.0f, 0.0f, -1.0f))
        {
            position = _pos;
            worldUp = _up;
            yaw = _yaw;
            pitch = _pitch;
            updateCameraVectors();
        }

        ~Camera() = default;

        mat4 getViewMatrix() 
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
            viewMat[3][0] = -dot(r, position);
            viewMat[3][1] = -dot(u, position);
            viewMat[3][2] =  dot(d, position);

            return viewMat;

            //return glm::lookAt(position, position + dir, up);
        }
};

