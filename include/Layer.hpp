#pragma once

class Event;

class Layer {
   public:
    virtual ~Layer() = default;

    virtual void onEvent(Event &event) {}

    virtual void onUpdate(float ts) {}

    virtual void onRender() {}
};
