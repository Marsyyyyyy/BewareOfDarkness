#include "Player.hpp"
#include "Constants.hpp"
#include <SFML/Window/Keyboard.hpp>

Player::Player()
{
    body.setSize({ 40,60 });
    body.setFillColor(sf::Color::White);
    body.setPosition({ 200,200 });

    isGrounded = false;
}

void Player::update(float dt, std::vector<Platform>& platforms)
{
    velocity.x = 0;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        velocity.x = -Constants::PLAYER_SPEED;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        velocity.x = Constants::PLAYER_SPEED;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && isGrounded)
    {
        velocity.y = Constants::PLAYER_JUMP;
        isGrounded = false;
    }

    velocity.y += Constants::GRAVITY * dt;

    body.move(velocity * dt);

    for (auto& p : platforms)
    {
        if (body.getGlobalBounds().findIntersection(p.getBounds()))
        {
            velocity.y = 0;
            isGrounded = true;
        }
    }
}

void Player::render(sf::RenderWindow& window)
{
    window.draw(body);
}