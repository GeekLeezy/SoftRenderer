#include "Window.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	//��Ϣ����
	switch (uMsg) {
		//�����ڱ�����ʱ����ֹ��Ϣѭ��
	case WM_DESTROY:
		PostQuitMessage(0);	//��ֹ��Ϣѭ����������WM_QUIT��Ϣ���˳�����
		return 0;
	default:
		//������û�д������Ϣת����Ĭ�ϵĴ��ڹ���
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return 0;
}

Window::Window(const int& w, const int& h) : width(w), height(h)
{
	//ע�ᴰ���ָ࣬��������������봰�ڻص�����
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);				  //�ö���ʵ��ռ�ö���ڴ�
	wcex.cbClsExtra = 0;							  //�Ƿ���Ҫ����ռ�
	wcex.cbWndExtra = 0;							  //�Ƿ���Ҫ�����ڴ�
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);  //����������ľ���GDI����
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);		  //����һ����ͷ���
	wcex.hIcon = NULL;								  //Ӧ�ó����ڴ�������ʾ��ͼ��
	wcex.hIconSm = NULL;							  //Ӧ�ó�����ʾ�����Ͻǵ�ͼ��1
	wcex.hInstance = GetModuleHandle(NULL);						  //����ʵ��
	wcex.lpszClassName = L"MySoftRender";			  //��������
	wcex.lpszMenuName = NULL;
	wcex.style = CS_VREDRAW | CS_HREDRAW;			  //��ô���ƴ��ڴ�ֱ��ˮƽ�ػ�
	wcex.lpfnWndProc = WindowProc;					  //��Ϣ������
	if (!RegisterClassEx(&wcex)) {
		MessageBox(NULL, L"ע�ᴰ����ʧ�ܣ�", L"Tip", NULL);
		//return NULL;
	}

	//�������ڣ�ָ��ע�ᴰ���࣬���ڱ��⣬���ڵĴ�С
	RECT Rect = { 0, 0, width, height };
	//�ͻ�������ָ��ȫ����һ���ͻ��������С���Σ����ھ�����ָ��ȫ����һ�����ڵ���С���Σ��ô��ڰ����ͻ�����ǿͻ�����
	AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, false);	//���ݴ��ڵĿͻ�����С���㴰�ڵĴ�С
	int newWidth = Rect.right - Rect.left;
	int newHeight = Rect.bottom - Rect.top;


	//�������ھ��
	hWnd = CreateWindowEx(
		NULL,//���ڶ�����
		L"MySoftRender",//��������
		L"SoftRenderV1.0",//����ʾ�ڴ��ڱ�������
		WS_OVERLAPPEDWINDOW,//���ڷ��
		100, 100,//���ڵ�����
		width, height,
		NULL,//�����ھ��
		NULL,//�˵����
		wcex.hInstance,//����ʵ��
		NULL);
	if (!hWnd) {
		MessageBox(NULL, L"��������ʧ�ܣ�", L"Tip", NULL);
	}



	// ������ͼ���õ�λͼ
	bmpBuffer = NULL;

	hDC = GetDC(hWnd);//��ʾ
	hMem = CreateCompatibleDC(hDC);//�滭

	BITMAPINFO bmpInfo;
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFO);//����ṹ�ĳ����ǹ̶��ģ�Ϊ40���ֽ�(LONGΪ32λ����)
	bmpInfo.bmiHeader.biWidth = width;         //ָ��ͼ��Ŀ�ȣ���λ������
	bmpInfo.bmiHeader.biHeight = height;       //ָ��ͼ��ĸ߶ȣ���λ������
	bmpInfo.bmiHeader.biPlanes = 1;               //������1�����ÿ���
	bmpInfo.bmiHeader.biBitCount = 32;            //ָ����ʾ��ɫʱҪ�õ���λ�������õ�ֵΪ1(�ڰ׶�ɫͼ), 4(16ɫͼ), 8(256ɫ), 24(���ɫͼ)(�µ�.bmp��ʽ֧��32λɫ)
	bmpInfo.bmiHeader.biCompression = BI_RGB;     //ʵ�ʴ洢��ʽΪBGR

	bmpInfo.bmiHeader.biSizeImage = 0;      //ָ��ʵ�ʵ�λͼ����ռ�õ��ֽ�������ʵҲ���Դ����µĹ�ʽ�м������ biSizeImage=biWidth�� �� biHeight
						//Ҫע����ǣ�������ʽ�е�biWidth��������4��������(���Բ���biWidth������biWidth������ʾ���ڻ����biWidth�ģ���ӽ�4����������
						//�ٸ����ӣ����biWidth=240����biWidth��=240�����biWidth=241��biWidth��=244
						//���biCompressionΪBI_RGB����������Ϊ��

	bmpInfo.bmiHeader.biXPelsPerMeter = 0;        //ָ��Ŀ���豸��ˮƽ�ֱ��ʣ���λ��ÿ�׵����ظ���
	bmpInfo.bmiHeader.biYPelsPerMeter = 0;        //ָ��Ŀ���豸�Ĵ�ֱ�ֱ��ʣ���λͬ��
	bmpInfo.bmiHeader.biClrUsed = 0;              //ָ����ͼ��ʵ���õ�����ɫ���������ֵΪ�㣬���õ�����ɫ��Ϊ2biBitCount
	bmpInfo.bmiHeader.biClrImportant = 0;         //ָ����ͼ������Ҫ����ɫ���������ֵΪ�㣬����Ϊ���е���ɫ������Ҫ��

	//�����ﴴ��bmpBuffer�ڴ�
	//CreateDIBSection���������λͼ�ṹ��Ϣ��pbmi�������ڴ�ռ䣬�㲻��Ϊ�������ڴ棬����ڴ�Ҳ����Ҫ���ͷţ�ϵͳ���Լ��ͷŵġ�   
	//Ȼ��λͼ�е�ͼ�����ݶ�������ڴ��ַ����ʾ����
	HBITMAP hBmp = CreateDIBSection(hDC, &bmpInfo, DIB_RGB_COLORS, (void**)&bmpBuffer, 0, 0);

	SelectObject(hMem, hBmp);//��һ������(λͼ�����ʡ���ˢ��)ѡ��ָ�����豸������

	memset(bmpBuffer, 0, width * height * 4);//���bmpBufferΪ0��32λλͼÿ������ռ4���ֽ�



	//��ʾ���ڣ����´���
	ShowWindow(hWnd, SW_NORMAL);
	UpdateWindow(hWnd);
}

Window::~Window()
{
}
