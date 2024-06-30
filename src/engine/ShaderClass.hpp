//
// Created by gbian on 26/06/2024.
//

#pragma once

// NOLINTBEGIN(*-include-cleaner)
#include "SGLDM/Core.hpp"

enum SHADER_TYPE : std::uint8_t { VERTEX, FRAGMENT };
inline std::string_view shader_t_to_string(SHADER_TYPE e) noexcept {
    switch(e) {
    case VERTEX:
        return "VERTEX";
    case FRAGMENT:
        return "FRAGMENT";
    default:
        return "unknown";
    }
}
std::string get_file_contents(const fs::path &filename);

class Shader {
public:
    GLuint ID;
    Shader(const fs::path &vertexFile, const fs::path &fragmentFile);

    void Activate() const noexcept;
    void Delete() const noexcept;
    static void checkShaderCompilation(GLuint shader, SHADER_TYPE shaderType);
    static void checkProgranlinking(GLuint program);
};
// NOLINTEND(*-include-cleaner)