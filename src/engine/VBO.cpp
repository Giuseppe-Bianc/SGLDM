//
// Created by gbian on 26/06/2024.
//

#include "VBO.hpp"

// Constructor that generates a Vertex Buffer Object and links it to vertices
VBO::VBO(GLfloat *vertices, GLsizeiptr size) noexcept {
    assert(vertices != nullptr && size > 0);
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

// Binds the VBO
void VBO::Bind() noexcept { glBindBuffer(GL_ARRAY_BUFFER, ID); }

// Unbinds the VBO
void VBO::Unbind() noexcept { glBindBuffer(GL_ARRAY_BUFFER, 0); }

// Deletes the VBO
void VBO::Delete() noexcept { glDeleteBuffers(1, &ID); }
