#pragma once

#include <iostream>
#include <string>
#include <array>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

constexpr int SP_WIDTH = 300;
constexpr int W_HEIGHT = 720;
constexpr int W_WIDTH = W_HEIGHT + SP_WIDTH;

class Gui
{
private:
	GLFWwindow* m_window;

	const char* m_version = "M1";
	const char* m_error_correction = "L";
	const char* m_encoding = "Alphanumeric";
	char m_input[50] = { 0 };

	float f = 0.5f;
	int v = 1;
	int counter = 0;

public:
	Gui();
	~Gui();

	void Draw();
	bool WindowShouldClose();
};