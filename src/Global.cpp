#include "Global.h"

//�任����
glm::mat4 modelMatrix(1.0f);
glm::mat4 viewMatrix;
glm::mat4 projectMatrix;
glm::mat4 viewPortMatrix;

//��Ķ���
class Camera;
class Render;
class Material;

class Light;

//ȫ��ָ��
Camera* camera;
Render* renderer;
Material* curMaterial;
Light* dirLight;