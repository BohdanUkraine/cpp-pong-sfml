#include "AIController.h"

AIController::AIController(Paddle& paddle, const Ball& ball)
    : m_controlledPaddle(paddle), m_ball(ball) {
}

void AIController::update(float dt) {
    float ballY = m_ball.getBounds().position.y + m_ball.getBounds().size.y / 2.f;
    float paddleY = m_controlledPaddle.getBounds().position.y + m_controlledPaddle.getBounds().size.y / 2.f;

    if (std::abs(ballY - paddleY) > m_reactionThreshold) {
        if (ballY > paddleY) {
            m_controlledPaddle.moveDown(dt);
        }
        else {
            m_controlledPaddle.moveUp(dt);
        }
    }
    m_controlledPaddle.update(dt);
}