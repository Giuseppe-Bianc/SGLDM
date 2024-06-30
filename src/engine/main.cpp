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
#include "Texture.hpp"
#include "VAO.hpp"
#include "openglCalback.hpp"

#include <internal_use_only/config.hpp>

static inline constexpr auto sizefactor = 67.5;
static inline constexpr auto WWIDTH = C_I(sizefactor * 16);
static inline constexpr auto WHEIGHT = C_I(sizefactor * 9);
static inline constexpr std::string_view WTITILE = "GLFW Window";
static inline constexpr float sqrt3 = 1.7320507764816284180;
static inline constexpr auto indexOffset = 3 * TypeSizes::sizeOfFloat;
static inline constexpr auto textutreOffset = 6 * TypeSizes::sizeOfFloat;
void setHints() {
    vnd::AutoTimer timer("set glfw hints");
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
}

void initializeGLFW() {
    vnd::AutoTimer timer("glfw setup");
    if(!glfwInit()) { throw std::runtime_error("Failed to initialize GLFW."); }
    glfwSetErrorCallback(errorCallback);
}

GLFWwindow *createWindow() {
    vnd::AutoTimer ttt("window creation");
    GLFWwindow *window = glfwCreateWindow(WWIDTH, WHEIGHT, WTITILE.data(), nullptr, nullptr);
    if(!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window.");
    }
    glfwSetKeyCallback(window, keyCallback);
    // glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;
}

std::pair<int, int> centerWindow(GLFWwindow *window) {
    vnd::Timer monitort("get primary Monitor");
    GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
    if(!primaryMonitor) {
        glfwDestroyWindow(window);
        glfwTerminate();
        throw std::runtime_error("Failed to get the primary monitor.");
    }
    LINFO("{}", monitort);
    vnd::Timer modet("get mode");
    const GLFWvidmode *mode = glfwGetVideoMode(primaryMonitor);
    if(!mode) {
        glfwDestroyWindow(window);
        glfwTerminate();
        throw std::runtime_error("Failed to get the video mode of the primary monitor.");
    }
    LINFO("{}", modet);
    vnd::Timer crepositiont("calculating for reposition");
    const int monitorWidth = mode->width;
    const int monitorHeight = mode->height;
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    auto centerX = CALC_CENTRO(monitorWidth, windowWidth);
    auto centerY = CALC_CENTRO(monitorHeight, windowHeight);
    LINFO("{}", crepositiont);
    vnd::Timer wrepositiont("window reposition");
    glfwSetWindowPos(window, centerX, centerY);
    int posX = 0, posY = 0;
    glfwGetWindowPos(window, &posX, &posY);
    if(posX != centerX || posY != centerY) {
        glfwDestroyWindow(window);
        glfwTerminate();
        throw std::runtime_error("Failed to position the window at the center.");
    }
    LINFO("{}", wrepositiont);
    return std::make_pair(centerX, centerY);
}

void initializeGLAD() {
    vnd::AutoTimer timer("init glad");
    if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) { throw std::runtime_error("Failed to initialize GLAD"); }
}  // NOLINTNEXTLINE(bugprone-exception-escape)
void setOpenGlDebugContext() {
    vnd::AutoTimer timer("set openGl debug context");
    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if(flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);  // Ensure that errors are reported synchronously
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);  // Enable all messages
    }
}

