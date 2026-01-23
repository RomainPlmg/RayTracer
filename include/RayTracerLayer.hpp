#pragma once

#include <memory>

#include "Buffer.hpp"
#include "Camera.hpp"
#include "Layer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

struct Material {
    glm::vec4 color;
    alignas(16) float roughness;
};

struct Sphere {
    alignas(16) glm::vec3 center;
    alignas(16) float radius;
    Material material;
};

class RayTracerLayer : public Layer {
   public:
    RayTracerLayer();

    void onEvent(Event &event) override;
    void onUpdate(float ts) override;
    void onRender() override;

   private:
    std::unique_ptr<Texture> m_texture;
    std::unique_ptr<Camera> m_camera;
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<ShaderStorageBuffer<Sphere>> m_ssbo;
    uint32_t m_fbo;

    std::vector<Sphere> m_spheres;

    void blit();
    void handleInputs(float ts);
};