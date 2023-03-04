#include "Window.h"
#include "Canvas.h"

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

			Color color1(0, 255, 0, 0);
			Color color2(0, 0, 255, 0);
			Color color3(255, 0, 0, 0);
			Point p1(100, 100, color1);
			Point p2(400, 500, color2);
			Point p3(400, 300, color3);
			//Point p4(100, 200, color1);
			//Point p5(200, 100, color2);
			//Point p6(600, 200, color1);
			canvas->drawLine(p1, p2);
			canvas->drawLine(p1, p3);
			//canvas->drawLine(p1, p4);
			//canvas->drawLine(p1, p5);
			//canvas->drawLine(p2, p6);

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