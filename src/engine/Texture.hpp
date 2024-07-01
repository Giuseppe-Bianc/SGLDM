//
// Created by gbian on 29/06/2024.
//

#pragma once

#include "SGLDM/Core.hpp"
#include "ShaderClass.hpp"

class Texture {
public:
    GLuint ID;
    GLenum type;
    Texture(const fs::path &image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

    // Assigns a texture unit to a texture
    void texUnit(const Shader &shader, const char *uniform, GLuint unit) noexcept;
    // Binds a texture
    void Bind() const noexcept;
    // Unbinds a texture
    void Unbind() const noexcept;
    // Deletes a texture
    void Delete() const noexcept;
};
