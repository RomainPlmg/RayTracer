#include "Application.hpp"
#include "GUILayer.hpp"
#include "RayTracerLayer.hpp"

int main(int, char**) {
    Application app;

    RayTracerSceneData data;

    app.pushLayer<RayTracerLayer>(data);
    app.pushLayer<GUILayer>(data);

    app.run();
    app.stop();

    return 0;
}
