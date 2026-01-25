#include "Texture.hpp"

#include "GLContext.hpp"

Texture::Texture(int width, int height) : m_width(width), m_height(height) {
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);

    // Immutable allocation
    glTextureStorage2D(m_id, 1, GL_RGBA32F, width, height);

    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::~Texture() { glDeleteTextures(1, &m_id); }

void Texture::bind(const uint32_t unit, const GLuint access) const {
    glBindImageTexture(unit, m_id, 0, GL_FALSE, 0, access, GL_RGBA32F);
}