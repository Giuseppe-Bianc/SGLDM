add_executable(engine main.cpp
        ShaderClass.cpp
        ShaderClass.hpp
        VBO.cpp
        VBO.hpp
        VAO.cpp
        VAO.hpp
        EBO.cpp
        EBO.hpp
        OpenGLContext.cpp
        OpenGLContext.hpp
        openglCalback.hpp
        Texture.cpp
        Texture.hpp
        Camera.cpp
        Camera.hpp)

target_link_libraries(
        engine
        PRIVATE SGLDM::SGLDM_options
        SGLDM::SGLDM_warnings)

target_link_system_libraries(
        engine
        PRIVATE
        CLI11::CLI11
        Core
)

target_include_directories(engine PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
