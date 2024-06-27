//
// Created by gbian on 26/06/2024.
//

#include "VAO.hpp"

VAO::VAO() { glGenVertexArrays(1, &ID); }

// Links a VBO to the VAO using a certain layout
void VAO::LinkAttrib(VBO &VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void *offset) {
    VBO.Bind();
    glCheckError(glVertexAttribPointer(layout, numComponents, type, GL_FALSE, static_cast<GLsizei>(stride), offset));
    glCheckError(glEnableVertexAttribArray(layout));
    VBO.Unbind();
}

// Binds the VAO
void VAO::Bind() { glCheckError(glBindVertexArray(ID)); }

// Unbinds the VAO
void VAO::Unbind() { glCheckError(glBindVertexArray(0)); }

// Deletes the VAO
void VAO::Delete() { glCheckError(glDeleteVertexArrays(1, &ID)); }