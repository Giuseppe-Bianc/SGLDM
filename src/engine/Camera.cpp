//
// Created by gbian on 30/06/2024.
//

#include "Camera.hpp"

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader &shader, const char *uniform) {
    // Initializes matrices since otherwise they will be the null matrix
    auto view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    // Makes camera look in the right direction from the right position
    view = glm::lookAt(Position, Position + Orientation, Up);
    // Adds perspective to the scene
    projection = glm::perspective(glm::radians(FOVdeg), C_F(width / height), nearPlane, farPlane);

    // Exports the camera matrix to the Vertex Shader
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}

void Camera::Inputs(GLFWwindow *window) {
    // Handles key inputs
    glm::vec3 forwardDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 rightDirection = glm::normalize(glm::cross(Orientation, Up));

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        Position += speed * forwardDirection;
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        Position -= speed * rightDirection;
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        Position -= speed * forwardDirection;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        Position += speed * rightDirection;
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) { Position += speed * Up; }
    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) { Position -= speed * Up; }
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        speed = 0.4f;
    } else {
        speed = 0.1f;
    }
    LINFO("position {}", Position);

    // Handle key for resetting position
    if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        // Reset the camera position
        Position = initialPosition;  // Assuming you have a member variable initialPosition set somewhere
    }

    // Handles mouse inputs
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        // Hides mouse cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // Prevents camera from jumping on the first click
        if(firstClick) {
            glfwSetCursorPos(window, (width / 2), (height / 2));
            firstClick = false;
        }

        // Fetches the coordinates of the cursor
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // Calculate mouse movement
        float deltaX = sensitivity * C_F(mouseX - (width / 2.0)) / width;
        float deltaY = sensitivity * C_F(mouseY - (height / 2.f)) / height;

        // Update orientation using quaternion rotation
        auto rotX = glm::angleAxis(-deltaY, glm::normalize(glm::cross(Orientation, Up)));
        auto rotY = glm::angleAxis(-deltaX, Up);

        // Apply rotations
        Orientation = rotX * Orientation;
        Orientation = glm::normalize(Orientation * rotY);

        // Set cursor position to the middle of the screen
        glfwSetCursorPos(window, (width / 2.0), (height / 2.0));
    } else {
        // Unhides cursor since camera is not looking around anymore
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        // Reset firstClick flag
        firstClick = true;
    }
}
