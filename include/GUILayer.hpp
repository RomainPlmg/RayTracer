#pragma once

#include "Layer.hpp"

class GUILayer : public Layer {
   public:
    GUILayer();
    ~GUILayer();

    void onEvent(Event& event) override;
    void onUpdate(float ts) override;
    void onRender() override;
};
