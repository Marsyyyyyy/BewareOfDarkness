#include "Game.hpp"
#include "Constants.hpp"

Game::Game(sf::RenderWindow& window, const GameSettings& settings) :
    window(window),
    settings(settings),
    scene(window.getSize(), settings),
    camera(window.getSize())
{
    camera.setLevelBounds(sf::FloatRect({ 0.f, 0.f }, { 4000.f, 2000.f }));
}

void Game::run()
{
    sf::Clock clock;

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        processEvents();
        update(dt);
        render(window);
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

    // Update camera to follow the scene's player
    camera.update(
        dt,
        scene.getPlayerPosition(),
        scene.getPlayerVelocity().x
    );
}

void Game::render(sf::RenderWindow& window)
{
    camera.apply(window);
    scene.render(window);
}
