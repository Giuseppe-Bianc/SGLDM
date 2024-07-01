//
// Created by gbian on 26/06/2024.
//

#include "VAO.hpp"

DISABLE_WARNINGS_PUSH(26472)

VAO::VAO() noexcept { glGenVertexArrays(1, &ID); }  // NOLINT(*-pro-type-member-init)

// Links a VBO to the VAO using a certain layout
void VAO::LinkAttrib(VBO &VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void *offset) noexcept {
    VBO.Bind();
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, static_cast<GLsizei>(stride), offset);
    glEnableVertexAttribArray(layout);
    VBO.Unbind();
}

// Binds the VAO
void VAO::Bind() noexcept { glBindVertexArray(ID); }

// Unbinds the VAO
void VAO::Unbind() noexcept { glBindVertexArray(0); }

// Deletes the VAO
void VAO::Delete() noexcept { glDeleteVertexArrays(1, &ID); }

DISABLE_WARNINGS_POP()
