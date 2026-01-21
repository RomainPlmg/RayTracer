#include "Application.hpp"
#include "RayTracerLayer.hpp"

int main(int, char**) {
    Application app;

    app.pushLayer<RayTracerLayer>();

    app.run();
    app.stop();

    return 0;
}
