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
#include "EBO.hpp"
#include "OpenGLContext.hpp"
#include "SGLDM/Core.hpp"
#include "ShaderClass.hpp"
#include "VAO.hpp"
#include "openglCalback.hpp"

#include <internal_use_only/config.hpp>

static inline constexpr auto sizefactor = 67.5;
static inline constexpr auto WWIDTH = C_I(sizefactor * 16);
static inline constexpr auto WHEIGHT = C_I(sizefactor * 9);
static inline constexpr std::string_view WTITILE = "GLFW Window";
static inline constexpr float sqrt3 = 1.7320507764816284180;

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
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
        // glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
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
        /*LINFO("Vendor:          {}", glGetString(GL_VENDOR));
            LINFO("Renderer:        {}", glGetString(GL_RENDERER));
            LINFO("Version OpenGL:  {}", glGetString(GL_VERSION));
            LINFO("Version GLSL:    {}", glGetString(GL_SHADING_LANGUAGE_VERSION));*/

        // Initialize GLAD
        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LERROR("Failed to initialize GLAD");
            return EXIT_FAILURE;
        }

        int flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if(flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);  // Ensure that errors are reported synchronously
            glDebugMessageCallback(glDebugOutput, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);  // Enable all messages
        }
        // Set the viewport and register the framebuffer size callback
        OpenGLContext open_gl_context;
        open_gl_context.logOpenGLInfo();

        glViewport(0, 0, WWIDTH, WHEIGHT);  // NOLINT(*-avoid-magic-numbers)
        glfwSwapInterval(1);
        glfwShowWindow(window);
        // Vertex data
        std::array<GLfloat, 36> vertices = {
            -0.5F,  -0.5F * sqrt3 * 1 / 3, 0.0F, 0.8F, 0.3F,  0.02F,  // Lower left corner
            0.5F,   -0.5F * sqrt3 * 1 / 3, 0.0F, 0.8F, 0.3F,  0.02F,  // Lower right corner
            0.0F,   0.5F * sqrt3 * 2 / 3,  0.0F, 1.0F, 0.6F,  0.32F,  // Upper corner
            -0.25F, 0.5F * sqrt3 * 1 / 6,  0.0F, 0.9F, 0.45F, 0.17F,  // Inner left
            0.25F,  0.5F * sqrt3 * 1 / 6,  0.0F, 0.9F, 0.45F, 0.17F,  // Inner right
            0.0F,   -0.5F * sqrt3 * 1 / 3, 0.0F, 0.8F, 0.3F,  0.02F   // Inner down
        };

        // Defining the indices array using std::array
        std::array<GLuint, 9> indices = {
            0, 3, 5,  // Lower left triangle
            3, 2, 4,  // Lower right triangle
            5, 4, 1   // Upper triangle
        };
        //  Generates Shader object using shaders defualt.vert and default.frag
        Shader shaderProgram("../../../src/engine/default.vert", "../../../src/engine/default.frag");

        // Generates Vertex Array Object and binds it
        VAO VAO1;
        VAO1.Bind();

        // Generates Vertex Buffer Object and links it to vertices
        VBO VBO1(vertices.data(), sizeof(vertices));
        // Generates Element Buffer Object and links it to indices
        EBO EBO1(indices.data(), sizeof(indices));

        // Links VBO to VAO
        VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void *)0);
        VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void *)(3 * sizeof(float)));
        // Unbind all to prevent accidentally modifying them
        VAO1.Unbind();
        VBO1.Unbind();
        EBO1.Unbind();

        // Render loop
        // Gets ID of uniform called "scale"
        GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

        long double m_LastFrameTime = 0;
        float step = 0;
        static long double times = 0.0L;
        // Main while loop
        while(!glfwWindowShouldClose(window)) {
            auto time = C_LD(glfwGetTime());
            auto timestep = Timestep(time - m_LastFrameTime);
            m_LastFrameTime = time;
            glClearColor(0.2F, 0.3F, 0.3F, 1.0F);  // NOLINT(*-avoid-magic-numbers)
            glClear(GL_COLOR_BUFFER_BIT);
            shaderProgram.Activate();

            // Increment the time
            times += timestep.GetMilliseconds() / 150;
            step = std::clamp(C_F((1.0L + sin(times)) / 2.0L), 0.0F, 0.75F);
            // Pass the step value to the shader
            glUniform1f(uniID, 0.5F);
            VAO1.Bind();
            glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
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
