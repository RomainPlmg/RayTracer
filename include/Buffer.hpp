#pragma once

#include <cstdint>
#include <vector>

#include "GLContext.hpp"

template <typename T>
class ShaderStorageBuffer {
   public:
    ShaderStorageBuffer(std::vector<T>& data, uint32_t binding) {
        glCreateBuffers(1, &m_id);
        glNamedBufferData(m_id, data.size() * sizeof(T), data.data(),
                          GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, m_id);
    }

    ~ShaderStorageBuffer() { glDeleteBuffers(1, &m_id); }

    void update(const std::vector<T>& data) {
        glNamedBufferSubData(m_id, 0, data.size() * sizeof(T), data.data());
    }

   private:
    uint32_t m_id;
};