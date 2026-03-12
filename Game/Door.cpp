#include "Door.hpp"

Door::Door(sf::Vector2f pos, sf::Vector2f size)
    : basePos(pos), currentHeight(size.y), targetHeight(size.y), fullHeight(size.y)
{
    shape.setSize(size);
    shape.setPosition(pos);
    shape.setFillColor(sf::Color(100, 180, 255)); // bleu visible
}

void Door::update(bool activated, float dt)
{
    open = activated;
    targetHeight = open ? 0.f : fullHeight;

    // Animation smooth
    currentHeight += (targetHeight - currentHeight) * 5.f * dt;

    // La porte monte depuis le bas (s'ouvre vers le haut)
    shape.setSize({ shape.getSize().x, currentHeight });
    shape.setPosition({ basePos.x, basePos.y + (fullHeight - currentHeight) });
}

void Door::render(sf::RenderWindow& window)
{
    if (currentHeight > 1.f)
        window.draw(shape);
}

sf::FloatRect Door::getBounds() const
{
    // Retourner bounds réelles même en cours d'animation
    // Seulement vide si complètement ouverte
    if (currentHeight < 2.f)
        return sf::FloatRect({ 0.f, 0.f }, { 0.f, 0.f });
    return shape.getGlobalBounds();
}