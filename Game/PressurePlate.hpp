#pragma once
#include <SFML/Graphics.hpp>

class PressurePlate
{
private:
    sf::RectangleShape shape;
    bool activated = false;

public:
    PressurePlate(sf::Vector2f pos);
    void update(sf::FloatRect boxBounds);
    void render(sf::RenderWindow& window);
    bool isActivated() const { return activated; }
    sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }
};