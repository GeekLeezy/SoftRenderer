#pragma once
typedef unsigned char byte;

struct Color
{
	//由于实际显示的是BGRA，所以换一下顺序，让其符合RGBA
	byte blue;
	byte green;
	byte red;
	byte alpha;

	Color(byte r = 255, byte g = 255, byte b = 255, byte a = 255) {
		red = r;
		green = g;
		blue = b;
		alpha = a;
	}
};

struct Point {
	float x;
	float y;
	Color pColor;

	Point(float _x, float _y, Color color) {
		x = _x;
		y = _y;
		pColor = color;
	}
};

class Canvas
{
	public:
		int width;
		int height;
		Color* colorBuffer;

		Canvas(int w, int h, Color* color);
		void drawPoint(Point p);
		void drawLine(Point p1, Point p2);
		void clearColorBuffer();

};

//颜色插值
static inline Color ColorLerp(Color color1, Color color2, float scale) {
	Color color;
	color.red = (float)color1.red + (float)(color2.red - color1.red) * scale;
	color.green = (float)color1.green + (float)(color2.green - color1.green) * scale;
	color.blue = (float)color1.blue + (float)(color2.blue - color1.blue) * scale;
	color.alpha = (float)color1.alpha + (float)(color2.alpha - color1.alpha) * scale;
	return color;
}

