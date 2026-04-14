#include "Ball.h"

Ball::Ball() {
    m_shape.setRadius(RADIUS);
    m_shape.setFillColor(sf::Color::White);
    m_shape.setOrigin(sf::Vector2f(RADIUS, RADIUS));
}

void Ball::update(float dt) {
    m_shape.setPosition(m_shape.getPosition() + m_velocity * dt);
}

void Ball::draw(sf::RenderWindow& window) {
    window.draw(m_shape);
}

sf::FloatRect Ball::getBounds() const {
    return m_shape.getGlobalBounds();
}

sf::Vector2f Ball::getPosition() const {
    return m_shape.getPosition();
}

sf::Vector2f Ball::getVelocity() const {
    return m_velocity;
}

void Ball::setPosition(sf::Vector2f pos) {
    m_shape.setPosition(pos);
}

void Ball::setVelocity(sf::Vector2f vel) {
    m_velocity = vel;
}
