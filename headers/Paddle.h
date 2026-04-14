#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

class Paddle : public Entity {
public:
    static constexpr float WIDTH = 20.f;
    static constexpr float HEIGHT = 100.f;
    static constexpr float SPEED = 400.f;

    Paddle();

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    sf::FloatRect getBounds()   const;
    sf::Vector2f  getPosition() const;
    sf::Vector2f  getVelocity() const;
    float         getSpeed()    const;

    void setPosition(sf::Vector2f pos);
    void setVelocity(sf::Vector2f vel);

private:
    sf::RectangleShape m_shape;
    sf::Vector2f       m_velocity;
};
