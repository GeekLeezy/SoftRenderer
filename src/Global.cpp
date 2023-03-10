#include "Global.h"

//变换矩阵
glm::mat4 modelMatrix(1.0f);
glm::mat4 viewMatrix;
glm::mat4 projectMatrix;
glm::mat4 viewPortMatrix;

//类的定义
class Camera;
class Render;
class Material;

class Light;

//全局指针
Camera* camera;
Render* renderer;
Material* curMaterial;
Light* dirLight;