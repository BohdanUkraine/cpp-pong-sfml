#include "Paddle.h"

Paddle::Paddle() {
    m_shape.setSize({ WIDTH, HEIGHT });
    m_shape.setFillColor(sf::Color::White);
    m_shape.setOrigin({ WIDTH / 2.f, HEIGHT / 2.f });
}

void Paddle::update(float dt) {
    m_shape.setPosition(m_shape.getPosition() + m_velocity * dt);
}

void Paddle::draw(sf::RenderWindow& window) {
    window.draw(m_shape);
}

sf::FloatRect Paddle::getBounds() const {
    return m_shape.getGlobalBounds();
}

sf::Vector2f Paddle::getPosition() const {
    return m_shape.getPosition();
}

sf::Vector2f Paddle::getVelocity() const {
    return m_velocity;
}

float Paddle::getSpeed() const {
    return SPEED;
}

void Paddle::setPosition(sf::Vector2f pos) {
    m_shape.setPosition(pos);
}

void Paddle::setVelocity(sf::Vector2f vel) {
    m_velocity = vel;
}
