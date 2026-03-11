#pragma once
// ============================================================
//  GameOver.hpp  -  Ecran Game Over neon rouge
//  SFML 3.0.2  |  C++20  |  Keyboard only
// ============================================================

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <random>
#include "Colors.hpp"

inline sf::Vector2f toFloatGO(sf::Vector2u v)
{
    return { static_cast<float>(v.x), static_cast<float>(v.y) };
}

// ---- Particule de sang/braise qui tombe --------------------
struct EmberParticle
{
    sf::Vector2f pos;
    sf::Vector2f vel;
    float radius;
    float alpha;
    float life;
    float maxLife;
    sf::Color color;
};

class GameOverScreen
{
public:
    enum class Action { None, Retry, Menu };

    GameOverScreen(sf::Vector2u windowSize, const sf::Font& font)
        : m_font(font)
        , m_windowSize(windowSize)
        , m_title(font, "GAME  OVER", 80u)
        , m_subtitle(font, "You  didn't  survive  the  darkness", 20u)
        , m_retryText(font, "RETRY", 30u)
        , m_menuText(font, "MENU", 30u)
        , m_hint(font, "UP / DOWN   -   ENTER", 13u)
    {
        float cx = windowSize.x / 2.f;

        // -- Titre GAME OVER --
        m_title.setLetterSpacing(8.f);
        m_title.setFillColor(sf::Color(255, 20, 20, 255));
        auto tb = m_title.getLocalBounds();
        m_title.setOrigin({ tb.position.x + tb.size.x / 2.f,
                            tb.position.y + tb.size.y / 2.f });
        m_title.setPosition({ cx, windowSize.y * 0.28f });

        // -- Sous-titre --
        m_subtitle.setLetterSpacing(4.f);
        m_subtitle.setFillColor(sf::Color(200, 60, 60, 180));
        auto sb = m_subtitle.getLocalBounds();
        m_subtitle.setOrigin({ sb.position.x + sb.size.x / 2.f,
                               sb.position.y + sb.size.y / 2.f });
        m_subtitle.setPosition({ cx, windowSize.y * 0.40f });

        // -- Ligne rouge --
        m_line.setSize({ 350.f, 2.f });
        m_line.setOrigin({ 175.f, 1.f });
        m_line.setPosition({ cx, windowSize.y * 0.47f });
        m_line.setFillColor(sf::Color(255, 30, 30, 60));

        // -- Boutons --
        setupButton(m_retryText, cx, windowSize.y * 0.58f);
        setupButton(m_menuText, cx, windowSize.y * 0.68f);

        // -- Boxes pour les boutons --
        m_retryBox.setSize({ 280.f, 55.f });
        m_retryBox.setOrigin({ 140.f, 27.5f });
        m_retryBox.setPosition({ cx, windowSize.y * 0.58f });

        m_menuBox.setSize({ 280.f, 55.f });
        m_menuBox.setOrigin({ 140.f, 27.5f });
        m_menuBox.setPosition({ cx, windowSize.y * 0.68f });

        // -- Hint --
        m_hint.setLetterSpacing(4.f);
        m_hint.setFillColor(sf::Color(80, 40, 40, 100));
        auto hb = m_hint.getLocalBounds();
        m_hint.setOrigin({ hb.position.x + hb.size.x / 2.f,
                           hb.position.y + hb.size.y / 2.f });
        m_hint.setPosition({ cx, windowSize.y * 0.85f });

        // Couches de glow pour le titre
        for (int i = 0; i < m_glowLayers; ++i)
        {
            sf::Text layer(font, "GAME  OVER", 80u);
            layer.setLetterSpacing(8.f);
            auto lb = layer.getLocalBounds();
            layer.setOrigin({ lb.position.x + lb.size.x / 2.f,
                              lb.position.y + lb.size.y / 2.f });
            layer.setPosition({ cx, windowSize.y * 0.28f });
            m_titleGlow.push_back(std::move(layer));
        }

        initEmbers(80);
    }

    void moveUp()
    {
        m_selected = (m_selected - 1 + 2) % 2;
    }

    void moveDown()
    {
        m_selected = (m_selected + 1) % 2;
    }

    Action confirm() const
    {
        return m_selected == 0 ? Action::Retry : Action::Menu;
    }

    void update(float dt)
    {
        m_time += dt;

        // Fade in au debut
        if (m_fadeIn < 1.f)
            m_fadeIn = std::min(m_fadeIn + dt * 0.8f, 1.f);

        updateEmbers(dt);
    }

