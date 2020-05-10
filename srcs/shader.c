#include "shader.h"

#include <stdio.h>
#include <stdlib.h>

GLuint compileShader(const char* shader_src, GLenum shader_type);

GLuint newShader(const char *vertex_path, const char *fragment_path)
{
    GLuint vertex_shader = compileShader(vertex_path, GL_VERTEX_SHADER);
    GLuint frag_shader = compileShader(fragment_path, GL_FRAGMENT_SHADER);

    GLuint prog_shader = glCreateProgram();
    glAttachShader(prog_shader, vertex_shader);
    glAttachShader(prog_shader, frag_shader);
    glBindFragDataLocation(prog_shader, 0, "out_color");
    glLinkProgram(prog_shader);

    int success;
    char info_log[512];
    glGetProgramiv(prog_shader, GL_LINK_STATUS, &success);
    if (!success)
	{
        glGetProgramInfoLog(prog_shader, 512, NULL, info_log);
        fprintf(stderr, "Shader Linking Error:\n");
        fprintf(stderr, "%s\n", info_log);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(frag_shader);

    return prog_shader;
}

void use(GLuint prog_shader)
{
    glUseProgram(prog_shader);
}

void setFloat(GLuint prog_shader, const char* name, float value)
{
    glUniform1f(glGetUniformLocation(prog_shader, name), value);
}

GLuint compileShader(const char* shader_src, GLenum shader_type)
{
    char* buffer = NULL;
    unsigned long length;
    FILE* shader_file = fopen(shader_src, "rb");
    if (shader_file)
	{
        fseek(shader_file, 0, SEEK_END);
        length = ftell(shader_file);
        fseek(shader_file, 0, SEEK_SET);
        buffer = malloc(length + 1);
        if (buffer)
		{
            if (fread(buffer, 1, length, shader_file) != length)
			{
                fclose(shader_file);
                free(buffer);
                fprintf(stderr, "fatal error: fail when reading file\nRead length not equal to file length\n");
                exit(-1);
            }
            buffer[length] = '\0';
        }
		else
		{
            fclose(shader_file);
            fprintf(stderr, "fatal error: fail when reading file\nMemory allocation error\n");
            exit(-1);
        }
        fclose(shader_file);
    }
    else
	{
        fclose(shader_file);
        fprintf(stderr, "fatal error: fail when reading file\nNo such file or unable to open\n");
        exit(-1);
    }
    GLuint shader;
    int success;
    char info_log[512];
    shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, (const GLchar**)&buffer, NULL);
    glCompileShader(shader);
    free(buffer);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
	{
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        fprintf(stderr, "Shader Compilation Error:\n");
        fprintf(stderr, "%s\n", info_log);
    }
    return shader;
}
