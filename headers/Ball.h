#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

class Ball : public Entity {
public:
    static constexpr float RADIUS = 10.f;
    static constexpr float INITIAL_SPEED = 350.f;

    Ball();

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    sf::FloatRect getBounds()   const;
    sf::Vector2f  getPosition() const;
    sf::Vector2f  getVelocity() const;

    void setPosition(sf::Vector2f pos);
    void setVelocity(sf::Vector2f vel);

private:
    sf::CircleShape m_shape;
    sf::Vector2f    m_velocity;
};
