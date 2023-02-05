#include <iostream>

#include "Ball.h"
#include "Field.h"
#include "Window.h"
#include "Shader.h"
#include "EventsHandler.h"
#include "SoundHandler.hpp"
#include "InterfaceHandler.hpp"

int main()
{
    int windowWidth = 1920;
    int windowHeight = 1080;

    std::string scenePath;
    std::string windowName = "OpenGL Pong";

    std::string ballTexturePath;
    std::string backgroundTexturePath;
    std::string leftPaddleTexturePath;
    std::string rightPaddleTexturePath;

    try
    {
        Utils::LoadConfig(&windowWidth, &windowHeight, &windowName, &scenePath);
        Utils::LoadField(scenePath, &ballTexturePath, &backgroundTexturePath, &leftPaddleTexturePath, &rightPaddleTexturePath);
    }
    catch (Exception& e)
    {
        std::cerr << e.what();
    }

    Window::Initialize(windowWidth, windowHeight, windowName);

    EventsHandler::Initialize();
    SoundHandler::Initialize("sounds/");
    InterfaceHandler::Initialize("fonts/SHOWG.TTF", 64);

    Shader uiShader("shaders/ui.vs.glsl", "shaders/ui.fs.glsl");
    Shader mainShader("shaders/main.vs.glsl", "shaders/main.fs.glsl");

    Field field(
            std::make_shared<Ball>(Texture::CreateTexture(ballTexturePath)),

            std::make_shared<CustomPaddleFactory<EasyAIPaddle>>(),
            std::make_shared<CustomPaddleFactory<PlayerPaddle>>(),

            Texture::CreateTexture(leftPaddleTexturePath),
            Texture::CreateTexture(rightPaddleTexturePath),
            Texture::CreateTexture(backgroundTexturePath)
    );

    float lastTime = 0.0;
    float deltaTime = 0.0;

    while (!Window::IsShouldClose())
    {
        auto curTime = (float) glfwGetTime();

        deltaTime = curTime - lastTime;
        lastTime = curTime;

        if (EventsHandler::IsJustPressed(Key::Space))
        {
            field.StartGame();
        }
        if (EventsHandler::IsJustPressed(Key::R))
        {
            field.ResetGame();
        }
        if (EventsHandler::IsJustPressed(Key::P))
        {
            field.SwitchPause();
        }

        glClearColor(0.0F, 0.0F, 0.0F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);

        field.Draw(mainShader);
        field.Update(deltaTime);

        auto score = field.GetScore();
        auto isGamePaused = field.IsGamePaused();
        auto isGameStarted = field.IsGameStarted();

        if (!isGameStarted)
        {
            std::string firstMessage  = "Use W/S to move your paddle";
            std::string secondMessage = "Press R to restart, P to pause";
            std::string thirdMessage  = "Press SPACE to start";
            std::string fourthMessage = "Press ESC to exit";

            const int fontSize = 40;

            float firstMessageLeftOffset  = - InterfaceHandler::GetTextWidth(firstMessage, fontSize) / 2;
            float secondMessageLeftOffset = - InterfaceHandler::GetTextWidth(secondMessage, fontSize) / 2;
            float thirdMessageLeftOffset  = - InterfaceHandler::GetTextWidth(thirdMessage, fontSize) / 2;
            float fourthMessageLeftOffset = - InterfaceHandler::GetTextWidth(fourthMessage, fontSize) / 2;

            InterfaceHandler::RenderText(uiShader, firstMessage , firstMessageLeftOffset , + (float) Window::GetHeight() / 4 - 0  , fontSize, glm::vec3(255.0F, 255.0F, 255.0F) / 255.0F);
            InterfaceHandler::RenderText(uiShader, secondMessage, secondMessageLeftOffset, + (float) Window::GetHeight() / 4 - 50 , fontSize, glm::vec3(255.0F, 255.0F, 255.0F) / 255.0F);
            InterfaceHandler::RenderText(uiShader, thirdMessage , thirdMessageLeftOffset , + (float) Window::GetHeight() / 4 - 100, fontSize, glm::vec3(255.0F, 255.0F, 255.0F) / 255.0F);
            InterfaceHandler::RenderText(uiShader, fourthMessage, fourthMessageLeftOffset, + (float) Window::GetHeight() / 4 - 150, fontSize, glm::vec3(255.0F, 255.0F, 255.0F) / 255.0F);
        }

        if (isGamePaused)
        {
            std::string pauseMessage = "Game is paused, press P to continue, R to restart";

            const int fontSize = 40;

            float pauseMessageLeftOffset = -InterfaceHandler::GetTextWidth(pauseMessage, fontSize) / 2;

            InterfaceHandler::RenderText(uiShader, pauseMessage, pauseMessageLeftOffset, 0.0F, fontSize, glm::vec3(255.0F, 255.0F, 90.0F) / 255.0F);
        }

        InterfaceHandler::RenderText(uiShader, std::to_string((int)score.x), -(float)Window::GetWidth() / 4, +(float)Window::GetHeight() / 4, 64, (isGameStarted && !isGamePaused) ? glm::vec3(1.0F) : glm::vec3(0.6F));
        InterfaceHandler::RenderText(uiShader, std::to_string((int)score.y), +(float)Window::GetWidth() / 4, +(float)Window::GetHeight() / 4, 64, (isGameStarted && !isGamePaused) ? glm::vec3(1.0F) : glm::vec3(0.6F));

        if (EventsHandler::IsJustPressed(Key::Escape))
        {
            Window::SetShouldClose(true);
        }

        Window::SwapBuffers();
        EventsHandler::PullEvents();
    }

    Window::Terminate();
}
