#include "Global.h"

glm::mat4 modelMatrix(1.0f);
glm::mat4 viewMatrix;
glm::mat4 projectMatrix;
glm::mat4 viewPortMatrix;

class Camera;
class Render;
class Material;

Camera* camera;
Render* renderer;
Material* currentMat;