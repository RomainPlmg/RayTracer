#pragma once

#include <cstdint>
#include <memory>

#include "Buffer.hpp"
#include "Camera.hpp"
#include "Layer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

struct Material {
    alignas(16) glm::vec3 colour = glm::vec3(0.0);
    float roughness = 1.0f;
    alignas(16) glm::vec3 emissionColour = glm::vec3(0.0);
    float emissionStrength = 0.0;
};

struct Sphere {
    alignas(16) glm::vec3 center = glm::vec3(0.0);
    float radius = 1.0;
    Material material;
};

struct RayTracerSceneData {
    std::vector<Sphere> spheres;
    CameraSettings cameraSettings;
    int raysPerPixel = 1;
    int rayBounces = 3;
    float divergeStrength = 1.0f;
    float aperture = 0.0f;
    float focusDistance = 0.0f;
    bool settingsChange = false;
};

class RayTracerLayer : public Layer {
   public:
    RayTracerLayer(RayTracerSceneData& sceneData);

    void onEvent(Event& event) override;
    void onUpdate(float ts) override;
    void onRender() override;

    RayTracerSceneData& getSceneData() { return m_data; }

   private:
    std::unique_ptr<Texture> m_TextureA, m_TextureB;
    std::unique_ptr<Camera> m_camera;
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<ShaderStorageBuffer<Sphere>> m_ssbo;
    uint32_t m_fbo;

    glm::vec2 m_viewportSize;
    RayTracerSceneData& m_data;
    uint32_t m_FrameIndex = 1;
    glm::mat4 m_oldInvView{0.0f};
    glm::mat4 m_oldInvProj{0.0f};

    void blit();
    void handleInputs(float ts);
    void GenerateRandomScene(std::vector<Sphere>& spheres, int count);
};