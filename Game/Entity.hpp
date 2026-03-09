#pragma once

#include <SFML/Graphics.hpp>

class Entity
{
protected:

    sf::RectangleShape body;
    sf::Vector2f velocity;

public:

    virtual void update(float dt) {}
    virtual void render(sf::RenderWindow& window);

    sf::Vector2f getPosition() const;
};