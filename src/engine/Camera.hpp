//
// Created by gbian on 30/06/2024.
//

// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "ShaderClass.hpp"
/*
class Camera {
public:
    glm::vec3 Position, initialPosition;
    glm::vec3 Orientation = glm::vec3(0.0F, 0.0F, -1.0F);
    glm::vec3 Up = glm::vec3(0.0F, 1.0F, 0.0F);
    glm::mat4 cameraMatrix = glm::mat4(1.0f);
    // Prevents the camera from jumping around when first clicking left click
    bool firstClick = true;
    int width;
    int height;
    long double halfwidth;
    long double halfheight;
    float speed = 0.2F;
    float sensitivity = 100.0F;

    Camera(const int width, const int height, const glm::vec3 &position)
      : Position(position), width(width), height(height), halfwidth(width / 2.0L), halfheight(height / 2.0L), initialPosition(position) {}

    // Updates the camera matrix to the Vertex Shader
    void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
    void Matrix(const Shader &shader, const char *uniform);
    // Handles camera inputs
    void Inputs(GLFWwindow *window, long double dt);
};
*/

class Camera {
public:
    glm::vec3 Position, initialPosition;
    glm::dquat Orientation = glm::dquat(glm::dvec3(0.0, 0.0, .0));
    glm::dvec3 Up = glm::dvec3(0.0f, 1.0f, 0.0f);
    glm::mat4 cameraMatrix = glm::mat4(1.0f);
    bool firstClick = true;
    int width;
    int height;
    float speed = 0.2f;
    float sensitivity = 1.f;

    Camera(int width, int height, const glm::vec3 &position)
      : Position(position), width(width), height(height), initialPosition(position) {}

    void updateMatrix(float FOVdeg, float nearPlane, float farPlane) {
        const auto view = glm::translate(glm::dmat4(1.0f), -glm::dvec3(Position)) * glm::toMat4(Orientation);
        const auto projection = glm::perspective(glm::radians(C_D(FOVdeg)), C_D(width / height), C_D(nearPlane), C_D(farPlane));
        cameraMatrix = glm::mat4(projection) * glm::mat4(view);
    }

    void Matrix(const Shader &shader, const char *uniform) {
        const auto uniformLoc = glGetUniformLocation(shader.ID, uniform);
        glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(cameraMatrix));
    }

    void Inputs(GLFWwindow *window, long double dt) {
        auto dtf = C_D(dt);

        // Helper function to check key states
        const auto isKeyPressed = [&](int key) { return glfwGetKey(window, key) == GLFW_PRESS; };

        // Cache key states
        bool moveForward = isKeyPressed(GLFW_KEY_W) || isKeyPressed(GLFW_KEY_UP);
        bool moveBackward = isKeyPressed(GLFW_KEY_S) || isKeyPressed(GLFW_KEY_DOWN);
        bool moveLeft = isKeyPressed(GLFW_KEY_A) || isKeyPressed(GLFW_KEY_LEFT);
        bool moveRight = isKeyPressed(GLFW_KEY_D) || isKeyPressed(GLFW_KEY_RIGHT);
        bool moveUp = isKeyPressed(GLFW_KEY_SPACE);
        bool moveDown = isKeyPressed(GLFW_KEY_LEFT_CONTROL);
        bool resetPosition = isKeyPressed(GLFW_KEY_R);
        bool isFastKeyPressed = isKeyPressed(GLFW_KEY_LEFT_SHIFT);
        bool leftmousebutton = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

        // Reset position if 'R' key is pressed
        if(resetPosition) {
            Position = initialPosition;
            Orientation = glm::dquat(glm::dvec3(0.0, 0.0, 0.0));
        }

        // Calculate speed based on whether shift key is pressed
        const auto currentSpeed = speed * (isFastKeyPressed ? 2.0f : 1.0f) * dtf;

        // Define direction vectors
        const auto forwardDirection = glm::rotate(Orientation, glm::dvec3(0.0, 0.0, -1.0));
        const auto backwardDirection = glm::rotate(Orientation, glm::dvec3(0.0, 0.0, 1.0));
        const auto leftDirection = glm::rotate(Orientation, glm::dvec3(-1.0, 0.0, 0.0));
        const auto rightDirection = glm::rotate(Orientation, glm::dvec3(1.0, 0.0, 0.0));

        // Move forward and backward (along camera's view direction)
        if(moveForward) { Position += currentSpeed * forwardDirection; }
        if(moveBackward) { Position += currentSpeed * backwardDirection; }

        // Move left and right (perpendicular to camera's view direction)
        if(moveLeft) { Position += currentSpeed * leftDirection; }
        if(moveRight) { Position += currentSpeed * rightDirection; }

        // Move up and down
        if(moveUp) { Position += currentSpeed * Up; }
        if(moveDown) { Position -= currentSpeed * Up; }

        // Rotate camera with left mouse button pressed
        if(leftmousebutton) {
            double mouseX;
            double mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);
            if(firstClick) {
                lastMouseX = mouseX;
                lastMouseY = mouseY;
                firstClick = false;
            }

            auto deltaX = (mouseX - lastMouseX) * dtf;
            auto deltaY = (mouseY - lastMouseY) * dtf;

            auto rotX = glm::angleAxis(sensitivity * deltaX, Up);
            auto rotY = glm::angleAxis(sensitivity * deltaY, glm::cross(Up, glm::dvec3(Orientation * glm::dvec4(0, 0, -1, 0))));

            Orientation = glm::normalize(rotX * rotY * Orientation);

            lastMouseX = mouseX;
            lastMouseY = mouseY;
        } else {
            firstClick = true;
        }
    }

private:
    double lastMouseX = 0.0;
    double lastMouseY = 0.0;
};
// NOLINTEND(*-include-cleaner)