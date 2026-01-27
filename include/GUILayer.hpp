#pragma once

#include "Layer.hpp"
#include "RayTracerLayer.hpp"

class GUILayer : public Layer {
   public:
    GUILayer(RayTracerSceneData& sceneData);
    ~GUILayer();

    void onEvent(Event& event) override;
    void onUpdate(float ts) override;
    void onRender() override;

   private:
    RayTracerSceneData& m_sceneData;
};
