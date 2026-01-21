#pragma once

#include <cstdint>

class Texture {
   public:
    Texture(int width, int height);
    ~Texture();

    void bind(uint32_t unit) const;
    uint32_t id() const { return m_id; }

   private:
    uint32_t m_id;
    int m_width, m_height;
};