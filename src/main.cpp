#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>

unsigned int shaderProgram = 0;
unsigned int VBO = 0;
unsigned int VAO = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
void	initShader(const char* _vertexPath, const char* _fragPath)
{
    shaderProgram = 0;
    std::string  _vertexCode("");
    std::string  _fragCode("");

    std::ifstream	_vShaderFile;
    std::ifstream	_fShaderFile;

    //增加try catch的扩展功能
    _vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    _fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        _vShaderFile.open(_vertexPath);
        _fShaderFile.open(_fragPath);

        std::stringstream	_vShaderStream, _fShaderStream;
        _vShaderStream << _vShaderFile.rdbuf();
        _fShaderStream << _fShaderFile.rdbuf();

        _vShaderFile.close();
        _fShaderFile.close();

        _vertexCode = _vShaderStream.str();
        _fragCode = _fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::string  errStr = "ffShader constructor fail to read filePath";
        std::cout << errStr << std::endl;
        return;
    }

    const char* _vShaderStr = _vertexCode.c_str();
    const char* _fShaderStr = _fragCode.c_str();

    unsigned int	_vertexID = 0, _fragID = 0;
    char	_infoLog[512];
    int		_successFlag;

    //创建顶点shader
    _vertexID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(_vertexID, 1, &_vShaderStr, NULL);
    glCompileShader(_vertexID);

    glGetShaderiv(_vertexID, GL_COMPILE_STATUS, &_successFlag);
    if (!_successFlag)
    {
        glGetShaderInfoLog(_vertexID, 512, NULL, _infoLog);
        std::string errStr(_infoLog);
        std::cout << errStr << std::endl;
    }

    //创建着色shader
    _fragID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(_fragID, 1, &_fShaderStr, NULL);
    glCompileShader(_fragID);

    glGetShaderiv(_fragID, GL_COMPILE_STATUS, &_successFlag);
    if (!_successFlag)
    {
        glGetShaderInfoLog(_fragID, 512, NULL, _infoLog);
        std::string errStr(_infoLog);
        std::cout << errStr << std::endl;
    }

    //创建shader程序
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, _vertexID);
    glAttachShader(shaderProgram, _fragID);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &_successFlag);
    if (!_successFlag)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, _infoLog);
        std::string errStr(_infoLog);
        std::cout << errStr << std::endl;
    }
    glDeleteShader(_vertexID);
    glDeleteShader(_fragID);
}
void initModel()
{
    float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void rend()
{
    glBindVertexArray(VAO);
    glUseProgram(shaderProgram);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUseProgram(0);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Core", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    initModel();
    initShader("vertexShader.glsl", "fragmentShader.glsl");

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        rend();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}