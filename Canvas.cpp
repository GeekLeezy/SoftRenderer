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
	//�����x�͡�y
	int deltaX = abs(p2.x - p1.x);
	int deltaY = abs(p2.y - p1.y);

	//��ʼ������
	int stepX = 0;
	int stepY = 0;

	Point p = p1;

	//�жϲ�������
	if (p2.x > p1.x) stepX = 1;
	else stepX = -1;

	if (p2.y > p1.y) stepY = 1;
	else stepY = -1;

	//�ж�б�ʣ����б��С��1����0�����Ǿ�ѡx��Ϊ��������
	//���б�ʴ���1�����Ǿ�ѡy��Ϊ��������
	int pk = 0;//���߲���
	bool useX = true;
	int sumStep = 0;//��Ҫ�����Ĵ���
	//б�ʴ���1
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

		//��ɫ��ֵ
		float scale = 0.0f;
		Color color;


		if (pk > 0) {
			if (useX) {
				p.y += stepY;
			}
			else {
				p.x += stepX;
			}

			//ֻ��pk>0ʱ������һ��
			pk = pk - 2 * deltaX;
		}

		//�����겻��pk�Ǻ�ֵ��Ҫ����
		if (useX) {
			p.x += stepX;
		}
		else {
			p.y += stepY;
		}

		//����pkΪ��ֵ������һ��
		pk = pk + 2 * deltaY;

		if (useX) {
			if (p2.x == p1.x) {//��ʱ�������غ�
				scale = 0.0f;
			}
			else {
				scale = (p.x - p1.x) / (p2.x - p1.x);
			}
		}
		else {
			if (p2.y == p1.y) {//��ʱ�������غ�
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