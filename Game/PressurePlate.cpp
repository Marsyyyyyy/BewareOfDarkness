#include "PressurePlate.hpp"

PressurePlate::PressurePlate(sf::Vector2f pos)
{
    shape.setSize({ 80.f, 15.f });
    shape.setPosition(pos);
    shape.setFillColor(sf::Color(180, 0, 180));
}

void PressurePlate::update(sf::FloatRect boxBounds)
{
    if (activated) return; // déjà activée, on ne change plus rien

    activated = shape.getGlobalBounds().findIntersection(boxBounds).has_value();
    shape.setFillColor(activated ? sf::Color::Green : sf::Color(180, 0, 180));
}

void PressurePlate::render(sf::RenderWindow& window)
{
    window.draw(shape);
}