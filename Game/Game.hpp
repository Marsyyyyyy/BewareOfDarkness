#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.hpp"
#include "Camera2D.hpp"
#include "Settings.hpp"
#include <functional>

class Game
{
private:
    sf::RenderWindow& window;
    GameSettings settings;
    Scene scene;
    Camera2D camera;
    std::function<void()> onVictory;

    void processEvents();

public:
    Game(sf::RenderWindow& window, const GameSettings& settings,
        std::function<void()> victoryCallback,
        std::function<void()> gameOverCallback);
    void update(float dt);
    void render(sf::RenderWindow& window);
};