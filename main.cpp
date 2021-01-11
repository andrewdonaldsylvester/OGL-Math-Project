#include <iostream>

#define GL_SILENCE_DEPRECATION

#include </usr/local/include/GLFW/glfw3.h>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#endif

#include </usr/local/include/glm/glm.hpp>
#include </usr/local/include/glm/gtx/string_cast.hpp>

#include "matrices.h"

using namespace glm;

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

const float aspect_ratio = WINDOW_WIDTH / WINDOW_HEIGHT;
const float FOV = 90.0f;
const float near = 0.1f;
const float far = 100.0f;

double prev_xpos = WINDOW_WIDTH  / 2;
double prev_ypos = WINDOW_HEIGHT / 2;

float vertices[] = {
     2.0f,  0.666f, 1.0f,
     1.0f,  2.0f, 1.0f,
     3.0f,  4.0f, 1.0f,
     0.5f,  1.0f, 1.0f
};

unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
};

mat4 MVP;
GLint MVP_location;

vec3 camera_pos = vec3(0.0f, 0.0f, 0.0f);
vec3 camera_rot = vec3(0.0f, 0.0f, 0.0f);

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 pos;\n"
    "uniform mat4 MVP;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = MVP * vec4(pos.x, pos.y, pos.z, 1.0f);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "   color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow *window);

int main(){
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OGL Math", NULL, NULL);
    
    if (window == NULL)
    {
        printf("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // setup buffers
    
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    
    // shader loading
    
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    int  success;
    char infoLog[512];
    
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    glUseProgram(shaderProgram);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    MVP_location = glGetUniformLocation(shaderProgram, "MVP");
    
    //
    
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        
        // render here
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // MVP construction
        
        mat4 proj = projection(aspect_ratio, FOV, near, far);
        
        mat4 trans = translation(camera_pos.x, camera_pos.y, camera_pos.z);
        
        mat4 rot = rotation(camera_rot.x, camera_rot.y, camera_rot.z);
        
        MVP = proj * rot * trans;
        
        //
        
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(MVP_location, 1, GL_FALSE, &MVP[0][0]);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        //
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera_pos -= vec3(0.0f, 0.0f, 0.01f);
    
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera_pos += vec3(0.0f, 0.0f, 0.01f);
    
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera_pos += vec3(0.01f, 0.0f, 0.0f);
    
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera_pos -= vec3(0.01f, 0.0f, 0.0f);
    
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera_pos += vec3(0.0f, 0.01f, 0.0f);
    
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera_pos -= vec3(0.0f, 0.01f, 0.0f);
    
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    
    double d_xpos = xpos - prev_xpos;
    double d_ypos = ypos - prev_ypos;
    
    prev_xpos = xpos;
    prev_ypos = ypos;
    
//    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
//        camera_rot -= vec3(0.0f, 0.0f, 1.0f);
//    
//    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
//        camera_rot += vec3(0.0f, 0.0f, 1.0f);
    
    camera_rot += vec3(-d_ypos, -d_xpos, 0.0f);
}
