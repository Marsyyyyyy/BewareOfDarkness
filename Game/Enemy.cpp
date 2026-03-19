#include "Enemy.hpp"
#include "ResourceManager.hpp"
#include "Constants.hpp"
#include <cmath>
#include <iostream>

Enemy::Enemy(std::function<void()> killCallback)
    : sprite(ResourceManager::getInstance().loadTexture("walk_1", "assets/Walk_1.png"))
    , spriteUp(ResourceManager::getInstance().loadTexture("enemyUp", "assets/Up.png"))
    , onKillPlayer(killCallback)
{
    state = EnemyState::Stalking;
    sprite.setOrigin({ 0.f, 0.f });
    sprite.setPosition({ 100.f -800.f, 200.f });

    // Charger les 10 frames
    auto& rm = ResourceManager::getInstance();
    walkFrames[0] = &rm.loadTexture("walk_1", "assets/Walk_1.png");
    walkFrames[1] = &rm.loadTexture("walk_2", "assets/Walk_2.png");
    walkFrames[2] = &rm.loadTexture("walk_3", "assets/Walk_3.png");
    walkFrames[3] = &rm.loadTexture("walk_4", "assets/Walk_4.png");
    walkFrames[4] = &rm.loadTexture("walk_5", "assets/Walk_5.png");
    walkFrames[5] = &rm.loadTexture("walk_6", "assets/Walk_6.png");
    walkFrames[6] = &rm.loadTexture("walk_7", "assets/Walk_7.png");
    walkFrames[7] = &rm.loadTexture("walk_8", "assets/Walk_8.png");
    walkFrames[8] = &rm.loadTexture("walk_9", "assets/Walk_9.png");
    walkFrames[9] = &rm.loadTexture("walk_10", "assets/Walk_10.png");

    float scaleUp = 400.f / 809.f;
    spriteUp.setScale({ scaleUp, scaleUp });
    spriteUp.setOrigin({ 809.f / 2.f, 0.f });
    spriteUp.setPosition({ 0.f, -400.f });
}

void Enemy::onDoorOpen()
{
    if (state == EnemyState::Stalking || state == EnemyState::Attacking)
    {
        state = EnemyState::Descending;
        velocity = { 0.f, 0.f };
        std::cout << ">>> DESCENTE <<<\n";
    }
}

bool Enemy::isInFlashlight(sf::Vector2f playerPos, float angleDeg) const
{
    sf::Vector2f flashlightPos = playerPos + sf::Vector2f(0.f, -40.f);
    sf::Vector2f enemyCenter = sprite.getPosition() + sf::Vector2f(254.f, 106.f);
    sf::Vector2f toEnemy = enemyCenter - playerPos;
    float dist = std::sqrt(toEnemy.x * toEnemy.x + toEnemy.y * toEnemy.y);

    if (dist > 1500.f) return false;

    float angleRad = angleDeg * 3.14159265f / 180.f;
    float enemyAngle = std::atan2(toEnemy.y, toEnemy.x);
    float angleSweep = std::atan2(120.f, 500.f) * 3.f;

    float diff = std::abs(enemyAngle - angleRad);
    if (diff > 3.14159265f) diff = 2.f * 3.14159265f - diff;

    return diff < angleSweep;
}

bool Enemy::isInFlashlightUp(sf::Vector2f playerPos, float angleDeg) const
{
    sf::Vector2f flashlightPos = playerPos + sf::Vector2f(0.f, -40.f);
    sf::Vector2f enemyCenter = spriteUp.getPosition() + sf::Vector2f(150.f, 150.f);
    sf::Vector2f toEnemy = enemyCenter - playerPos;
    float dist = std::sqrt(toEnemy.x * toEnemy.x + toEnemy.y * toEnemy.y);

    if (dist > 1500.f) return false; // hors portée

    // Calculer l'angle réel vers l'ennemi
    float enemyAngleDeg = std::atan2(toEnemy.y, toEnemy.x) * 180.f / 3.14159265f;

    // Différence angulaire
    float diff = enemyAngleDeg - angleDeg;
    // Normaliser entre -180 et 180
    while (diff > 180.f)  diff -= 360.f;
    while (diff < -180.f) diff += 360.f;

    float coneHalfAngle = 40.f; // demi-angle du cône en degrés
    return std::abs(diff) < 60.f;
}

