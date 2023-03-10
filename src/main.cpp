#include "Render.h"
#include "Model.h"
#include "Light.h"

#include <thread>

#define IS_KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define IS_KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)


const int width = 800;
const int height = 600;
int fps = 0;

Window* win = nullptr;
extern Render* renderer;
extern Camera* camera;
extern Material* curMaterial;
extern Light* dirLight;

extern mat4 modelMatrix;
extern mat4 viewMatrix;
extern mat4 projectMatrix;
extern mat4 viewPortMatrix;

void cameraInput(Camera* camera);
void Draw(Window* win);
void showFPS(Window* w);


// WinMain��һ���������ú����Ĺ����Ǳ�ϵͳ���ã���Ϊһ��32λӦ�ó������ڵ�
// hInstance��Ӧ�ó���ǰʵ���ľ�����������Ϊָ��ǰʵ����ָ��
// prevInstance��Ӧ�ó������ǰʵ���ľ��������ͬһ����������Σ������������ڵ�һ�δ򿪵Ĵ��ھ�����ǰʵ���Ĵ��ڡ�����һ��32λ���򣬸ò�����ΪNULL
// cmdLine��ָ��Ӧ�ó��������е��ַ�����ָ�룬������ִ���ļ���
// nCmdShow��ָ�����������ʾ
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int nCmdShow)
int main()
{
	win = new Window(width, height);

	std::thread t(showFPS, win);
	t.detach();
	
	Draw(win);
	 
	delete renderer;
	return 0;
}

void cameraInput(Camera* camera) 
{
	float moveSpeed = 0.1f;
	if (IS_KEY_DOWN('A'))
	{
		camera->moveRight(-moveSpeed);
	}

	if (IS_KEY_DOWN('S'))
	{
		camera->moveForward(-moveSpeed);
	}

	if (IS_KEY_DOWN('D'))
	{
		camera->moveRight(moveSpeed);
	}

	if (IS_KEY_DOWN('W'))
	{
		camera->moveForward(moveSpeed);
	}

	if (IS_KEY_DOWN(' '))
	{
		camera->moveUp(moveSpeed);
	}

	if (IS_KEY_DOWN('Z'))
	{
		camera->moveUp(-moveSpeed);
	}

	if (IS_KEY_DOWN('I'))
	{
		camera->rotatePitch(moveSpeed);
	}

	if (IS_KEY_DOWN('K'))
	{
		camera->rotatePitch(-moveSpeed);
	}

	if (IS_KEY_DOWN('J'))
	{
		camera->rotateYaw(-moveSpeed);
	}

	if (IS_KEY_DOWN('L'))
	{
		camera->rotateYaw(moveSpeed);
	}
}

void Draw(Window* win)
{
	//��ʼ��Ϣѭ��
	//������Ϣ�ṹ��
	MSG msg = { 0 };

	//������ɫ��
	Shader shader;

	//�������� & ����ģ��
	Material surface;
	surface.setShader(&shader);
	Texture bodyTexture("./asset/model/planet/planet.png");
	surface.setTexture(&bodyTexture);
	//curMaterial = &surface;

	Model planet("./asset/model/planet/planet.obj");
	planet.setMaterial(0, surface);

	vector<vec3> pv =
	{
		vec3(-2.0f, -1.0f, -2.0f),
		vec3(-2.0f, -1.0f, 2.0f),
		vec3(2.0f, -1.0f, 2.0f),
		vec3(2.0f, -1.0f, -2.0f)
	};
	Mesh planeMesh = createPlane(pv, vec3(0.0f, 1.0f, 0.0f));
	Mesh boxMesh = createBox(vec3(0.0, 0.0, 0.0), 0.5);

	Material wood;
	wood.setShader(&shader);
	Texture boxTexture("./asset/texture/wood.png");
	wood.setTexture(&boxTexture);
	curMaterial = &surface;

	Object Box(boxMesh, wood);
	Object Plane(planeMesh, wood);

	//�����ƹ�
	dirLight = new PointLight(vec3(0.0f, 1.0f, 0.0f));

	//����͸��ͶӰ�����Լ��ӿڱ任����
	viewPortMatrix = getViewPortMat(width, height);

	//�任����
	camera = new Camera(
		vec3(0.0f, 2.0f, 4.0f),
		vec3(0.0f, 1.0f, 0.0f),
		45.0, (float)width / (float)height,
		1.0f, 20.f,
		-75.0f, -45.0f);
	

	renderer = new Render(width, height, win);


	//���PeekMessage����������0��˵��û�н��ܵ�WM_QUIT
	while (msg.message != WM_QUIT) {

		//����������
		cameraInput(camera);

		//����д�����Ϣ�ͽ��д���
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);	//���̰���ת�������������Ϣת��Ϊ�ַ���Ϣ
			DispatchMessage(&msg);	//����Ϣ���ɸ���Ӧ�Ĵ��ڹ���
		}
		//else�ｫ������Ⱦ���ݣ�û�д�����Ϣ��Ҫ����ʱ������Ⱦ
		else {
			//������һ֡����ǰ��Ҫ����
			renderer->clearBuffer();
			
			//���¾���
			viewMatrix = camera->getViewMatrix();
			projectMatrix = camera->getProjectMatrix();
			renderer->updateViewPlanes();

			//Render
			//renderer->drawModel(planet);
			renderer->drawObject(Plane);
			renderer->drawObject(Box);

			//�����ﻭ���豸�ϣ�hMem�൱�ڻ�����
			BitBlt(win->hDC, 0, 0, width, height, win->hMem, 0, 0, SRCCOPY);//�ú�����ָ����Դ�豸���������е����ؽ���λ�飨bit_block��ת�����Դ��͵�Ŀ���豸����
			if (fps < 144)
				fps++;
		}
	}
}

void showFPS(Window* w) 
{
	while (1) {
		Sleep(1000);

		wchar_t titleBuffer[30] = L"SoftRenderV0.1 fps: ";
		wchar_t frameCount[10];
		_itow_s(fps, frameCount, 10, 10);

		titleBuffer[20] += frameCount[0];
		titleBuffer[21] += frameCount[1];
		titleBuffer[22] += frameCount[2];

		//�ڴ��ڱ�����ʾ
		SetWindowText(win->hWnd, titleBuffer);
		//ÿһ������fps
		fps = 0;
	}
}