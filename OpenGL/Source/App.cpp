#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

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
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
        0.0f, 0.5f,
        0.5f, -0.5f
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0);


    std::string vs = R"(
#version 330 core

layout (location = 0) in vec4 position;

void main()
{
    gl_Position = position;
}
)";


    std::string ps = R"(
#version 330 core

void main()
{
    gl_FragColor = vec4(0.0, 0.0, 1.0, 0.0);
}
)";

    unsigned int program = CreateProgram(vs, ps);
    glUseProgram(program);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

	return 0;
}