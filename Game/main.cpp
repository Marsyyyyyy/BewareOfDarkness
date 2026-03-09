#include "Player.hpp"
#include <SFML/Graphics.hpp>
#include "Settings.hpp"
#include "Menu.hpp"
#include "Game.hpp"
#include "Options.hpp"
#include "Sound_Indicator.hpp"
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

    // -- Sound Indicator (mode malentendant) --
    SoundIndicator soundIndicator(window.getSize(), font);

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

                // =============================================
                //  PLAYING (demo pour tester le SoundIndicator)
                // =============================================
                case GameState::Playing:
                {
                    if (key->code == sf::Keyboard::Key::Escape)
                        state = GameState::MainMenu;

                    // Touches de test pour le mode malentendant
                    // (a remplacer par ta vraie logique de jeu)
                    if (key->code == sf::Keyboard::Key::Num1)
                        soundIndicator.trigger(SoundDirection::Above,
                            SoundType::MonsterReal, 1.2f);
                    if (key->code == sf::Keyboard::Key::Num2)
                        soundIndicator.trigger(SoundDirection::Behind,
                            SoundType::MonsterBait, 0.8f);
                    if (key->code == sf::Keyboard::Key::Num3)
                        soundIndicator.trigger(SoundDirection::Left,
                            SoundType::Danger, 1.5f);
                    if (key->code == sf::Keyboard::Key::Num4)
                        soundIndicator.trigger(SoundDirection::Right,
                            SoundType::Ambient, 0.6f);
                    break;
                }
                }
            }
        }

        // -- Update --
        switch (state)
        {
        case GameState::MainMenu:
            mainMenu.update(dt);
            break;
        case GameState::Options:
            optionsMenu->update(dt);
            break;
        case GameState::Playing:
            // Update le sound indicator seulement si mode active
            if (settings.hearingMode)
                soundIndicator.update(dt);
            break;
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
            // Fond temporaire (a remplacer par ton jeu)
            sf::RectangleShape bg{ sf::Vector2f{
                static_cast<float>(window.getSize().x),
                static_cast<float>(window.getSize().y) } };
            bg.setFillColor(sf::Color(10, 10, 20));
            window.draw(bg);

            // Texte d'info
            sf::Text info(font, "PLAYING  -  ESC to menu", 20u);
            info.setFillColor(sf::Color(100, 100, 120));
            info.setPosition({ 20.f, 20.f });
            window.draw(info);

            // Mode malentendant : texte + indicateurs
            if (settings.hearingMode)
            {
                sf::Text hmode(font, "HEARING MODE ON  -  1/2/3/4 to test sounds", 16u);
                hmode.setFillColor(sf::Color(0, 255, 128, 180));
                hmode.setPosition({ 20.f, 50.f });
                window.draw(hmode);

                soundIndicator.draw(window);
            }
            break;
        }
        }

        window.display();
    }

    return 0;
}