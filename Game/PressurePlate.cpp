#include "PressurePlate.hpp"
#include "ResourceManager.hpp"

PressurePlate::PressurePlate(sf::Vector2f pos)
    : sprite(ResourceManager::getInstance().loadTexture("button", "assets/button.png"))
    , spriteActivated(ResourceManager::getInstance().loadTexture("button_pushed", "assets/button_pushed.png"))
{
    shape.setSize({ 80.f, 15.f });
    shape.setPosition(pos);
    shape.setFillColor(sf::Color::Transparent);

    float scaleX = 80.f / 256.f;
    float scaleY = 15.f / 64.f;

    sprite.setScale({ scaleX, scaleY });
    spriteActivated.setScale({ scaleX, scaleY });
    sprite.setPosition(pos);
    spriteActivated.setPosition(pos);
}

void PressurePlate::update(sf::FloatRect boxBounds)
{
    if (activated) return;
    activated = shape.getGlobalBounds().findIntersection(boxBounds).has_value();
}

void PressurePlate::render(sf::RenderWindow& window)
{
    sprite.setPosition(shape.getPosition());
    spriteActivated.setPosition(shape.getPosition());

    if (activated)
        window.draw(spriteActivated);
    else
        window.draw(sprite);
}