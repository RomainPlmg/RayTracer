#pragma once

#include <GLContext.hpp>
#include <cstdint>

enum class TextureAccess {
    READ,
    WRITE,
    READWRITE,
};

class Texture {
   public:
    Texture(int width, int height);
    ~Texture();

    void bind(uint32_t unit, GLuint access = GL_READ_ONLY) const;
    [[nodiscard]] uint32_t id() const { return m_id; }

   private:
    uint32_t m_id;
    int m_width, m_height;
};