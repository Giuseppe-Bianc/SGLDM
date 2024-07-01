//
// Created by gbian on 30/06/2024.
//

#include "Camera.hpp"

/*
void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane) {
    // Initializes matrices since otherwise they will be the null matrix
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    // Makes camera look in the right direction from the right position
    view = glm::lookAt(Position, Position + Orientation, Up);
    // Adds perspective to the scene
    projection = glm::perspective(glm::radians(FOVdeg), C_F(width / height), nearPlane, farPlane);

    // Sets new camera matrix
    cameraMatrix = projection * view;
}

void Camera::Matrix(const Shader &shader, const char *uniform) {
    // Exports camera matrix
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow *window, long double dt) {
    // Handles key inputs
    const glm::vec3 forwardDirection(0.0f, 0.0f, -1.0f);
    const glm::vec3 rightDirection = glm::normalize(glm::cross(Orientation, Up));
    float adjustedSpeed = speed * C_F(dt);

    // Cache key states
    bool moveForward = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS;
    bool moveBackward = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS;
    bool moveLeft = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS;
    bool moveRight = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS;
    bool moveUp = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
    bool moveDown = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;
    bool resetPosition = glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS;
    bool isFastKeyPressed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;

    // Adjust speed if fast key is pressed
    if(isFastKeyPressed) {
        speed = 0.4f;
    } else {
        speed = 0.2f;
    }

    // Move camera based on input
    if(moveForward) { Position += adjustedSpeed * forwardDirection; }
    if(moveBackward) { Position -= adjustedSpeed * forwardDirection; }
    if(moveLeft) { Position -= adjustedSpeed * rightDirection; }
    if(moveRight) { Position += adjustedSpeed * rightDirection; }
    if(moveUp) { Position += adjustedSpeed * Up; }
    if(moveDown) { Position -= adjustedSpeed * Up; }

    // Reset the camera position if 'R' key is pressed
    if(resetPosition) {
        Position = initialPosition;  // Assuming you have a member variable initialPosition set somewhere
    }

    // Handles mouse inputs
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        // Hides mouse cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // Prevents camera from jumping on the first click
        if(firstClick) {
            glfwSetCursorPos(window, C_D(halfwidth), C_D(halfheight));
            firstClick = false;
        }

        // Fetches the coordinates of the cursor
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // Calculate mouse movement
        const auto deltaX = C_F((sensitivity * (mouseX - halfwidth) / width) * dt);
        const auto deltaY = C_F((sensitivity * (mouseY - halfheight) / height) * dt);

        // Update orientation using quaternion rotation
        const auto rotX = glm::angleAxis(-deltaY, glm::normalize(glm::cross(Orientation, Up)));
        const auto rotY = glm::angleAxis(-deltaX, Up);
        LINFO("dtx {} dty {}", rotX, rotY);

        // Apply rotations
        Orientation = rotX * Orientation;
        Orientation = glm::normalize(Orientation * rotY);
        // Set cursor position to the middle of the screen
        glfwSetCursorPos(window, C_D(halfwidth), C_D(halfheight));
    } else {
        // Unhides cursor since camera is not looking around anymore
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        // Reset firstClick flag
        firstClick = true;
    }
}
*/