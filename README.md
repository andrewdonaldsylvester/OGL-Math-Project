# OGL-Math-Project
Firstly, we must understand how graphics APIs work. The API used in this project is called OpenGL. It is a very popular API and it is supported on almost every operating system. 

To demonstrate how the API is used we'll look at a basic script to render a triangle.

hello_triangle.cpp

```cpp
#include "shader_loading.hpp"


int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

float vertices[] = {
   0.0f, 0.0f,
   1.0f, 0.0f,
   0.5f, -0.5f
};

unsigned int indices[] = {
    0, 1, 2
};

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
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);

    
    GLuint shader_program = loadShaders("vertexshader.glsl",
                                        "fragmentshader.glsl");

    glUseProgram(shader_program);
    
    //
    
    while(!glfwWindowShouldClose(window))
    {
        
        // render here
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(shader_program);
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        
        glBindVertexArray(0);
        
        //
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    
    return 0;
}
```
And here's the shaders used:

vertexshader.glsl
```glsl
#version 330 core

in vec2 pos;

void main()
{
    gl_Position = vec4(pos.x, pos.y, 0.0f, 1.0f);
}
```

fragmentshader.glsl
```glsl
#version 330 core

out vec4 color;

void main()
{
   color = vec4(1.0f, 0.5f, 0.0f, 1.0f);
}
```

A lot of this may look confusing but there are a few relevant parts to understanding how this works.

The vertices array on line 7 defines the vertex x and y coordinates. The coordinates range from -1 to 1 for the x and y axes. This coordinate system is called normalized device coordinates (NDC). 0, 0 would be in the center of the window, -1, -1 would be in the bottom left and 1, 1 would be in the top right.

On line 59 we tell the program that this data comes in pairs of two, it now is able to pass the data to our shaders.

Looking at the vertex shader we can see that all it does it take the x and y coordinates, sets their z coordinate to 0 because this is a 2D rendering, and it sets the fourth coordinate, w, to 1. This fourth coordinate is not relevant to this example but we will see it in use later on. The fragment shader doesn't modfiy anything with our position data, it never will, it is only used to determine the color of the vertices.

Running this program we should end up with something like this:

![Hello Triangle Result](https://github.com/andrewdonaldsylvester/OGL-Math-Project/blob/main/hello_triangle.png)

This is nice but of course, this wouldn't be too exciting if we only did 2D. So we move to 3D.

The first thing we need to address is our coordinate system. We'll need to add a third coordinate, Z. From now on our vertex data will be stored as 3 dimensional vectors with any rational numbers as their coordiantes. We'll replace our 2D triangle with a 3D tetrahedron. We also modify the indices array to tell the program which vertices to connect.

```cpp
float vertices[] = {
   1.0f,  1.0f, -0.707f,
  -1.0f,  1.0f, -0.707f,
   0.0f,  2.0f,  0.707f,
   0.0f,  0.0f,  0.707f
};

unsigned int indices[] = {
    0, 1, 2,
    0, 1, 3,
    0, 2, 3,
    1, 2, 3
};
```

Now that we've expanded our coordiante system to 3 coordinates and world coordinates that go infinitely in any direction, we need to come up with a way to decide what to render and where.

The standard way of doing this is by using a virtual camera with a modifiable position and rotation. This way, we can move our camera around and obserce our 3D scene.

To keep track of our camera position and rotation we'll use two 3d vectors to store these values.

```cpp
glm::vec3 camera_pos = vec3(0.0f);
glm::vec3 camera_rot = vec3(0.0f);
```

We'll come back to these in a bit because first, we have to derive the matrices to do these transformations.



Homogenous coordinates:

As you may recall, our vertex shader assigned our coordinates to a **four** dimensional vector instead of a 3d one. This is because OpenGL actually uses four properties for it's vertices. X and Y for screen positioning, Z for determining what objects to render in front of the others, and W which is set to 0 if the vector represents and direction, and is set to 1 if it represents a position. With this in mind, our first transformation matrix, the translation matrix, will be structured like this:

![Translation Matrix Unknown](https://github.com/andrewdonaldsylvester/OGL-Math-Project/blob/main/transl_mat_unk.png)

And when we multiply by a direction, we don't want anything to change:

![Translation Matrix Unknown 2](https://github.com/andrewdonaldsylvester/OGL-Math-Project/blob/main/transl_mat_unk_2.png)

This is a pretty simple matrix to derive:

![Translation Matrix Solved](https://github.com/andrewdonaldsylvester/OGL-Math-Project/blob/main/transl_mat_solved.png)

Now to move onto rotation. Again we want a 4x4 matrix that returns xyzw coordiantes but this time we want it to affect the vector whether or not w is 1. Here is the matrix for rotating a 2d point around the origin:

