#include "RayTracerLayer.hpp"

#include <memory>

#include "Application.hpp"
#include "GLContext.hpp"

RayTracerLayer::RayTracerLayer() {
    m_camera = std::make_unique<Camera>(glm::vec3(0.0f));
    m_shader = std::make_unique<Shader>(ASSETS_DIR "shaders/raytracer.comp");

    // Create a framebuffer and attach it to the texture
    glCreateFramebuffers(1, &m_fbo);
}

void RayTracerLayer::onEvent(Event &event) {}

void RayTracerLayer::onUpdate(float ts) { m_camera->update(ts); }

void RayTracerLayer::onRender() {
    m_shader->use();

    auto size = Application::get().getFrameBufferSize();
    m_texture = std::make_unique<Texture>(size.x, size.y);
    glNamedFramebufferTexture(m_fbo, GL_COLOR_ATTACHMENT0, m_texture->id(), 0);

    m_texture->bind(0);

    m_shader->setMat4("u_InverseProjection", m_camera->getInvProj());
    m_shader->setMat4("u_InverseView", m_camera->getInvView());
    m_shader->setVec3("u_CameraPosition", m_camera->getPosition());

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