cmake_minimum_required(VERSION 3.17)
project(OpenGLPong)

set(CMAKE_CXX_STANDARD 17)

# Disabling min and max functions defined into the Windows.h header if compiling with MSVC (causes problems with std::numeric limits)
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_definitions(NOMINMAX)
endif()

add_compile_definitions(GLEW_STATIC)
add_compile_definitions(IRRKLANG_STATIC)

# necessary includes
include_directories(vendors/include/GLFW)
link_directories(vendors/lib/GLFW)

include_directories(vendors/include/GLEW)
link_directories(vendors/lib/GLEW)

include_directories(vendors/include/irrKlang)
link_directories(vendors/lib/irrKlang)


include_directories(src/)
include_directories(vendors/include)
include_directories(vendors/include/freetype)
link_directories(vendors/lib/freetype)


add_executable(OpenGLPong src/Window.h src/Window.cpp src/EventsHandler.h src/EventsHandler.cpp src/main.cpp src/Ball.cpp src/Ball.h src/Texture.cpp src/Texture.h src/Utils.hpp src/Utils.cpp src/Field.cpp src/Field.h src/Shader.cpp src/Shader.h src/Sprite.cpp src/Sprite.h src/Entity.hpp src/Paddle.cpp src/Paddle.h src/PaddleFactory.hpp src/InterfaceHandler.hpp src/SoundHandler.hpp)

target_link_libraries(OpenGLPong glew32s OpenGL32.lib glfw3 freetype irrKlang)
