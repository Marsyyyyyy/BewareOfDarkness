#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <random>
#include "Colors.hpp"

// ---- Utilitaire ------------------------------------------------
inline sf::Vector2f toFloat(sf::Vector2u v)
{
    return { static_cast<float>(v.x), static_cast<float>(v.y) };
}

// ---- Particule neon --------------------------------------------
struct NeonParticle
{
    sf::Vector2f pos;
    sf::Vector2f vel;
    float        radius = 2.f;
    float        alpha = 0.5f;
    float        speed = 20.f;
    sf::Color    baseColor = NeonColors::Cyan;
};

// ---- Texte avec glow neon style --------------------------------
class NeonGlowText
{
public:
    NeonGlowText(const sf::Font& font, const std::string& str,
        unsigned int size, sf::Color glowColor);

    void setPosition(sf::Vector2f pos);
    void update(float time);
    void draw(sf::RenderTarget& target) const;

private:
    static constexpr int   m_layers = 6;

    const sf::Font& m_font;
    sf::Color              m_glowColor;
    sf::Text               m_coreText;
    std::vector<sf::Text>  m_glowLayers;
    sf::Vector2f           m_position;
    float                  m_time = 0.f;
};

// ---- Bouton neon avec selection glow ---------------------------
class NeonButton
{
public:
    NeonButton(const sf::Font& font, const std::string& label,
        sf::Vector2f position, sf::Color neonColor);

    void update(bool selected, float dt, float totalTime);
    void draw(sf::RenderTarget& rt) const;

private:
    sf::Text           m_label;
    sf::RectangleShape m_box;
    sf::RectangleShape m_glow;
    sf::Color          m_neonColor;
    sf::Vector2f       m_position;
    float              m_selectT = 0.f;
};

// ---- Menu principal (keyboard driven) -------------------------
class NeonMenu
{
public:
    enum class Action { None, Play, Options, Exit };

    NeonMenu(sf::Vector2u windowSize, const sf::Font& font);

    void   moveUp();
    void   moveDown();
    Action confirm() const;
    void   update(float dt);
    void   draw(sf::RenderTarget& target) const;

private:
    void initParticles(int count);
    void updateParticles(float dt);
    void drawParticles(sf::RenderTarget& target) const;

    const sf::Font& m_font;
    sf::Vector2u               m_windowSize;
    std::vector<NeonButton>    m_buttons;
    NeonGlowText               m_title;
    sf::Text                   m_subtitle;
    sf::Text                   m_hint;
    sf::Text                   m_footer;
    sf::RectangleShape         m_line;
    std::vector<NeonParticle>  m_particles;
    float                      m_time = 0.f;
    int                        m_selected = 0;
};