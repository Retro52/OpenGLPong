//
// Created by Anton on 02.02.2023.
//

#include "Ball.h"


void Ball::Draw(const Shader& shader) const
{
    shader.Use();

    auto projection = glm::ortho((float)-Window::GetWidth() / 2,
        (float)+Window::GetWidth() / 2,
        (float)-Window::GetHeight() / 2,
        (float)+Window::GetHeight() / 2);

    shader.setMat4("transform", GetTransform());
    shader.setMat4("projection", projection);

    Quad::Draw(shader);
}