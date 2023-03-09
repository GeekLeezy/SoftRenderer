#include "FrameBuffer.h"

/* 画点函数
 * 根据坐标为像素点着色
 */
void FrameBuffer::drawPoint(const int& x, const int& y, const vec4& color)
{
	if (x < 0 || x >= width || y < 0 || y >= height)
	{
		return;
	}
	unsigned char* p = colorBuffer;
	int pos = y * width + x;

	/*colorBuffer[pos * 4] = color.b;
	colorBuffer[pos * 4 + 1] = color.g;
	colorBuffer[pos * 4 + 2] = color.r;
	colorBuffer[pos * 4 + 3] = color.a;*/
	*(p + pos * 4) = saturate(color.b) * 255;
	*(p + pos * 4 + 1) = saturate(color.g) * 255;
	*(p + pos * 4 + 2) = saturate(color.r) * 255;
	*(p + pos * 4 + 3) = saturate(color.a) * 255;

}

//向深度缓存中写入更新后的深度
void FrameBuffer::writeDepth(const int& x, const int& y, const float& depth)
{
	if (x < 0 || x >= width || y < 0 || y >= height)
	{
		return;
	}

	int pos = y * width + x;
	depthBuffer[pos] = depth;
}

//获取深度图中的深度
float FrameBuffer::getDepth(const int& x, const int& y)
{
	if (x < 0 || x >= width || y < 0 || y >= height)
	{
		return 1.0f;
	}

	return depthBuffer[y * width + x];
}

//清理屏幕颜色缓存
void FrameBuffer::clearColorBuffer()
{
	if (colorBuffer != NULL)
	{
		memset(colorBuffer, 0, width * height * sizeof(unsigned char) * 4);
	}

	/*unsigned char* p = colorBuffer;
	for (int i = 0; i < width * height * 4; i += 4) {
		*(p + i) = 0;
		*(p + i + 1) = 0;
		*(p + i + 2) = 0;
		*(p + i + 3) = 0;
	}*/
}

void FrameBuffer::clearDepthBuffer()
{
	if (depthBuffer != NULL)
	{
		std::fill_n(depthBuffer, width * height, 1.0f);
		//memset(depthBuffer, 1.0, width * height * sizeof(float));
	}
}