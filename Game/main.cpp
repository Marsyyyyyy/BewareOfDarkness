#include "Player.hpp"
#include <SFML/Graphics.hpp>
#include "Settings.hpp"
#include "Menu.hpp"
#include "Game.hpp"
#include "Options.hpp"
#include "GameOver.hpp"
#include "Victory.hpp"
#include <iostream>
#include <filesystem>
#include <memory>

enum class GameState { MainMenu, Options, Playing, GameOver, Victory };

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({ 1920u, 1080u }),
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
    NeonMenu     mainMenu(window.getSize(), font);

    auto optionsMenu = std::make_unique<OptionsMenu>(window.getSize(), font, settings);
    auto gameOverScr = std::make_unique<GameOverScreen>(window.getSize(), font);
    auto victoryScr = std::make_unique<VictoryScreen>(window.getSize(), font);

    // FIX: Game est cr une seule fois ici, pas  chaque frame
    std::unique_ptr<Game> game;

    GameState  state = GameState::MainMenu;
    sf::Clock  clock;

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        // ---- Events ------------------------------------------------
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* key = event->getIf<sf::Event::KeyPressed>())
            {
                switch (state)
                {
                    // -- Main menu --
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
                            game = std::make_unique<Game>(window, settings);
                            state = GameState::Playing;
                            std::cout << ">>> PLAY <<<\n";
                        }
                        else if (action == NeonMenu::Action::Options)
                        {
                            optionsMenu = std::make_unique<OptionsMenu>(
                                window.getSize(), font, settings);
                            state = GameState::Options;
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

                // -- Options --
                case GameState::Options:
                {
                    if (optionsMenu->handleKey(key->code))
                        state = GameState::MainMenu;
                    break;
                }

                // -- Playing --
                case GameState::Playing:
                {
                    if (key->code == sf::Keyboard::Key::Escape)
                    {
                        gameOverScr = std::make_unique<GameOverScreen>(
                            window.getSize(), font);
                        state = GameState::GameOver;
                    }
                    else if (key->code == sf::Keyboard::Key::V)
                    {
                        victoryScr = std::make_unique<VictoryScreen>(
                            window.getSize(), font);
                        state = GameState::Victory;
                    }
                    break;
                }

                // -- Game Over --
                case GameState::GameOver:
                {
                    switch (key->code)
                    {
                    case sf::Keyboard::Key::Up:
                        gameOverScr->moveUp();
                        break;
                    case sf::Keyboard::Key::Down:
                        gameOverScr->moveDown();
                        break;
                    case sf::Keyboard::Key::Enter:
                    {
                        auto action = gameOverScr->confirm();
                        if (action == GameOverScreen::Action::Retry)
                        {
                            game = std::make_unique<Game>(window, settings);
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

                // -- Victory --
                case GameState::Victory:
                {
                    switch (key->code)
                    {
                    case sf::Keyboard::Key::Up:
                        victoryScr->moveUp();
                        break;
                    case sf::Keyboard::Key::Down:
                        victoryScr->moveDown();
                        break;
                    case sf::Keyboard::Key::Enter:
                    {
                        auto action = victoryScr->confirm();
                        if (action == VictoryScreen::Action::Continue)
                        {
                            game = std::make_unique<Game>(window, settings);
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
                }
            }
        }

        // ---- Update ------------------------------------------------
        switch (state)
        {
        case GameState::MainMenu:  mainMenu.update(dt);        break;
        case GameState::Options:   optionsMenu->update(dt);    break;
        case GameState::Playing:   game->update(dt);           break;
        case GameState::GameOver:  gameOverScr->update(dt);    break;
        case GameState::Victory:   victoryScr->update(dt);     break;
        }

        // ---- Draw --------------------------------------------------
        window.clear();

        if (state != GameState::Playing)
        {
            window.setView(window.getDefaultView());
        }

        switch (state)
        {
        case GameState::MainMenu:  mainMenu.draw(window);      break;
        case GameState::Options:   optionsMenu->draw(window);  break;
        case GameState::Playing:
            if (game) game->render(window);
            break;
        case GameState::GameOver:  gameOverScr->draw(window);  break;
        case GameState::Victory:   victoryScr->draw(window);   break;
        }

        window.display();
    }

    return 0;
}