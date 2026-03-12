#include "VictoryZone.hpp"

VictoryZone::VictoryZone(sf::Vector2f pos, sf::Vector2f size)
{
    shape.setSize(size);
    shape.setPosition(pos);
    shape.setFillColor(sf::Color(0, 255, 0, 60)); // vert transparent
}

bool VictoryZone::checkPlayer(sf::FloatRect playerBounds) const
{
    return shape.getGlobalBounds().findIntersection(playerBounds).has_value();
}

void VictoryZone::render(sf::RenderWindow& window)
{
    window.draw(shape);
}