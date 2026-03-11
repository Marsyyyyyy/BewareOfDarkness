#include "Player.hpp"
#include <SFML/Graphics.hpp>
#include "Settings.hpp"
#include "Menu.hpp"
#include "Game.hpp"
#include "Options.hpp"
<<<<<<< HEAD
=======
#include "GameOver.hpp"
#include "Victory.hpp"
>>>>>>> 92483e7effe16c599d3dcbdc828916494b02ad60
#include <iostream>
#include <filesystem>
#include <memory>

enum class GameState { MainMenu, Options, Playing, GameOver, Victory };

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({ 1280u, 720u }),
        "Beware Of Darkness",
        sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);

    sf::Font font;
    try
    {
        font = sf::Font("assets/Neon Goodshine.ttf");
    }
    catch (const sf::Exception& e)
    {
        std::cerr << "ERREUR FONT: " << e.what() << std::endl;
        std::cerr << "Repertoire: "
            << std::filesystem::current_path() << std::endl;
        return -1;
    }

    GameSettings settings;
    NeonMenu mainMenu(window.getSize(), font);
    auto optionsMenu = std::make_unique<OptionsMenu>(
        window.getSize(), font, settings);
<<<<<<< HEAD
=======
    auto gameOverScreen = std::make_unique<GameOverScreen>(
        window.getSize(), font);
    auto victoryScreen = std::make_unique<VictoryScreen>(
        window.getSize(), font);
>>>>>>> 92483e7effe16c599d3dcbdc828916494b02ad60

    GameState state = GameState::MainMenu;
    sf::Clock clock;

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* key = event->getIf<sf::Event::KeyPressed>())
            {
                switch (state)
                {
                case GameState::MainMenu:
                {
                    switch (key->code)
                    {
                    case sf::Keyboard::Key::Up:
                        mainMenu.moveUp();
                        break;
                    case sf::Keyboard::Key::Down:
                        mainMenu.moveDown();
                        break;
                    case sf::Keyboard::Key::Enter:
                    {
                        auto action = mainMenu.confirm();
                        if (action == NeonMenu::Action::Play)
                        {
                            state = GameState::Playing;
                            std::cout << ">>> PLAY <<<\n";
                        }
                        else if (action == NeonMenu::Action::Options)
                        {
                            state = GameState::Options;
                            optionsMenu = std::make_unique<OptionsMenu>(
                                window.getSize(), font, settings);
                        }
                        else if (action == NeonMenu::Action::Exit)
                        {
                            window.close();
                        }
                        break;
                    }
                    case sf::Keyboard::Key::Escape:
                        window.close();
                        break;
                    default:
                        break;
                    }
                    break;
                }

                case GameState::Options:
                {
                    bool back = optionsMenu->handleKey(key->code);
                    if (back)
                        state = GameState::MainMenu;
                    break;
                }

<<<<<<< HEAD
=======
                case GameState::Playing:
                {
                    // ESC = game over (test), V = victory (test)
                    if (key->code == sf::Keyboard::Key::Escape)
                    {
                        gameOverScreen = std::make_unique<GameOverScreen>(
                            window.getSize(), font);
                        state = GameState::GameOver;
                    }
                    if (key->code == sf::Keyboard::Key::V)
                    {
                        victoryScreen = std::make_unique<VictoryScreen>(
                            window.getSize(), font);
                        state = GameState::Victory;
                    }
                    break;
                }

                case GameState::GameOver:
                {
                    switch (key->code)
                    {
                    case sf::Keyboard::Key::Up:
                        gameOverScreen->moveUp();
                        break;
                    case sf::Keyboard::Key::Down:
                        gameOverScreen->moveDown();
                        break;
                    case sf::Keyboard::Key::Enter:
                    {
                        auto action = gameOverScreen->confirm();
                        if (action == GameOverScreen::Action::Retry)
                        {
                            state = GameState::Playing;
                            std::cout << ">>> RETRY <<<\n";
                        }
                        else if (action == GameOverScreen::Action::Menu)
                        {
                            state = GameState::MainMenu;
                        }
                        break;
                    }
                    default:
                        break;
                    }
                    break;
                }

                case GameState::Victory:
                {
                    switch (key->code)
                    {
                    case sf::Keyboard::Key::Up:
                        victoryScreen->moveUp();
                        break;
                    case sf::Keyboard::Key::Down:
                        victoryScreen->moveDown();
                        break;
                    case sf::Keyboard::Key::Enter:
                    {
                        auto action = victoryScreen->confirm();
                        if (action == VictoryScreen::Action::Continue)
                        {
                            state = GameState::Playing;
                            std::cout << ">>> CONTINUE <<<\n";
                        }
                        else if (action == VictoryScreen::Action::Menu)
                        {
                            state = GameState::MainMenu;
                        }
                        break;
                    }
                    default:
                        break;
                    }
                    break;
                }
>>>>>>> 92483e7effe16c599d3dcbdc828916494b02ad60
                }
            }
        }

        switch (state)
        {
<<<<<<< HEAD
            case GameState::MainMenu:
            {
                mainMenu.update(dt);
                break;
            }

            case GameState::Options:
            {
                optionsMenu->update(dt);
                break;
            }

        }
    
        // -- Draw --
=======
        case GameState::MainMenu:
            mainMenu.update(dt);
            break;
        case GameState::Options:
            optionsMenu->update(dt);
            break;
        case GameState::Playing:
            break;
        case GameState::GameOver:
            gameOverScreen->update(dt);
            break;
        case GameState::Victory:
            victoryScreen->update(dt);
            break;
        }

>>>>>>> 92483e7effe16c599d3dcbdc828916494b02ad60
        window.clear();

        switch (state)
        {
        case GameState::MainMenu:
            mainMenu.draw(window);
            break;
        case GameState::Options:
            optionsMenu->draw(window);
            break;
        case GameState::Playing:
        {
<<<<<<< HEAD
            Game game;
            game.run();
=======
            sf::RectangleShape bg{ sf::Vector2f{
                static_cast<float>(window.getSize().x),
                static_cast<float>(window.getSize().y) } };
            bg.setFillColor(sf::Color(10, 10, 20));
            window.draw(bg);

            sf::Text info(font, "PLAYING  -  ESC:gameover  V:victory", 20u);
            info.setFillColor(sf::Color(100, 100, 120));
            info.setPosition({ 20.f, 20.f });
            window.draw(info);
            break;
>>>>>>> 92483e7effe16c599d3dcbdc828916494b02ad60
        }
        case GameState::GameOver:
            gameOverScreen->draw(window);
            break;
        case GameState::Victory:
            victoryScreen->draw(window);
            break;
        }

        window.display();
    }
    return 0;
}