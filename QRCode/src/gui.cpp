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

    // Turn Vsync ON so the laptop doesn't go BOOM
    glfwSwapInterval(1);

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

    //static const char* current_version = versions[0];

    ImGui::Text("QR code version");

    if (ImGui::BeginCombo("##combo", m_version)) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < IM_ARRAYSIZE(versions); n++)
        {
            bool is_selected = (m_version == versions[n]); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(versions[n], is_selected))
                m_version = versions[n];
            if (is_selected)
                ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
        }
        ImGui::EndCombo();
    }

    ImGui::NewLine();

    const char* err_corr_levels[] = { "L (7%)", "M (15%)", "Q (25%)", "H (30%)" };

    ImGui::Text("Error correction level");

    if (ImGui::BeginCombo("##combo_level", m_error_correction)) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < IM_ARRAYSIZE(err_corr_levels); n++)
        {
            bool is_selected = (m_error_correction == err_corr_levels[n]); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(err_corr_levels[n], is_selected))
                m_error_correction = err_corr_levels[n];
            if (is_selected)
                ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
        }
        ImGui::EndCombo();
    }

    ImGui::NewLine();

    const char* encodings[] = { "Numeric", "Alphanumeric", "Byte", "Kanji" };

    //static const char* current_encoding = encodings[1];

    ImGui::Text("Encoding mode");

    if (ImGui::BeginCombo("##combo_encoding", m_encoding)) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < IM_ARRAYSIZE(encodings); n++)
        {
            bool is_selected = (m_encoding == encodings[n]); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(encodings[n], is_selected))
                m_encoding = encodings[n];
            if (is_selected)
                ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
        }
        ImGui::EndCombo();
    }

    ImGui::NewLine();

    // The input size needs to be based on the version of the QR code
    ImGui::Text("Input");
    ImGui::InputText("##input", m_input, IM_ARRAYSIZE(m_input));

    ImGui::NewLine();

    // Align item center horizontally
    //auto textWidth = ImGui::CalcTextSize("Generate").x;

    //ImGui::SetCursorPosX((SP_WIDTH - textWidth) * 0.5f);

    if (ImGui::Button("Generate"))
    {
        std::cout << m_input << std::endl;
        std::cout << "Generating QR code..." << std::endl;
    }

    ImGui::End();


    ImGui::SetNextWindowPos(ImVec2(0.0f, W_HEIGHT / 2), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(SP_WIDTH, W_HEIGHT / 2), ImGuiCond_Always);
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(15.0f, 10.0f));

    ImGui::Begin("Scanner", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    ImGui::PopStyleVar();
    ImGui::PopStyleVar();

    ImGui::TextWrapped("Select where do you want to scan the QR code from.");
    ImGui::NewLine();

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
