#pragma once
#include "Entity.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include "Platform.hpp"
#include <vector>

enum class EnemyState { Stalking, Attacking, Descending, DeadState };

class Enemy
{
private:
    EnemyState state;
    sf::Sprite sprite;
    sf::Sprite spriteUp;
    sf::Vector2f velocity = { 0.f, 0.f };

    float stalkSpeed = 180.f;  // vitesse de rapprochement
    float attackSpeed = 400.f; // vitesse quand il attaque
    float attackRange = 80.f;  // distance pour toucher le joueur
    float descendSpeed = 40.f; // vitesse de descente lente
    float descendY = -300.f; // position Y de départ (au dessus de l'écran)
    float sleepTimer = 5.f; // secondes d'attente au début
    bool sleeping = true;   // en train de dormir

    std::function<void()> onKillPlayer;

    bool doorOpened = false;
    bool isInFlashlight(sf::Vector2f playerPos, float angle) const;
    bool isInFlashlightUp(sf::Vector2f playerPos, float angle) const;

    // Animation marche
    std::array<sf::Texture*, 10> walkFrames;
    int currentFrame = 0;
    float animTimer = 0.f;
    float animFrameTime = 0.05f;

public:
    Enemy(std::function<void()> killCallback);
    void onDoorOpen();
    void update(float dt, sf::Vector2f playerPos, std::vector<Platform>& platforms,
        bool lightsEnabled, float flashlightAngle);
    void render(sf::RenderWindow& window);
    sf::Vector2f getPosition() const { return sprite.getPosition(); }
};