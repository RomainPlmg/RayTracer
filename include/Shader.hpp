#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Shader {
   public:
    explicit Shader(const std::string& computePath);
    ~Shader();

    void use() const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    void setVec3(const std::string& name, const glm::vec3& vec) const;
    void setFloat(const std::string& name, float val) const;
    void setInt(const std::string& name, int val) const;
    void setUInt(const std::string& name, uint32_t val) const;

   private:
    uint32_t m_id;

    [[nodiscard]] int getLoc(const std::string& name) const;
    std::string readFile(const std::string& path);
    void checkCompileErrors(uint32_t object, const std::string& type);
};