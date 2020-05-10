#ifndef SHADER_H
#define SHADER_H

#include <stdbool.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct ShaderStruct * ShaderPtr;

ShaderPtr NewShader(const char *vertex_path, const char *fragment_path);

void Use(ShaderPtr shader);
void SetBool(ShaderPtr shader, const char* name, bool value);
void SetInt(ShaderPtr shader, const char* name, int value);
void SetUnsignedInt(ShaderPtr shader, const char* name, unsigned int value);
void SetFloat(ShaderPtr shader, const char* name, float value);

GLuint GetID(ShaderPtr shader);


#endif
