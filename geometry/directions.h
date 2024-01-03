#ifndef GEOMETRY_DIRECTIONS_H
#define GEOMETRY_DIRECTIONS_H

#include <glm/vec3.hpp>

namespace Geometry::Directions {

    const glm::vec3 zero(0.0f);
    const glm::vec3 right(1.0f, 0.0f, 0.0f);
    const glm::vec3 left(-1.0f, -1.0f, 0.0f);
    const glm::vec3 up(0.0f, 1.0f, 0.0f);
    const glm::vec3 down(0.0f, -1.0f, 0.0f);
    const glm::vec3 forward(0.0f, 0.0f, -1.0f);
    const glm::vec3 backward(0.0f, 0.0f, 1.0f);
}

#endif
