#include <iostream>
#include "../core3d/common.h"
#include "../core3d/directions.h"
#include "../common/debug.h"
#include "../io/obj.h"
#include "basicdemo.h"

#include <memory>

#include "..\rendering\textures\environment.h"

namespace Demos {

    BasicDemo::BasicDemo()
    {
        initWindow();
        initShaders();
        initMeshes();
        initTextures();
        initEnvironment();
        initCamera();
    }

    BasicDemo::~BasicDemo()
    {
        glfwTerminate();
    }

    void BasicDemo::run()
    {
        float currentFrameElapsed = 0.0f;
        float prevFrameElapsed = 0.0f;
        float dt = 0.0f;

        const glm::mat4 projectionMat = glm::perspective(
                glm::radians(m_fov), m_aspectRatio, m_near, m_far);

        m_glShaderProgram->use();
        m_glShaderProgram->setUniform("uProjection", projectionMat);
        m_glShaderProgram->setUniform("uModel", m_modelMat);
        m_glShaderProgram->setUniform("uNormalModel", m_normalModelMat);
        m_glShaderProgram->setUniform("uAlbedo", 0);
        m_glShaderProgram->setUniform("uNormalMap", 1);
        if (m_roughness != nullptr)
            m_glShaderProgram->setUniform("uRoughness", 2);
        if (m_metallic != nullptr)
            m_glShaderProgram->setUniform("uMetallic", 3);
        if (m_ambientOcclusion != nullptr)
            m_glShaderProgram->setUniform("uAO", 4);
        m_glShaderProgram->setUniform("uIrradiance", 5);
        m_glShaderProgram->setUniform("uPrefilteredEnv", 6);
        m_glShaderProgram->setUniform("uBrdfLUT", 7);

        m_envCubemapShaderProgram->use();
        m_envCubemapShaderProgram->setUniform("uProjection", projectionMat);
        m_envCubemapShaderProgram->setUniform("uEnvironment", 0);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        // glEnable(GL_FRAMEBUFFER_SRGB);

        glViewport(0, 0, m_width, m_height);

        while (!glfwWindowShouldClose(m_window))
        {
            currentFrameElapsed = glfwGetTime();
            dt = currentFrameElapsed - prevFrameElapsed;
            prevFrameElapsed = currentFrameElapsed;

            glfwSetWindowTitle(
                    m_window,
                    (std::to_string(static_cast<int>(1.0f / dt)) + " fps").c_str());

            glClearColor(
                    m_backgroundColor.r,
                    m_backgroundColor.g,
                    m_backgroundColor.b,
                    m_backgroundColor.a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            handleCameraMovement(dt);
            render();

            glfwSwapBuffers(m_window);
            glfwPollEvents();
        }
    }

    void BasicDemo::initWindow()
    {
        m_width = 1920;
        m_height = 1080;
        m_aspectRatio = static_cast<float>(m_width) / m_height;
        const int glVersionMajor = 4;
        const int glVersionMinor = 6;
        const auto cursorMode = Rendering::GlfwCursorMode::Hidden;

        m_window = Rendering::initWindow(
            m_width, m_height,
            glVersionMajor, glVersionMinor,
            cursorMode);

        glfwSetFramebufferSizeCallback(m_window, resizeCallback);
        glfwSetCursorPosCallback(m_window, cursorCallback);
        glfwSetScrollCallback(m_window, scrollCallback);
        glfwSetKeyCallback(m_window, keyCallback);
        glfwSetMouseButtonCallback(m_window, mouseButtonCallback);

        glfwSetWindowUserPointer(m_window, this);
    }

    void BasicDemo::initShaders()
    {
        const std::string vertPath = R"(C:\Users\kirillov_n_s\Desktop\Projects\engine\data\shaders\main_with_tbn.vert)";
        const std::string fragPath = R"(C:\Users\kirillov_n_s\Desktop\Projects\engine\data\shaders\pbr_with_tbn_with_full_ibl.frag)";

        std::string error = "";

        const Rendering::Shaders::GlShader vertShader(
            vertPath,
            Rendering::Shaders::GlShaderType::VertexShader,
            error);
        Common::exitOnError(error, 1);

        const Rendering::Shaders::GlShader fragShader(
            fragPath,
            Rendering::Shaders::GlShaderType::FragmentShader,
            error);
        Common::exitOnError(error, 2);

        m_glShaderProgram = std::make_shared<Rendering::Shaders::GlShaderProgram>(
            std::vector{vertShader, fragShader},
            error);
        Common::exitOnError(error, 3);
    }

    void BasicDemo::initMeshes()
    {
        const std::string objPath = R"(C:\Users\kirillov_n_s\Desktop\Projects\engine\data\meshes\uv_sphere.obj)";
        // const std::string objPath = R"(C:\Users\kirillov_n_s\Desktop\Projects\engine\data\meshes\watermelon_23k.obj)";
        // const std::string objPath = R"(C:\Users\kirillov_n_s\Desktop\Assets\Meshes\TCom_debris_stick_004_lod0\stick.obj)";
        // const std::string objPath = R"(C:\Users\kirillov_n_s\Desktop\Assets\Meshes\Cerberus_by_Andrew_Maximov\Cerberus_LP.obj)";

        std::string error = "";

        const Core3d::PolygonMesh objPoly = IO::readObj(
            objPath,
            error,
            IO::ObjReaderSettings {
                .readTexcoords = true
            });
        Common::exitOnError(error, 4);

        const std::vector<int> triangleVertexIndices = Core3d::triangulate(
            objPoly.vertexIndices, objPoly.polygonStarts);
        const std::vector<int> triangleTexcoordIndices = Core3d::triangulate(
            objPoly.texcoordIndices, objPoly.polygonStarts);
        const std::vector<glm::vec3> normals = Core3d::computeNormals(
            objPoly.vertices, triangleVertexIndices);
        const Core3d::TriangleMesh objTri(
            objPoly.vertices,
            triangleVertexIndices,
            normals,
            objPoly.texcoords,
            triangleTexcoordIndices);

        m_meshBuffer = std::make_shared<Rendering::Meshes::MeshBuffer>(objTri);
        m_glMesh = std::make_shared<Rendering::Meshes::GlMesh>(*m_meshBuffer);
    }

    void BasicDemo::initTextures()
    {
        // const std::string albedoPath = R"(C:\Users\kirillov_n_s\Desktop\Projects\engine\data\textures\watermelon_albedo.png)";
        // const Core2d::Image albedoImage(albedoPath);
        // m_albedo = std::make_shared<Rendering::Textures::GlTexture>(albedoImage, true);
        //
        // const std::string normalMapPath = R"(C:\Users\kirillov_n_s\Desktop\Projects\engine\data\textures\watermelon_normalmap.png)";
        // const Core2d::Image normalMapImage(normalMapPath);
        // m_normalMap = std::make_shared<Rendering::Textures::GlTexture>(normalMapImage, false);
        //
        // const std::string roughnessPath = R"(C:\Users\kirillov_n_s\Desktop\Projects\engine\data\textures\watermelon_roughness.png)";
        // const Core2d::Image roughnessImage(roughnessPath, Core2d::ImageFormat::Grayscale);
        // m_roughness = std::make_shared<Rendering::Textures::GlTexture>(roughnessImage, false);


        // const std::string albedoPath = R"(C:\Users\kirillov_n_s\Desktop\Assets\Meshes\TCom_debris_stick_004_lod0\albedo.png)";
        // const Core2d::Image albedoImage(albedoPath);
        // m_albedo = std::make_shared<Rendering::Textures::GlTexture>(albedoImage, true);
        //
        // const std::string normalMapPath = R"(C:\Users\kirillov_n_s\Desktop\Assets\Meshes\TCom_debris_stick_004_lod0\normal.png)";
        // const Core2d::Image normalMapImage(normalMapPath);
        // m_normalMap = std::make_shared<Rendering::Textures::GlTexture>(normalMapImage, false);
        //
        // const std::string roughnessPath = R"(C:\Users\kirillov_n_s\Desktop\Assets\Meshes\TCom_debris_stick_004_lod0\roughness.png)";
        // const Core2d::Image roughnessImage(roughnessPath, Core2d::ImageFormat::Grayscale);
        // m_roughness = std::make_shared<Rendering::Textures::GlTexture>(roughnessImage, false);


        const std::string commonPath = R"(C:\Users\kirillov_n_s\Desktop\Assets\Materials\TCom_Plastic_SpaceBlanketFolds_4K\)";

        const std::string albedoPath = commonPath + "albedo.png";
        const Core2d::Image albedoImage(albedoPath);
        m_albedo = std::make_shared<Rendering::Textures::GlTexture>(albedoImage, false);

        const std::string normalMapPath = commonPath + "normal.png";
        const Core2d::Image normalMapImage(normalMapPath);
        m_normalMap = std::make_shared<Rendering::Textures::GlTexture>(normalMapImage, false);

        const std::string roughnessPath = commonPath + "roughness.png";
        const Core2d::Image roughnessImage(roughnessPath, Core2d::ImageFormat::Grayscale);
        m_roughness = std::make_shared<Rendering::Textures::GlTexture>(roughnessImage, false);

        const std::string metallicPath = commonPath + "metallic.png";
        const Core2d::Image metallicImage(metallicPath, Core2d::ImageFormat::Grayscale);
        m_metallic = std::make_shared<Rendering::Textures::GlTexture>(metallicImage, false);

        const std::string aoPath = commonPath + "ao.png";
        const Core2d::Image aoImage(aoPath, Core2d::ImageFormat::Grayscale);
        m_ambientOcclusion = std::make_shared<Rendering::Textures::GlTexture>(aoImage, false);

        // const std::string commonPath = R"(C:\Users\kirillov_n_s\Desktop\Assets\Meshes\Cerberus_by_Andrew_Maximov\)";
        // const std::string albedoPath = commonPath + "Cerberus_A.tga";
        // const Core2d::Image albedoImage(albedoPath);
        // m_albedo = std::make_shared<Rendering::Textures::GlTexture>(albedoImage, false);
        //
        // const std::string normalMapPath = commonPath + "Cerberus_N.tga";
        // const Core2d::Image normalMapImage(normalMapPath);
        // m_normalMap = std::make_shared<Rendering::Textures::GlTexture>(normalMapImage, false);
        //
        // const std::string roughnessPath = commonPath + "Cerberus_R.tga";
        // const Core2d::Image roughnessImage(roughnessPath, Core2d::ImageFormat::Grayscale);
        // m_roughness = std::make_shared<Rendering::Textures::GlTexture>(roughnessImage, false);
        //
        // const std::string metallicPath = commonPath + "Cerberus_M.tga";
        // const Core2d::Image metallicImage(metallicPath, Core2d::ImageFormat::Grayscale);
        // m_metallic = std::make_shared<Rendering::Textures::GlTexture>(metallicImage, false);
        //
        // const std::string aoPath = commonPath + "Cerberus_AO.tga";
        // const Core2d::Image aoImage(aoPath, Core2d::ImageFormat::Grayscale);
        // m_ambientOcclusion = std::make_shared<Rendering::Textures::GlTexture>(aoImage, false);
    }

    void BasicDemo::initEnvironment()
    {
        const std::string commonEnvPath = R"(C:\Users\kirillov_n_s\Desktop\Assets\Environments\)";
        // const std::string equirectangularMapPath = commonEnvPath + "TCom_HDRPanorama0035_colorful_alley_8K_hdri_sphere.hdr";
        // const std::string equirectangularMapPath = commonEnvPath + "TCom_NorwayForest_8K_hdri_sphere.hdr";
        // const std::string equirectangularMapPath = commonEnvPath + "TCom_JapanParkingGarageB_8K_hdri_sphere.hdr";
        const std::string equirectangularMapPath = commonEnvPath + "pretville_cinema_16k.hdr";
        const Core2d::Image equirectangularMapImage(equirectangularMapPath);
        const std::array<std::shared_ptr<Core2d::Image>, 6> &cubeFaces =
            Rendering::Textures::equirectangularMapToCubemapFaces(equirectangularMapImage, 4096);
        const std::array<std::shared_ptr<Core2d::Image>, 6> &irradianceCubeFaces =
            Rendering::Textures::cubemapToIrradianceMap(cubeFaces, 256);
        m_envCubemap = std::make_shared<Rendering::Textures::GlCubemap>(cubeFaces);
        m_irradianceCubemap = std::make_shared<Rendering::Textures::GlCubemap>(irradianceCubeFaces);
        m_prefilteredCubemap = Rendering::Textures::prefilterCubemap(
            cubeFaces, 1024, 8);
        m_brdfLutTexture = Rendering::Textures::integrateBrdf(512);

        std::string error;
        const std::string commonShaderPath = R"(C:\Users\kirillov_n_s\Desktop\Projects\engine\data\shaders\)";

        const Rendering::Shaders::GlShader vertShader(
            commonShaderPath + "cubemap.vert",
            Rendering::Shaders::GlShaderType::VertexShader,
            error);
        Common::exitOnError(error, 420);

        const Rendering::Shaders::GlShader fragShader(
            commonShaderPath + "cubemap.frag",
            Rendering::Shaders::GlShaderType::FragmentShader,
            error);
        Common::exitOnError(error, 421);

        m_envCubemapShaderProgram = std::make_shared<Rendering::Shaders::GlShaderProgram>(
            std::vector{vertShader, fragShader},
            error);
        Common::exitOnError(error, 422);
    }

    void BasicDemo::initCamera()
    {
        const glm::vec3 cameraPos = Core3d::Directions::backward * 50.0f;
        m_camera = Rendering::Camera(cameraPos);
    }

    void BasicDemo::handleCameraMovement(const float dt)
    {
        glm::vec3 direction { 0.0f, 0.0f, 0.0f };
        if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
            direction += m_camera.front();
        if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
            direction -= m_camera.front();
        if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
            direction += m_camera.right();
        if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
            direction -= m_camera.right();
        if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS)
            direction += Core3d::Directions::up;
        if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS)
            direction -= Core3d::Directions::up;
        m_camera.move(direction * dt * m_speed);
    }

    void BasicDemo::render()
    {
        m_glShaderProgram->use();
        m_glShaderProgram->setUniform("uView", m_camera.view());
        m_glShaderProgram->setUniform("uCameraPos", m_camera.position());
        m_albedo->use(0);
        m_normalMap->use(1);
        if (m_roughness != nullptr)
            m_roughness->use(2);
        if (m_metallic != nullptr)
            m_metallic->use(3);
        if (m_ambientOcclusion != nullptr)
            m_ambientOcclusion->use(4);
        m_irradianceCubemap->use(5);
        m_prefilteredCubemap->use(6);
        m_brdfLutTexture->use(7);
        m_glMesh->draw();

        m_envCubemapShaderProgram->use();
        m_envCubemapShaderProgram->setUniform("uView", m_camera.view());
        m_envCubemap->draw(0);
    }

    void BasicDemo::resizeCallback(
            GLFWwindow *window,
            int newWidth, int newHeight)
    {
        auto *userPtr = static_cast<BasicDemo *>(glfwGetWindowUserPointer(window));
        glViewport(
                0,
                0,
                userPtr->m_width = newWidth,
                userPtr->m_height = newHeight);
    }

    void BasicDemo::cursorCallback(
            GLFWwindow *window,
            double xPos, double yPos)
    {
        auto *userPtr = static_cast<BasicDemo *>(glfwGetWindowUserPointer(window));
        if (!userPtr->m_camRotationOn) {
            userPtr->m_xPrevPos = xPos;
            userPtr->m_yPrevPos = yPos;
            return;
        }
        const float xOffset = xPos - userPtr->m_xPrevPos;
        const float yOffset = userPtr->m_yPrevPos - yPos;
        userPtr->m_xPrevPos = xPos;
        userPtr->m_yPrevPos = yPos;
        userPtr->m_camera.rotate(
                xOffset * userPtr->m_sensitivity,
                yOffset * userPtr->m_sensitivity);
    }

    void BasicDemo::scrollCallback(
            GLFWwindow *window,
            double xOffset, double yOffset)
    {
        auto *userPtr = static_cast<BasicDemo *>(glfwGetWindowUserPointer(window));
        userPtr->m_speed += yOffset;
        if (userPtr->m_speed < 0.f)
            userPtr->m_speed = 0.f;
    }

    void BasicDemo::keyCallback(
            GLFWwindow* window,
            int key, int scancode, int action, int mods)
    {
        auto *userPtr = static_cast<BasicDemo *>(glfwGetWindowUserPointer(window));
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(userPtr->m_window, GLFW_TRUE);
        if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
            glfwMaximizeWindow(userPtr->m_window);
    }

    void BasicDemo::mouseButtonCallback(
            GLFWwindow* window,
            int button, int action, int mods)
    {
        auto *userPtr = static_cast<BasicDemo *>(glfwGetWindowUserPointer(window));
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            userPtr->m_camRotationOn = true;
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
            userPtr->m_camRotationOn = false;
    }
}
