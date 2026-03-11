#include "Enemy.hpp"
#include "ResourceManager.hpp"
#include <cmath>

Enemy::Enemy() : sprite(ResourceManager::getInstance().loadTexture("enemy", "assets/Enemy.png"))
{
    state = EnemyState::Idle;
    sprite.setPosition({ 800.f, 600.f });
}

void Enemy::update(float dt, sf::Vector2f playerPos)
{
    float dist = std::abs(playerPos.x - sprite.getPosition().x);

    if (dist < 300)
        state = EnemyState::Chase;
    else
        state = EnemyState::Idle;

    if (state == EnemyState::Chase)
    {
        if (playerPos.x < sprite.getPosition().x)
            velocity.x = -100;
        else
            velocity.x = 100;
    }
    else
    {
        velocity.x = 0;
    }

    sprite.move(velocity * dt);
}

void Enemy::render(sf::RenderWindow& window)
{
    window.draw(sprite);
}