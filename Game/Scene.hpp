#pragma once
#include "Player.hpp"
#include "Platform.hpp"
#include "Enemy.hpp"
#include "ParallaxBackground.hpp"
#include "LightSystem.hpp"
#include "Box.hpp"
#include "Door.hpp"
#include "PressurePlate.hpp"
#include "VictoryZone.hpp"
#include <vector>
#include <functional>
#include <memory>

class Scene
{
private:
    Player player;
    std::vector<Platform> platforms;
    Enemy enemy;
    LightSystem lights;

    Box box;
    Door door;
    PressurePlate plate;
    VictoryZone victoryZone;

    bool victoryTriggered = false;
    std::function<void()> onVictory;
    std::function<void()> onGameOver;

    GameSettings settings;
    ParallaxBackground background;

public:
    Scene(sf::Vector2u windowSize, const GameSettings& settings,
        std::function<void()> victoryCallback,
        std::function<void()> gameOverCallback);
    void update(float dt);
    void render(sf::RenderWindow& window);
    void renderWorld(sf::RenderWindow& window);
    sf::Vector2f getPlayerPosition() const;
    sf::Vector2f getPlayerVelocity() const;
};