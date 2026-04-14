#pragma once
#include "Paddle.h"
#include "Ball.h"

class AIController {
public:
    AIController(Paddle& paddle, const Ball& ball);

    void update();

private:
    Paddle& m_controlledPaddle;
    const Ball& m_ball;

    static constexpr float REACTION_THRESHOLD = 10.f;
};
