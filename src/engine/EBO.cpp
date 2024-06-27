//
// Created by gbian on 26/06/2024.
//

#include "EBO.hpp"

// Constructor that generates a Elements Buffer Object and links it to indices
EBO::EBO(GLuint *indices, GLsizeiptr size) {
    glCheckError(glCheckError(glGenBuffers(1, &ID)));
    glCheckError(glCheckError(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID)));
    glCheckError(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW));
}

// Binds the EBO
void EBO::Bind() { glCheckError(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID)); }

// Unbinds the EBO
void EBO::Unbind() { glCheckError(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); }

// Deletes the EBO
void EBO::Delete() { glCheckError(glDeleteBuffers(1, &ID)); }