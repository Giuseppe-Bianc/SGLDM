//
// Created by gbian on 26/06/2024.
//

#pragma once

#include "SGLDM/Core.hpp"

class VBO {
public:
    // Reference ID of the Vertex Buffer Object
    GLuint ID;
    // Constructor that generates a Vertex Buffer Object and links it to vertices
    VBO(GLfloat *vertices, GLsizeiptr size) noexcept;

    // Binds the VBO
    void Bind() noexcept;
    // Unbinds the VBO
    void Unbind() noexcept;
    // Deletes the VBO
    void Delete() noexcept;
};
