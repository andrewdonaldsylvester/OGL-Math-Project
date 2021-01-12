# OGL-Math-Project

```cpp
#include "shader_loading.hpp"
#include "matrices.hpp"

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

float aspect_ratio = WINDOW_WIDTH / WINDOW_HEIGHT;
float FOV = 100.0f;
float near = 0.01f;
float far = 1000.0f;

double prev_xpos = WINDOW_WIDTH  / 2;
double prev_ypos = WINDOW_HEIGHT / 2;

float vertices[] = {
   /* Position                  Color */
      1.0f,  1.0f, -0.707f,     0.259f, 0.529f, 0.961f,
     -1.0f,  1.0f, -0.707f,     0.196f, 0.659f, 0.322f,
      0.0f,  2.0f,  0.707f,     0.922f, 0.251f, 0.204f,
      0.0f,  0.0f,  0.707f,     0.988f, 0.729f, 0.012f,
    
      3.0f,  -1.0f,  3.0f,      0.259f, 0.529f, 0.961f,
     -3.0f,  -1.0f,  3.0f,      0.196f, 0.659f, 0.322f,
     -3.0f,  -1.0f, -3.0f,      0.922f, 0.251f, 0.204f,
      3.0f,  -1.0f, -3.0f,      0.988f, 0.729f, 0.012f
};

unsigned int indices[] = {
    0, 1, 2,
    0, 1, 3,
    0, 2, 3,
    1, 2, 3,
    
    4, 5, 6,
    4, 6, 7
};

glm::mat4 MVP;
GLint MVP_location;

glm::vec3 camera_pos = glm::vec3(0.0f);
glm::vec3 camera_rot = glm::vec3(0.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void process_input(GLFWwindow *window);

int main()
{
    
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
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
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

//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); 
    glEnableVertexAttribArray(1);

    
    GLuint shader_program = loadShaders("vertexshader.glsl",
                                        "fragmentshader.glsl");

    glUseProgram(shader_program);
    
    MVP_location = glGetUniformLocation(shader_program, "MVP");
    
    //
    
    while(!glfwWindowShouldClose(window))
    {
        process_input(window);
        
        // render here
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // MVP construction
        
        glm::mat4 proj = projection(aspect_ratio, FOV, near, far);
        
        glm::mat4 trans = translation(camera_pos.x, camera_pos.y, camera_pos.z);
        
        glm::mat4 rot = rotation(camera_rot.x, camera_rot.y, camera_rot.z);
        
        MVP = proj * rot * trans;
        
        //
        
        glUseProgram(shader_program);
        
        glUniformMatrix4fv(MVP_location, 1, GL_FALSE, &MVP[0][0]);
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
        
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
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
    
    aspect_ratio = WINDOW_WIDTH / WINDOW_HEIGHT;
    
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

void process_input(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera_pos -= glm::vec3(0.01f * sin(glm::radians(camera_rot.y)), 0.0f, 0.01f * cos(glm::radians(camera_rot.y)));
    
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera_pos += glm::vec3(0.01f * sin(glm::radians(camera_rot.y)), 0.0f, 0.01f * cos(glm::radians(camera_rot.y)));
    
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera_pos -= glm::vec3(0.01f * sin(glm::radians(camera_rot.y) + 1.571f), 0.0f, 0.01f * cos(glm::radians(camera_rot.y) + 1.571f));
    
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera_pos += glm::vec3(0.01f * sin(glm::radians(camera_rot.y) + 1.571f), 0.0f, 0.01f * cos(glm::radians(camera_rot.y) + 1.571f));
    
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera_pos -= glm::vec3(0.0f, 0.01f, 0.0f);
    
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera_pos += glm::vec3(0.0f, 0.01f, 0.0f);
    
//    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
//        camera_rot.z += 1.0f;
//
//    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
//        camera_rot.z -= 1.0f;
    
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    
    double d_xpos = xpos - prev_xpos;
    double d_ypos = ypos - prev_ypos;
    
    prev_xpos = xpos;
    prev_ypos = ypos;
    
    camera_rot += glm::vec3(-d_ypos, d_xpos, 0.0f);
    
    if (camera_rot.x < -90)
        camera_rot.x = -90;
    if (camera_rot.x > 90)
        camera_rot.x = 90;
    
    camera_rot.y = fmod(camera_rot.y + 360.0f, 360.0f);
    
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        camera_pos = glm::vec3(0.0f);
        camera_rot = glm::vec3(0.0f);
    }
    
    std::cout << glm::to_string(camera_pos) << std::endl << std::endl << glm::to_string(camera_rot) << std::endl;
}
```
