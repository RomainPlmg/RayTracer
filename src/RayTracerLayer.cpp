#include "RayTracerLayer.hpp"

#include <memory>

#include "Application.hpp"
#include "Buffer.hpp"
#include "GLContext.hpp"
#include "Input.hpp"
#include "glm/fwd.hpp"

constexpr float VELOCITY = 15.0f;

RayTracerLayer::RayTracerLayer() {
    m_camera = std::make_unique<Camera>(glm::vec3(0.0f));
    m_shader = std::make_unique<Shader>(ASSETS_DIR "shaders/raytracer.comp");

    m_spheres.emplace_back(Sphere{glm::vec3(0.0, 0.0, -5.0), 1.0,
                                Material{glm::vec4(1.0, 0.2, 0.2, 1.0), 1.0}});
    m_spheres.emplace_back(Sphere{glm::vec3(0.0, -201.0, -5.0), 200.0,
                                Material{glm::vec4(0.9, 0.9, 0.9, 1.0), 1.0}});

    m_ssbo = std::make_unique<ShaderStorageBuffer<Sphere>>(m_spheres, 0);

    // Create a framebuffer and attach it to the texture
    glCreateFramebuffers(1, &m_fbo);
}

void RayTracerLayer::onEvent(Event &event) {}

void RayTracerLayer::onUpdate(float ts) {
    handleInputs(ts);
    m_camera->update(ts);
}

void RayTracerLayer::onRender() {
    m_shader->use();

    auto size = Application::get().getFrameBufferSize();
    m_texture = std::make_unique<Texture>(size.x, size.y);
    glNamedFramebufferTexture(m_fbo, GL_COLOR_ATTACHMENT0, m_texture->id(), 0);

    m_texture->bind(0);

    m_shader->setMat4("u_InverseProjection", m_camera->invProj());
    m_shader->setMat4("u_InverseView", m_camera->invView());
    m_shader->setVec3("u_CameraPosition", m_camera->position());
    m_shader->setFloat("u_Bounces", 1);
    m_shader->setInt("u_SphereCount", m_spheres.size());

    // Execute the compute shader -> asynchronous
    glDispatchCompute(size.x / 8, size.y / 8, 1);

    // Waiting for GPU to finish
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    // Print texture on screen
    blit();
}

void RayTracerLayer::blit() {
    auto size = Application::get().getFrameBufferSize();

    glBlitNamedFramebuffer(m_fbo, 0,              // Source FBO
                           0, 0, size.x, size.y,  // Source quad
                           0, 0, size.x, size.y,  // Destination quad
                           GL_COLOR_BUFFER_BIT,   // Copy colors
                           GL_NEAREST             // Filtering
    );
}

void RayTracerLayer::handleInputs(float ts) {
    glm::vec3 frontXZ = glm::normalize(
        glm::vec3(m_camera->front().x, 0.0f, m_camera->front().z));
    glm::vec3 rightXZ = glm::normalize(
        glm::vec3(m_camera->right().x, 0.0f, m_camera->right().z));

    auto worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 dir{0.0f};
    if (Input::isKeyPressed(GLFW_KEY_D)) dir += rightXZ;
    if (Input::isKeyPressed(GLFW_KEY_A)) dir -= rightXZ;
    if (Input::isKeyPressed(GLFW_KEY_W)) dir += frontXZ;
    if (Input::isKeyPressed(GLFW_KEY_S)) dir -= frontXZ;
    if (Input::isKeyPressed(GLFW_KEY_SPACE)) dir += worldUp;
    if (Input::isKeyPressed(GLFW_KEY_LEFT_SHIFT)) dir -= worldUp;

    auto camPos = m_camera->position();
    camPos += dir * VELOCITY * ts;
    m_camera->setPosition(camPos);
}