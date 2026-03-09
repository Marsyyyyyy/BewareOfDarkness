#include "Enemy.hpp"
#include <cmath>

Enemy::Enemy()
{
    body.setSize({ 50,50 });
    body.setFillColor(sf::Color::Red);
    body.setPosition({ 800,600 });

    state = EnemyState::Idle;
}

void Enemy::update(float dt, sf::Vector2f playerPos)
{
    float dist = std::abs(playerPos.x - body.getPosition().x);

    if (dist < 300)
        state = EnemyState::Chase;
    else
        state = EnemyState::Idle;

    if (state == EnemyState::Chase)
    {
        if (playerPos.x < body.getPosition().x)
            velocity.x = -100;
        else
            velocity.x = 100;
    }
    else
    {
        velocity.x = 0;
    }

    body.move(velocity * dt);
}

void Enemy::render(sf::RenderWindow& window)
{
    window.draw(body);
}