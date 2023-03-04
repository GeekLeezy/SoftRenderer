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
	//开始消息循环
	//定义消息结构体
	MSG msg = { 0 };
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