void Enemy::update(float dt, sf::Vector2f playerPos, std::vector<Platform>& platforms,
    bool lightsEnabled, float flashlightAngle)
{
    switch (state)
    {
    case EnemyState::Stalking:
    {
        // Sleep au début
        if (sleeping)
        {
            sleepTimer -= dt;
            if (sleepTimer <= 0.f)
                sleeping = false;
            break;
        }

        sf::Vector2f pos = sprite.getPosition();

        velocity.y += Constants::GRAVITY * dt;
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

        // Recul si éclairé
        if (lightsEnabled && isInFlashlight(playerPos, flashlightAngle))
            velocity.x = -1200.f; // recul rapide
        else
            velocity.x = stalkSpeed;

        if (pos.x + 508.f > playerPos.x - 70.f && !isInFlashlight(playerPos, flashlightAngle))
        {
            state = EnemyState::Attacking;
            std::cout << ">>> ATTAQUE <<<\n";
        }

        // Animation
        animTimer += dt;
        if (animTimer >= animFrameTime)
        {
            animTimer = 0.f;
            currentFrame = (currentFrame + 1) % 10;
            sprite.setTexture(*walkFrames[currentFrame]);
        }

        sprite.move({ velocity.x * dt, 0.f });
        break;
    }

    case EnemyState::Attacking:
    {
        sf::Vector2f pos = sprite.getPosition();

        velocity.y += Constants::GRAVITY * dt;
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

        velocity.x = attackSpeed;
        sprite.move({ velocity.x * dt, 0.f });

        pos = sprite.getPosition();
        if (pos.x + 508.f >= playerPos.x)
        {
            onKillPlayer();
            state = EnemyState::Stalking;
            sprite.setPosition({ playerPos.x - 508.f - 300.f, sprite.getPosition().y });
            velocity = { 0.f, 0.f };

        }

    // Animation
        animTimer += dt;
        if (animTimer >= animFrameTime)
        {
            animTimer = 0.f;
            currentFrame = (currentFrame + 1) % 10;
            sprite.setTexture(*walkFrames[currentFrame]);
        }
        break;
    }

    case EnemyState::Descending:
    {
        // Remonter si éclairé
        if (lightsEnabled && isInFlashlightUp(playerPos, flashlightAngle))
        {
            float newY = spriteUp.getPosition().y - 300.f * dt; // remonte vite
            spriteUp.setPosition({ spriteUp.getPosition().x, newY });
            break;
        }

        if (spriteUp.getPosition().y < -300.f || spriteUp.getPosition().y == -400.f)
            spriteUp.setPosition({ playerPos.x, playerPos.y - 850.f });

        float targetX = playerPos.x + 70.f;
        float newY = spriteUp.getPosition().y + descendSpeed * dt;
        spriteUp.setPosition({ targetX, newY });

        sf::FloatRect upBounds = spriteUp.getGlobalBounds();
        sf::FloatRect playerBounds = { playerPos - sf::Vector2f(30.f, 60.f), { 60.f, 120.f } };
        if (upBounds.findIntersection(playerBounds).has_value())
        {
            onKillPlayer();
            spriteUp.setPosition({ targetX, playerPos.y - 850.f });
        }
        break;
    }

    case EnemyState::DeadState:
        break;
    }
}

void Enemy::render(sf::RenderWindow& window)
{
    if (state == EnemyState::Descending)
        window.draw(spriteUp);
    else
        window.draw(sprite);
}