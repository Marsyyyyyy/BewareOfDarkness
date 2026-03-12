#include "Player.hpp"
#include "Constants.hpp"
#include "ResourceManager.hpp"
#include <SFML/Window/Keyboard.hpp>

Player::Player() : sprite(ResourceManager::getInstance().loadTexture("player", "assets/PlayerUp.png"))
{
    isGrounded = false;
    sprite.setPosition({ 100.f, 100.f });

    // Default to aiming right
    lastAimAngle = 0.f;
}

sf::Vector2f Player::getPosition() const
{
    sf::FloatRect bounds = sprite.getGlobalBounds();
    return { bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f };
}

sf::Vector2f Player::getVelocity() const
{
    return velocity;
}

float Player::getAimAngle() const
{
    return lastAimAngle;
}

sf::FloatRect Player::getBounds() const { return sprite.getGlobalBounds(); }

void Player::update(float dt, std::vector<Platform>& platforms, const GameSettings& settings)
{
    velocity.x = 0;

    // Check horizontal movement and update aim
    if (sf::Keyboard::isKeyPressed(settings.moveLeft))
    {
        velocity.x = -Constants::PLAYER_SPEED;
        lastAimAngle = 180.f; // Aim Left
    }
    if (sf::Keyboard::isKeyPressed(settings.moveRight))
    {
        velocity.x = Constants::PLAYER_SPEED;
        lastAimAngle = 0.f; // Aim Right
    }

    if (sf::Keyboard::isKeyPressed(settings.moveUp))
    {
        lastAimAngle = -90.f; // Aim Up
    }

    if (sf::Keyboard::isKeyPressed(settings.flashlight) && isGrounded)
    {
        velocity.y = Constants::PLAYER_JUMP;
        isGrounded = false;
    }

    velocity.y += Constants::GRAVITY * dt;

    sprite.move({ velocity.x * dt, 0.f });
    for (auto& p : platforms)
    {
        auto intersection = sprite.getGlobalBounds().findIntersection(p.getBounds());
        if (intersection)
        {
            if (velocity.x > 0) sprite.move({ -intersection->size.x, 0.f });
            if (velocity.x < 0) sprite.move({ intersection->size.x, 0.f });
        }
    }

    sprite.move({ 0.f, velocity.y * dt });
    isGrounded = false;
    for (auto& p : platforms)
    {
        auto intersection = sprite.getGlobalBounds().findIntersection(p.getBounds());
        if (intersection)
        {
            if (velocity.y > 0)
            {
                sprite.move({ 0.f, -intersection->size.y });
                velocity.y = 0;
                isGrounded = true;
            }
            else if (velocity.y < 0)
            {
                sprite.move({ 0.f, intersection->size.y });
                velocity.y = 0;
            }
        }
    }
}

void Player::render(sf::RenderWindow& window)
{
    window.draw(sprite);
}

void Player::setPositionY(float y)
{
    sprite.setPosition({ sprite.getPosition().x, y });
}

void Player::setGrounded()
{
    velocity.y = 0.f;
    isGrounded = true;
}

void Player::moveX(float dx)
{
    sprite.move({ dx, 0.f });
}