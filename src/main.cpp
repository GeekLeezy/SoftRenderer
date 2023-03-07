#include "Window.h"
#include "Canvas.h"
#include "Camera.h"
#include "Utils.h"

#include <glm/gtc/matrix_transform.hpp>
#include <vector>

const int width = 800;
const int height = 600;

//HWND hWnd;
//HDC hDC;
//HDC hMem;

Canvas* canvas = NULL;

void Render(Window *win) 
{
	//开始消息循环
	//定义消息结构体
	MSG msg = { 0 };
	float z = 3.0f;
	//如果PeekMessage函数不等于0，说明没有接受到WM_QUIT
	while (msg.message != WM_QUIT) {
		//如果有窗口消息就进行处理
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);	//键盘按键转换，将虚拟键消息转换为字符消息
			DispatchMessage(&msg);	//把消息分派给相应的窗口过程
		}
		//else里将放置渲染内容，没有窗口信息需要处理时进行渲染
		else {
			//进行下一帧绘制前需要清屏
			canvas->clearColorBuffer();

			std::vector<Color> cubeColor;
			cubeColor.push_back(Color(255, 0, 0, 0));
			cubeColor.push_back(Color(0, 255, 0, 0));
			cubeColor.push_back(Color(0, 0, 255, 0));
			

			std::vector<Vertex> cubePoints;
			cubePoints.push_back(Vertex(vec4(0.0f, 0.0f, 0.0f, 1.0f), cubeColor[0]));
			cubePoints.push_back(Vertex(vec4(1.0f, 0.0f, 0.0f, 1.0f), cubeColor[1]));
			cubePoints.push_back(Vertex(vec4(0.0f, 1.0f, 0.0f, 1.0f), cubeColor[2]));
			cubePoints.push_back(Vertex(vec4(1.0f, 1.0f, 0.0f, 1.0f), cubeColor[0]));
			cubePoints.push_back(Vertex(vec4(0.0f, 1.0f, -1.0f, 1.0f), cubeColor[1]));
			cubePoints.push_back(Vertex(vec4(1.0f, 1.0f, -1.0f, 1.0f), cubeColor[2]));
			cubePoints.push_back(Vertex(vec4(0.0f, 0.0f, -1.0f, 1.0f), cubeColor[0]));
			cubePoints.push_back(Vertex(vec4(1.0f, 0.0f, -1.0f, 1.0f), cubeColor[1]));

			//变换矩阵
			Camera camera(vec3(0.5f, 2.0f, 3.0f + sin(z)), vec3(0.0f, 1.0f, 0.0f), -90.0f, -30.0f);
			z += 0.01f;
			mat4 viewMat = camera.getViewMatrix();
			mat4 projectionMat = getProjectMat(glm::radians(45.0f), (float)width / (float)height, 1.0f, 50.0f);
			mat4 viewPortMat = getViewPortMat(width, height);

			//创建着色器
			Shader shader;
			shader.setViewMat(viewMat);
			shader.setProjectMat(projectionMat);

			std::vector<VerToFrag> pixelPoints;
			for (auto i = 0; i < cubePoints.size(); i++)
			{
				VerToFrag v2f;
				v2f = shader.vertexShader(cubePoints[i]);
				
				//裁剪操作


				//透视除法
				v2f.fragPos /= v2f.fragPos.w;

				//视口变换
				v2f.fragPos = viewPortMat * v2f.fragPos;
				pixelPoints.push_back(v2f);
			}

			
			/*canvas->drawTriangle(pixelPoints[5], pixelPoints[1], pixelPoints[7]);
			canvas->drawTriangle(pixelPoints[5], pixelPoints[3], pixelPoints[1]);
			canvas->drawTriangle(pixelPoints[5], pixelPoints[4], pixelPoints[7]);
			canvas->drawTriangle(pixelPoints[4], pixelPoints[6], pixelPoints[7]);
			canvas->drawTriangle(pixelPoints[4], pixelPoints[6], pixelPoints[0]);
			canvas->drawTriangle(pixelPoints[4], pixelPoints[2], pixelPoints[0]);

			canvas->drawTriangle(pixelPoints[4], pixelPoints[2], pixelPoints[5]);
			canvas->drawTriangle(pixelPoints[2], pixelPoints[3], pixelPoints[5]);
			canvas->drawTriangle(pixelPoints[0], pixelPoints[6], pixelPoints[7]);
			canvas->drawTriangle(pixelPoints[0], pixelPoints[1], pixelPoints[7]);
			canvas->drawTriangle(pixelPoints[2], pixelPoints[3], pixelPoints[1]);
			canvas->drawTriangle(pixelPoints[0], pixelPoints[1], pixelPoints[2]);*/

			canvas->drawTriangle2(pixelPoints[1], pixelPoints[2], pixelPoints[3]);
			canvas->drawTriangle2(pixelPoints[0], pixelPoints[1], pixelPoints[2]);

			//在这里画到设备上，hMem相当于缓冲区
			BitBlt(win->hDC, 0, 0, width, height, win->hMem, 0, 0, SRCCOPY);//该函数对指定的源设备环境区域中的像素进行位块（bit_block）转换，以传送到目标设备环境
		}
	}
	
}

// WinMain是一个函数，该函数的功能是被系统调用，作为一个32位应用程序的入口点
// hInstance：应用程序当前实例的句柄，可以理解为指向当前实例的指针
// prevInstance：应用程序的先前实例的句柄。对于同一个程序打开两次，出现两个窗口第一次打开的窗口就是先前实例的窗口。对于一个32位程序，该参数总为NULL
// cmdLine：指向应用程序命令行的字符串的指针，不包括执行文件名
// nCmdShow：指明窗口如何显示
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int nCmdShow)
int main()
{
	Window* win = new Window(width, height);

	canvas = new Canvas(win->width, win->height, (Color*)win->bmpBuffer);

	Render(win);

	delete canvas;

	return 0;
}