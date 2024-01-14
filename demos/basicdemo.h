#ifndef DEMOS_BASIC_DEMO_H
#define DEMOS_BASIC_DEMO_H

#include <memory>
#include "../core3d/directions.h"
#include "../rendering/window.h"
#include "../rendering/camera.h"
#include "../rendering/textures/gltexture.h"
#include "../rendering/meshes/glmesh.h"
#include "../rendering/shaders/glshaderprogram.h"
#include "../rendering/textures/glcubemap.h"

namespace Demos {

    class BasicDemo
    {
    public:
        BasicDemo();
        ~BasicDemo();

        void run();

    private:
        void initWindow();
        void initShaders();
        void initMeshes();
        void initTextures();
        void initEnvironment();
        void initCamera();

        void handleCameraMovement(const float dt);
        void render();

        GLFWwindow *m_window;

        std::shared_ptr<Rendering::Shaders::GlShaderProgram> m_glShaderProgram;
        std::shared_ptr<Rendering::Shaders::GlShaderProgram> m_envCubemapShaderProgram;

        Rendering::Camera m_camera;

        std::shared_ptr<Rendering::Meshes::MeshBuffer> m_meshBuffer;
        std::shared_ptr<Rendering::Meshes::GlMesh> m_glMesh;

        std::shared_ptr<Rendering::Textures::GlTexture> m_albedo;
        std::shared_ptr<Rendering::Textures::GlTexture> m_normalMap;
        std::shared_ptr<Rendering::Textures::GlTexture> m_roughness;
        std::shared_ptr<Rendering::Textures::GlTexture> m_metallic;
        std::shared_ptr<Rendering::Textures::GlTexture> m_ambientOcclusion;
        std::shared_ptr<Rendering::Textures::GlTexture> m_heightMap;

        std::shared_ptr<Rendering::Textures::GlCubemap> m_envCubemap;
        std::shared_ptr<Rendering::Textures::GlCubemap> m_irradianceCubemap;
        std::shared_ptr<Rendering::Textures::GlCubemap> m_prefilteredCubemap;

        std::shared_ptr<Rendering::Textures::GlTexture> m_brdfLutTexture;

        int m_width = 0;
        int m_height = 0;
        float m_aspectRatio = 0.0f;

        float m_sensitivity = 0.1f;
        float m_speed = 2.5f;
        float m_fov = 45.0f;
        float m_near = 0.1f;
        float m_far = 1000.0f;

        float m_xPrevPos = -1.0f;
        float m_yPrevPos = -1.0f;

        bool m_camRotationOn = false;

        // glm::mat4 m_modelMat = glm::rotate(
        //         glm::identity<glm::mat4>(),
        //         -glm::pi<float>() * 0.5f,
        //         Core3d::Directions::right);
        glm::mat4 m_modelMat = glm::identity<glm::mat4>();
        glm::mat4 m_normalModelMat = glm::transpose(glm::inverse(m_modelMat));

        glm::vec4 m_backgroundColor = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);

        static void resizeCallback(
                GLFWwindow *window,
                int newWidth, int newHeight);
        static void cursorCallback(
                GLFWwindow *window,
                double xPos, double yPos);
        static void scrollCallback(
                GLFWwindow *window,
                double xOffset, double yOffset);
        static void keyCallback(
                GLFWwindow* window,
                int key, int scancode, int action, int mods);
        static void mouseButtonCallback(
                GLFWwindow* window,
                int button, int action, int mods);
    };
}

#endif
