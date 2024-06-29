//
// Created by gbian on 26/06/2024.
//

// NOLINTBEGIN(*-include-cleaner)
#include "ShaderClass.hpp"

std::string get_file_contents(const char *filename) {
    const auto &filePath = fs::path(filename);
    if(!fs::exists(filePath)) { throw std::runtime_error(FORMAT("File not found: {}", filePath)); }
    if(!fs::is_regular_file(filePath)) { throw std::runtime_error(FORMAT("Path is not a regular file: {}", filePath)); }

    vnd::AutoTimer timer(FORMAT("reading shader {}", filename));
    std::stringstream buffer;
    // NOLINTNEXTLINE(*-include-cleaner,  hicpp-signed-bitwise)
    if(std::ifstream fileStream{filePath, std::ios::in | std::ios::binary}; fileStream.is_open()) {
        // Ensure
        // that the file is opened securely
        fileStream.exceptions(std::ios::failbit | std::ios::badbit);  // NOLINT(hicpp-signed-bitwise)

        try {
            buffer << fileStream.rdbuf();
        } catch(const std::ios_base::failure &e) {
            throw std::runtime_error(FORMAT("Unable to read file: {}. Reason: {}", filePath, e.what()));
        }
    } else {
        // Handle the case when the file cannot be opened,
        // You might throw an exception or return an error indicator
        throw std::runtime_error(FORMAT("Unable to open file: {}", filePath));
    }

    // Extract the content as a string
    return buffer.str();
}

void Shader::checkShaderCompilation(GLuint shader, SHADER_TYPE shaderType) {
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> infoLog(logLength);
        glGetShaderInfoLog(shader, logLength, nullptr, infoLog.data());
        // Get the source code lines of the shader that caused the error
        GLint sourceLength = 0;
        glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &sourceLength);
        std::vector<char> sourceCode(sourceLength);
        glGetShaderSource(shader, sourceLength, nullptr, sourceCode.data());
        LERROR("SHADER::{}::COMPILATION_FAILED\nShader source:\n{}\nCompilation error:\n{}", shader_t_to_string(shaderType),
               std::string_view(sourceCode.data()), std::string_view(infoLog.data()));
    }
}

void Shader::checkProgranlinking(GLuint program) {
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        GLint logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> infoLog(logLength);
        glGetProgramInfoLog(program, logLength, nullptr, infoLog.data());
        LERROR("SHADER::PROGRAM::COMPILATION_FAILED\n{}", std::string_view(infoLog.data()));
    }
}
Shader::Shader(const char *vertexFile, const char *fragmentFile) {
    // Read vertexFile and fragmentFile and store the strings
    std::string vertexCode = get_file_contents(vertexFile);
    std::string fragmentCode = get_file_contents(fragmentFile);
    // Convert the shader source strings into character arrays
    const char *vertexSource = vertexCode.c_str();
    const char *fragmentSource = fragmentCode.c_str();

    // Create Vertex Shader Object and get its reference
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Attach Vertex Shader source to the Vertex Shader Object
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    // Compile the Vertex Shader into machine code
    glCompileShader(vertexShader);
    checkShaderCompilation(vertexShader, SHADER_TYPE::VERTEX);

    // Create Fragment Shader Object and get its reference
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Attach Fragment Shader source to the Fragment Shader Object
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    // Compile the Vertex Shader into machine code
    glCompileShader(fragmentShader);
    checkShaderCompilation(fragmentShader, SHADER_TYPE::FRAGMENT);
    // Create Shader Program Object and get its reference
    ID = glCreateProgram();
    // Attach the Vertex and Fragment Shaders to the Shader Program
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    // Wrap-up/Link all the shaders together into the Shader Program
    glLinkProgram(ID);
    checkProgranlinking(ID);
}
void Shader::Activate() const { glUseProgram(ID); }

// Deletes the Shader Program
void Shader::Delete() const { glDeleteProgram(ID); }
// NOLINTEND(*-include-cleaner)