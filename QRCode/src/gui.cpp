#include "gui.h"

Gui::Gui()
{
    m_qrcode = QRCode();

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

    if (ImGui::BeginCombo("##combo", m_qrcode.m_version)) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < IM_ARRAYSIZE(versions); n++)
        {
            bool is_selected = (m_qrcode.m_version == versions[n]); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(versions[n], is_selected))
                m_qrcode.m_version = versions[n];
            if (is_selected)
                ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
        }
        ImGui::EndCombo();
    }

    ImGui::NewLine();

    const char* err_corr_levels[] = {"L (7%)", "M (15%)", "Q (25%)", "H (30%)"};

    ImGui::Text("Error correction level");

    if (ImGui::BeginCombo("##combo_level", m_qrcode.m_error_correction)) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < IM_ARRAYSIZE(err_corr_levels); n++)
        {
            bool is_selected = (m_qrcode.m_error_correction == err_corr_levels[n]); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(err_corr_levels[n], is_selected))
                m_qrcode.m_error_correction = err_corr_levels[n];
            if (is_selected)
                ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
        }
        ImGui::EndCombo();
    }

    ImGui::NewLine();

    const char* encodings[] = { "Numeric", "Alphanumeric", "Byte", "Kanji" };

    //static const char* current_encoding = encodings[1];

    ImGui::Text("Encoding mode");

    if (ImGui::BeginCombo("##combo_encoding", m_qrcode.m_encoding)) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < IM_ARRAYSIZE(encodings); n++)
        {
            bool is_selected = (m_qrcode.m_encoding == encodings[n]); // You can store your selection however you want, outside or inside your objects
            if (ImGui::Selectable(encodings[n], is_selected))
                m_qrcode.m_encoding = encodings[n];
            if (is_selected)
                ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
        }
        ImGui::EndCombo();
    }

    ImGui::NewLine();

    // The input size needs to be based on the version of the QR code
    ImGui::Text("Input");
    ImGui::InputText("##input", m_qrcode.m_input, IM_ARRAYSIZE(m_qrcode.m_input));

    ImGui::NewLine();

    // Align item center horizontally
    //auto textWidth = ImGui::CalcTextSize("Generate").x;

    //ImGui::SetCursorPosX((SP_WIDTH - textWidth) * 0.5f);

    if (ImGui::Button("Generate"))
    {
        std::cout << m_qrcode.m_input << std::endl;
        std::cout << "Generating QR code..." << std::endl;
        m_qrcode.GenerateCode();
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

    DrawCode();

    /* Swap front and back buffers */
    glfwSwapBuffers(m_window);

    /* Poll for and process events */
    glfwPollEvents();
}

void Gui::DrawCode()
{
    const char* vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec2 aTexCoord;\n"
        "out vec2 texCoord;"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "   texCoord = aTexCoord;"
        "}\0";

    const char* fragmentShaderSource = "#version 330 core\n"
        "in vec2 texCoord;"
        "uniform sampler2D texture1;"
        "out vec4 fragColor;\n"
        "void main()\n"
        "{\n"
        "   fragColor = texture(texture1, texCoord);\n"
        "}\n\0";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Compute square (QR Code image) location

    float x_deviation = float(SP_WIDTH) / W_WIDTH;

    float percentage = 1.0f;
    int height = W_HEIGHT * percentage;
    float x = float(height * 100) / W_WIDTH;
    x /= 100;

    float vertices[] = {
        x + x_deviation, percentage, 0.0f, x + x_deviation, percentage,
        x + x_deviation, -percentage, 0.0f, x + x_deviation, 0.0f,
       -x + x_deviation, -percentage, 0.0f, 0.0f, 0.0f,
       -x + x_deviation, percentage, 0.0f, 0.0f, percentage
    };

    //float x_deviation = float(SP_WIDTH) / W_WIDTH;

    //float y_deviation = float(W_WIDTH / 2) * 100;
    //std::cout << y_deviation << std::endl;

    //y_deviation /= W_HEIGHT;
    //std::cout << y_deviation << std::endl;

    //y_deviation /= W_HEIGHT;
    //std::cout << y_deviation << std::endl;

    //float vertices[] = {
    //    0.5f + x_deviation,  0.5f + y_deviation, 0.0f,
    //    0.5f + x_deviation, -0.5f - y_deviation, 0.0f,
    //   -0.5f + x_deviation, -0.5f - y_deviation, 0.0f,
    //   -0.5f + x_deviation,  0.5f + y_deviation, 0.0f
    //};

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
        code = m_qrcode.GetCode();

        cv::resize(code, code, cv::Size(height, height), 0, 0, cv::INTER_NEAREST);

        //std::cout << int(code.at<uchar>(0, 0)) << std::endl;
        //std::cout << m_qrcode.GetCode() << std::endl;

        //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_qrcode.GetCode().rows, m_qrcode.GetCode().cols, 0, 
        //    GL_LUMINANCE, GL_UNSIGNED_BYTE, m_qrcode.GetCode().ptr());
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, code.rows, code.cols, 0,
            GL_LUMINANCE, GL_UNSIGNED_BYTE, code.ptr());
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

bool Gui::WindowShouldClose()
{
    return glfwWindowShouldClose(m_window);
}
