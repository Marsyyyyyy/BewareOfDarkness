#include "Player.hpp"
#include "Constants.hpp"
#include "ResourceManager.hpp"
#include <SFML/Window/Keyboard.hpp>

Player::Player() : sprite(ResourceManager::getInstance().loadTexture("player", "assets/PlayerUp.png"))
{
    isGrounded = false;
    sprite.setPosition({ 100.f, 100.f });
}

sf::Vector2f Player::getPosition() const
{
    return sprite.getPosition();
}

sf::Vector2f Player::getVelocity() const
{
    return velocity;
}

void Player::update(float dt, std::vector<Platform>& platforms)
{
    velocity.x = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
        velocity.x = -Constants::PLAYER_SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        velocity.x = Constants::PLAYER_SPEED;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && isGrounded)
    {
        velocity.y = Constants::PLAYER_JUMP;
        isGrounded = false;
    }

    velocity.y += Constants::GRAVITY * dt;
    sprite.move(velocity * dt);

    for (auto& p : platforms)
    {
        if (sprite.getGlobalBounds().findIntersection(p.getBounds()))
        {
            velocity.y = 0;
            isGrounded = true;
        }
    }
}

void Player::render(sf::RenderWindow& window)
{
    window.draw(sprite);
}