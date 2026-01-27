#include "GUILayer.hpp"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include "Application.hpp"
#include "Camera.hpp"
#include "glm/gtc/type_ptr.hpp"

GUILayer::GUILayer(RayTracerSceneData& sceneData) : m_sceneData(sceneData) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |=
        ImGuiConfigFlags_DockingEnable;  // IF using Docking Branch

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(Application::get().getWindow().getHandle(),
                                 true);
    ImGui_ImplOpenGL3_Init();
}

GUILayer::~GUILayer() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUILayer::onEvent(Event& event) { Layer::onEvent(event); }

void GUILayer::onUpdate(float ts) { Layer::onUpdate(ts); }

void GUILayer::onRender() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    bool changed = false;

    ImGui::Begin("Scene settings");
    ImGui::Text("Camera");
    changed |= ImGui::DragFloat3("Position", glm::value_ptr(m_sceneData.cameraSettings.position), 0.1f);
    changed |= ImGui::SliderFloat("FOV", &m_sceneData.cameraSettings.fov, 10.0f, 120.0f); // Based on Minecraft interval
    ImGui::SliderFloat("Sensitivity", &m_sceneData.cameraSettings.sensitivity, 1.0f, 250.0f);
    ImGui::Separator();
    ImGui::Text("Ray Tracer");
    changed |= ImGui::SliderInt("Bounces", &m_sceneData.rayBounces, 0, 20);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}