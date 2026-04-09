#include "Ball.h"

Ball::Ball(float startX, float startY) {
    m_shape.setRadius(10.f);
    m_shape.setFillColor(sf::Color::White);
    m_shape.setOrigin(sf::Vector2f(10.f, 10.f));

    reset(startX, startY);
}

void Ball::reset(float x, float y) {
    position = { x, y };
    m_velocity = { m_initialSpeed, m_initialSpeed * 0.5f };
    m_shape.setPosition(position);
}

void Ball::update(float dt) {
    position += m_velocity * dt;
    m_shape.setPosition(position);
}

void Ball::draw(sf::RenderWindow& window) {
    window.draw(m_shape);
}

void Ball::bounceX() {
    m_velocity.x = -m_velocity.x;
    m_velocity *= 1.08f;
}

void Ball::bounceY() {
    m_velocity.y = -m_velocity.y;
}

sf::FloatRect Ball::getBounds() const {
    return m_shape.getGlobalBounds();
}

sf::Vector2f Ball::getVelocity() const {
    return m_velocity;
}