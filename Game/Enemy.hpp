#pragma once
#include "Entity.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include "Platform.hpp"
#include <vector>

enum class EnemyState { Stalking, Attacking, Dead };

class Enemy
{
private:
    EnemyState state;
    sf::Sprite sprite;
    sf::Vector2f velocity = { 0.f, 0.f };

    float stalkSpeed = 60.f;  // vitesse de rapprochement lente
    float attackSpeed = 400.f; // vitesse quand il attaque
    float attackRange = 80.f;  // distance pour toucher le joueur

    std::function<void()> onKillPlayer;

public:
    Enemy(std::function<void()> killCallback);
    void update(float dt, sf::Vector2f playerPos, std::vector<Platform>& platforms);
    void render(sf::RenderWindow& window);
    sf::Vector2f getPosition() const { return sprite.getPosition(); }
};