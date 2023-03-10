#pragma once

#include "Window.h"
#include "FrameBuffer.h"
#include "Camera.h"
#include "Clip.h"
#include "Mesh.h"
#include "Shader.h"
#include "Model.h"
#include "Cull.h"

using namespace std;

enum RenderMode {
	Line,
	Fill
};

class Render
{
	private:
		int width;
		int height;
		FrameBuffer* frontBuffer;

		vector<vec4> viewPlanes;

		RenderMode renderMod;

	public:
		Render(const int& w, const int& h, Window* win) : width(w), height(h)
		{
			frontBuffer = new FrameBuffer(win->width, win->height, (unsigned char*)win->bmpBuffer, false);
			viewPlanes.resize(6, vec4(0.0));
		}

		void clearBuffer();

		~Render() {
			if (frontBuffer)
				delete frontBuffer;

			frontBuffer = nullptr;
		}


#pragma region stateMachine
		void changeRenderMode();
		void updateViewPlanes();
#pragma endregion stateMachine

#pragma region pipeLine


		//͸�ӳ����Լ�͸�Ӳ�ֵ
		void perspectiveDivision(VerToFrag& v2f);

		/*
		 * ��������
		 * ���������ڵ��������彫���������뵽������ɫ��
		 * �ٶԶ�����ɫ��������м������͸�ӳ����Լ��޳�
		 * �����ӿڱ任�����������
		 */
		void drawObject(Object& obj);

		void drawModel(Model& model);

#pragma endregion pipeLine
		
#pragma region Rasterization
		void MSAA(const VerToFrag& f1, const VerToFrag& f2, const VerToFrag& f3, const int& x, const int& y);

		void drawTriangle(const VerToFrag& f1, const VerToFrag& f2, const VerToFrag& f3);

		/* ���ߺ���
		 * ʹ��Bresenham�㷨�����߶εĻ���
		 */
		void drawLine(const VerToFrag& f1, const VerToFrag& f2);
#pragma endregion Rasterization

};

