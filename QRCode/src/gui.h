#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "qr_code.h"
#include "renderer.h"
#include "shader.h"

constexpr int SP_WIDTH = 300;
constexpr int W_HEIGHT = 720;
constexpr int W_WIDTH = W_HEIGHT + SP_WIDTH;

class Gui
{
private:
	GLFWwindow* m_window;
	Renderer m_renderer;
	Shader m_shader;
	QRCode m_qrcode;

	float f = 0.5f;
	int v = 1;
	int counter = 0;

	void Setup();

public:
	Gui();
	~Gui();

	void Draw();
	void DrawQRCode();
	void DrawGeneratorPane();
	void DrawScannerPane();
	void SetImGuiStyle();

	bool WindowShouldClose();
};