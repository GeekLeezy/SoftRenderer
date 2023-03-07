#include "Window.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	//消息处理
	switch (uMsg) {
		//当窗口被销毁时，终止消息循环
	case WM_DESTROY:
		PostQuitMessage(0);	//终止消息循环，并发出WM_QUIT消息，退出程序
		return 0;
	default:
		//将上面没有处理的消息转发给默认的窗口过程
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return 0;
}

Window::Window(const int& w, const int& h) : width(w), height(h)
{
	//注册窗口类，指定窗口类的名称与窗口回调函数
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);				  //该对象实际占用多大内存
	wcex.cbClsExtra = 0;							  //是否需要额外空间
	wcex.cbWndExtra = 0;							  //是否需要额外内存
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);  //如果有设置哪就是GDI擦除
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);		  //设置一个箭头光标
	wcex.hIcon = NULL;								  //应用程序在磁盘上显示的图标
	wcex.hIconSm = NULL;							  //应用程序显示在左上角的图标1
	wcex.hInstance = GetModuleHandle(NULL);						  //窗口实例
	wcex.lpszClassName = L"MySoftRender";			  //窗口名字
	wcex.lpszMenuName = NULL;
	wcex.style = CS_VREDRAW | CS_HREDRAW;			  //怎么绘制窗口垂直和水平重绘
	wcex.lpfnWndProc = WindowProc;					  //消息处理函数
	if (!RegisterClassEx(&wcex)) {
		MessageBox(NULL, L"注册窗口类失败！", L"Tip", NULL);
		//return NULL;
	}

	//创建窗口，指定注册窗口类，窗口标题，窗口的大小
	RECT Rect = { 0, 0, width, height };
	//客户矩形是指完全包含一个客户区域的最小矩形；窗口矩形是指完全包含一个窗口的最小矩形，该窗口包含客户区与非客户区。
	AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, false);	//根据窗口的客户区大小计算窗口的大小
	int newWidth = Rect.right - Rect.left;
	int newHeight = Rect.bottom - Rect.top;


	//声明窗口句柄
	hWnd = CreateWindowEx(
		NULL,//窗口额外风格
		L"MySoftRender",//窗口名称
		L"SoftRenderV1.0",//会显示在窗口标题栏上
		WS_OVERLAPPEDWINDOW,//窗口风格
		100, 100,//窗口的坐标
		width, height,
		NULL,//副窗口句柄
		NULL,//菜单句柄
		wcex.hInstance,//窗口实例
		NULL);
	if (!hWnd) {
		MessageBox(NULL, L"创建窗口失败！", L"Tip", NULL);
	}



	// 创建绘图所用的位图
	bmpBuffer = NULL;

	hDC = GetDC(hWnd);//显示
	hMem = CreateCompatibleDC(hDC);//绘画

	BITMAPINFO bmpInfo;
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFO);//这个结构的长度是固定的，为40个字节(LONG为32位整数)
	bmpInfo.bmiHeader.biWidth = width;         //指定图象的宽度，单位是像素
	bmpInfo.bmiHeader.biHeight = height;       //指定图象的高度，单位是像素
	bmpInfo.bmiHeader.biPlanes = 1;               //必须是1，不用考虑
	bmpInfo.bmiHeader.biBitCount = 32;            //指定表示颜色时要用到的位数，常用的值为1(黑白二色图), 4(16色图), 8(256色), 24(真彩色图)(新的.bmp格式支持32位色)
	bmpInfo.bmiHeader.biCompression = BI_RGB;     //实际存储方式为BGR

	bmpInfo.bmiHeader.biSizeImage = 0;      //指定实际的位图数据占用的字节数，其实也可以从以下的公式中计算出来 biSizeImage=biWidth’ × biHeight
						//要注意的是：上述公式中的biWidth’必须是4的整倍数(所以不是biWidth，而是biWidth’，表示大于或等于biWidth的，最接近4的整倍数。
						//举个例子，如果biWidth=240，则biWidth’=240；如果biWidth=241，biWidth’=244
						//如果biCompression为BI_RGB，则该项可能为零

	bmpInfo.bmiHeader.biXPelsPerMeter = 0;        //指定目标设备的水平分辨率，单位是每米的像素个数
	bmpInfo.bmiHeader.biYPelsPerMeter = 0;        //指定目标设备的垂直分辨率，单位同上
	bmpInfo.bmiHeader.biClrUsed = 0;              //指定本图像实际用到的颜色数，如果该值为零，则用到的颜色数为2biBitCount
	bmpInfo.bmiHeader.biClrImportant = 0;         //指定本图象中重要的颜色数，如果该值为零，则认为所有的颜色都是重要的

	//在这里创建bmpBuffer内存
	//CreateDIBSection函数会根据位图结构信息（pbmi）分配内存空间，你不用为它分配内存，这块内存也不需要你释放，系统会自己释放的。   
	//然后将位图中的图像数据读入这个内存地址，显示即可
	HBITMAP hBmp = CreateDIBSection(hDC, &bmpInfo, DIB_RGB_COLORS, (void**)&bmpBuffer, 0, 0);

	SelectObject(hMem, hBmp);//把一个对象(位图、画笔、画刷等)选入指定的设备描述表

	memset(bmpBuffer, 0, width * height * 4);//清空bmpBuffer为0，32位位图每个像素占4个字节



	//显示窗口，更新窗口
	ShowWindow(hWnd, SW_NORMAL);
	UpdateWindow(hWnd);
}

Window::~Window()
{
}
