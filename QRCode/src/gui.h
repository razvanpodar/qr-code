#pragma once

#include <iostream>
#include <string>
#include <array>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

#include "qr_code.h"

constexpr int SP_WIDTH = 300;
constexpr int W_HEIGHT = 720;
constexpr int W_WIDTH = W_HEIGHT + SP_WIDTH;

class Gui
{
private:
	GLFWwindow* m_window;

	QRCode m_qrcode;

	float f = 0.5f;
	int v = 1;
	int counter = 0;

public:
	Gui();
	~Gui();

	void Draw();
	void DrawCode();
	bool WindowShouldClose();
};