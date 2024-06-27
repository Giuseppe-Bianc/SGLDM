//
// Created by gbian on 26/06/2024.
//

#include "VBO.hpp"

// Constructor that generates a Vertex Buffer Object and links it to vertices
VBO::VBO(GLfloat *vertices, GLsizeiptr size) {
    glCheckError(glGenBuffers(1, &ID));
    glCheckError(glBindBuffer(GL_ARRAY_BUFFER, ID));
    glCheckError(glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW));
}

// Binds the VBO
void VBO::Bind() { glCheckError(glBindBuffer(GL_ARRAY_BUFFER, ID)); }

// Unbinds the VBO
void VBO::Unbind() { glCheckError(glBindBuffer(GL_ARRAY_BUFFER, 0)); }

// Deletes the VBO
void VBO::Delete() { glCheckError(glDeleteBuffers(1, &ID)); }