//
// Created by Anton on 02.02.2023.
//

#ifndef OPENGLPONG_BALL_H
#define OPENGLPONG_BALL_H

#include <random>
#include <utility>
#include <iostream>
#include <functional>

#include "Sprite.h"
#include "Window.h"
#include "Shader.h"
#include "Texture.h"
#include "Entity.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Ball : public Quad, public Entity
{
public:
    explicit Ball()
    {
        defaultColor = glm::vec3(1.0F);
    }

    explicit Ball(std::shared_ptr<Texture> ballTexture) : Quad(std::move(ballTexture))
    {
        defaultColor = glm::vec3(1.0F);
    };

    explicit Ball(const glm::vec2& startPos)
    {
        pos = startPos;
        defaultColor = glm::vec3(1.0F);
    }

    Ball(const glm::vec2& startPos, std::shared_ptr<Texture> ballTexture) : Quad(std::move(ballTexture))
    {
        pos = startPos;
        defaultColor = glm::vec3(1.0F);
    }

    ~Ball() = default;

    [[nodiscard]] glm::mat4 GetTransform() const override
    {
        return glm::translate(glm::mat4(1.0F), glm::vec3(pos.x, pos.y, 0.0F)) * glm::scale(glm::mat4(1.0F), glm::vec3(radius));
    }
    
    void Draw(const Shader& shader) const override;

    void GiveRandomVelocity()
    {
        static std::default_random_engine eng;
        static std::uniform_real_distribution<float> distributionPositive(defaultSpeed / 2, defaultSpeed);
        static std::uniform_real_distribution<float> distributionNegative(- defaultSpeed, -defaultSpeed / 2);
        static auto randomBool = [](){ static std::uniform_int_distribution<> distributionBoolean(0, 1); return distributionBoolean(eng); };

        velocity.x = randomBool() ? distributionPositive(eng) : distributionNegative(eng);
        velocity.y = randomBool() ? distributionPositive(eng) : distributionNegative(eng);
    }

    void Reset()
    {
        pos = { 0, 0 };
        velocity = { 0, 0 };
    }

    void Update(float deltaTime) override
    {
        velocity.x = glm::clamp(velocity.x, - speedLimit, speedLimit);
        velocity.y = glm::clamp(velocity.y, - speedLimit, speedLimit);

        pos.x += deltaTime * velocity.x;
        pos.y += deltaTime * velocity.y;
    };

    [[nodiscard]] float GetRadius() const { return radius; }

private:
    const float radius = 50.0F, defaultSpeed = 1.0F, speedLimit = 7.0F;
};


#endif //OPENGLPONG_BALL_H
