//
// Created by Anton on 02.02.2023.
//

#ifndef OPENGLPONG_FIELD_H
#define OPENGLPONG_FIELD_H

#include <chrono>
#include <utility>

#include "Ball.h"
#include "glm/glm.hpp"
#include "EventsHandler.h"
#include "SoundHandler.hpp"
#include "PaddleFactory.hpp"
#include "InterfaceHandler.hpp"

class Field : public Quad, public Entity
{
public:
    explicit Field(std::shared_ptr<Ball> ball, std::shared_ptr<PaddleFactory> rPaddleFactory, std::shared_ptr<PaddleFactory> lPaddleFactory)
    : ball(std::move(ball)), rPaddleFactory(std::move(rPaddleFactory)), lPaddleFactory(std::move(lPaddleFactory))
    {
        lPaddle = this->lPaddleFactory->CreatePaddle(glm::vec2(- 1, 0));
        rPaddle = this->rPaddleFactory->CreatePaddle(glm::vec2(+ 1, 0));
        defaultColor = glm::vec3(0.0F);
    };

    explicit Field(std::shared_ptr<Ball> ball, std::shared_ptr<PaddleFactory> rPaddleFactory, std::shared_ptr<PaddleFactory> lPaddleFactory, std::shared_ptr<Texture> backgroundImage)
    :ball(std::move(ball)), rPaddleFactory(std::move(rPaddleFactory)), lPaddleFactory(std::move(lPaddleFactory)), Quad(std::move(backgroundImage))
    {
        lPaddle = this->lPaddleFactory->CreatePaddle(glm::vec2(- 1, 0));
        rPaddle = this->rPaddleFactory->CreatePaddle(glm::vec2(+ 1, 0));
        defaultColor = glm::vec3(0.0F);
    };

    explicit Field(std::shared_ptr<Ball> ball, std::shared_ptr<PaddleFactory> rPaddleFactory, std::shared_ptr<PaddleFactory> lPaddleFactory, std::shared_ptr<Texture> lPaddleTexture,
                   std::shared_ptr<Texture> rPaddleTexture, std::shared_ptr<Texture> backgroundImage)
            :ball(std::move(ball)), rPaddleFactory(std::move(rPaddleFactory)), lPaddleFactory(std::move(lPaddleFactory)), Quad(std::move(backgroundImage))
    {
        lPaddle = this->lPaddleFactory->CreatePaddle(glm::vec2(- 1, 0), std::move(lPaddleTexture));
        rPaddle = this->rPaddleFactory->CreatePaddle(glm::vec2(+ 1, 0), std::move(rPaddleTexture));
        defaultColor = glm::vec3(0.0F);
    };

    void Draw(const Shader& shader) const override;

    void Update(float deltaTime) override;

    [[nodiscard]] glm::vec2 GetScore() const
    {
        return score;
    }

    [[nodiscard]] bool IsGameStarted() const
    {
        return isGameStarted;
    }

    [[nodiscard]] bool IsGamePaused() const
    {
        return isGamePaused;
    }

    void SwitchPause()
    {
        if (!isGameStarted)
        {
            return;
        }

        isGamePaused = !isGamePaused;
    }

    void StartGame()
    {
        if (isGameStarted)
        {
            return;
        }

        ball->Reset();
        rPaddle->Reset();
        lPaddle->Reset();

        ball->GiveRandomVelocity();
        
        isGamePaused = false;
        isGameStarted = true;
    }

    void ResetGame()
    {
        ball->Reset();
        rPaddle->Reset();
        lPaddle->Reset();

        score = { 0, 0 };

        isGamePaused = false;
        isGameStarted = false;
    }
private:
    void Goal(const glm::vec2& pointDiff)
    {
        score += pointDiff;

        ball->Reset();
        ball->GiveRandomVelocity();

        SoundHandler::PlayAudio("sounds\\bleep.mp3");
    }

    void ClampPaddlePosition(std::shared_ptr<Paddle>& paddle)
    {
        auto paddlePos    = paddle->GetPos();
        auto paddleWidth  = paddle->GetWidth() / (float) Window::GetWidth();
        auto paddleHeight = paddle->GetHeight() / (float) Window::GetHeight();

        paddlePos.x = paddlePos.x < 0 ? xBorders.x + paddleWidth : xBorders.y - paddleWidth;
        paddlePos.y = glm::clamp(paddlePos.y, yBorders.x + paddleHeight, yBorders.y - paddleHeight);

        paddle->SetPosition(paddlePos);
    }

    void PlayCollisionSound()
    {
        SoundHandler::PlayAudio("sounds\\solid.wav");
    }
private:
    glm::vec2 score { 0, 0 };
    glm::vec2 xBorders { -1.0F, 1.0F }, yBorders { -1.0F, 1.0F };

    bool isGamePaused = false;
    bool isGameStarted = false;

    std::shared_ptr<Ball> ball;

    std::shared_ptr<PaddleFactory> lPaddleFactory;
    std::shared_ptr<PaddleFactory> rPaddleFactory;

    std::shared_ptr<Paddle> lPaddle;
    std::shared_ptr<Paddle> rPaddle;

    std::shared_ptr<Texture> background = nullptr;
};


#endif //OPENGLPONG_FIELD_H
