#include "shader.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

#define SCREEN_WIDTH 800.0
#define SCREEN_HEIGHT 600.0

void init_glfw();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

int main(void)
{
    init_glfw();
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "RayMarching", NULL, NULL);
    if (!window)
	{
        glfwTerminate();
        fprintf(stderr, "fatal error: Failed to create GLFW window\n");
        return -1;
    }
    glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    GLuint shader = newShader("shaders/vertex.vert", "shaders/fragment.frag");

	// VAO
    GLuint vao;
    glGenVertexArrays(1, &vao);
    // VBO
    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    const static GLfloat vertices[] = {
		-1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,
		1.0f,  -1.0f, -1.0f, -1.0f, -1.0f,  1.0f};
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    float resolution[2] = {SCREEN_WIDTH, SCREEN_HEIGHT};
    glUniform2fv(glGetUniformLocation(shader, "iResolution"), 1, &resolution[0]);

    // unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // render loop
    while (!glfwWindowShouldClose(window))
	{
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(vao);
        use(shader);
        setFloat(shader, "iTime", glfwGetTime());
        glUniform2fv(glGetUniformLocation(shader, "iResolution"), 1, &resolution[0]);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }
    return 0;
}

void init_glfw()
{
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
