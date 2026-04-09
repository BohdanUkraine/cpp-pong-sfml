#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

class Ball : public Entity {
public:
    Ball(float startX, float startY);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    void bounceX();
    void bounceY();

    void reset(float x, float y);

    sf::FloatRect getBounds() const;
    sf::Vector2f getVelocity() const;

private:
    sf::CircleShape m_shape;
    sf::Vector2f m_velocity;
    const float m_initialSpeed = 350.f;
};