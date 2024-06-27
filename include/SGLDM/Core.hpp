#pragma once

#include "Log.hpp"
#include "glad/glad.h"
#include "headers.hpp"
#include "timer/Timer.hpp"

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

template <> struct fmt::formatter<const GLubyte *> {
    constexpr auto parse(fmt::format_parse_context &ctx) { return ctx.begin(); }

    template <typename FormatContext> auto format(const GLubyte *p, FormatContext &ctx) {
        // Format the pointer value or the pointed-to string/bytes
        return fmt::format_to(ctx.out(), "{}", reinterpret_cast<const void *>(p));
    }
};