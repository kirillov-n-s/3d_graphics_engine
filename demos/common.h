#ifndef DEMOS_COMMON_H
#define DEMOS_COMMON_H

#include "../rendering/window.h"
#include "../rendering/camera.h"

namespace Demos {

    void resizeCallback(
            GLFWwindow *window,
            int newWidth, int newHeight);
    void cursorCallback(
            GLFWwindow *window,
            double xPos, double yPos);
    void scrollCallback(
            GLFWwindow *window,
            double xOffset, double yOffset);
    void keyCallback(
            GLFWwindow* window,
            int key, int scancode, int action, int mods);
    void mouseButtonCallback(
            GLFWwindow* window,
            int button, int action, int mods);

    class CommonDemo
    {
    public:


    protected:
        GLFWwindow *m_window;

    };
}

#endif
