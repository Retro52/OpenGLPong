//
// Created by Anton on 12.12.2021.
//

#include "EventsHandler.h"
#include "Window.h"
#include <array>
#include <iostream>


/**
 * Cursor movement callback
 * @param window current glfw window
 * @param xpos mouse x position
 * @param ypos mouse y position
 */
void EventsHandler::CursorCallback(GLFWwindow *window /* window */, double xpos, double ypos)
{
    if (EventsHandler::isCursorStarted)
    {
        EventsHandler::deltaX += (float) xpos - EventsHandler::x;
        EventsHandler::deltaY += (float) ypos - EventsHandler::y;
    }
    else
    {
        EventsHandler::isCursorStarted = true;
    }

    EventsHandler::x = (float) xpos;
    EventsHandler::y = (float) ypos;
}

/**
 * Mouse button click callback, look OpenGL button defines for more
 * @param window current glfw window
 * @param button id of the button
 * @param action id of the action
 * @param mode id of the mode
 */
void EventsHandler::MouseCallback(GLFWwindow * /* window */, int button, int action, int /* mode */)
{
    if (MOUSE_BUTTONS_OFFSET + button > 1032 || button < 0)
    {
        return;
    }
    if (action == GLFW_PRESS)
    {
        EventsHandler::pressedKeys.at(MOUSE_BUTTONS_OFFSET + button) = true;
        EventsHandler::keyPressFrame.at(MOUSE_BUTTONS_OFFSET + button) = EventsHandler::curFrame;
    }
    else if (action == GLFW_RELEASE)
    {
        EventsHandler::pressedKeys.at(MOUSE_BUTTONS_OFFSET + button) = false;
        EventsHandler::keyPressFrame.at(MOUSE_BUTTONS_OFFSET + button) = EventsHandler::curFrame;
    }
}

/**
 * Key action callback
 * @param window current glfw window
 * @param key key id, look OpenGL defines for more
 * @param scancode scancode id
 * @param action action id
 * @param mode mode id
 */
void EventsHandler::KeyCallback(GLFWwindow * /* window */, int key, int /* scancode */ , int action, int /* mode */)
{
    if (key > MOUSE_BUTTONS_OFFSET || key < 0)
    {
        return;
    }
    if (action == GLFW_PRESS)
    {
        EventsHandler::pressedKeys.at(key) = true;
        EventsHandler::keyPressFrame.at(key) = EventsHandler::curFrame;
    }
    else if (action == GLFW_RELEASE)
    {
        EventsHandler::pressedKeys.at(key) = false;
        EventsHandler::keyPressFrame.at(key) = EventsHandler::curFrame;
    }
}

/**
 * Callback on window size change
 * @param window glfw current window
 * @param width new width
 * @param height new height
 */
void EventsHandler::WindowCallback(GLFWwindow * /*window*/, int width, int height)
{
    glViewport(0,0, width, height);
    Window::SetHeight(height);
    Window::SetWidth(width);
    Window::Update();
}

int EventsHandler::Initialize()
{
    GLFWwindow * window = Window::GetCurrentWindow();

    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseCallback);
    glfwSetCursorPosCallback(window, CursorCallback);
    glfwSetWindowSizeCallback(window, WindowCallback);
    return 0;
}

bool EventsHandler::IsPressed(int keycode)
{
    if (keycode < 0 || keycode >= MOUSE_BUTTONS_OFFSET)
    {
        return false;
    }
    return pressedKeys.at(keycode);
}

bool EventsHandler::IsJustPressed(int keycode)
{
    if (keycode < 0 || keycode >= MOUSE_BUTTONS_OFFSET)
    {
        return false;
    }
    return pressedKeys.at(keycode) && keyPressFrame.at(keycode) == curFrame;
}

bool EventsHandler::IsClicked(int button)
{
    int index = MOUSE_BUTTONS_OFFSET + button;
    return pressedKeys.at(index);
}

bool EventsHandler::IsJustClicked(int button)
{
    int index = MOUSE_BUTTONS_OFFSET + button;
    return pressedKeys.at(index) && keyPressFrame.at(index) == curFrame;
}

void EventsHandler::ToggleCursor()
{
    isCursorLocked = !isCursorLocked;
    Window::SetCursorMode(isCursorLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void EventsHandler::ToggleCursor(bool shouldBeVisible)
{
    isCursorLocked = shouldBeVisible;
    Window::SetCursorMode(isCursorLocked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void EventsHandler::PullEvents()
{
    curFrame++;
    deltaX = 0.0f;
    deltaY = 0.0f;
    glfwPollEvents();
}