#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"

namespace Rendering {

    Camera::Camera(
            const glm::vec3 &position,
            const float yaw, const float pitch)
    : m_position(position),
      m_yaw(yaw),
      m_pitch(pitch)
    {
        updateDerivedVectors();
    }

    glm::vec3 Camera::position() const
    {
        return m_position;
    }

    glm::vec3 Camera::front() const
    {
        return m_front;
    }

    glm::vec3 Camera::right() const
    {
        return m_right;
    }

    glm::vec3 Camera::up() const
    {
        return m_up;
    }

    glm::mat4 Camera::view() const
    {
        return glm::lookAt(
                m_position,
                m_position + m_front,
                m_up);
    }

    void Camera::move(const glm::vec3 &offset)
    {
        m_position += offset;
    }

    void Camera::rotate(const float yawOffset, const float pitchOffset)
    {
        m_yaw += yawOffset;
        m_pitch += pitchOffset;
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;
        updateDerivedVectors();
    }

    void Camera::updateDerivedVectors()
    {
        const glm::vec3 front {
                std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch)),
                std::sin(glm::radians(m_pitch)),
                std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch))
        };

        m_front = glm::normalize(front);
        m_right = glm::normalize(glm::cross(
                m_front,
                glm::vec3 {0.0f, 1.0f, 0.0f}));
        m_up = glm::cross(m_right, m_front);
    }
}
