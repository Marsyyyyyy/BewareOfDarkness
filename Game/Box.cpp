#include "Box.hpp"
#include "Constants.hpp"    
#include "ResourceManager.hpp"
#include <iostream>

Box::Box(sf::Vector2f pos)
    : sprite(ResourceManager::getInstance().loadTexture("box", "assets/box.png"))
{
    shape.setSize({ 60.f, 60.f });
    shape.setPosition(pos);
    shape.setFillColor(sf::Color::Transparent);

    float scale = 60.f / 300.f;
    sprite.setScale({ scale, scale });
    sprite.setPosition(pos);
}

void Box::update(float dt, std::vector<Platform>& platforms,
    sf::FloatRect playerBounds, sf::Vector2f playerVel,
    sf::FloatRect plateBounds)
{
    velocity.y += Constants::GRAVITY * dt;

    // Reset velocity X chaque frame
    velocity.x = 0.f;

    auto intersection = shape.getGlobalBounds().findIntersection(playerBounds);
    if (intersection)
    {
        float playerBottom = playerBounds.position.y + playerBounds.size.y;
        float boxTop = shape.getPosition().y;
        bool playerOnTop = playerBottom < boxTop + 8.f;

        if (!playerOnTop && std::abs(playerVel.x) > 10.f)
        {
            velocity.x = playerVel.x;
        }
    }

    // Déplacement X
    shape.move(sf::Vector2f(velocity.x * dt, 0.f));
    for (auto& p : platforms)
    {
        auto inter = shape.getGlobalBounds().findIntersection(p.getBounds());
        if (inter)
        {
            if (velocity.x > 0) shape.move(sf::Vector2f(-inter->size.x, 0.f));
            if (velocity.x < 0) shape.move(sf::Vector2f(inter->size.x, 0.f));
            velocity.x = 0;
        }
    }

    // Déplacement Y
    shape.move(sf::Vector2f(0.f, velocity.y * dt));
    isGrounded = false;
    for (auto& p : platforms)
    {
        auto inter = shape.getGlobalBounds().findIntersection(p.getBounds());
        if (inter)
        {
            if (velocity.y > 0) { shape.move(sf::Vector2f(0.f, -inter->size.y)); velocity.y = 0; isGrounded = true; }
            if (velocity.y < 0) { shape.move(sf::Vector2f(0.f, inter->size.y)); velocity.y = 0; }
        }
    }

    auto onPlate = shape.getGlobalBounds().findIntersection(plateBounds);
    if (onPlate && isGrounded)
        spriteOffset = -8.f; // monter le sprite
    else
        spriteOffset = 0.f;
}

void Box::render(sf::RenderWindow& window)
{
    sprite.setPosition(shape.getPosition() + sf::Vector2f(0.f, spriteOffset));
    window.draw(sprite);
}