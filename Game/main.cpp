#include <SFML/Graphics.hpp>

#include "Player.hpp"
#include "Game.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 200, 200 }), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        Game game;
        game.run();

        return 0;

        window.clear();
        window.draw(shape);
        window.display();
    }
}