#include "Paddle.h"


Paddle::Paddle(float startX, float startY) {
    m_shape.setSize(sf::Vector2f(20.f, 100.f));
    m_shape.setFillColor(sf::Color::White);
    m_shape.setOrigin(sf::Vector2f(10.f, 50.f));
    position = { startX, startY };
    m_shape.setPosition(position);
}

void Paddle::update(float dt) {
    m_shape.setPosition(position);
}

void Paddle::draw(sf::RenderWindow& window) {
    window.draw(m_shape);
}

void Paddle::moveUp(float dt) {
    position.y -= m_speed * dt;
    if (position.y < 50.f) position.y = 50.f;
}

void Paddle::moveDown(float dt) {
    position.y += m_speed * dt;
    if (position.y > 550.f) position.y = 550.f;
}

sf::FloatRect Paddle::getBounds() const {
    return m_shape.getGlobalBounds();
}