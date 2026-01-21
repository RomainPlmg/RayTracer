#pragma once

#include <memory>

#include "Layer.hpp"
#include "Texture.hpp"

class RayTracerLayer : public Layer {
   public:
    RayTracerLayer();

    void onEvent(Event &event) override;
    void onUpdate(float ts) override;
    void onRender() override;

   private:
    std::unique_ptr<Texture> m_texture;
};