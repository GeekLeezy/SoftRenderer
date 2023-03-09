#pragma once
#include "Math.h"

class FrameBuffer
{
	public:
		int width;
		int height;
		unsigned char* colorBuffer;
		float* depthBuffer;

		FrameBuffer(const int& w, const int& h, unsigned char* color)
		{
			width = w;
			height = h;
			colorBuffer = color;
			depthBuffer = new float[w * h];
			std::fill_n(depthBuffer, w * h, 1.0f);
		}

		void drawPoint(const int& x, const int& y, const vec4& color);
		void writeDepth(const int& x, const int& y, const float& depth);
		float getDepth(const int& x, const int& y);

		void clearColorBuffer();
		void clearDepthBuffer();

		~FrameBuffer() = default;
};

