//
// Created by gbian on 30/06/2024.
//

#pragma once

#include "ShaderClass.hpp"

class Camera {
public:
    glm::vec3 Position, initialPosition;
    glm::vec3 Orientation = glm::vec3(0.0F, 0.0F, -1.0F);
    glm::vec3 Up = glm::vec3(0.0F, 1.0F, 0.0F);
    // Prevents the camera from jumping around when first clicking left click
    bool firstClick = true;
    int width;
    int height;
    float speed = 0.1F;
    float sensitivity = 100.0F;

    Camera(const int width, const int height, const glm::vec3 &position)
      : Position(position), width(width), height(height), initialPosition(position) {}

    // Updates and exports the camera matrix to the Vertex Shader
    void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader &shader, const char *uniform);
    // Handles camera inputs
    void Inputs(GLFWwindow *window);
};