fs::path calculateRelativePathToSrcRes(const fs::path &executablePath, const fs::path &targetFile) {
    // Get the parent directory of the executable path
    fs::path parentDir = executablePath.parent_path();

    // Traverse up the directory tree until we find a directory containing "src"
    while(!fs::exists(parentDir / "src")) {
        parentDir = parentDir.parent_path();
        // Check if we reached the root directory and "src" was not found
        if(parentDir == parentDir.root_path()) {
            std::cerr << "Error: 'src' directory not found in the path." << std::endl;
            return "";  // Return an empty path or handle error as needed
        }
    }

    // Move up one more level to reach the parent directory of "src"
    parentDir = parentDir.parent_path();
    auto resp = fs::path("src") / "engine" / "res";
    // Construct the relative path to the target file
    auto relativePathToTarget = parentDir / resp / targetFile;
    // Construct the path to the target file under "src/engine/res"

    // Calculate the relative path from the executable's directory
    auto relativePath = fs::relative(relativePathToTarget, executablePath);

    return relativePath.lexically_normal();
}
auto main(int argc, const char *const argv[]) -> int {
    // NOLINTNEXTLINE
    INIT_LOG()

    try {
        // Initialize GLFW
        initializeGLFW();
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
        setHints();
        GLFWwindow *window = createWindow();
        const auto [cx, cy] = centerWindow(window);
        glfwMakeContextCurrent(window);
        // Initialize GLAD
        initializeGLAD();
        setOpenGlDebugContext();
        // Set the viewport and register the framebuffer size callback
        OpenGLContext open_gl_context;
        open_gl_context.logOpenGLInfo();

        glViewport(0, 0, WWIDTH, WHEIGHT);  // NOLINT(*-avoid-magic-numbers)
        glfwSwapInterval(1);
        LINFO("created the window {0}: (w: {1}, h: {2}, pos:(x:{3}, y:{4}))", WTITILE.data(), WWIDTH, WHEIGHT, cx, cy);
        glfwShowWindow(window);
        // Vertices coordinates and colors
        std::array<GLfloat, 32> vertices = {
            // Coordinates          // Colors
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // Lower left corner, red
            -0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // Upper left corner, green
            0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // Upper right corner, blue
            0.5f,  -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,  // Lower right corner, white
        };

        // Indices for vertices order
        std::array<GLuint, 6> indices = {
            0, 2, 1,  // First triangle
            0, 3, 2   // Second triangle
        };
        // Get the current path of the executable
        const fs::path currentPath = fs::current_path();
        fs::path pop_catPath = calculateRelativePathToSrcRes(currentPath, fs::path("pop_cat.png"));
        fs::path vertPath = calculateRelativePathToSrcRes(currentPath, fs::path("default.vert"));
        fs::path fragPath = calculateRelativePathToSrcRes(currentPath, fs::path("default.frag"));
        //   Generates Shader object using shaders defualt.vert and default.frag
        const auto shaderProgram = Shader(vertPath, fragPath);

        // Generates Vertex Array Object and binds it
        const VAO VAO1;
        VAO1.Bind();

        // Generates Vertex Buffer Object and links it to vertices
        const VBO VBO1(vertices.data(), sizeof(vertices));
        // Generates Element Buffer Object and links it to indices
        const EBO EBO1(indices.data(), sizeof(indices));

        // Links VBO to VAO
        VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), nullptr);
        VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), std::bit_cast<void *>(indexOffset));
        VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), std::bit_cast<void *>(textutreOffset));
        // Unbind all to prevent accidentally modifying them
        VAO1.Unbind();
        VBO1.Unbind();
        EBO1.Unbind();

        // Render loop
        // Gets ID of uniform called "scale"
        const GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

        Texture popCat(pop_catPath, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
        popCat.texUnit(shaderProgram, "tex0", 0);

        long double m_LastFrameTime = 0;
        // float step = 0;
        static long double times = 0.0L;
        // Main while loop
        while(!glfwWindowShouldClose(window)) {
            const auto time = C_LD(glfwGetTime());
            const auto timestep = Timestep(time - m_LastFrameTime);
            m_LastFrameTime = time;
            glClearColor(0.2F, 0.3F, 0.3F, 1.0F);  // NOLINT(*-avoid-magic-numbers)
            glClear(GL_COLOR_BUFFER_BIT);
            shaderProgram.Activate();

            // Increment the time
            // times += timestep.GetMilliseconds() / 150;
            // step = std::clamp(C_F((1.0L + sin(times)) / 2.0L), 0.0F, 0.75F);
            // Pass the step value to the shader
            glUniform1f(uniID, 0.5F);
            popCat.Bind();
            VAO1.Bind();
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        // Clean up resources
        VAO1.Delete();
        VBO1.Delete();
        popCat.Delete();
        shaderProgram.Delete();
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_SUCCESS;
    } catch(const std::exception &e) { LERROR("Unhandled exception in main: {}", e.what()); }
}
// NOLINTEND(*-include-cleaner)
