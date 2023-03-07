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
	//��ʼ��Ϣѭ��
	//������Ϣ�ṹ��
	MSG msg = { 0 };
	float z = 3.0f;
	//���PeekMessage����������0��˵��û�н��ܵ�WM_QUIT
	while (msg.message != WM_QUIT) {
		//����д�����Ϣ�ͽ��д���
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);	//���̰���ת�������������Ϣת��Ϊ�ַ���Ϣ
			DispatchMessage(&msg);	//����Ϣ���ɸ���Ӧ�Ĵ��ڹ���
		}
		//else�ｫ������Ⱦ���ݣ�û�д�����Ϣ��Ҫ����ʱ������Ⱦ
		else {
			//������һ֡����ǰ��Ҫ����
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

			//�任����
			Camera camera(vec3(0.5f, 2.0f, 3.0f + sin(z)), vec3(0.0f, 1.0f, 0.0f), -90.0f, -30.0f);
			z += 0.01f;
			mat4 viewMat = camera.getViewMatrix();
			mat4 projectionMat = getProjectMat(glm::radians(45.0f), (float)width / (float)height, 1.0f, 50.0f);
			mat4 viewPortMat = getViewPortMat(width, height);

			//������ɫ��
			Shader shader;
			shader.setViewMat(viewMat);
			shader.setProjectMat(projectionMat);

			std::vector<VerToFrag> pixelPoints;
			for (auto i = 0; i < cubePoints.size(); i++)
			{
				VerToFrag v2f;
				v2f = shader.vertexShader(cubePoints[i]);
				
				//�ü�����


				//͸�ӳ���
				v2f.fragPos /= v2f.fragPos.w;

				//�ӿڱ任
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

			//�����ﻭ���豸�ϣ�hMem�൱�ڻ�����
			BitBlt(win->hDC, 0, 0, width, height, win->hMem, 0, 0, SRCCOPY);//�ú�����ָ����Դ�豸���������е����ؽ���λ�飨bit_block��ת�����Դ��͵�Ŀ���豸����
		}
	}
	
}

// WinMain��һ���������ú����Ĺ����Ǳ�ϵͳ���ã���Ϊһ��32λӦ�ó������ڵ�
// hInstance��Ӧ�ó���ǰʵ���ľ�����������Ϊָ��ǰʵ����ָ��
// prevInstance��Ӧ�ó������ǰʵ���ľ��������ͬһ����������Σ������������ڵ�һ�δ򿪵Ĵ��ھ�����ǰʵ���Ĵ��ڡ�����һ��32λ���򣬸ò�����ΪNULL
// cmdLine��ָ��Ӧ�ó��������е��ַ�����ָ�룬������ִ���ļ���
// nCmdShow��ָ�����������ʾ
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int nCmdShow)
int main()
{
	Window* win = new Window(width, height);

	canvas = new Canvas(win->width, win->height, (Color*)win->bmpBuffer);

	Render(win);

	delete canvas;

	return 0;
}