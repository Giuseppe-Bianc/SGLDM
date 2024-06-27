//
// Created by gbian on 26/06/2024.
//

#pragma once

// NOLINTBEGIN(*-include-cleaner)
#include "SGLDM/Core.hpp"

std::string get_file_contents(const char *filename);

class Shader {
public:
    GLuint ID;
    Shader(const char *vertexFile, const char *fragmentFile);

    void Activate();
    void Delete();
};
// NOLINTEND(*-include-cleaner)