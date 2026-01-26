#include "Application.hpp"

#include <GLFW/glfw3.h>

#include <iostream>

#include "Input.hpp"

static Application *s_application = nullptr;

static void glfwErrorCallback(int error, const char *description) {
    std::cerr << "[GLFW Error " << error << "] " << description << std::endl;
}

Application::Application(const ApplicationSpecification &specification)
    : m_specification(specification) {
    s_application = this;

    glfwSetErrorCallback(glfwErrorCallback);
    glfwInit();

    // Set window title to app name if empty
    if (m_specification.window_spec.title.empty()) {
        m_specification.window_spec.title = m_specification.name;
    }

    m_window = std::make_shared<Window>(m_specification.window_spec);
    m_window->init();
    m_window->setVsync(false);

    Input::init(*m_window);
}

Application::~Application() {
    m_window->destroy();
    m_layerStack.clear();
    glfwTerminate();
    s_application = nullptr;

    std::cout << "Goodbye!" << std::endl;
}

void Application::run() {
    running_ = true;
    float last_time = getTime();

    // Main application loop
    while (running_) {
        glfwPollEvents();

        if (m_window->shouldClose()) {
            stop();
            break;
        }

        const float current_time = getTime();
        const float timestep =
            glm::clamp(current_time - last_time, 0.001f, 0.1f);
        last_time = current_time;

        // Main layer update here
        for (const auto &layer : m_layerStack) {
            layer->onUpdate(timestep);
        }

        m_window->clear();

        // NOTE: rendering can be done elsewhere (eg. render thread)
        for (const auto &layer : m_layerStack) {
            layer->onRender();
        }

        m_window->swapBuffers();
    }
}

void Application::stop() { running_ = false; }

Application &Application::get() {
    assert(s_application);
    return *s_application;
}

float Application::getTime() { return glfwGetTime(); }