#pragma once

// Disable clang-format to ensure glad is included before GLFW
// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <glm/glm.hpp>

class GLContext {
   public:
    bool init();
    void shutdown();
    void clear();

    void setClearColor(const glm::vec4 &rgba) { clear_color_ = rgba; }

   private:
    glm::vec4 clear_color_{0.0f};
};