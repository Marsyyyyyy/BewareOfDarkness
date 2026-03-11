#include "Platform.hpp"
#include "ResourceManager.hpp"

Platform::Platform(sf::Vector2f pos, sf::Vector2f size)
    : sprite(ResourceManager::getInstance().loadTexture("floor", "assets/floor.png"))
    , bounds(pos, size)
{
    auto& tex = ResourceManager::getInstance().getTexture("floor");

    // Tiling : répéter la texture sur toute la largeur
    tex.setRepeated(true);
    sprite.setTextureRect(sf::IntRect(
        { 0, 0 },
        { (int)size.x, (int)tex.getSize().y }
    ));

    // Scale en hauteur pour correspondre à la taille de la plateforme
    float scaleY = size.y / tex.getSize().y;
    sprite.setScale({ 1.f, scaleY });
    sprite.setPosition(pos);
}

void Platform::render(sf::RenderWindow& window)
{
    window.draw(sprite);
}

sf::FloatRect Platform::getBounds()
{
    return bounds;
}