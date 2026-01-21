#include "Window.hpp"

#include <cassert>
#include <stdexcept>

#include "GLContext.hpp"

Window::Window(const WindowSpecification &specification)
    : m_specification(specification) {
    m_glContext = std::make_unique<GLContext>();
}

Window::~Window() { destroy(); }

void Window::init() {
    m_handle =
        glfwCreateWindow(m_specification.width, m_specification.height,
                         m_specification.title.c_str(), nullptr, nullptr);
    if (!m_handle) {
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(m_handle);  // Make the window as OpenGL context

    // Init OpenGL
    if (m_glContext->init()) {
        throw std::runtime_error("Failed to init backend graphics API.");
    }
}

void Window::destroy() {
    if (m_handle) {
        glfwDestroyWindow(m_handle);
    }

    m_handle = nullptr;
}

void Window::clear() {
    m_glContext->setClearColor(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
    m_glContext->clear();
}

void Window::swapBuffers() {
    assert(m_handle);
    glfwSwapBuffers(m_handle);
}
void Window::setVsync(const bool enabled) { glfwSwapInterval(enabled); }

bool Window::shouldClose() const { return glfwWindowShouldClose(m_handle); }

glm::vec2 Window::getFrameBufferSize() const {
    int width, height;
    glfwGetFramebufferSize(m_handle, &width, &height);
    return {width, height};
}