#pragma once
#include <SFML/Graphics.hpp>

class VictoryZone
{
private:
    sf::RectangleShape shape;

public:
    VictoryZone(sf::Vector2f pos, sf::Vector2f size);
    bool checkPlayer(sf::FloatRect playerBounds) const;
    void render(sf::RenderWindow& window);
};