#pragma once

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>

class Camera {
   public:
    explicit Camera(glm::vec3 position);

    void setPosition(const glm::vec3 position) { m_position = position; }
    [[nodiscard]] glm::vec3 position() const { return m_position; }
    [[nodiscard]] glm::vec3 front() const { return m_front; }
    [[nodiscard]] glm::vec3 right() const { return m_right; }
    [[nodiscard]] glm::vec3 up() const { return m_up; }
    [[nodiscard]] glm::mat4 invView() const { return m_invView; }
    [[nodiscard]] glm::mat4 invProj() const { return m_invProj; }

    void update(float ts);

   private:
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;

    float m_yaw = 0.0f;    // Rotation left/right
    float m_pitch = 0.0f;  // Rotation up/down

    glm::vec2 m_lastMousePos;

    glm::mat4 m_invView;
    glm::mat4 m_invProj;

    void updateVectors();
    void updateMatrices();
};