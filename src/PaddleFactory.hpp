//
// Created by Anton on 03.02.2023.
//

#ifndef OPENGLPONG_PADDLEFACTORY_HPP
#define OPENGLPONG_PADDLEFACTORY_HPP

#include "Paddle.h"

class PaddleFactory
{
public:
    [[nodiscard]] virtual std::shared_ptr<Paddle> CreatePaddle() const = 0;
    [[nodiscard]] virtual std::shared_ptr<Paddle> CreatePaddle(const glm::vec2& paddlePos) const = 0;
    [[nodiscard]] virtual std::shared_ptr<Paddle> CreatePaddle(const glm::vec2& paddlePos, std::shared_ptr<Texture> paddleTexture) const = 0;
};

template<typename T>
class CustomPaddleFactory : public PaddleFactory
{
public:
    [[nodiscard]] std::shared_ptr<Paddle> CreatePaddle() const override
    {
        static_assert(std::is_base_of_v<Paddle, T>);
        return std::make_shared<T>();
    }

    [[nodiscard]] std::shared_ptr<Paddle> CreatePaddle(const glm::vec2& paddlePos) const override
    {
        static_assert(std::is_base_of_v<Paddle, T>);
        return std::make_shared<T>(paddlePos);
    }

    [[nodiscard]] std::shared_ptr<Paddle> CreatePaddle(const glm::vec2& paddlePos, std::shared_ptr<Texture> paddleTexture) const override
    {
        static_assert(std::is_base_of_v<Paddle, T>);
        return std::make_shared<T>(paddlePos, paddleTexture);
    }
};


#endif //OPENGLPONG_PADDLEFACTORY_HPP
