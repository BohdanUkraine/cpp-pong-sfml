#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"

class Paddle : public Entity {
public:
    Paddle(float startX, float startY);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) override;

    void moveUp(float dt);
    void moveDown(float dt);

    sf::FloatRect getBounds() const;

private:
    sf::RectangleShape m_shape;
    const float m_speed = 400.f;
};