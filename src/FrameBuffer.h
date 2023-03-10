#pragma once
#include "Math.h"

class FrameBuffer
{
	public:
		int width;
		int height;
		unsigned char* colorBuffer;
		float* depthBuffer;

		//MSAA
		bool MSAA;
		float* superColorBuffer;
		float* superDepthBuffer;

		FrameBuffer(const int& w, const int& h, unsigned char* color, bool antiAlias)
		{
			width = w;
			height = h;
			colorBuffer = color;
			depthBuffer = new float[w * h];
			std::fill_n(depthBuffer, w * h, 1.0f);
			MSAA = antiAlias;
			superColorBuffer = nullptr;
			superDepthBuffer = nullptr;

			if (MSAA)
			{
				superColorBuffer = new float[w * h * 4 * 4];
				superDepthBuffer = new float[w * h * 4];
				std::fill_n(superColorBuffer, w * h * 4 * 4, 0.0f);
				std::fill_n(superDepthBuffer, w * h * 4, 1.0f);
			}
			
		}

		void drawPoint(const int& x, const int& y, const vec4& color);
		void writeDepth(const int& x, const int& y, const float& depth);
		float getDepth(const int& x, const int& y);

		void clearColorBuffer();
		void clearDepthBuffer();

		~FrameBuffer() = default;
};

