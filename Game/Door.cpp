#include "Door.hpp"
#include "ResourceManager.hpp"

Door::Door(sf::Vector2f pos, sf::Vector2f size)
    : basePos(pos), currentHeight(size.y), targetHeight(size.y), fullHeight(size.y)
    , sprite(ResourceManager::getInstance().loadTexture("door", "assets/door.png"))
{
    shape.setSize(size);
    shape.setPosition(pos);
    shape.setFillColor(sf::Color::Transparent); // invisible

    // Scale X pour correspondre à la largeur de la porte
    float scaleX = size.x / 50.f;
    sprite.setScale({ scaleX, 1.f }); // Y pas scalé, on utilise textureRect
    sprite.setPosition(pos);
}

void Door::update(bool activated, float dt)
{
    open = activated;
    targetHeight = open ? 0.f : fullHeight;

    currentHeight += (targetHeight - currentHeight) * 5.f * dt;

    shape.setSize({ shape.getSize().x, currentHeight });
    shape.setPosition({ basePos.x, basePos.y + (fullHeight - currentHeight) });
}

void Door::render(sf::RenderWindow& window)
{
    if (currentHeight < 1.f) return;

    float ratio = currentHeight / fullHeight;
    int texHeight = static_cast<int>(900.f * ratio);

    // On prend le bas de la texture (la porte monte = on cache le haut)
    sprite.setTextureRect(sf::IntRect(
        { 0, 900 - texHeight },  // partir du bas
        { 50, texHeight }
    ));

    sprite.setPosition(shape.getPosition());
    window.draw(sprite);
}

sf::FloatRect Door::getBounds() const
{
    if (currentHeight < 2.f)
        return sf::FloatRect({ 0.f, 0.f }, { 0.f, 0.f });
    return shape.getGlobalBounds();
}