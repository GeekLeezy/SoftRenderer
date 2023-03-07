#include "Canvas.h"

Canvas::Canvas(const int& w, const int& h, Color* color) {
	width = w;
	height = h;
	colorBuffer = color;
}

/* ���㺯��
 * ��������Ϊ���ص���ɫ
 */
void Canvas::drawPoint(const VerToFrag &v2f)
{
	vec4 p = v2f.fragPos;
	if (p.x < 0 || p.x >= width || p.y < 0 || p.y >= height)
	{
		return;
	}
	int pos = (int)p.y * width + (int)p.x;
	colorBuffer[pos] = v2f.color;
}

/* ���ߺ���
 * ʹ��Bresenham�㷨�����߶εĻ���
 */
void Canvas::drawLine(const VerToFrag& f1, const VerToFrag& f2)
{

	vec4 p1 = f1.fragPos;
	vec4 p2 = f2.fragPos;

	//�����x�͡�y
	int deltaX = abs(p2.x - p1.x);
	int deltaY = abs(p2.y - p1.y);

	//��ʼ������
	int stepX = 0;
	int stepY = 0;

	VerToFrag f = f1;
	vec4 p= f.fragPos;

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

	for (int i = 0; i <= sumStep; i++) {
		drawPoint(f);

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
			pk -= 2 * deltaX;
		}

		//�����겻��pk�Ǻ�ֵ��Ҫ����
		if (useX) {
			p.x += stepX;
		}
		else {
			p.y += stepY;
		}

		//����pkΪ��ֵ������һ��
		pk += 2 * deltaY;

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
		color = lerp(f1, f2, scale).color;
		f.fragPos.x = p.x;
		f.fragPos.y = p.y;
		f.color = color;
	}


}

//��Χ��-�������
void Canvas::drawTriangle(const VerToFrag& f1, const VerToFrag& f2, const VerToFrag& f3)
{
	vec4 p1 = f1.fragPos;
	vec4 p2 = f2.fragPos;
	vec4 p3 = f3.fragPos;

	//������Χ��
	int max_x = max(p1.x, max(p2.x, p3.x));
	int min_x = min(p1.x, min(p2.x, p3.x));
	int max_y = max(p1.y, max(p2.y, p3.y));
	int min_y = min(p1.y, min(p2.y, p3.y));

	//����жϵ��Ƿ����������ڲ�
	for (auto i = min_x; i <= max_x; i++)
	{
		for (auto j = min_y; j <= max_y; j++)
		{
			float r1 = (i - p1.x) * (p2.y - p1.y) - (j - p1.y) * (p2.x - p1.x);
			float r2 = (i - p2.x) * (p3.y - p2.y) - (j - p2.y) * (p3.x - p2.x);
			float r3 = (i - p3.x) * (p1.y - p3.y) - (j - p3.y) * (p1.x - p3.x);

			if (r1 >= 0 && r2 >= 0 && r3 >= 0)
			{
				VerToFrag f = barycenterLerp(f1, f2, f3, vec2(i, j));
				drawPoint(f);
			}
				
			else if (r1 <= 0 && r2 <= 0 && r3 <= 0)
			{
				VerToFrag f = barycenterLerp(f1, f2, f3, vec2(i, j));
				drawPoint(f);
			}
			else
				continue;
		}
	}
}

//ɨ���߷���ƽ��ƽ�������Σ�p0Ϊyֵ�����������㲻ͬ�ĵ�
void Canvas::drawTriangleFlat(const VerToFrag& f1, const VerToFrag& f2, const VerToFrag& f0)
{
	vec4 p1 = f1.fragPos;
	vec4 p2 = f2.fragPos;
	vec4 p0 = f0.fragPos;

	float k1 = FLT_MAX;
	float k2 = FLT_MAX;
	float b1 = 0.0f;
	float b2 = 0.0f;

	//��ʼ����ֵ��ɫ
	VerToFrag begin1;
	VerToFrag begin2;
	VerToFrag end1;
	VerToFrag end2;

	if (p1.x != p0.x) {
		k1 = (p1.y - p0.y) / (p1.x - p0.x);
		b1 = p0.y - k1 * p0.x;
	}
	if (p2.x != p0.x) {
		k2 = (p2.y - p0.y) / (p2.x - p0.x);
		b2 = p0.y - k2 * p0.x;
	}

	float min_y = min(p1.y, p0.y);
	float max_y = max(p1.y, p0.y);

	//������ɫ��ֵ��yֵ��С����
	if (p0.y < p1.y) {
		begin1 = f0;
		begin2 = f0;
		end1 = f1;
		end2 = f2;
	}
	else {
		begin1 = f1;
		begin2 = f2;
		end1 = f0;
		end2 = f0;
	}

	for (auto y = min_y; y <= max_y; y++)
	{
		float scale = (y - min_y) / (max_y - min_y);
		VerToFrag pt1 = lerp(begin1, end1, scale);
		VerToFrag pt2 = lerp(begin2, end2, scale);

		drawLine(pt1, pt2);
	}
}

//ɨ���߷�������������
void Canvas::drawTriangle2(const VerToFrag& f1, const VerToFrag& f2, const VerToFrag& f3)
{
	//�Ӵ�С����
	std::vector<VerToFrag> pv{ f1, f2, f3 };
	std::sort(pv.begin(), pv.end(), yOrderSort);

	//����������ƽ��ƽ�������Σ�ֱ�ӻ���
	if (pv[0].fragPos.y == pv[1].fragPos.y)
	{
		drawTriangleFlat(pv[0], pv[1], pv[2]);
		return;
	}
	if (pv[1].fragPos.y == pv[2].fragPos.y)
	{
		drawTriangleFlat(pv[2], pv[1], pv[0]);
		return;
	}

	float scale = (pv[1].fragPos.y - pv[2].fragPos.y) / (pv[0].fragPos.y - pv[2].fragPos.y);
	VerToFrag pm = lerp(pv[2], pv[0], scale);

	drawTriangleFlat(pm, pv[1], pv[0]);
	drawTriangleFlat(pm, pv[1], pv[2]);
}

//������Ļ��ɫ����
void Canvas::clearColorBuffer()
{
	if (colorBuffer != NULL)
	{
		memset(colorBuffer, 0, width * height * sizeof(Color));
	}
}