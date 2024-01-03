#include <iostream>
#include "window.h"

namespace Rendering {

    GLFWwindow *initWindow(
        const int width, const int height,
        const int glVersionMajor, const int glVersionMinor,
        const GlfwCursorMode cursorMode)
    {
        glfwInit();
        glfwWindowHint(
            GLFW_CONTEXT_VERSION_MAJOR,
            glVersionMajor);
        glfwWindowHint(
            GLFW_CONTEXT_VERSION_MINOR,
            glVersionMinor);
        glfwWindowHint(
            GLFW_OPENGL_PROFILE,
            GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow* window = glfwCreateWindow(
            width,
            height,
            "",
            nullptr,
            nullptr);
        if (window == nullptr) {
            glfwTerminate();
            std::cerr << "Cannot open window";
            exit(-1);
        }

        glfwMakeContextCurrent(window);
        glfwSetInputMode(
            window,
            GLFW_CURSOR,
            static_cast<int>(cursorMode));
        glViewport(0, 0, width, height);

        glewInit();

        return window;
    }
}

