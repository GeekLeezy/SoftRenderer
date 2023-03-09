#pragma once

#include "Window.h"
#include "FrameBuffer.h"
#include "Camera.h"
#include "Clip.h"
#include "Mesh.h"
#include "Shader.h"
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
		Shader* shader;
		Texture* texture;
		vector<vec4> viewPlanes;

		RenderMode renderMod;

	public:
		Render(const int& w, const int& h, Window* win, const string& path) : width(w), height(h), shader(nullptr)
		{
			frontBuffer = new FrameBuffer(win->width, win->height, (unsigned char*)win->bmpBuffer);
			texture = new Texture(path);
			viewPlanes.resize(6, vec4(0.0));
		}

		void Init();

		void clearBuffer();

		~Render() {
			if (frontBuffer)
				delete frontBuffer;
			if (shader)
				delete shader;
			if (texture)
				delete texture;

			frontBuffer = nullptr;
			shader = nullptr;
			texture = nullptr;
		}


#pragma region stateMachine
		void changeRenderMode();
		void updateViewPlanes();
#pragma endregion stateMachine

#pragma region pipeLine


		//透视除法以及透视插值
		void perspectiveDivision(VerToFrag& v2f);

		/*
		 * 绘制网格
		 * 根据网格内的索引缓冲将三角形输入到顶点着色器
		 * 再对顶点着色器输出的中间体进行透视除法以及剔除
		 * 进行视口变换后绘制三角形
		 */
		void drawMesh(const Mesh& mesh);

#pragma endregion pipeLine
		
#pragma region Rasterization
		void drawTriangle(const VerToFrag& f1, const VerToFrag& f2, const VerToFrag& f3);

		/* 画线函数
		 * 使用Bresenham算法进行线段的绘制
		 */
		void drawLine(const VerToFrag& f1, const VerToFrag& f2);
#pragma endregion Rasterization

};

