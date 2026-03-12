#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.hpp"
#include "Camera2D.hpp"
#include "Player.hpp"
#include "Settings.hpp"

class Game
{
private:
    sf::RenderWindow& window;
    GameSettings settings;
    Scene scene;
    Camera2D camera;

    void processEvents();

public:
    Game(sf::RenderWindow& window, const GameSettings& settings);
    void update(float dt);
    void render(sf::RenderWindow& window);
    void run();
};