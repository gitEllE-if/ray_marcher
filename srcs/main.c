#include "shader.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

#define SCREEN_WIDTH 800.0
#define SCREEN_HEIGHT 600.0
#define FPS 60

void init_glfw();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

int main(void)
{

    // Initialise GLFW
    init_glfw();

    // Create GLFW window
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "RayMarching", NULL, NULL);
    if (!window) {
        glfwTerminate();
        fprintf(stderr, "fatal error: Failed to create GLFW window\n");

        return -1;
    }
    glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Clear GL color buffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // Create shader
    ShaderPtr shader = NewShader("shaders/vertex.vert", "shaders/fragment.frag");
    // Generate vertex array object
    GLuint vao;
    glGenVertexArrays(1, &vao);

    // Generate vertex buffer object
    GLuint vbo;
    glGenBuffers(1, &vbo);

    // Bind vao to store VertexAttribPointer call
    glBindVertexArray(vao);

    // Buffer data into vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    const static GLfloat vertices[] = {
            // Position
            -1.0f,  1.0f, // Top-left
            1.0f,  1.0f,  // Top-right
            1.0f, -1.0f,  // Bottom-right

            1.0f, -1.0f,  // Bottom-right
            -1.0f, -1.0f, // Bottom-left
            -1.0f,  1.0f, // Top-left
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set vertex attribute for position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Send resolution of window to shader
    float resolution[2] = {SCREEN_WIDTH, SCREEN_HEIGHT};
    glUniform2fv(glGetUniformLocation(GetID(shader), "iResolution"), 1, &resolution[0]);

    // Unbind vao
    glBindVertexArray(0);

    // Unbind buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Game loop
    while (!glfwWindowShouldClose(window)) {
        // Handle delta time

        // Poll events for controls
        glfwPollEvents();


        // Rendering
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        Use(shader);

        SetFloat(shader, "iTime", glfwGetTime());
        glUniform2fv(glGetUniformLocation(GetID(shader), "iResolution"), 1, &resolution[0]);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }
    
    return 0;
}

void init_glfw() {
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	(void)window;
    glViewport(0, 0, width, height);
}