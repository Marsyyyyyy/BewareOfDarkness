#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <functional>
#include "Settings.hpp"
#include "Colors.hpp"
#include "LightSystem.hpp"

// Type d'option
enum class OptionType { KeyBind, Slider, Toggle, Label, Back };

// Un item dans le menu options
struct OptionItem
{
    std::string       name;
    OptionType        type;
    // Pour KeyBind : pointe vers la touche dans GameSettings
    sf::Keyboard::Key* keyPtr = nullptr;
    // Pour Slider : pointe vers la valeur int (0-100)
    int* sliderPtr = nullptr;
    // Pour Toggle : pointe vers le bool
    bool* togglePtr = nullptr;
    // Couleur neon de cet item
    sf::Color         neonColor = NeonColors::Cyan;
};

class OptionsMenu
{
public:
    OptionsMenu(sf::Vector2u windowSize, const sf::Font& font, GameSettings& settings)
        : m_font(font)
        , m_windowSize(windowSize)
        , m_settings(settings)
        , m_titleText(font, "OPTIONS", 48u)
        , m_hintText(font, "UP/DOWN : Navigate   LEFT/RIGHT : Adjust   ENTER : Rebind   ESC : Back", 12u)
        , m_previewLights(windowSize)
    {
        float cx = windowSize.x / 2.f;

        // -- Titre --
        m_titleText.setLetterSpacing(6.f);
        m_titleText.setFillColor(NeonColors::Pink);
        auto tb = m_titleText.getLocalBounds();
        m_titleText.setOrigin({ tb.position.x + tb.size.x / 2.f,
                                tb.position.y + tb.size.y / 2.f });
        m_titleText.setPosition({ cx, windowSize.y * 0.08f });

        // -- Hint en bas --
        m_hintText.setLetterSpacing(2.f);
        m_hintText.setFillColor(sf::Color(80, 80, 100, 120));
        auto hb = m_hintText.getLocalBounds();
        m_hintText.setOrigin({ hb.position.x + hb.size.x / 2.f,
                               hb.position.y + hb.size.y / 2.f });
        m_hintText.setPosition({ cx, windowSize.y * 0.95f });

        // -- Construction des items --
        // Labels = titres de section (non selectionnables)
        m_items.push_back({ "--- CONTROLS ---",  OptionType::Label,   nullptr, nullptr, nullptr, NeonColors::Cyan });
        m_items.push_back({ "Move Up",           OptionType::KeyBind, &settings.moveUp,    nullptr, nullptr, NeonColors::Cyan });
        m_items.push_back({ "Move Left",         OptionType::KeyBind, &settings.moveLeft,  nullptr, nullptr, NeonColors::Cyan });
        m_items.push_back({ "Move Right",        OptionType::KeyBind, &settings.moveRight, nullptr, nullptr, NeonColors::Cyan });
        m_items.push_back({ "Jump",        OptionType::KeyBind, &settings.flashlight, nullptr, nullptr, NeonColors::Cyan });
        m_items.push_back({ "UV Flashlight", OptionType::KeyBind, &settings.flashlightMode, nullptr, nullptr, NeonColors::Cyan });

        m_items.push_back({ "--- AUDIO ---",     OptionType::Label,   nullptr, nullptr, nullptr, NeonColors::Pink });
        m_items.push_back({ "Volume",            OptionType::Slider,  nullptr, &settings.volume,     nullptr, NeonColors::Pink });

        m_items.push_back({ "--- DISPLAY ---",   OptionType::Label,   nullptr, nullptr, nullptr, NeonColors::Purple });
        m_items.push_back({ "Brightness",        OptionType::Slider,  nullptr, &settings.brightness, nullptr, NeonColors::Purple });
        m_items.push_back({ "Night Mode",        OptionType::Toggle,  nullptr, nullptr, &settings.lightsEnabled, NeonColors::Purple });

        m_items.push_back({ "BACK",              OptionType::Back,    nullptr, nullptr, nullptr, NeonColors::Orange });

        // Selectionner le premier item navigable
        m_selected = 0;
        skipToNextSelectable(1);
    }

    // Retourne true si on veut quitter les options (Back)
    bool handleKey(sf::Keyboard::Key key)
    {
        // -- Mode rebind : on attend une touche --
        if (m_rebinding)
        {
            if (key == sf::Keyboard::Key::Escape)
            {
                m_rebinding = false;  // annuler
                return false;
            }
            auto& item = m_items[m_selected];
            if (item.keyPtr)
                *item.keyPtr = key;
            m_rebinding = false;
            return false;
        }

        switch (key)
        {
        case sf::Keyboard::Key::Up:
            moveSelection(-1);
            break;

        case sf::Keyboard::Key::Down:
            moveSelection(1);
            break;

        case sf::Keyboard::Key::Left:
            adjustValue(-5);
            break;

        case sf::Keyboard::Key::Right:
            adjustValue(5);
            break;

        case sf::Keyboard::Key::Enter:
        {
            auto& item = m_items[m_selected];
            if (item.type == OptionType::Back)
                return true;  // quitter options
            if (item.type == OptionType::KeyBind)
                m_rebinding = true;
            if (item.type == OptionType::Toggle && item.togglePtr)
                *item.togglePtr = !(*item.togglePtr);
            break;
        }

        case sf::Keyboard::Key::Escape:
            return true;  // quitter options

        default:
            break;
        }
        return false;
    }

