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


// WinMain是一个函数，该函数的功能是被系统调用，作为一个32位应用程序的入口点
// hInstance：应用程序当前实例的句柄，可以理解为指向当前实例的指针
// prevInstance：应用程序的先前实例的句柄。对于同一个程序打开两次，出现两个窗口第一次打开的窗口就是先前实例的窗口。对于一个32位程序，该参数总为NULL
// cmdLine：指向应用程序命令行的字符串的指针，不包括执行文件名
// nCmdShow：指明窗口如何显示
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
	//开始消息循环
	//定义消息结构体
	MSG msg = { 0 };

	//创建着色器
	Shader shader;

	//创建场景 & 加载模型
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

	//创建灯光
	dirLight = new PointLight(vec3(0.0f, 1.0f, 0.0f));

	//计算透视投影矩阵以及视口变换矩阵
	viewPortMatrix = getViewPortMat(width, height);

	//变换矩阵
	camera = new Camera(
		vec3(0.0f, 2.0f, 4.0f),
		vec3(0.0f, 1.0f, 0.0f),
		45.0, (float)width / (float)height,
		1.0f, 20.f,
		-75.0f, -45.0f);
	

	renderer = new Render(width, height, win);


	//如果PeekMessage函数不等于0，说明没有接受到WM_QUIT
	while (msg.message != WM_QUIT) {

		//处理按键输入
		cameraInput(camera);

		//如果有窗口消息就进行处理
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);	//键盘按键转换，将虚拟键消息转换为字符消息
			DispatchMessage(&msg);	//把消息分派给相应的窗口过程
		}
		//else里将放置渲染内容，没有窗口信息需要处理时进行渲染
		else {
			//进行下一帧绘制前需要清屏
			renderer->clearBuffer();
			
			//更新矩阵
			viewMatrix = camera->getViewMatrix();
			projectMatrix = camera->getProjectMatrix();
			renderer->updateViewPlanes();

			//Render
			//renderer->drawModel(planet);
			renderer->drawObject(Plane);
			renderer->drawObject(Box);

			//在这里画到设备上，hMem相当于缓冲区
			BitBlt(win->hDC, 0, 0, width, height, win->hMem, 0, 0, SRCCOPY);//该函数对指定的源设备环境区域中的像素进行位块（bit_block）转换，以传送到目标设备环境
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

		//在窗口标题显示
		SetWindowText(win->hWnd, titleBuffer);
		//每一秒重置fps
		fps = 0;
	}
}