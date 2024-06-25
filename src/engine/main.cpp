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
static inline void errorCallback(int error, const char *description) { LERROR("GLFW Error ({0}): {1}", error, description); }

static inline void keyCallback(GLFWwindow *window, int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
    switch(key) {
    case GLFW_KEY_ESCAPE:
        if(action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            LINFO("escape close");
        }
        break;
    // Add more cases for other keys if needed
    [[likely]] default:
        // Handle other keys here
        break;
    }
}
static void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); }

static inline constexpr auto WWIDTH = 800;
static inline constexpr auto WHEIGHT = 600;
static inline constexpr std::string_view WTITILE = "GLFW Window";
// NOLINTNEXTLINE(bugprone-exception-escape)
int main(int argc, const char **argv) {
    // NOLINTNEXTLINE
    INIT_LOG()

    try {
        vnd::Timer t("glfw setup");
        // Initialize GLFW
        if(!glfwInit()) [[unlikely]] {
            LCRITICAL("Failed to initialize GLFW.");
            return EXIT_FAILURE;
        }
        glfwSetErrorCallback(errorCallback);
        LINFO("{}", t);
        CLI::App app{FORMAT("{} version {}", SGLDM::cmake::project_name, SGLDM::cmake::project_version)};

        std::optional<std::string> message;
        app.add_option("-m,--message", message, "A message to print back out");
        bool show_version = false;
        app.add_flag("--version", show_version, "Show version information");

        CLI11_PARSE(app, argc, argv);

        if(show_version) {
            LINFO("{}\n", SGLDM::cmake::project_version);
            return EXIT_SUCCESS;
        }
        vnd::Timer tt("set glfw hints");
        // Set GLFW context version and profile
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        LINFO("{}", tt);
        vnd::Timer ttt("window creation");
        // Create a GLFW window
        GLFWwindow *window = glfwCreateWindow(WWIDTH, 600, WTITILE.data(), nullptr, nullptr);
        if(window == nullptr) [[unlikely]] {
            LCRITICAL("Failed to create GLFW window.");
            glfwTerminate();
            return EXIT_FAILURE;
        }
        glfwSetKeyCallback(window, keyCallback);
        // glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        LINFO("{}", ttt);

        // Ottenimento delle dimensioni del monitor primario
        vnd::Timer tttt("get primary Monitor");
        GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
        if(primaryMonitor == nullptr) [[unlikely]] {
            LCRITICAL("Failed to get the primary monitor.");
            glfwDestroyWindow(window);
            glfwTerminate();
            return EXIT_FAILURE;
        }
        LINFO("{}", tttt);

        vnd::Timer ttttt("get mode");
        static const GLFWvidmode *mode = glfwGetVideoMode(primaryMonitor);
        if(mode == nullptr) [[unlikely]] {
            LCRITICAL("Failed to get the video mode of the primary monitor.");
            glfwDestroyWindow(window);
            glfwTerminate();
            return EXIT_FAILURE;
        }
        LINFO("{}", ttttt);
        vnd::Timer tttttt("calculating for reposition");
        static const int monitorWidth = mode->width;
        static const int monitorHeight = mode->height;

        // Calcolo delle coordinate per centrare la finestra
        int windowWidth;
        int windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        static const int centerX = CALC_CENTRO(monitorWidth, windowWidth);
        static const int centerY = CALC_CENTRO(monitorHeight, windowHeight);
        LINFO("{}", tttttt);
        vnd::Timer ttttttt("window reposition");

        // Posizionamento della finestra al centro del monitor
        glfwSetWindowPos(window, centerX, centerY);

        int posX = 0;
        int posY = 0;
        glfwGetWindowPos(window, &posX, &posY);
        if(posX != centerX || posY != centerY) [[unlikely]] {
            LCRITICAL("Failed to position the window at the center.");
            glfwDestroyWindow(window);
            glfwTerminate();
            return EXIT_FAILURE;
        }
        LINFO("{}", ttttttt);
        LINFO("created the window {0}: (w: {1}, h: {2}, pos:(x:{3}, y:{4}))", WTITILE.data(), WWIDTH, WHEIGHT, centerX, centerY);
        glfwMakeContextCurrent(window);

        // Initialize GLAD
        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LERROR("Failed to initialize GLAD");
            return EXIT_FAILURE;
        }

        // Set the viewport and register the framebuffer size callback
        glViewport(0, 0, WWIDTH, WHEIGHT);  // NOLINT(*-avoid-magic-numbers)
        glfwShowWindow(window);

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
    } catch(const std::exception &e) { LERROR("Unhandled exception in main: {}", e.what()); }
}
// NOLINTEND(*-include-cleaner)
