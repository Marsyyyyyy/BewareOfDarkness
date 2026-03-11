#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.hpp"
#include "Camera2D.hpp"
#include "Player.hpp"

class Game
{
private:

    sf::RenderWindow window;
    Scene scene;
    Camera2D camera;
    Player player;

    void processEvents();

public:

    Game();

    void update(float dt);
    void render();
    void run();
};