    void update(float dt)
    {
        m_time += dt;
        m_previewLights.clearLights();
        // Add a central glow
        m_previewLights.addLight({ m_windowSize.x / 2.f, m_windowSize.y / 2.f }, 250.f);
        // Add a slowly sweeping flashlight
        m_previewLights.addFlashlight({ m_windowSize.x / 2.f, m_windowSize.y / 2.f }, m_time * 30.f, 150.f, 600.f);
    }

    void draw(sf::RenderWindow& window)
    {
        // Fond
        sf::RectangleShape bg{ sf::Vector2f{
            static_cast<float>(m_windowSize.x),
            static_cast<float>(m_windowSize.y) } };
        bg.setFillColor(NeonColors::DarkBg);
        window.draw(bg);

        // Draw the live preview lights (passing the live settings reference)
        m_previewLights.render(window, m_settings);

        // Titre
        window.draw(m_titleText);

        // Items
        float startY = m_windowSize.y * 0.16f;
        float lineH = 38.f;
        float cx = m_windowSize.x / 2.f;

        for (int i = 0; i < static_cast<int>(m_items.size()); ++i)
        {
            const auto& item = m_items[i];
            float y = startY + i * lineH;
            bool isSel = (i == m_selected);

            if (item.type == OptionType::Label)
            {
                // Section header
                sf::Text label(m_font, item.name, 20u);
                label.setLetterSpacing(4.f);
                float pulse = std::sin(m_time * 2.f) * 0.2f + 0.8f;
                uint8_t a = static_cast<uint8_t>(200 + 55 * pulse);
                label.setFillColor({ item.neonColor.r, item.neonColor.g,
                                     item.neonColor.b, a });
                auto lb = label.getLocalBounds();
                label.setOrigin({ lb.position.x + lb.size.x / 2.f,
                                  lb.position.y + lb.size.y / 2.f });
                label.setPosition({ cx, y });
                window.draw(label);
                continue;
            }

            // Nom de l'option (a gauche)
            sf::Text nameText(m_font, item.name, 22u);
            nameText.setLetterSpacing(2.f);

            sf::Color col = isSel
                ? sf::Color{ item.neonColor.r, item.neonColor.g, item.neonColor.b, 255 }
            : sf::Color{ 140, 140, 160, 200 };
            nameText.setFillColor(col);

            float leftX = cx - 200.f;
            auto nb = nameText.getLocalBounds();
            nameText.setOrigin({ 0.f, nb.position.y + nb.size.y / 2.f });
            nameText.setPosition({ leftX, y });

            if (isSel)
            {
                float s = 1.f + std::sin(m_time * 4.f) * 0.02f;
                nameText.setScale({ s, s });
            }

            if (item.type != OptionType::Back)
                window.draw(nameText);

            float rightX = cx + 120.f;

            if (item.type == OptionType::KeyBind && item.keyPtr)
            {
                std::string valStr;
                if (m_rebinding && isSel)
                    valStr = "[ ... ]";
                else
                    valStr = "[ " + keyToString(*item.keyPtr) + " ]";

                sf::Text valText(m_font, valStr, 22u);
                valText.setLetterSpacing(2.f);

                if (m_rebinding && isSel)
                {
                    // Clignotement pendant le rebind
                    float blink = std::sin(m_time * 6.f);
                    uint8_t ba = static_cast<uint8_t>(blink > 0.f ? 255 : 80);
                    valText.setFillColor({ 255, 255, 100, ba });
                }
                else
                {
                    valText.setFillColor(col);
                }

                auto vb = valText.getLocalBounds();
                valText.setOrigin({ 0.f, vb.position.y + vb.size.y / 2.f });
                valText.setPosition({ rightX, y });
                window.draw(valText);
            }
            else if (item.type == OptionType::Slider && item.sliderPtr)
            {
                int val = *item.sliderPtr;
                drawSlider(window, rightX, y, val, item.neonColor, isSel);
            }
            else if (item.type == OptionType::Toggle && item.togglePtr)
            {
                bool on = *item.togglePtr;
                std::string valStr = on ? "ON" : "OFF";
                sf::Text valText(m_font, valStr, 22u);
                valText.setLetterSpacing(3.f);

                if (on)
                    valText.setFillColor(NeonColors::Green);
                else
                    valText.setFillColor(sf::Color(100, 100, 100, 200));

                auto vb = valText.getLocalBounds();
                valText.setOrigin({ 0.f, vb.position.y + vb.size.y / 2.f });
                valText.setPosition({ rightX, y });
                window.draw(valText);
            }
            else if (item.type == OptionType::Back)
            {
                // Centrer le "BACK"
                nameText.setPosition({ cx, y });
                auto backBounds = nameText.getLocalBounds();
                nameText.setOrigin({ backBounds.position.x + backBounds.size.x / 2.f,
                                     backBounds.position.y + backBounds.size.y / 2.f });
                // On redraw par dessus
                window.draw(nameText);
            }

            // Selection indicator (petit triangle)
            if (isSel && item.type != OptionType::Label)
            {
                float triX = leftX - 25.f;
                sf::CircleShape tri(8.f, 3);
                tri.setOrigin({ 8.f, 8.f });
                tri.setPosition({ triX, y });
                tri.setRotation(sf::degrees(90.f));
                float pulse = std::sin(m_time * 4.f) * 0.3f + 0.7f;
                uint8_t ta = static_cast<uint8_t>(200 * pulse);
                tri.setFillColor({ item.neonColor.r, item.neonColor.g,
                                   item.neonColor.b, ta });
                window.draw(tri);
            }
        }

        // Hint
        float hp = std::sin(m_time * 1.2f) * 0.3f + 0.7f;
        m_hintText.setFillColor({ 80, 80, 100,
            static_cast<uint8_t>(60 + 60 * hp) });
        window.draw(m_hintText);
    }

private:
    void moveSelection(int dir)
    {
        int n = static_cast<int>(m_items.size());
        m_selected = (m_selected + dir + n) % n;
        skipToNextSelectable(dir);
    }

