//
// Created by gbian on 26/06/2024.
//

#pragma once

#include "SGLDM/Core.hpp"

class EBO {
public:
    // ID reference of Elements Buffer Object
    GLuint ID;
    // Constructor that generates a Elements Buffer Object and links it to indices
    EBO(GLuint *indices, GLsizeiptr size) noexcept;

    // Binds the EBO
    void Bind() const noexcept;
    // Unbinds the EBO
    void Unbind() const noexcept;
    // Deletes the EBO
    void Delete() const noexcept;
};
