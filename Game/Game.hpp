#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.hpp"

class Game
{
private:

    sf::RenderWindow window;
    Scene scene;

    void processEvents();
    void update(float dt);
    void render();

public:

    Game();
    void run();
};