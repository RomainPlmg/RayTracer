#include "Application.hpp"
#include "GUILayer.hpp"
#include "RayTracerLayer.hpp"

int main(int, char**) {
    Application app;

    app.pushLayer<RayTracerLayer>();
    app.pushLayer<GUILayer>();

    app.run();
    app.stop();

    return 0;
}