    void skipToNextSelectable(int dir)
    {
        int n = static_cast<int>(m_items.size());
        int tries = 0;
        while (m_items[m_selected].type == OptionType::Label && tries < n)
        {
            m_selected = (m_selected + dir + n) % n;
            ++tries;
        }
    }

    void adjustValue(int delta)
    {
        auto& item = m_items[m_selected];
        if (item.type == OptionType::Slider && item.sliderPtr)
        {
            *item.sliderPtr = std::clamp(*item.sliderPtr + delta, 0, 100);
        }
        else if (item.type == OptionType::Toggle && item.togglePtr)
        {
            *item.togglePtr = !(*item.togglePtr);
        }
    }

    void drawSlider(sf::RenderWindow& window, float x, float y,
        int value, sf::Color neonColor, bool selected) const
    {
        float barW = 150.f;
        float barH = 8.f;

        // Fond de la barre
        sf::RectangleShape barBg{ sf::Vector2f{ barW, barH } };
        barBg.setOrigin({ 0.f, barH / 2.f });
        barBg.setPosition({ x, y });
        barBg.setFillColor(sf::Color(30, 30, 50, 200));
        window.draw(barBg);

        // Remplissage
        float fillW = barW * (value / 100.f);
        if (fillW > 0.f)
        {
            sf::RectangleShape barFill{ sf::Vector2f{ fillW, barH } };
            barFill.setOrigin({ 0.f, barH / 2.f });
            barFill.setPosition({ x, y });

            uint8_t a = selected ? static_cast<uint8_t>(255) : static_cast<uint8_t>(160);
            barFill.setFillColor({ neonColor.r, neonColor.g, neonColor.b, a });
            window.draw(barFill);
        }

        // Glow si selectionne
        if (selected)
        {
            float pulse = std::sin(m_time * 3.f) * 0.3f + 0.7f;
            sf::RectangleShape glow{ sf::Vector2f{ barW + 8.f, barH + 8.f } };
            glow.setOrigin({ 4.f, (barH + 8.f) / 2.f });
            glow.setPosition({ x - 4.f, y });
            uint8_t ga = static_cast<uint8_t>(25 * pulse);
            glow.setFillColor({ neonColor.r, neonColor.g, neonColor.b, ga });
            window.draw(glow);
        }

        // Pourcentage
        sf::Text pctText(m_font, std::to_string(value) + "%", 18u);
        pctText.setFillColor(selected
            ? sf::Color{ neonColor.r, neonColor.g, neonColor.b, 255 }
        : sf::Color{ 140, 140, 160, 200 });
        auto pb = pctText.getLocalBounds();
        pctText.setOrigin({ 0.f, pb.position.y + pb.size.y / 2.f });
        pctText.setPosition({ x + barW + 12.f, y });
        window.draw(pctText);
    }

    const sf::Font& m_font;
    sf::Vector2u              m_windowSize;
    GameSettings& m_settings;
    std::vector<OptionItem>   m_items;
    sf::Text                  m_titleText;
    mutable sf::Text          m_hintText;
    LightSystem               m_previewLights;
    int                       m_selected = 0;
    bool                      m_rebinding = false;
    float                     m_time = 0.f;
};