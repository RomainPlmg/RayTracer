#include "RayTracerLayer.hpp"

#include <memory>

#include "Application.hpp"


RayTracerLayer::RayTracerLayer() {
    auto frameBufferSize = Application::get().getFrameBufferSize();
    m_texture = std::make_unique<Texture>(frameBufferSize.x, frameBufferSize.y);
}

void RayTracerLayer::onEvent(Event &event) {}

void RayTracerLayer::onUpdate(float ts) {}

void RayTracerLayer::onRender() {}