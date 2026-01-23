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
    void setFloat(const std::string& name, const float val) const;
    void setInt(const std::string& name, const int val) const;

   private:
    uint32_t m_id;

    [[nodiscard]] int getLoc(const std::string& name) const;
    std::string readFile(const std::string& path);
    void checkCompileErrors(uint32_t object, const std::string& type);
};