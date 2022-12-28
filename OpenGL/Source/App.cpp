#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>

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

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 720, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);


    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }

    fprintf(stdout, "GLEW : Using GLEW %s\n", glewGetString(GLEW_VERSION));
    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;


    float positions[] =
    {
        -0.5f, -0.5f,
        -0.5f, 0.5f,
        0.5f, 0.5f,
        0.5f, -0.5f,
    };

    unsigned int indices[] =
    {
        0,1,2,
        0,2,3
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, positions, GL_STATIC_DRAW);

    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0);


    ShaderSource shader = ParseShader("Source/Shaders/Basic.glsl");
    unsigned int program = CreateProgram(shader.vs, shader.ps);
    glUseProgram(program);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        if (glfwGetKey(window, GLFW_KEY_ENTER))
        {
            glfwSetWindowShouldClose(window, true);
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(program);

    glfwTerminate();

	return 0;
}