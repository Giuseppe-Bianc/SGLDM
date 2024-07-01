//
// Created by gbian on 26/06/2024.
//

#include "EBO.hpp"

// Constructor that generates a Elements Buffer Object and links it to indices
EBO::EBO(GLuint *indices, GLsizeiptr size) noexcept {  // NOLINT(*-pro-type-member-init)
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

// Binds the EBO
void EBO::Bind() noexcept { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); }

// Unbinds the EBO
void EBO::Unbind() noexcept { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

// Deletes the EBO
void EBO::Delete() noexcept { glDeleteBuffers(1, &ID); }
