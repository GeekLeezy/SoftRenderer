#include "Canvas.h"
#include <iostream>
#include <math.h>

Canvas::Canvas(int w, int h, Color* color) {
	width = w;
	height = h;
	colorBuffer = color;
}

void Canvas::drawPoint(Point p)
{
	if (p.x < 0 || p.x >= width || p.y < 0 || p.y >= height)
	{
		return;
	}
	int pos = (int)p.y * width + (int)p.x;
	colorBuffer[pos] = p.pColor;
}

void Canvas::drawLine(Point p1, Point p2) 
{
	//计算△x和△y
	int deltaX = abs(p2.x - p1.x);
	int deltaY = abs(p2.y - p1.y);

	//初始化步长
	int stepX = 0;
	int stepY = 0;

	Point p = p1;

	//判断步进方向
	if (p2.x > p1.x) stepX = 1;
	else stepX = -1;

	if (p2.y > p1.y) stepY = 1;
	else stepY = -1;

	//判断斜率，如果斜率小于1大于0，我们就选x轴为主步进轴
	//如果斜率大于1，我们就选y轴为主步进轴
	int pk = 0;//决策参数
	bool useX = true;
	int sumStep = 0;//需要步进的次数
	//斜率大于1
	if (deltaY > deltaX) {
		useX = false;
		sumStep = deltaY;
		std::swap(deltaX, deltaY);
	}
	else {
		useX = true;
		sumStep = deltaX;
	}
	pk = 2 * deltaY - deltaX;

	for (int i = 0; i < sumStep; i++) {
		drawPoint(p);

		//颜色插值
		float scale = 0.0f;
		Color color;


		if (pk > 0) {
			if (useX) {
				p.y += stepY;
			}
			else {
				p.x += stepX;
			}

			//只有pk>0时才有这一项
			pk = pk - 2 * deltaX;
		}

		//主坐标不管pk是何值都要步进
		if (useX) {
			p.x += stepX;
		}
		else {
			p.y += stepY;
		}

		//无论pk为何值都有这一项
		pk = pk + 2 * deltaY;

		if (useX) {
			if (p2.x == p1.x) {//此时两个点重合
				scale = 0.0f;
			}
			else {
				scale = (p.x - p1.x) / (p2.x - p1.x);
			}
		}
		else {
			if (p2.y == p1.y) {//此时两个点重合
				scale = 0.0f;
			}
			else {
				scale = (p.y - p1.y) / (p2.y - p1.y);
			}
		}
		color = ColorLerp(p1.pColor, p2.pColor, scale);
		p.pColor = color;
	}


}

void Canvas::clearColorBuffer()
{
	if (colorBuffer != NULL)
	{
		memset(colorBuffer, 0, width * height * sizeof(Color));
	}
}