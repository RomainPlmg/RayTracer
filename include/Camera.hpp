#pragma once

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>

#include "glm/ext/matrix_transform.hpp"

class Camera {
   public:
    explicit Camera(glm::vec3 position);

    void setPosition(const glm::vec3 position) { m_position = position; }
    [[nodiscard]] glm::vec3 getPosition() const { return m_position; }
    [[nodiscard]] glm::mat4 getInvView() const { return m_invView; }
    [[nodiscard]] glm::mat4 getInvProj() const { return m_invProj; }

    void update(float ts);

   private:
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;

    float m_yaw = 0.0f;    // Rotation left/right
    float m_pitch = 0.0f;  // Rotation up/down

    float m_sensitivity = 15.0f;
    glm::vec2 m_lastMousePos;

    glm::mat4 m_invView;
    glm::mat4 m_invProj;

    void updateVectors();
    void updateMatrices();
};