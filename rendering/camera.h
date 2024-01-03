#ifndef RENDERING_CAMERA_H
#define RENDERING_CAMERA_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace Rendering {

    class Camera
    {
    public:
        Camera() = default;
        Camera(const Camera &) = default;
        explicit Camera(
                const glm::vec3 &position,
                const float yaw = -90.f, const float pitch = 0.f);

        glm::vec3 position() const;

        glm::vec3 front() const;
        glm::vec3 right() const;
        glm::vec3 up() const;

        glm::mat4 view() const;

        void move(const glm::vec3& offset);
        void rotate(const float yawOffset, const float pitchOffset);

    private:
        glm::vec3 m_position;
        float m_yaw;
        float m_pitch;

        glm::vec3 m_front;
        glm::vec3 m_right;
        glm::vec3 m_up;

        void updateDerivedVectors();
    };
}

#endif
