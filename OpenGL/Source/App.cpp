#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>

void APIENTRY MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    std::cout << std::endl;

    // OpenGL Source
    std::cout << "Source: ";
    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             std::cout << "API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           std::cout << "Other"; break;
    }
    std::cout << std::endl;

    // Debug Message
    std::cout << "Message: " << message << std::endl;

    // Error Type
    std::cout << "Type: ";
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:               std::cout << "Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Performance"; break;
    case GL_DEBUG_TYPE_OTHER:               std::cout << "Other"; break;
    }

    // Error ID
    std::cout << std::endl;
    std::cout << "Id: " << id << std::endl;

    // Error Severity
    std::cout << "Severity: ";
    switch (severity) {
    case GL_DEBUG_SEVERITY_LOW:
        std::cout << "LOW";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        std::cout << "MEDIUM";
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        std::cout << "HIGH";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        std::cout << "NOTIFICATION";
        break;
    }
    std::cout << std::endl;

    // Backtrack error location
#ifdef _DEBUG
        __debugbreak();
#endif
}

struct ShaderSource
{
    std::string vs;
    std::string ps;
};

ShaderSource ParseShader(const std::string& filePath)
{
    enum class ShaderType
    {
        Invalid = -1, Vertex, Pixel
    };

    std::ifstream file;
    file.open(filePath);
    if (!file.is_open())
    {
        std::cout << "ERROR : File ( " << filePath << " ) could not be opened\n";
        return {};
    }

    std::string line;
    ShaderType mode = ShaderType::Invalid;
    std::string sources[2];
    while (getline(file, line))
    {
        if (line.find("#shader")!=std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                mode = ShaderType::Vertex;
            else if (line.find("pixel") != std::string::npos)
                mode = ShaderType::Pixel;
            else
                mode = ShaderType::Invalid;
        }
        else
        {
            line += '\n';
            sources[(int)mode] += line;
        }
    }

    file.close();
    return { sources[(int)ShaderType::Vertex], sources[(int)ShaderType::Pixel]};
}

unsigned int CompileShader(const std::string& shader, unsigned int type)
{
    unsigned int shaderID = glCreateShader(type);
    const char* source = shader.c_str();
    glShaderSource(shaderID, 1, &source, nullptr);
    glCompileShader(shaderID);

    // Error Handling
    int result;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
        char* message = new char[length];
        glGetShaderInfoLog(shaderID, length, &length, message);

        std::cout << "ERROR : Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "pixel") << "shader.\n";
        std::cout << message << std::endl;
        return 0;
    }

    return shaderID;
}

unsigned int CreateProgram(const std::string& vertexShader, const std::string& pixelShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int ps = CompileShader(pixelShader, GL_FRAGMENT_SHADER);

    glAttachShader(program, vs);
    glAttachShader(program, ps);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(ps);

    return program;
}

int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 720, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);


    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }

    fprintf(stdout, "GLEW : Using GLEW %s\n", glewGetString(GLEW_VERSION));
    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;


    // During init, enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(MessageCallback, 0);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);


    float positions[] =
    {
        -0.5f, -0.5f,
        -0.5f, 0.5f,
        0.5f, 0.5f,
        0.5f , -0.5f
    };

    unsigned int indices[] =
    {
        0,1,2,
        0,2,3
    };

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW);

    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0);


    ShaderSource shader = ParseShader("Source/Shaders/Basic.glsl");
    unsigned int program = CreateProgram(shader.vs, shader.ps);
    glUseProgram(program);

    int location = glGetUniformLocation(program, "uColor");


    /* Loop until the user closes the window */
    float prev = glfwGetTime();
    int frames = 0;
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

        float r = sin(glfwGetTime() * 0.2f);
        float g = sin(glfwGetTime() * 0.1f);
        float b = sin(glfwGetTime() * 0.5f);

        glUniform4f(location, r, g, b, 1.0f);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        if (glfwGetKey(window, GLFW_KEY_ENTER))
        {
            glfwSetWindowShouldClose(window, true);
        }

        float now = glfwGetTime();
        if (now - prev > 1.0f)
        {
            std::string title = " FPS : " + std::to_string(frames) + " Delta : " + std::to_string(1.0f / frames * 1000).substr(0, 4) + "ms";
            glfwSetWindowTitle(window, title.c_str());
            prev = now;
            frames = 0;
        }
        frames++;


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(program);

    glfwTerminate();

	return 0;
}