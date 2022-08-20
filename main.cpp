// use (modern) OpenGL
#define GLEW_STATIC
#include <GL/glew.h>

// windowing and input
#include <GLFW/glfw3.h>

// utilities for creating windows etc.
#include "util.h"


int main(int argc, char** argv)
{

    const char* vertexSource = R"glsl(
    #version 150 core

    in vec2 position;
    in vec3 color;

    out vec3 Color;

    void main()
    {
        Color = color;
        gl_Position = vec4(position, 0.0, 1.0);
    }
    )glsl";

    const char* fragmentSource = R"glsl(
    #version 150 core

    in vec3 Color;

    out vec4 outColor;

    void main()
    {
        outColor = vec4(Color, 1.0);
    }
    )glsl";

    GLFWwindow* window = setup();

    // vertecies in the format [x, y, z, r, g, b]
    /* float vertices[] = { */
    /*     -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left */
    /*      0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right */
    /*      0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right */
    /*     -0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left */
    /* }; */
    float vertices[] = {
         0.0f,  0.5f, 1.0f, 0.0f, 0.0f, // Vertex 1: Red
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Vertex 2: Green
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f  // Vertex 3: Blue
    };

    // generate a VAO(Vertex Array Object) to save state for later use I guess
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // create buffer for triangle and bind it so we can manipulate it
    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo); // GL_ARRAY_BUFFER is just a basic buffer I guess
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 1st: Use the currently bound GL_ARRAY_BUFFER, 2nd: The size in bytes of the uploaded data, 3th: the data, 4th: data won't change(just a hint)

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // why tf does this one return the "id" and glGenBuffers doesn't? Maybe cuz glGenBuffers can generate multiple Buffers?
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram(); // create a shader program and attach the shaders
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram); // linking the shader program...
    glUseProgram(shaderProgram);

    // tell OpenGL how the vertex data is formatted so it can put it in our shader programs as variables or whatsoever
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib); // enable the attribute
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0); // 1st is our position attribute thingy. 2nd is the number of parts per unit I guess. 3th is the type of the variables/parts. 4th is about normalizing or not. 5th: how many bytes are between each data point. 6th: offset, how many bytes after the start of the array begins this specific data

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));

    // shows which indecies form the two triangles
    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0,
    };

    // creating an EBO(Element Buffer Object) to optimize reusability of vertecies
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    while (!glfwWindowShouldClose(window))
    {
        // draw the currently bound array buffer
        glDrawArrays(GL_TRIANGLES, 0, 3); // 1st: type of primitive to draw, 2nd: how many vertecies to skip(why would you want to do this? ._.), 3th: number of vertecies
        /* glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); */
        glfwSwapBuffers(window);
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    }

    terminate();
}
