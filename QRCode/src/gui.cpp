#include "gui.h"

#include <iostream>
#include <array>
#include <string>
#include <fstream>

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

#include "opencv2/highgui.hpp"

Gui::Gui()
{
    /* Initialize the library */
    if (!glfwInit())
        std::cout << "ERROR:: Glfw initialization failed!" << std::endl;

    glfwWindowHint(GLFW_RESIZABLE, false);

    /* Create a windowed mode window and its OpenGL context */
    m_window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "QR code", NULL, NULL);

    if (!m_window)
    {
        glfwTerminate();
        std::cout << "ERROR:: Glfw window initialization failed!" << std::endl;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(m_window);

    // Turn Vsync ON so the laptop doesn't go BOOM
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Glew init Error!" << std::endl;

    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init();

    m_qrcode = QRCode();
    m_renderer = Renderer();
    m_shader = Shader();

    Setup();
}

Gui::~Gui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}

void Gui::Setup()
{
    std::vector<float> vertices = {
        -1.0f,  1.0f, 0.0f,  0.0f,  1.0f,
        -1.0f, -1.0f, 0.0f,  0.0f,  0.0f,
         1.0f, -1.0f, 0.0f,  1.0f,  0.0f,
         1.0f,  1.0f, 0.0f,  1.0f,  1.0f
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        2, 3, 0
    };

    m_renderer.BindBuffers(vertices, indices);
    m_shader.CompileShaders("res/shaders/shader.vs", "res/shaders/shader.fs");
}

void Gui::Draw()
{
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    SetImGuiStyle();

    DrawGeneratorPane();
    DrawScannerPane();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    DrawQRCode();

    /* Swap front and back buffers */
    glfwSwapBuffers(m_window);

    /* Poll for and process events */
    glfwPollEvents();
}

void Gui::DrawQRCode()
{
    // Change viewport to display the QR code to the right of the UI
    glViewport(SP_WIDTH, 0, W_WIDTH - SP_WIDTH, W_HEIGHT);

    // Map qrcode cv::Mat as a Texture
    GLuint texture_id;

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    // Set wrapping type
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    // Set filtering type
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (!m_qrcode.IsEmpty())
    {
        cv::Mat code;

        // OpenCV has a different coordinate system than OpenGL
        // So the image has to be flipped by Y
        cv::flip(m_qrcode.GetCode(), code, 0);

        // Resize code to fit the window for display
        cv::resize(code, code, cv::Size(W_HEIGHT, W_HEIGHT), 0, 0, cv::INTER_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, code.rows, code.cols, 0,
            GL_LUMINANCE, GL_UNSIGNED_BYTE, code.ptr());
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    m_shader.Use();

    glUniform1i(glGetUniformLocation(m_shader.GetShaderProgram(), "texture1"), 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    m_renderer.Draw();

    glUseProgram(0);

    glViewport(0, 0, W_WIDTH, W_HEIGHT);
}

void Gui::DrawGeneratorPane()
{
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(SP_WIDTH, W_HEIGHT / 2), ImGuiCond_Always);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15.0f, 10.0f));

    ImGui::Begin("Generator", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    ImGui::PopStyleVar();
    ImGui::PopStyleVar();

    ImGui::TextWrapped("Select the configuration for the QR code you want to generate.");
    ImGui::NewLine();

    const char* versions[] = {
        "M1", "M2", "M3", "M4", "1", "2",
        "3", "4", "5", "6", "7", "8", "9",
        "10", "11", "12", "13", "14", "15",
        "16", "17", "18", "19", "20", "21",
        "22", "23", "24", "25", "26", "27",
        "28", "29", "30", "31", "32", "33",
        "34", "35", "36", "37", "38", "39",
        "40"
    };

    ImGui::Text("QR code version");

    if (ImGui::BeginCombo("##combo", m_qrcode.m_version))
    {
        for (int n = 0; n < IM_ARRAYSIZE(versions); n++)
        {
            bool is_selected = (m_qrcode.m_version == versions[n]);
            if (ImGui::Selectable(versions[n], is_selected))
                m_qrcode.m_version = versions[n];
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::NewLine();

    const char* err_corr_levels[] = { "L (7%)", "M (15%)", "Q (25%)", "H (30%)" };

    ImGui::Text("Error correction level");

    if (ImGui::BeginCombo("##combo_level", m_qrcode.m_error_correction))
    {
        for (int n = 0; n < IM_ARRAYSIZE(err_corr_levels); n++)
        {
            bool is_selected = (m_qrcode.m_error_correction == err_corr_levels[n]);
            if (ImGui::Selectable(err_corr_levels[n], is_selected))
                m_qrcode.m_error_correction = err_corr_levels[n];
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::NewLine();

    const char* encodings[] = { "Numeric", "Alphanumeric", "Byte", "Kanji" };

    ImGui::Text("Encoding mode");

    if (ImGui::BeginCombo("##combo_encoding", m_qrcode.m_encoding))
    {
        for (int n = 0; n < IM_ARRAYSIZE(encodings); n++)
        {
            bool is_selected = (m_qrcode.m_encoding == encodings[n]);
            if (ImGui::Selectable(encodings[n], is_selected))
                m_qrcode.m_encoding = encodings[n];
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::NewLine();

    // The input size needs to be based on the version of the QR code
    ImGui::Text("Input");
    ImGui::InputText("##input", m_qrcode.m_input, IM_ARRAYSIZE(m_qrcode.m_input));

    ImGui::NewLine();

    if (ImGui::Button("Generate"))
    {
        std::cout << m_qrcode.m_input << std::endl;
        std::cout << "Generating QR code..." << std::endl;
        m_qrcode.GenerateCode();
    }

    ImGui::End();
}

void Gui::DrawScannerPane()
{
    ImGui::SetNextWindowPos(ImVec2(0.0f, W_HEIGHT / 2), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(SP_WIDTH, W_HEIGHT / 2), ImGuiCond_Always);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15.0f, 10.0f));

    ImGui::Begin("Scanner", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    ImGui::PopStyleVar();
    ImGui::PopStyleVar();

    ImGui::TextWrapped("Select where do you want to scan the QR code from.");
    ImGui::NewLine();

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

    if (ImGui::Button("Button"))
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}

void Gui::SetImGuiStyle()
{
    ImGuiStyle* style = &ImGui::GetStyle();

    style->ScrollbarRounding = 9.0f;
    style->GrabMinSize = 5.0f;
    style->GrabRounding = 3.0f;

    style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_WindowBg] = ImVec4(0.5f, 0.5f, 0.5f, 1.00f);
    style->Colors[ImGuiCol_PopupBg] = ImVec4(0.3f, 0.3f, 0.3f, 1.00f);
    style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
    style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
    style->Colors[ImGuiCol_FrameBg] = ImVec4(0.3f, 0.3f, 0.3f, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_TitleBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.0f, 1.0f, 1.0f, 0.75f);
    style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.2f, 0.2f, 0.2f, 1.00f);
    style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.2f, 0.2f, 0.2f, 1.00f);
    style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_Button] = ImVec4(0.2f, 0.2f, 0.2f, 1.00f);
    style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_Header] = ImVec4(0.2f, 0.2f, 0.2f, 1.00f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2f, 1.00f, 0.00f, 0.43f);
}

bool Gui::WindowShouldClose()
{
    return glfwWindowShouldClose(m_window);
}
