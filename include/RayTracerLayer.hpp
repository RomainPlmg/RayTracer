#pragma once

#include <memory>

#include "Buffer.hpp"
#include "Camera.hpp"
#include "Layer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

struct Material {
    alignas(16) glm::vec3 colour = glm::vec3(0.0);
    float emissionStrength = 0.0;
    alignas(16) glm::vec3 emissionColour = glm::vec3(0.0);
    float _pad0 = 0.0f;
};

struct Sphere {
    alignas(16) glm::vec3 center = glm::vec3(0.0);
    float radius = 1.0;
    Material material;
};

class RayTracerLayer : public Layer {
   public:
    RayTracerLayer();

    void onEvent(Event &event) override;
    void onUpdate(float ts) override;
    void onRender() override;

   private:
    std::unique_ptr<Texture> m_TextureA, m_TextureB;
    std::unique_ptr<Camera> m_camera;
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<ShaderStorageBuffer<Sphere>> m_ssbo;
    uint32_t m_fbo;

    glm::vec2 m_viewportSize;
    std::vector<Sphere> m_spheres;
    uint32_t m_FrameIndex = 1;
    glm::mat4 m_oldInvView{0.0f};

    void blit();
    void handleInputs(float ts);
};