//
// Created by Anton on 02.02.2023.
//

#include "Field.h"

void Field::Draw(const Shader &shader) const
{
    shader.Use();

    auto transform = glm::scale(glm::mat4(1.0F), glm::vec3(Window::GetWidth(), Window::GetHeight(), 0.0F));

    auto projection = glm::ortho((float)-Window::GetWidth() / 2,
        (float)+Window::GetWidth() / 2,
        (float)-Window::GetHeight() / 2,
        (float)+Window::GetHeight() / 2);

    shader.setMat4("transform", transform);
    shader.setMat4("projection", projection);

    Quad::Draw(shader);

    ball->Draw(shader);
    lPaddle->Draw(shader);
    rPaddle->Draw(shader);
}

void Field::Update(float deltaTime)
{
    // clamping paddles position
    ClampPaddlePosition(rPaddle);
    ClampPaddlePosition(lPaddle);

    // the following code is basically collision checks and score updates, no need to do all that if game was not started or is paused
    if (!isGameStarted || isGamePaused)
    {
        return;
    }

    // receiving all the necessary variables
    auto ballPos  = ball->GetPos();
    auto ballVel  = ball->GetVelocity();
    auto ballRadX = ball->GetRadius() / (float) Window::GetWidth();
    auto ballRadY = ball->GetRadius() / (float) Window::GetHeight();

    auto lPaddlePos    = lPaddle->GetPos();
    auto lPaddleWidth  = lPaddle->GetWidth() / (float) Window::GetWidth();
    auto lPaddleHeight = lPaddle->GetHeight() / (float) Window::GetHeight();

    auto rPaddlePos    = rPaddle->GetPos();
    auto rPaddleWidth  = rPaddle->GetWidth() / (float) Window::GetWidth();
    auto rPaddleHeight = rPaddle->GetHeight() / (float) Window::GetHeight();

    bool collisionSolved = false;

    const float ballVelOnHitMult = 1.1F;

    // top bottom collision check
    if ((ballPos.y + ballRadY > yBorders.y && ballVel.y > 0) || (ballPos.y - ballRadY < yBorders.x && ballVel.y < 0))
    {
        collisionSolved = true;

        ballVel.y = -ballVel.y * ballVelOnHitMult;
        PlayCollisionSound();
    }

    // right & left paddle | ball collision solving
    //  check ball direction | ball is overlapping paddle along OX              |    ball bottom is lower than paddle top               |      ball top is higher than paddle bottom
    if ((ballVel.x > 0) && (ballPos.x + ballRadX > rPaddlePos.x - rPaddleWidth) && (ballPos.y - ballRadY < rPaddlePos.y + rPaddleHeight && ballPos.y + ballRadY > rPaddlePos.y - rPaddleHeight) ||
        (ballVel.x < 0) && (ballPos.x - ballRadX < lPaddlePos.x + lPaddleWidth) && (ballPos.y - ballRadY < lPaddlePos.y + lPaddleHeight && ballPos.y + ballRadY > lPaddlePos.y - lPaddleHeight)
            )
    {
        collisionSolved = true;

        ballVel.x = - ballVel.x * ballVelOnHitMult;
        PlayCollisionSound();
    }

    // left & right walls | ball collision solving
    if (ballPos.x + ballRadX > xBorders.y && !collisionSolved)
    {
        Goal( {1, 0} );
    }
    else if (ballPos.x - ballRadX < xBorders.x && !collisionSolved)
    {
        Goal( {0, 1} );
    }
    else
    {
        ball->SetVelocity(ballVel);
    }

    rPaddle->UpdateControls(ball);
    lPaddle->UpdateControls(ball);

    ball->Update(deltaTime);

    rPaddle->Update(deltaTime);
    lPaddle->Update(deltaTime);
}