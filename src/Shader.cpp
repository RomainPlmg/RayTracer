#include "Shader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include "GLContext.hpp"

Shader::Shader(const std::string& computePath) {
    std::string code = readFile(computePath);
    const char* cCode = code.c_str();

    // Create the shader
    GLuint shader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(shader, 1, &cCode, nullptr);
    glCompileShader(shader);
    checkCompileErrors(shader, "COMPUTE");

    // Create the program
    m_id = glCreateProgram();
    glAttachShader(m_id, shader);
    glLinkProgram(m_id);
    checkCompileErrors(m_id, "PROGRAM");

    // After linking, delete the shader
    glDeleteShader(shader);
}

Shader::~Shader() { glDeleteProgram(m_id); }

void Shader::use() const { glUseProgram(m_id); }

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
    glProgramUniformMatrix4fv(m_id, getLoc(name), 1, GL_FALSE,
                              glm::value_ptr(mat));
}

void Shader::setVec3(const std::string& name, const glm::vec3& vec) const {
    glProgramUniform3fv(m_id, getLoc(name), 1, glm::value_ptr(vec));
}

void Shader::setFloat(const std::string& name, const float val) const {
    glProgramUniform1fv(m_id, getLoc(name), 1, &val);
}

void Shader::setInt(const std::string& name, const int val) const {
    glProgramUniform1iv(m_id, getLoc(name), 1, &val);
}

void Shader::setUInt(const std::string& name, const uint32_t val) const {
    glProgramUniform1uiv(m_id, getLoc(name), 1, &val);
}

int Shader::getLoc(const std::string& name) const {
    return glGetUniformLocation(m_id, name.c_str());
}

std::string Shader::readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "File \"" << path << "\" not found" << std::endl;
        assert(false);
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

void Shader::checkCompileErrors(const uint32_t object,
                                const std::string& type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(object, 1024, nullptr, infoLog);
            std::cerr << "| ERROR::SHADER_COMPILATION_ERROR of type: " << type
                      << "\n"
                      << infoLog << "\n -- " << std::endl;
        }
    } else {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(object, 1024, nullptr, infoLog);
            std::cerr << "| ERROR::PROGRAM_LINKING_ERROR of type: " << type
                      << "\n"
                      << infoLog << "\n -- " << std::endl;
        }
    }
}