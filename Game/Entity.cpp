#include "Entity.hpp"

void Entity::render(sf::RenderWindow& window)
{
    window.draw(body);
}

sf::Vector2f Entity::getPosition() const
{
    return body.getPosition();
}