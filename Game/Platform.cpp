#include "Platform.hpp"

Platform::Platform(sf::Vector2f pos, sf::Vector2f size)
{
    shape.setPosition(pos);
    shape.setSize(size);
    shape.setFillColor(sf::Color(80, 80, 80));
}

void Platform::render(sf::RenderWindow& window)
{
    window.draw(shape);
}

sf::FloatRect Platform::getBounds()
{
    return shape.getGlobalBounds();
}