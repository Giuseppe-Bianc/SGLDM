//
// Created by gbian on 29/06/2024.
//

#pragma once

#include "Log.hpp"
#include "glad/glad.h"
#include <GLFW/glfw3.h>

inline void glClearErrors() {
    while(glGetError() != GL_NO_ERROR) {}
}
inline GLenum glCheckError_(const char *function, const char *file, int line) {
    GLenum errorCode;
    while((errorCode = glGetError()) != GL_NO_ERROR) {
        std::string error;
        switch(errorCode) {
        case GL_INVALID_ENUM:
            error = "INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            error = "INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            error = "INVALID_OPERATION";
            break;
        case GL_STACK_OVERFLOW:
            error = "STACK_OVERFLOW";
            break;
        case GL_STACK_UNDERFLOW:
            error = "STACK_UNDERFLOW";
            break;
        case GL_OUT_OF_MEMORY:
            error = "OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            error = "INVALID_FRAMEBUFFER_OPERATION";
            break;
        default:
            error = "UNKOWN";
            break;
        }
        LERROR("OpenGL:  {}| {} {}({})", error, function, file, line);
    }
    return errorCode;
}
#define glCheckError(x)                                                                                                                    \
    glClearErrors();                                                                                                                       \
    x;                                                                                                                                     \
    glCheckError_(#x, __FILE__, __LINE__)

static inline void errorCallback(int error, const char *description) { LERROR("GLFW Error ({0}): {1}", error, description); }
static inline void keyCallback(GLFWwindow *window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
    switch(key) {
    case GLFW_KEY_ESCAPE:
        if(action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            LINFO("escape close");
        }
        break;
    // Add more cases for other keys if needed
    [[likely]] default:
        // Handle other keys here
        break;
    }
}
static inline void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}
