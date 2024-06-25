// NOLINTBEGIN(*-include-cleaner)
#include <array>
#include <functional>
#include <iostream>
#include <optional>

#include <random>

#include <CLI/CLI.hpp>
#include <spdlog/spdlog.h>

// This file will be generated automatically when cur_you run the CMake
// configuration step. It creates a namespace called `SGLDM`. You can modify
// the source template at `configured_files/config.hpp.in`.
#include "SGLDM/Core.hpp"
#include <GLFW/glfw3.h>
#include <internal_use_only/config.hpp>

// Callback function for resizing the window
static void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); }

// NOLINTNEXTLINE(bugprone-exception-escape)
int main(int argc, const char **argv) {
    try {
        // Initialize GLFW
        if(!glfwInit()) {
            fmt::print("Failed to initialize GLFW\n");
            return EXIT_FAILURE;
        }
        CLI::App app{fmt::format("{} version {}", SGLDM::cmake::project_name, SGLDM::cmake::project_version)};

        std::optional<std::string> message;
        app.add_option("-m,--message", message, "A message to print back out");
        bool show_version = false;
        app.add_flag("--version", show_version, "Show version information");

        CLI11_PARSE(app, argc, argv);

        if(show_version) {
            fmt::print("{}\n", SGLDM::cmake::project_version);
            return EXIT_SUCCESS;
        }

        // Set GLFW context version and profile
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create a GLFW window
        GLFWwindow *window = glfwCreateWindow(800, 600, "GLFW Window", nullptr, nullptr);
        if(!window) {
            fmt::print("Failed to create GLFW window\n");
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);

        // Initialize GLAD
        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            fmt::print("Failed to initialize GLAD\n");
            return -1;
        }

        // Set the viewport and register the framebuffer size callback
        glViewport(0, 0, 800, 600);  // NOLINT(*-avoid-magic-numbers)
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        // Render loop
        while(!glfwWindowShouldClose(window)) {
            // Process input
            if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { glfwSetWindowShouldClose(window, true); }
            // Rendering commands here
            glClearColor(0.2F, 0.3F, 0.3F, 1.0F);  // NOLINT(*-avoid-magic-numbers)
            glClear(GL_COLOR_BUFFER_BIT);

            // Swap buffers and poll IO events
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        // Clean up resources
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_SUCCESS;
    } catch(const std::exception &e) { spdlog::error("Unhandled exception in main: {}", e.what()); }
}
// NOLINTEND(*-include-cleaner)
