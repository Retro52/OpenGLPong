//
// Created by Anton on 12.12.2021.
//

#include "Window.h"
#include "EventsHandler.h"
#include <iostream>
#include <thread>


void Window::Initialize(int w, int h, const std::string &name)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    Window::window = glfwCreateWindow(w, h, name.c_str(), nullptr, nullptr);

    if (Window::window == nullptr)
    {
        glfwTerminate();
        throw std::runtime_error("Failure during GLFW window creation");
    }

    glfwMakeContextCurrent(Window::window);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        throw std::runtime_error("Failure during GLEW window creation");
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    glClearColor(0.0f, 0.0f, 0.0f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwSwapInterval(0);
    Window::width = w;
    Window::height = h;
}

void Window::Terminate()
{
    glfwTerminate();
}

void Window::SetCursorMode(int cursorMode)
{
    glfwSetInputMode(Window::window, GLFW_CURSOR, cursorMode);
}

bool Window::IsShouldClose()
{
    return glfwWindowShouldClose(Window::window) != 0;
}

void Window::SetShouldClose(bool shouldClose)
{
    glfwSetWindowShouldClose(Window::window, shouldClose);
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(Window::window);
}

void Window::SetWidth(int w)
{
    if (400 < w && w < 3840)
    {
        Window::width = w;
    }
}

void Window::SetHeight(int h)
{
    if (400 < h && h < 2160)
    {
        Window::height = h;
    }
}

GLFWwindow * Window::GetCurrentWindow()
{
    return Window::window;
}

void Window::UpdateTitle(const char * newTitle)
{
    glfwSetWindowTitle(window, newTitle);
}

void Window::Update()
{
    glfwSetWindowSize(window, width, height);
}

int Window::GetWidth()
{
    return width;
}

int Window::GetHeight()
{
    return height;
}

