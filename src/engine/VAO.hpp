//
// Created by gbian on 26/06/2024.
//

#pragma once

#include "VBO.hpp"

class VAO {
public:
    // ID reference for the Vertex Array Object
    GLuint ID;
    // Constructor that generates a VAO ID
    VAO() noexcept;

    // Links a VBO to the VAO using a certain layout
    void LinkAttrib(const VBO &VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void *offset) const noexcept;
    // Binds the VAO
    void Bind() const noexcept;
    // Unbinds the VAO
    void Unbind() const noexcept;
    // Deletes the VAO
    void Delete() const noexcept;
};
