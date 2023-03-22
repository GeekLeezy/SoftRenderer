#pragma once

#include "Window.h"
#include "FrameBuffer.h"
#include "Camera.h"
#include "Clip.h"
#include "Mesh.h"
#include "Shader.h"
#include "Model.h"
#include "Cull.h"
#include "Light.h"

using namespace std;


extern Light* dirLight;

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
		FrameBuffer* shadowBuffer;
		ShadowShader* shadowShader;

		vector<vec4> viewPlanes;
		RenderMode renderMod;

	public:
		Render(const int& w, const int& h, Window* win) : width(w), height(h)
		{
			frontBuffer = new FrameBuffer(win->width, win->height, (unsigned char*)win->bmpBuffer, false);
			shadowBuffer = new FrameBuffer(win->width, win->height);

			

			shadowShader = new ShadowShader();

			viewPlanes.resize(6, vec4(0.0));
			renderMod = Fill;
		}

		void clearBuffer();

		~Render() {
			if (frontBuffer)
				delete frontBuffer;
			if (shadowBuffer)
				delete shadowBuffer;
			if (shadowShader)
				delete shadowShader;


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

		void drawShadowBuffer(Object& obj);

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

