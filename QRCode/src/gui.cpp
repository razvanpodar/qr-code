#include "gui.h"

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

    if (glewInit() != GLEW_OK)
        std::cout << "Glew init Error!" << std::endl;

    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init();
}

Gui::~Gui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}

void Gui::Draw()
{
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(SP_WIDTH, W_HEIGHT / 2), ImGuiCond_Always);

    ImGui::Begin("Generator", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)


    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::SliderInt("QR code version", &v, 1, 40);

    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(0.0f, W_HEIGHT / 2), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(SP_WIDTH, W_HEIGHT / 2), ImGuiCond_Always);

    ImGui::Begin("Scanner", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    /* Swap front and back buffers */
    glfwSwapBuffers(m_window);

    /* Poll for and process events */
    glfwPollEvents();
}

bool Gui::WindowShouldClose()
{
    return glfwWindowShouldClose(m_window);
}
