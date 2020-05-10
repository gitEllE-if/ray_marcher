#include "shader.h"

#include <stdio.h>
#include <stdlib.h>

struct ShaderStruct {
    GLuint m_ID;
};

GLuint CompileShader(const char* shader_src, GLenum shader_type);

ShaderPtr NewShader(const char *vertex_path, const char *fragment_path) {
    ShaderPtr shader = (ShaderPtr) malloc(sizeof(struct ShaderStruct));

    // Compile shaders
    GLuint vertex_shader = CompileShader(vertex_path, GL_VERTEX_SHADER);
    GLuint frag_shader = CompileShader(fragment_path, GL_FRAGMENT_SHADER);

    // Create program and link shaders to it
    GLuint prog_ID = glCreateProgram();
    glAttachShader(prog_ID, vertex_shader);
    glAttachShader(prog_ID, frag_shader);
    glBindFragDataLocation(prog_ID, 0, "out_color");
    glLinkProgram(prog_ID);

    // Print linking errors if any
    int success;
    char info_log[512];
    glGetProgramiv(prog_ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(prog_ID, 512, NULL, info_log);
        fprintf(stderr, "Shader Linking Error:\n");
        fprintf(stderr, "%s\n", info_log);
    }

    // Delete shaders since they're already linked, and are no longer necessary
    glDeleteShader(vertex_shader);
    glDeleteShader(frag_shader);

    // Set actual shader object's ID to the program's ID
    shader->m_ID = prog_ID;

    return shader;
}

void Use(ShaderPtr shader) {
    glUseProgram(shader->m_ID);
}

void SetBool(ShaderPtr shader, const char* name, bool value) {
    glUniform1i(glGetUniformLocation(shader->m_ID, name), (int) value);
}

void SetInt(ShaderPtr shader, const char* name, int value) {
    glUniform1i(glGetUniformLocation(shader->m_ID, name), value);
}

void SetUnsignedInt(ShaderPtr shader, const char* name, unsigned int value) {
    glUniform1ui(glGetUniformLocation(shader->m_ID, name), value);
}

void SetFloat(ShaderPtr shader, const char* name, float value) {
    glUniform1f(glGetUniformLocation(shader->m_ID, name), value);
}

GLuint GetID(ShaderPtr shader) {
    return shader->m_ID;
}

GLuint CompileShader(const char* shader_src, GLenum shader_type) {
    ///////////////////////////////////
    // Read shader files into memory //
    ///////////////////////////////////

    char* buffer = NULL;
    unsigned long length;
    FILE* shader_file = fopen(shader_src, "rb");
    if (shader_file)
	{
        // Get length of file
        fseek(shader_file, 0, SEEK_END);
        length = ftell(shader_file);
        fseek(shader_file, 0, SEEK_SET);

        // Read file into memory
        buffer = malloc(length + 1);
        if (buffer)
		{
            if (fread(buffer, 1, length, shader_file) != length)
			{
                fclose(shader_file);
                free(buffer);

                fprintf(stderr, "fatal error: error when reading file into memory.\nLength read not equal to length of file\n");
                exit(-1);
            }
            buffer[length] = '\0';
        }
		else
		{
            fclose(shader_file);

            fprintf(stderr, "fatal error: error when reading file into memory.\nMemory failed to allocate\n");
            exit(-1);
        }

        fclose(shader_file);
    }
    else
	{
        fclose(shader_file);

        fprintf(stderr, "fatal error: error when reading file into memory.\nFile unable to open\n");
        exit(-1);
    }

    GLuint shader;
    int success;
    char info_log[512];

    // Compile shader

    shader = glCreateShader(shader_type);


    glShaderSource(shader, 1, (const GLchar**)&buffer, NULL);
    glCompileShader(shader);
    free(buffer);

    // Print any compilation errors
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        fprintf(stderr, "Shader Compilation Error:\n");
        fprintf(stderr, "%s\n", info_log);
    }

    return shader;
}
