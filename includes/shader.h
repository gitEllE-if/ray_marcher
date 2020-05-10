#ifndef SHADER_H
#define SHADER_H

#include <stdbool.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLuint	newShader(const char *vertex_path, const char *fragment_path);
void	use(GLuint prog_shader);
void	setFloat(GLuint prog_shader, const char* name, float value);

#endif
