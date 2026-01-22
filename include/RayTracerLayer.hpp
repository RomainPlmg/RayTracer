#pragma once

#include <memory>

#include "Camera.hpp"
#include "Layer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

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
    uint32_t m_fbo;

    void blit();
    void handleInputs(float ts);
};