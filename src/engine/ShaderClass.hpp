//
// Created by gbian on 26/06/2024.
//

#pragma once

// NOLINTBEGIN(*-include-cleaner)
#include "SGLDM/Core.hpp"

enum SHADER_TYPE : std::uint8_t { VERTEX, FRAGMENT };
inline std::string_view shader_t_to_string(SHADER_TYPE e) {
    switch(e) {
    case VERTEX:
        return "VERTEX";
    case FRAGMENT:
        return "FRAGMENT";
    default:
        return "unknown";
    }
}
std::string get_file_contents(const char *filename);

class Shader {
public:
    GLuint ID;
    Shader(const char *vertexFile, const char *fragmentFile);

    void Activate() const;
    void Delete() const;
    static void checkShaderCompilation(GLuint shader, SHADER_TYPE shaderType);
    static void checkProgranlinking(GLuint program);
};
// NOLINTEND(*-include-cleaner)