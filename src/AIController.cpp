#include "AIController.h"
#include <cmath>

AIController::AIController(Paddle& paddle, const Ball& ball)
    : m_controlledPaddle(paddle), m_ball(ball) {
}

void AIController::update() {
    float ballCenterY = m_ball.getPosition().y;
    float paddleCenterY = m_controlledPaddle.getPosition().y;
    float diff = ballCenterY - paddleCenterY;

    if (std::abs(diff) > REACTION_THRESHOLD) {
        float dir = (diff > 0.f) ? 1.f : -1.f;
        m_controlledPaddle.setVelocity({ 0.f, dir * m_controlledPaddle.getSpeed() });
    }
    else {
        m_controlledPaddle.setVelocity({ 0.f, 0.f });
    }
}
