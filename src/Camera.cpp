#include "Camera.h"

void Camera::updateCameraVectors()
{
    // 根据欧拉角计算dir
    vec3 direction;
    direction.x = cos(radians(yaw)) * cos(radians(pitch));
    direction.y = sin(radians(pitch));
    direction.z = sin(radians(yaw)) * cos(radians(pitch));
    dir = normalize(direction);
    // 重新计算right、up向量
    // 归一化防止运动速度的不一致
    right = normalize(cross(dir, worldUp));
    up = normalize(cross(right, dir));
}

mat4 Camera::getViewMatrix()
{
    return getViewMat(position, dir, right, up);
}

mat4 Camera::getProjectMatrix()
{
    return getProjectMat(fov, aspect, n, f);
}

void Camera::updateFov(float fov) {
    fov = glm::radians(fov);
}
void Camera::updateAspect(int w, int h) {
    aspect = (float)w / h;
}
void Camera::moveForward(float distance) {
    position += dir * distance;
}
void Camera::moveRight(float distance) {
    position += right * distance;
}
void Camera::moveUp(float distance) {
    position += up * distance;
}
void Camera::rotatePitch(float angle) {
    pitch += angle;
    if (pitch > 89.0)
        pitch = 89.0;
    if (pitch < -89.0)
        pitch = -89.0;
    updateCameraVectors();
}
void Camera::rotateYaw(float angle) {
    yaw += angle;
    updateCameraVectors();
}
