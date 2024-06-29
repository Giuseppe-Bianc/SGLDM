//
// Created by gbian on 29/06/2024.
//

#pragma once
#include "SGLDM/Core.hpp"

// NOLINTNEXTLINE(*-easily-swappable-parameters)
inline void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, [[maybe_unused]] GLsizei length,
                                   const GLchar *message, [[maybe_unused]] const void *userParam) {
    // Ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    // clang-format off
    std::string_view sourceStr;
    switch (source) {
    case GL_DEBUG_SOURCE_API:             sourceStr = "API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   sourceStr = "Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceStr = "Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     sourceStr = "Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     sourceStr = "Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           sourceStr = "Other"; break;
    default:                              sourceStr = "Unknown"; break;
    }

    std::string_view typeStr;
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:               typeStr = "Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeStr = "Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  typeStr = "Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         typeStr = "Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         typeStr = "Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              typeStr = "Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          typeStr = "Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           typeStr = "Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               typeStr = "Other"; break;
    default:                                typeStr = "Unknown"; break;
    }

    std::string_view severityStr;
    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:         severityStr = "High"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       severityStr = "Medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          severityStr = "Low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: severityStr = "Notification"; break;
    default:                             severityStr = "Unknown"; break;
    }
    // clang-format on

    // Determine color based on severity
    switch(severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        LERROR("Debug message ({}): {}\nSource: {}\nType: {}\nSeverity: {}", id, message, sourceStr, typeStr, severityStr);
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        LWARN("Debug message ({}): {}\nSource: {}\nType: {}\nSeverity: {}", id, message, sourceStr, typeStr, severityStr);
        break;
    case GL_DEBUG_SEVERITY_LOW:
        LINFO("Debug message ({}): {}\nSource: {}\nType: {}\nSeverity: {}", id, message, sourceStr, typeStr, severityStr);
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        LTRACE("Debug message ({}): {}\nSource: {}\nType: {}\nSeverity: {}", id, message, sourceStr, typeStr, severityStr);
        break;
    default:
        LDEBUG("Debug message ({}): {}\nSource: {}\nType: {}\nSeverity: {}", id, message, sourceStr, typeStr, severityStr);
        break;
    }
    DEBUG_BREAK;
}