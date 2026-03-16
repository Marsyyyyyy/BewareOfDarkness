#include "Enemy.hpp"
#include "ResourceManager.hpp"
#include "Constants.hpp"
#include <cmath>
#include <iostream>

Enemy::Enemy(std::function<void()> killCallback)
    : sprite(ResourceManager::getInstance().loadTexture("enemy", "assets/Enemy.png"))
    , onKillPlayer(killCallback)
{
    state = EnemyState::Stalking;
    sprite.setOrigin({ 0.f, 0.f });

    // Démarre loin derrière le joueur (position initiale du joueur ~100)
    sprite.setPosition({ 100.f -800.f, 200.f });
}

void Enemy::update(float dt, sf::Vector2f playerPos, std::vector<Platform>& platforms)
{
    sf::Vector2f pos = sprite.getPosition();
    float distX = playerPos.x - pos.x;

    // Gravité
    velocity.y += Constants::GRAVITY * dt;

    // Déplacement Y + collisions
    sprite.move({ 0.f, velocity.y * dt });
    for (auto& p : platforms)
    {
        auto inter = sprite.getGlobalBounds().findIntersection(p.getBounds());
        if (inter)
        {
            if (velocity.y > 0) { sprite.move({ 0.f, -inter->size.y }); velocity.y = 0.f; }
            if (velocity.y < 0) { sprite.move({ 0.f,  inter->size.y }); velocity.y = 0.f; }
        }
    }

    switch (state)
    {
    case EnemyState::Stalking:
    {
        // Avance toujours vers le joueur, ne recule jamais
        velocity.x = stalkSpeed;

        // Le bord droit (pos.x + 508) est proche du joueur
        if (pos.x + 508.f > playerPos.x - 20.f)
        {
            state = EnemyState::Attacking;
            std::cout << ">>> ATTAQUE <<<\n";
        }
        break;
    }

    case EnemyState::Attacking:
    {
        velocity.x = attackSpeed;
        if (pos.x + 508.f >= playerPos.x)
        {
            onKillPlayer();
            state = EnemyState::Stalking;
            sprite.setPosition({ playerPos.x - 508.f - 300.f, sprite.getPosition().y });
        }
        break;
    }

    case EnemyState::Dead:
        velocity.x = 0.f;
        break;
    }

    sprite.move({ velocity.x * dt, 0.f });
}

void Enemy::render(sf::RenderWindow& window)
{
    window.draw(sprite);
}