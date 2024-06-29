//
// Created by gbian on 26/06/2024.
//

#include "VAO.hpp"

VAO::VAO() { glGenVertexArrays(1, &ID); }  // NOLINT(*-pro-type-member-init)

// Links a VBO to the VAO using a certain layout
void VAO::LinkAttrib(VBO &VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void *offset) {
    VBO.Bind();
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, static_cast<GLsizei>(stride), offset);
    glEnableVertexAttribArray(layout);
    VBO.Unbind();
}

// Binds the VAO
void VAO::Bind() const { glBindVertexArray(ID); }

// Unbinds the VAO
void VAO::Unbind() const { glBindVertexArray(0); }

// Deletes the VAO
void VAO::Delete() const { glDeleteVertexArrays(1, &ID); }