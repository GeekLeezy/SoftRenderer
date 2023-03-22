#pragma once

#include "Math.h"
#include "Clip.h"

const float YAW = -90.0f;
const float PITCH = 0.0f;

class Camera
{
    private:
        void updateCameraVectors();
	public:
        // 相机基本属性
        vec3 position;
        vec3 dir;
        vec3 up;
        vec3 right;
        vec3 worldUp;

        float n;
        float f;
        float fov;
        float aspect;

        //欧拉角
        float yaw;
        float pitch;

        Camera(
            const vec3& _pos = vec3(0.0f, 0.0f, 0.0f),
            const vec3& _up = vec3(0.0f, 1.0f, 0.0f),
            const float& _fov = 45.0f,
            const float& _aspect = 800.0 / 600.0,
            const float& _n = 1.0f, 
            const float& _f = 10.0f, 
            const float& _yaw = YAW,
            const float& _pitch = PITCH) : dir(vec3(0.0f, 0.0f, -1.0f))
        {
            position = _pos;
            worldUp = _up;
            fov = _fov;
            aspect = _aspect;
            n = _n;
            f = _f;
            yaw = _yaw;
            pitch = _pitch;
            updateCameraVectors();
        }

        ~Camera() = default;

        mat4 getViewMatrix();
        mat4 getProjectMatrix();
        mat4 getOrthoMatrix() 
        {
            return ortho(-2.0f, 2.0f, -2.0f, 2.0f, 1.0f, 10.0f);
        }

#pragma region stateUpdate

        void updateFov(float fov = 45.0f);
        void updateAspect(int w, int h);
        void moveForward(float distance);
        void moveRight(float distance);
        void moveUp(float distance);
        void rotatePitch(float angle);
        void rotateYaw(float angle);

#pragma endregion stateUpdate
};

