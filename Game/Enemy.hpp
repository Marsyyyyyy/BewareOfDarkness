#pragma once
#include "Entity.hpp"
#include <SFML/Graphics.hpp>

enum class EnemyState { Idle, Patrol, Chase, Attack };

class Enemy : public Entity
{
private:
    EnemyState state;
    sf::Sprite sprite;

public:
    Enemy();
    void update(float dt, sf::Vector2f playerPos);
    void render(sf::RenderWindow& window);
};