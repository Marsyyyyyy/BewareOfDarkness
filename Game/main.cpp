#include "Player.hpp"
#include <SFML/Graphics.hpp>
#include "Settings.hpp"
#include "Menu.hpp"
#include "Game.hpp"
#include "Options.hpp"
#include <iostream>
#include <filesystem>
#include <memory>

enum class GameState { MainMenu, Options, Playing };


int main()
{
    sf::RenderWindow window(
        sf::VideoMode({ 1280u, 720u }),
        "Beware Of Darkness",
        sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);

    // -- Font --
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

    // -- Settings globaux --
    GameSettings settings;

    // -- Menus --
    NeonMenu     mainMenu(window.getSize(), font);
    auto         optionsMenu = std::make_unique<OptionsMenu>(
        window.getSize(), font, settings);

    GameState state = GameState::MainMenu;
    sf::Clock clock;

    // Timer demo pour tester le SoundIndicator en mode Playing
    float demoTimer = 0.f;

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
                    // =============================================
                    //  MAIN MENU
                    // =============================================
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
                            // Recreer le menu options (refresh des valeurs)
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

                // =============================================
                //  OPTIONS
                // =============================================
                case GameState::Options:
                {
                    bool back = optionsMenu->handleKey(key->code);
                    if (back)
                        state = GameState::MainMenu;
                    break;
                }

                }
            }
        }

        // -- Update --
        switch (state)
        {
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
            Game game;
            game.run();
        }
        }

        window.display();
    }
    return 0;
}