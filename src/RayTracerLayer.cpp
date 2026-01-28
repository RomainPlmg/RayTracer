#include "RayTracerLayer.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <memory>
#include <random>

#include "Application.hpp"
#include "Buffer.hpp"
#include "GLContext.hpp"
#include "Input.hpp"

constexpr float VELOCITY = 15.0f;

RayTracerLayer::RayTracerLayer(RayTracerSceneData& sceneData)
    : m_data(sceneData) {
    m_camera = std::make_unique<Camera>(m_data.cameraSettings);
    m_shader = std::make_unique<Shader>(ASSETS_DIR "shaders/raytracer.comp");

    m_data.spheres.emplace_back(
        Sphere{glm::vec3(0.0, -201.0, -5.0), 200.0, Material{glm::vec3(0.9f)}});
    m_data.spheres.emplace_back(
        Sphere{glm::vec3(0.0, 10.0f, -5.0), 5.0f,
               Material{glm::vec3(0.9f), 1.0f, glm::vec3(1.0f), 20.0f}});

    generateRandomScene(m_data.spheres, 200);

    m_ssbo = std::make_unique<ShaderStorageBuffer<Sphere>>(m_data.spheres, 0);

    // Create a framebuffer and attach it to the texture
    glCreateFramebuffers(1, &m_fbo);

    m_viewportSize = Application::get().getFrameBufferSize();

    m_TextureA = std::make_unique<Texture>(m_viewportSize.x, m_viewportSize.y);
    glNamedFramebufferTexture(m_fbo, GL_COLOR_ATTACHMENT0, m_TextureA->id(), 0);

    m_TextureB = std::make_unique<Texture>(m_viewportSize.x, m_viewportSize.y);
}

void RayTracerLayer::onEvent(Event& event) {}

void RayTracerLayer::onUpdate(float ts) {
    handleInputs(ts);
    m_camera->update(ts);
}

void RayTracerLayer::onRender() {
    auto size = Application::get().getFrameBufferSize();

    if (m_camera->invProj() != m_oldInvProj) {
        m_TextureA = std::make_unique<Texture>(size.x, size.y);
        m_TextureB = std::make_unique<Texture>(size.x, size.y);
        glNamedFramebufferTexture(m_fbo, GL_COLOR_ATTACHMENT0, m_TextureA->id(),
                                  0);
        m_viewportSize = size;
        m_oldInvProj = m_camera->invProj();
        m_FrameIndex = 1;
    }

    if (m_camera->invView() != m_oldInvView) {
        m_oldInvView = m_camera->invView();
        m_FrameIndex = 1;
    }

    if (m_data.settingsChange) {
        m_FrameIndex = 1;
        m_data.settingsChange = false;
    }

    if (m_data.saveRequested) {
        std::string filename = "Render_" + std::to_string(time(0)) + ".png";
        saveImage(filename);
        m_data.saveRequested = false;
    }

    m_shader->use();
    m_TextureA->bind(0, GL_WRITE_ONLY);
    m_TextureB->bind(1, GL_READ_WRITE);

    // Set uniforms
    m_shader->setMat4("u_InverseProjection", m_camera->invProj());
    m_shader->setMat4("u_InverseView", m_camera->invView());
    m_shader->setVec3("u_CameraPosition", m_data.cameraSettings.position);
    m_shader->setVec3("u_CameraRight", m_camera->right());
    m_shader->setVec3("u_CameraUp", m_camera->up());
    m_shader->setFloat("u_Bounces", m_data.rayBounces);
    m_shader->setInt("u_SphereCount", m_data.spheres.size());
    m_shader->setUInt("u_FrameIndex", m_FrameIndex++);
    m_shader->setUInt("u_NbRaysPerPixel", m_data.raysPerPixel);
    m_shader->setFloat("u_DivergeStrength", m_data.divergeStrength);
    m_shader->setFloat("u_Aperture", m_data.aperture);
    m_shader->setFloat("u_FocusDistance", m_data.focusDistance);

    // Execute the compute shader -> asynchronous
    glDispatchCompute((size.x + 31) / 32, (size.y + 31) / 32, 1);

    // Waiting for GPU to finish
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    // Print texture on screen
    blit();
}

void RayTracerLayer::blit() {
    auto size = Application::get().getFrameBufferSize();

    glBlitNamedFramebuffer(m_fbo, 0,              // Source FBO
                           0, 0, size.x, size.y,  // Source quad
                           0, 0, size.x, size.y,  // Destination quad
                           GL_COLOR_BUFFER_BIT,   // Copy colors
                           GL_NEAREST             // Filtering
    );
}

void RayTracerLayer::handleInputs(float ts) {
    glm::vec3 frontXZ = glm::normalize(
        glm::vec3(m_camera->front().x, 0.0f, m_camera->front().z));
    glm::vec3 rightXZ = glm::normalize(
        glm::vec3(m_camera->right().x, 0.0f, m_camera->right().z));

    auto worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 dir{0.0f};
    if (Input::isKeyPressed(GLFW_KEY_D)) dir += rightXZ;
    if (Input::isKeyPressed(GLFW_KEY_A)) dir -= rightXZ;
    if (Input::isKeyPressed(GLFW_KEY_W)) dir += frontXZ;
    if (Input::isKeyPressed(GLFW_KEY_S)) dir -= frontXZ;
    if (Input::isKeyPressed(GLFW_KEY_SPACE)) dir += worldUp;
    if (Input::isKeyPressed(GLFW_KEY_LEFT_SHIFT)) dir -= worldUp;

    auto camPos = m_data.cameraSettings.position;
    camPos += dir * VELOCITY * ts;
    m_camera->setPosition(camPos);
}
void RayTracerLayer::generateRandomScene(std::vector<Sphere>& spheres,
                                         int count) {
    std::mt19937 JohnsonGen(
        1337);  // Seed fixe pour pouvoir retrouver la même scène
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    std::uniform_real_distribution<float> colorDis(0.0f, 1.0f);

    for (int i = 0; i < count; i++) {
        Sphere s;
        // Position : on les étale sur X et Z, et on les pose sur le sol (Y = 0)
        // en supposant que ton rayon est 1.0
        s.center = glm::vec3(dis(JohnsonGen) * 15.0f, 0.0f,
                             dis(JohnsonGen) * 15.0f - 10.0f);
        s.radius = dis(JohnsonGen);

        // Matériau aléatoire
        s.material.colour = glm::vec3(
            colorDis(JohnsonGen), colorDis(JohnsonGen), colorDis(JohnsonGen));

        // On décide aléatoirement si c'est un miroir ou du mat
        float isMirror = colorDis(JohnsonGen);
        if (isMirror > 0.8f) {
            s.material.roughness = 0.0f;  // Miroir parfait
        } else {
            s.material.roughness = colorDis(JohnsonGen);  // Rugosité variée
        }

        // Par défaut, pas d'émission (tu les ajoutes à la main après)
        s.material.emissionColour = glm::vec3(0.0f);
        s.material.emissionStrength = 0.0f;

        spheres.push_back(s);
    }
}

void RayTracerLayer::saveImage(const std::filesystem::path& path) {
    std::vector<uint8_t> pixels(m_viewportSize.x * m_viewportSize.y * 4);

    glBindTexture(GL_TEXTURE_2D, m_TextureA->id());
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

    stbi_flip_vertically_on_write(true);

    stbi_write_png(path.string().c_str(), m_viewportSize.x, m_viewportSize.y, 4,
                   pixels.data(), m_viewportSize.x * 4);
}