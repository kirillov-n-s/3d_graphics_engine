cmake_minimum_required (VERSION 3.8)

project ("engine")

add_executable (
        engine
        "main.cpp"

        "common/debug.h" "common/debug.cpp"
        "common/common.h" "common/common.cpp"

        "core3d/directions.h"
        "core3d/common.h" "core3d/common.cpp"
        "core3d/trianglemesh.h" "core3d/trianglemesh.cpp"
        "core3d/polygonmesh.h" "core3d/polygonmesh.cpp"
        "core3d/shapes.h" "core3d/shapes.cpp"

        "core2d/image.h" "core2d/image.cpp"

        "io/obj.h" "io/obj.cpp"
        "io/image.h" "io/image.cpp"
        "io/stb_image/stb_image.h" "io/stb_image/stb_image.cpp"
        "io/stb_image/stb_image_write.h" "io/stb_image/stb_image_write.cpp"

        "rendering/camera.h" "rendering/camera.cpp"
        "rendering/window.h" "rendering/window.cpp"

        "rendering/meshes/meshbuffer.h" "rendering/meshes/meshbuffer.cpp"
        "rendering/meshes/glmesh.h" "rendering/meshes/glmesh.cpp"
        "rendering/meshes/glcube.h" "rendering/meshes/glcube.cpp"
        "rendering/meshes/glquad.h" "rendering/meshes/glquad.cpp"

        "rendering/shaders/glshaderprogram.h" "rendering/shaders/glshaderprogram.cpp"
        "rendering/shaders/glshader.h" "rendering/shaders/glshader.cpp"

        "rendering/textures/gltexture.h" "rendering/textures/gltexture.cpp"
        "rendering/textures/glcubemap.h" "rendering/textures/glcubemap.cpp"
        "rendering/textures/environment.h" "rendering/textures/environment.cpp"
        "rendering/textures/rendertocubemap.h" "rendering/textures/rendertocubemap.cpp"
        "rendering/textures/rendertotexture.h" "rendering/textures/rendertotexture.cpp"

        "demos/common.h" "demos/common.cpp"
        "demos/basicdemo.h" "demos/basicdemo.cpp"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET engine PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(engine opengl32.lib)
target_link_libraries(engine glew32.lib)
target_link_libraries(engine glfw3.lib)
