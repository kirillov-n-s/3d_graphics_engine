#ifndef RENDERING_WINDOW_H
#define RENDERING_WINDOW_H

#include <gl/glew.h>
#include <GLFW/glfw3.h>

namespace Rendering {

    enum class GlfwCursorMode
    {
        Normal = GLFW_CURSOR_NORMAL,
        Hidden = GLFW_CURSOR_HIDDEN,
        Disabled = GLFW_CURSOR_DISABLED
    };

    GLFWwindow *initWindow(
        const int width, const int height,
        const int glVersionMajor = 4, const int glVersionMinor = 3,
        const GlfwCursorMode cursorMode = GlfwCursorMode::Normal);
}

#endif
