//
// Created by Anton on 03.02.2023.
//

#ifndef OPENGLPONG_PADDLE_H
#define OPENGLPONG_PADDLE_H

#include <utility>

#include "Ball.h"
#include "EventsHandler.h"

class Paddle : public Quad, public Entity
{
public:
    explicit Paddle()
    {
        defaultColor = glm::vec3(1.0F);
    };
    explicit Paddle(const glm::vec2& paddlePos) : Paddle()
    {
        pos = paddlePos;
    };

    explicit Paddle(const glm::vec2& paddlePos, std::shared_ptr<Texture> paddleTexture) : Quad(std::move(paddleTexture))
    {
        pos = paddlePos;
        defaultColor = glm::vec3(1.0F);
    };


    virtual void UpdateControls(const std::shared_ptr<Ball>& ball) {};

    void Update(float deltaTime) override
    {
        pos.y += velocity.y * deltaTime;
    };

    [[nodiscard]] glm::mat4 GetTransform() const override
    {
        return glm::translate(glm::mat4(1.0F), glm::vec3(pos.x, pos.y, 0.0F)) * glm::scale(glm::mat4(1.0F), glm::vec3(width, height, 1.0F));
    }

    void Draw(const Shader& shader) const override;

    void Reset()
    {
        pos.y = 0;
        velocity = { 0, 0 };
    }

    [[nodiscard]] float GetWidth() const
    {
        return width;
    }

    [[nodiscard]] float GetHeight() const
    {
        return height;
    }

    [[nodiscard]] float GetSpeed() const
    {
        return speed;
    }
protected:
    const float width = 30.0F, height = 300.0F, speed = 3.0F;
};

class EasyAIPaddle : public Paddle
{
public:
    explicit EasyAIPaddle() = default;
    explicit EasyAIPaddle(const glm::vec2& paddlePos) : Paddle(paddlePos) {};
    explicit EasyAIPaddle(const glm::vec2& paddlePos, std::shared_ptr<Texture> paddleTexture) : Paddle(paddlePos, std::move(paddleTexture)) {};


    void UpdateControls(const std::shared_ptr<Ball>& ball) override
    {
        velocity.y = 0.0F;

        auto ballPos = ball->GetPos();
        auto ballVel = ball->GetVelocity();

        const float dumbnessMultiplyer = 0.4F;

        if ((ballVel.x > 0 && pos.x > 0) || (ballVel.x < 0 && pos.x < 0))
        {
            if (ballPos.y > pos.y)
            {
                velocity.y = speed * dumbnessMultiplyer;
            }
            else if (ballPos.y < pos.y)
            {
                velocity.y = -speed * dumbnessMultiplyer;
            }
        }
        else
        {
            if (pos.y < 0)
            {
                velocity.y = speed * dumbnessMultiplyer;
            }
            else if (pos.y > 0)
            {
                velocity.y = -speed * dumbnessMultiplyer;
            }
        }
    }
};

class AIPaddle : public Paddle
{
public:
    explicit AIPaddle() = default;
    explicit AIPaddle(const glm::vec2& paddlePos) : Paddle(paddlePos) {};
    explicit AIPaddle(const glm::vec2& paddlePos, std::shared_ptr<Texture> paddleTexture) : Paddle(paddlePos, std::move(paddleTexture)) {};

    void UpdateControls(const std::shared_ptr<Ball>& ball) override
    {
        velocity.y = 0.0F;

        auto ballPos = ball->GetPos();
        auto ballVel = ball->GetVelocity();

        if ((ballVel.x > 0 && pos.x > 0) || (ballVel.x < 0 && pos.x < 0))
        {
            if (ballPos.y > pos.y)
            {
                velocity.y = speed;
            }
            else if (ballPos.y < pos.y)
            {
                velocity.y = - speed;
            }
        }
        else
        {
            if (pos.y < 0)
            {
                velocity.y = speed;
            }
            else if (pos.y > 0)
            {
                velocity.y = - speed;
            }
        }
    }
};

class PlayerPaddle : public Paddle
{
public:
    explicit PlayerPaddle() = default;
    explicit PlayerPaddle(const glm::vec2& paddlePos) : Paddle(paddlePos) {};
    explicit PlayerPaddle(const glm::vec2& paddlePos, std::shared_ptr<Texture> paddleTexture) : Paddle(paddlePos, std::move(paddleTexture)) {};

    void UpdateControls(const std::shared_ptr<Ball>& /* ball */) override
    {
        velocity.y = 0.0F;

        if (EventsHandler::IsPressed(Key::W))
        {
            velocity.y = speed;
        }

        if (EventsHandler::IsPressed(Key::S))
        {
            velocity.y = - speed;
        }
    }
};

#endif //OPENGLPONG_PADDLE_H
