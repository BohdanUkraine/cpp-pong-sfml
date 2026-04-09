#pragma once
#include "Paddle.h"
#include "Ball.h"

class AIController {
public:
    AIController(Paddle& paddle, const Ball& ball);

    void update(float dt);

private:
    Paddle& m_controlledPaddle;
    const Ball& m_ball;

    const float m_reactionThreshold = 10.f;
};