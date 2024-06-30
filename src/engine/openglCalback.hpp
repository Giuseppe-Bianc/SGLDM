//
// Created by gbian on 29/06/2024.
//
// NOLINTBEGIN(*-include-cleaner *-easily-swappable-parameters)
#pragma once
#include "SGLDM/Core.hpp"

[[nodiscard]] inline std::string_view glSourceToString(GLenum source) noexcept {
    switch(source) {
    case GL_DEBUG_SOURCE_API:
        return "API";
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        return "Window System";
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        return "Shader Compiler";
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        return "Third Party";
    case GL_DEBUG_SOURCE_APPLICATION:
        return "Application";
    case GL_DEBUG_SOURCE_OTHER:
        return "Other";
    default:
        return "Unknown";
    }
}

[[nodiscard]] inline std::string_view glTypeToString(GLenum type) noexcept {
    switch(type) {
    case GL_DEBUG_TYPE_ERROR:
        return "Error";
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        return "Deprecated Behaviour";
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        return "Undefined Behaviour";
    case GL_DEBUG_TYPE_PORTABILITY:
        return "Portability";
    case GL_DEBUG_TYPE_PERFORMANCE:
        return "Performance";
    case GL_DEBUG_TYPE_MARKER:
        return "Marker";
    case GL_DEBUG_TYPE_PUSH_GROUP:
        return "Push Group";
    case GL_DEBUG_TYPE_POP_GROUP:
        return "Pop Group";
    case GL_DEBUG_TYPE_OTHER:
        return "Other";
    default:
        return "Unknown";
    }
}

[[nodiscard]] inline std::string_view glSeverityToString(GLenum severity) noexcept {
    switch(severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        return "High";
    case GL_DEBUG_SEVERITY_MEDIUM:
        return "Medium";
    case GL_DEBUG_SEVERITY_LOW:
        return "Low";
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        return "Notification";
    default:
        return "Unknown";
    }
}
inline void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, [[maybe_unused]] GLsizei length,
                                   const GLchar *message, [[maybe_unused]] const void *userParam) {
    // Ignore non-significant error/warning codes
    vnd::Timer timer("glDebugOutput");
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    // clang-format off
    auto sourceStr = glSourceToString(source);
    auto typeStr = glTypeToString(type);
    auto severityStr = glSeverityToString(severity);
    // clang-format on

    // Determine color based on severity
    switch(severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        LERROR("Debug message ({}): {}\nSource: {}\nType: {}\nSeverity: {}\n{}", id, message, sourceStr, typeStr, severityStr, timer);
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        LWARN("Debug message ({}): {}\nSource: {}\nType: {}\nSeverity: {}\n{}", id, message, sourceStr, typeStr, severityStr, timer);
        break;
    case GL_DEBUG_SEVERITY_LOW:
        LINFO("Debug message ({}): {}\nSource: {}\nType: {}\nSeverity: {}\n{}", id, message, sourceStr, typeStr, severityStr, timer);
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        LTRACE("Debug message ({}): {}\nSource: {}\nType: {}\nSeverity: {}\n{}", id, message, sourceStr, typeStr, severityStr, timer);
        break;
    default:
        LDEBUG("Debug message ({}): {}\nSource: {}\nType: {}\nSeverity: {}\n{}", id, message, sourceStr, typeStr, severityStr, timer);
        break;
    }
    DEBUG_BREAK;
}

// NOLINTEND(*-include-cleaner *-easily-swappable-parameters)
