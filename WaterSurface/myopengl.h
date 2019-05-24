#ifndef MYOPENGL_H
#define MYOPENGL_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Logger.h"

#define ERROR_LOG_LENGTH 2000

const char* getErrorMessage(GLenum id)
{
    switch(id)
    {
        case GL_NO_ERROR:          return "";
        case GL_INVALID_ENUM:      return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE:     return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
        case GL_OUT_OF_MEMORY:     return "GL_OUT_OF_MEMORY";
        case GL_STACK_UNDERFLOW:   return "GL_STACK_UNDERFLOW";
        case GL_STACK_OVERFLOW:    return "GL_STACK_OVERFLOW";
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return "GL_INVALID_FRAMEBUFFER_OPERATION";
        default: return "unknown opengl error";
    };
}


void checkForErrors(const char* message){
    GLuint error = glGetError();
    if(error != GL_NO_ERROR)
    {
        cz_mg_logger::Logger::log(std::string(message) + std::string(": ") + getErrorMessage(error));
        exit(-1);
    }
}


char* loadFile(const char* path)
{
    // open file
    FILE* file = fopen(path, "rb");
    if(file == 0){
        cz_mg_logger::Logger::log(std::string("Could not open file: ") + path);
        exit(-1);
    }

    // get file size
    fseek(file, 0L, SEEK_END);
    int file_size = ftell(file);
    rewind(file);

    // allocate memory for string
    char* program = (char*) malloc(sizeof(char) * file_size);

    // read all characters from file
    int read_size = fread(program, sizeof(char), file_size, file);
    if(read_size != file_size)
    {
        cz_mg_logger::Logger::log(std::string("Error while reading file: ") + path);
        exit(-1);
    }

    // add terminating character
    program[file_size-1] = 0;

    return program;
}


GLuint createShader(GLenum type, const char* code)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &code, NULL);
    glCompileShader(shader);

    GLint result = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE)
    {
        char errorLog[ERROR_LOG_LENGTH];
        glGetShaderInfoLog(shader, ERROR_LOG_LENGTH, NULL, errorLog);
        cz_mg_logger::Logger::log(std::string("Could not compile shader:"));
        cz_mg_logger::Logger::log(std::string(errorLog));
        exit(-1);
    }

    return shader;
}


GLuint createProgram(const char* vs_code, const char* fs_code)
{
    GLuint program = glCreateProgram();
    GLuint vertexShader = createShader(GL_VERTEX_SHADER, vs_code);
    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fs_code);

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint result = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if(result == GL_FALSE)
    {
        char errorLog[ERROR_LOG_LENGTH];
        glGetProgramInfoLog(program, ERROR_LOG_LENGTH, NULL, errorLog);
        cz_mg_logger::Logger::log(std::string("Could not link program:"));
        cz_mg_logger::Logger::log(std::string(errorLog));
        exit(-1);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}


GLuint loadProgram(const char* vs_path, const char* fs_path)
{
    char* vs_code = loadFile(vs_path);
    char* fs_code = loadFile(fs_path);
    GLint program = createProgram(vs_code, fs_code);
    free(vs_code);
    free(fs_code);
    return program;
}


#endif