    void draw(sf::RenderTarget& target) const
    {
        // Fond noir avec vignette rouge
        sf::RectangleShape bg{ toFloatGO(m_windowSize) };
        bg.setFillColor(sf::Color(12, 4, 4, 255));
        target.draw(bg);

        // Vignette rouge pulsante sur les bords
        drawVignette(target);

        // Particules braise
        drawEmbers(target);

        // Ligne
        float linePulse = std::sin(m_time * 2.f) * 0.4f + 0.6f;
        sf::RectangleShape line = m_line;
        line.setFillColor({ 255, 30, 30,
            static_cast<uint8_t>(40 + 60 * linePulse * m_fadeIn) });
        target.draw(line);

        // Titre glow (couches rouges derriere)
        drawTitleGlow(target);

        // Titre principal
        {
            // Tremblement subtil
            float shake = std::sin(m_time * 25.f) * 1.2f * std::max(0.f, 1.f - m_time * 0.3f);
            sf::Text title = m_title;

            // Couleur qui pulse entre rouge vif et rouge sombre
            float pulse = std::sin(m_time * 2.5f) * 0.2f + 0.8f;
            uint8_t r = static_cast<uint8_t>(200 + 55 * pulse * m_fadeIn);
            uint8_t g = static_cast<uint8_t>(20 * pulse);
            title.setFillColor({ r, g, static_cast<uint8_t>(g),
                static_cast<uint8_t>(255 * m_fadeIn) });
            title.setPosition({ m_title.getPosition().x + shake,
                                m_title.getPosition().y + shake * 0.5f });
            target.draw(title);
        }

        // Sous-titre (fade in retarde)
        {
            float subFade = std::clamp(m_fadeIn * 2.f - 0.4f, 0.f, 1.f);
            float subPulse = std::sin(m_time * 1.5f) * 0.1f + 0.9f;
            sf::Text sub = m_subtitle;
            sub.setFillColor({ 220, 70, 70,
                static_cast<uint8_t>(255 * subFade * subPulse) });
            target.draw(sub);
        }

        // Boutons
        drawButton(target, m_retryText, m_retryBox, 0);
        drawButton(target, m_menuText, m_menuBox, 1);

        // Hint
        float hp = std::sin(m_time * 1.2f) * 0.3f + 0.7f;
        sf::Text hint = m_hint;
        hint.setFillColor({ 80, 40, 40,
            static_cast<uint8_t>(60 + 50 * hp * m_fadeIn) });
        target.draw(hint);
    }

private:
    void setupButton(sf::Text& text, float x, float y)
    {
        text.setLetterSpacing(3.f);
        text.setFillColor(sf::Color(160, 100, 100, 200));
        auto b = text.getLocalBounds();
        text.setOrigin({ b.position.x + b.size.x / 2.f,
                         b.position.y + b.size.y / 2.f });
        text.setPosition({ x, y });
    }

    void drawButton(sf::RenderTarget& target, const sf::Text& text,
        const sf::RectangleShape& box, int index) const
    {
        bool sel = (index == m_selected);
        float pulse = std::sin(m_time * 3.f) * 0.15f + 0.85f;

        // Selecteur smooth
        float t = sel ? 1.f : 0.f;

        // Box
        sf::RectangleShape b = box;
        if (sel)
        {
            uint8_t oa = static_cast<uint8_t>(150 + 105 * pulse);
            b.setOutlineThickness(2.f);
            b.setOutlineColor({ 255, 40, 40, oa });
            b.setFillColor(sf::Color(255, 20, 20, static_cast<uint8_t>(25 * pulse)));

            // Glow
            sf::RectangleShape glow;
            glow.setSize({ 296.f, 67.f });
            glow.setOrigin({ 148.f, 33.5f });
            glow.setPosition(box.getPosition());
            glow.setFillColor({ 255, 20, 20, static_cast<uint8_t>(15 * pulse) });
            target.draw(glow);
        }
        else
        {
            b.setOutlineThickness(1.f);
            b.setOutlineColor(sf::Color(100, 30, 30, 80));
            b.setFillColor(sf::Color(20, 8, 8, 150));
        }
        target.draw(b);

        // Texte
        sf::Text txt = text;
        if (sel)
        {
            txt.setFillColor({ 255, static_cast<uint8_t>(60 + 40 * pulse),
                               static_cast<uint8_t>(60 + 40 * pulse), 255 });
            float s = 1.f + pulse * 0.04f;
            txt.setScale({ s, s });
        }
        else
        {
            txt.setFillColor(sf::Color(140, 80, 80, 200));
        }
        target.draw(txt);
    }

