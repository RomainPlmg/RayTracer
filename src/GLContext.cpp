#include <iostream>

#include "GLContext.hpp"

bool GLContext::init() {
    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        return true;
    }

    std::cout << "Loaded OpenGL " << GLAD_VERSION_MAJOR(version) << "."
              << GLAD_VERSION_MINOR(version) << std::endl;
    return false;
}

void GLContext::shutdown() {}

void GLContext::clear() {
    glClearColor(clear_color_.r, clear_color_.g, clear_color_.b,
                 clear_color_.a);
    glClear(GL_COLOR_BUFFER_BIT);
}