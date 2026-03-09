#pragma once

#include <SFML/Graphics.hpp>

class Platform
{
private:

    sf::RectangleShape shape;

public:

    Platform(sf::Vector2f pos, sf::Vector2f size);

    void render(sf::RenderWindow& window);

    sf::FloatRect getBounds();
};