    void drawVignette(sf::RenderTarget& target) const
    {
        float pulse = std::sin(m_time * 1.5f) * 0.3f + 0.7f;
        uint8_t a = static_cast<uint8_t>(30 * pulse * m_fadeIn);

        float w = static_cast<float>(m_windowSize.x);
        float h = static_cast<float>(m_windowSize.y);
        float thickness = 80.f;

        // Haut
        sf::RectangleShape top{ sf::Vector2f{ w, thickness } };
        top.setFillColor({ 180, 0, 0, a });
        target.draw(top);

        // Bas
        sf::RectangleShape bot{ sf::Vector2f{ w, thickness } };
        bot.setPosition({ 0.f, h - thickness });
        bot.setFillColor({ 180, 0, 0, a });
        target.draw(bot);

        // Gauche
        sf::RectangleShape left{ sf::Vector2f{ thickness, h } };
        left.setFillColor({ 180, 0, 0, a });
        target.draw(left);

        // Droite
        sf::RectangleShape right{ sf::Vector2f{ thickness, h } };
        right.setPosition({ w - thickness, 0.f });
        right.setFillColor({ 180, 0, 0, a });
        target.draw(right);
    }

    void drawTitleGlow(sf::RenderTarget& target) const
    {
        float pulse = std::sin(m_time * 2.f) * 0.2f + 0.8f;

        for (int i = m_glowLayers - 1; i >= 0; --i)
        {
            float t = static_cast<float>(i) / m_glowLayers;
            float scale = 1.f + t * 0.12f * pulse;

            sf::Text layer = m_titleGlow[i];
            layer.setScale({ scale, scale });

            uint8_t a = static_cast<uint8_t>((1.f - t) * 100.f * pulse * m_fadeIn);
            layer.setFillColor({ 255, 0, 0, a });

            // Tremblement qui diminue avec le temps
            float shake = std::sin(m_time * 25.f + i * 0.5f)
                * 1.5f * std::max(0.f, 1.f - m_time * 0.3f);
            layer.setPosition({ m_titleGlow[i].getPosition().x + shake,
                                m_titleGlow[i].getPosition().y + shake * 0.3f });
            target.draw(layer);
        }
    }

    // ---- Particules braise / etincelles ---------------------
    void initEmbers(int count)
    {
        std::mt19937 rng(123);
        std::uniform_real_distribution<float> dX(0.f, static_cast<float>(m_windowSize.x));
        std::uniform_real_distribution<float> dY(0.f, static_cast<float>(m_windowSize.y));
        std::uniform_real_distribution<float> dR(0.8f, 3.f);
        std::uniform_real_distribution<float> dL(2.f, 6.f);
        std::uniform_real_distribution<float> dS(-15.f, 15.f);
        std::uniform_int_distribution<int>    dC(0, 2);

        sf::Color palette[] = {
            sf::Color(255, 40, 20),
            sf::Color(255, 100, 20),
            sf::Color(200, 10, 10)
        };

        for (int i = 0; i < count; ++i)
        {
            EmberParticle p;
            p.pos = { dX(rng), dY(rng) };
            p.vel = { dS(rng), -(10.f + dR(rng) * 8.f) };
            p.radius = dR(rng);
            p.life = dL(rng);
            p.maxLife = p.life;
            p.alpha = 0.6f;
            p.color = palette[dC(rng)];
            m_embers.push_back(p);
        }
    }

    void updateEmbers(float dt)
    {
        float w = static_cast<float>(m_windowSize.x);
        float h = static_cast<float>(m_windowSize.y);

        for (auto& p : m_embers)
        {
            p.pos += p.vel * dt;
            p.life -= dt;

            // Flicker
            p.alpha = 0.3f + 0.5f * std::sin(m_time * p.radius * 3.f);

            // Respawn en bas quand sort par le haut
            if (p.pos.y < -10.f)
            {
                p.pos.y = h + 10.f;
                p.life = p.maxLife;
            }
            if (p.pos.x < -10.f)   p.pos.x = w + 10.f;
            if (p.pos.x > w + 10.f) p.pos.x = -10.f;
        }
    }

    void drawEmbers(sf::RenderTarget& target) const
    {
        for (const auto& p : m_embers)
        {
            float lifeRatio = std::clamp(p.life / p.maxLife, 0.f, 1.f);
            float a = p.alpha * lifeRatio * m_fadeIn;

            sf::CircleShape dot(p.radius);
            dot.setOrigin({ p.radius, p.radius });
            dot.setPosition(p.pos);
            dot.setFillColor({ p.color.r, p.color.g, p.color.b,
                static_cast<uint8_t>(std::clamp(a * 255.f, 0.f, 255.f)) });
            target.draw(dot);
        }
    }

    const sf::Font& m_font;
    sf::Vector2u                   m_windowSize;
    sf::Text                       m_title;
    sf::Text                       m_subtitle;
    sf::Text                       m_retryText;
    sf::Text                       m_menuText;
    sf::Text                       m_hint;
    sf::RectangleShape             m_line;
    sf::RectangleShape             m_retryBox;
    sf::RectangleShape             m_menuBox;
    std::vector<sf::Text>          m_titleGlow;
    std::vector<EmberParticle>     m_embers;
    float                          m_time = 0.f;
    float                          m_fadeIn = 0.f;
    int                            m_selected = 0;
    static constexpr int           m_glowLayers = 5;
};