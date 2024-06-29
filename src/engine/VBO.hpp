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
    VBO(GLfloat *vertices, GLsizeiptr size);

    // Binds the VBO
    void Bind() const;
    // Unbinds the VBO
    void Unbind() const;
    // Deletes the VBO
    void Delete() const;
};