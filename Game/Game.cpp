#include "Game.hpp"
#include "Constants.hpp"

Game::Game() :
    window(sf::VideoMode({ Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT }), "Limbo Clone")
{
}

void Game::run()
{
    sf::Clock clock;

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        processEvents();
        update(dt);
        render();
    }
}

void Game::processEvents()
{
    while (auto event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();
    }
}

void Game::update(float dt)
{
    scene.update(dt);
}

void Game::render()
{
    window.clear(sf::Color::Black);
    scene.render(window);
    window.display();
}