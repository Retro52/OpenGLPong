//
// Created by Anton on 03.02.2023.
//

#include "Paddle.h"

void Paddle::Draw(const Shader &shader) const
{
    shader.Use();

    auto projection = glm::ortho((float) - Window::GetWidth() / 2,
                                 (float) + Window::GetWidth() / 2,
                                 (float) - Window::GetHeight() / 2,
                                 (float) + Window::GetHeight() / 2);

    shader.setMat4("transform", GetTransform());
    shader.setMat4("projection", projection);

    Quad::Draw(shader);
}
