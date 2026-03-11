#pragma once
// ============================================================
//  Victory.hpp  -  Ecran de victoire neon dore + feux d'artifice
//  SFML 3.0.2  |  C++20  |  Keyboard only
// ============================================================

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <random>
#include "Colors.hpp"

inline sf::Vector2f toFloatVC(sf::Vector2u v)
{
    return { static_cast<float>(v.x), static_cast<float>(v.y) };
}

struct SparkParticle
{
    sf::Vector2f pos;
    sf::Vector2f vel;
    float radius = 1.f;
    float alpha = 0.5f;
    float life = 3.f;
    float maxLife = 3.f;
    float twinkleSpeed = 4.f;
    sf::Color color = sf::Color(255, 220, 80);
};

struct FireworkParticle
{
    sf::Vector2f pos;
    sf::Vector2f vel;
    float radius = 2.f;
    float life = 1.f;
    float maxLife = 1.f;
    float drag = 0.95f;
    sf::Color color = sf::Color(255, 220, 60);
};

struct Firework
{
    sf::Vector2f origin;
    sf::Vector2f risePos;
    std::vector<FireworkParticle> particles;
    float age = 0.f;
    bool  exploded = false;
    float riseSpeed = 500.f;
    float targetY = 200.f;
    float trailTimer = 0.f;
    sf::Color color = sf::Color(255, 220, 60);
};

class VictoryScreen
{
public:
    enum class Action { None, Continue, Menu };

    VictoryScreen(sf::Vector2u windowSize, const sf::Font& font)
        : m_font(font)
        , m_windowSize(windowSize)
        , m_title(font, "VICTORY", 90u)
        , m_subtitle(font, "You  conquered  the  darkness", 20u)
        , m_continueText(font, "CONTINUE", 30u)
        , m_menuText(font, "MENU", 30u)
        , m_hint(font, "UP / DOWN   -   ENTER", 13u)
    {
        float cx = windowSize.x / 2.f;

        m_title.setLetterSpacing(10.f);
        m_title.setFillColor(sf::Color(255, 220, 80, 255));
        auto tb = m_title.getLocalBounds();
        m_title.setOrigin({ tb.position.x + tb.size.x / 2.f,
                            tb.position.y + tb.size.y / 2.f });
        m_title.setPosition({ cx, windowSize.y * 0.25f });

        m_subtitle.setLetterSpacing(5.f);
        m_subtitle.setFillColor(sf::Color(200, 180, 100, 220));
        auto sb = m_subtitle.getLocalBounds();
        m_subtitle.setOrigin({ sb.position.x + sb.size.x / 2.f,
                               sb.position.y + sb.size.y / 2.f });
        m_subtitle.setPosition({ cx, windowSize.y * 0.38f });

        m_line.setSize({ 400.f, 2.f });
        m_line.setOrigin({ 200.f, 1.f });
        m_line.setPosition({ cx, windowSize.y * 0.45f });
        m_line.setFillColor(sf::Color(255, 200, 50, 60));

        m_line2.setSize({ 300.f, 1.f });
        m_line2.setOrigin({ 150.f, 0.5f });
        m_line2.setPosition({ cx, windowSize.y * 0.455f });
        m_line2.setFillColor(sf::Color(255, 200, 50, 30));

        setupButton(m_continueText, cx, windowSize.y * 0.58f);
        setupButton(m_menuText, cx, windowSize.y * 0.68f);

        m_continueBox.setSize({ 300.f, 55.f });
        m_continueBox.setOrigin({ 150.f, 27.5f });
        m_continueBox.setPosition({ cx, windowSize.y * 0.58f });

        m_menuBox.setSize({ 300.f, 55.f });
        m_menuBox.setOrigin({ 150.f, 27.5f });
        m_menuBox.setPosition({ cx, windowSize.y * 0.68f });

        m_hint.setLetterSpacing(4.f);
        m_hint.setFillColor(sf::Color(80, 70, 40, 100));
        auto hb = m_hint.getLocalBounds();
        m_hint.setOrigin({ hb.position.x + hb.size.x / 2.f,
                           hb.position.y + hb.size.y / 2.f });
        m_hint.setPosition({ cx, windowSize.y * 0.85f });

        for (int i = 0; i < m_glowLayers; ++i)
        {
            sf::Text layer(font, "VICTORY", 90u);
            layer.setLetterSpacing(10.f);
            auto lb = layer.getLocalBounds();
            layer.setOrigin({ lb.position.x + lb.size.x / 2.f,
                              lb.position.y + lb.size.y / 2.f });
            layer.setPosition({ cx, windowSize.y * 0.25f });
            m_titleGlow.push_back(std::move(layer));
        }

        initSparks(70);
        m_rng.seed(42);
    }

    void moveUp() { m_selected = (m_selected - 1 + 2) % 2; }
    void moveDown() { m_selected = (m_selected + 1) % 2; }

    Action confirm() const
    {
        return m_selected == 0 ? Action::Continue : Action::Menu;
    }

    void update(float dt)
    {
        m_time += dt;
        if (m_fadeIn < 1.f)
            m_fadeIn = std::min(m_fadeIn + dt * 1.0f, 1.f);

        updateSparks(dt);
        updateFireworks(dt);

        // Feux d'artifice seulement pendant les 3 premieres secondes
            m_nextFireworkTimer -= dt;
            if (m_nextFireworkTimer <= 0.f)
            {
                spawnFirework();
                std::uniform_real_distribution<float> dInterval(0.2f, 0.6f);
                m_nextFireworkTimer = dInterval(m_rng);
            }
    }

    void draw(sf::RenderTarget& target) const
    {
        sf::RectangleShape bg{ toFloatVC(m_windowSize) };
        bg.setFillColor(sf::Color(6, 6, 12, 255));
        target.draw(bg);

        drawFireworks(target);
        drawSparks(target);

        // Lignes
        {
            float lp = std::sin(m_time * 2.f) * 0.3f + 0.7f;
            sf::RectangleShape line = m_line;
            line.setFillColor({ 255, 200, 50,
                static_cast<uint8_t>(50 + 80 * lp * m_fadeIn) });
            target.draw(line);

            sf::RectangleShape line2 = m_line2;
            line2.setFillColor({ 255, 200, 50,
                static_cast<uint8_t>(20 + 40 * lp * m_fadeIn) });
            target.draw(line2);
        }

        drawTitleGlow(target);

        // Titre principal
        {
            float scaleIn = std::min(m_fadeIn * 1.5f, 1.f);
            float breathe = std::sin(m_time * 1.8f) * 0.02f + 1.f;
            float s = scaleIn * breathe;

            sf::Text title = m_title;
            title.setScale({ s, s });

            float pulse = std::sin(m_time * 2.f) * 0.15f + 0.85f;
            uint8_t r = static_cast<uint8_t>(230 + 25 * pulse);
            uint8_t g = static_cast<uint8_t>(190 + 40 * pulse);
            uint8_t b = static_cast<uint8_t>(40 + 40 * pulse);
            title.setFillColor({ r, g, b,
                static_cast<uint8_t>(255 * m_fadeIn) });
            target.draw(title);
        }

        // Sous-titre
        {
            float subFade = std::clamp(m_fadeIn * 2.f - 0.5f, 0.f, 1.f);
            float subPulse = std::sin(m_time * 1.5f) * 0.1f + 0.9f;
            sf::Text sub = m_subtitle;
            sub.setFillColor({ 220, 200, 120,
                static_cast<uint8_t>(240 * subFade * subPulse) });
            target.draw(sub);
        }

        drawButton(target, m_continueText, m_continueBox, 0);
        drawButton(target, m_menuText, m_menuBox, 1);

        float hp = std::sin(m_time * 1.2f) * 0.3f + 0.7f;
        sf::Text hint = m_hint;
        hint.setFillColor({ 100, 90, 50,
            static_cast<uint8_t>(60 + 50 * hp * m_fadeIn) });
        target.draw(hint);
    }

private:
    void setupButton(sf::Text& text, float x, float y)
    {
        text.setLetterSpacing(3.f);
        text.setFillColor(sf::Color(160, 140, 80, 200));
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

        sf::RectangleShape b = box;
        if (sel)
        {
            uint8_t oa = static_cast<uint8_t>(140 + 115 * pulse);
            b.setOutlineThickness(2.f);
            b.setOutlineColor({ 255, 210, 60, oa });
            b.setFillColor(sf::Color(255, 200, 40,
                static_cast<uint8_t>(20 * pulse)));

            sf::RectangleShape glow;
            glow.setSize({ 316.f, 67.f });
            glow.setOrigin({ 158.f, 33.5f });
            glow.setPosition(box.getPosition());
            glow.setFillColor({ 255, 200, 40,
                static_cast<uint8_t>(12 * pulse) });
            target.draw(glow);
        }
        else
        {
            b.setOutlineThickness(1.f);
            b.setOutlineColor(sf::Color(120, 100, 40, 80));
            b.setFillColor(sf::Color(15, 12, 5, 150));
        }
        target.draw(b);

        sf::Text txt = text;
        if (sel)
        {
            txt.setFillColor({ 255,
                static_cast<uint8_t>(210 + 30 * pulse),
                static_cast<uint8_t>(50 + 30 * pulse), 255 });
            float s = 1.f + pulse * 0.04f;
            txt.setScale({ s, s });
        }
        else
        {
            txt.setFillColor(sf::Color(150, 130, 70, 200));
        }
        target.draw(txt);
    }

    void drawTitleGlow(sf::RenderTarget& target) const
    {
        float pulse = std::sin(m_time * 2.f) * 0.2f + 0.8f;
        float scaleIn = std::min(m_fadeIn * 1.5f, 1.f);
        float breathe = std::sin(m_time * 1.8f) * 0.02f + 1.f;

        for (int i = m_glowLayers - 1; i >= 0; --i)
        {
            float t = static_cast<float>(i) / m_glowLayers;
            float scale = (1.f + t * 0.12f * pulse) * scaleIn * breathe;

            sf::Text layer = m_titleGlow[i];
            layer.setScale({ scale, scale });

            uint8_t a = static_cast<uint8_t>(
                (1.f - t) * 90.f * pulse * m_fadeIn);
            layer.setFillColor({ 255, 180, 20, a });
            target.draw(layer);
        }
    }

    // ---- Feux d'artifice ------------------------------------
    void spawnFirework()
    {
        // Spawn sur les cotes gauche ou droit uniquement
        std::uniform_int_distribution<int> dSide(0, 1);
        bool leftSide = dSide(m_rng) == 0;
        float minX, maxX;
        if (leftSide)
        {
            minX = m_windowSize.x * 0.03f;
            maxX = m_windowSize.x * 0.22f;
        }
        else
        {
            minX = m_windowSize.x * 0.78f;
            maxX = m_windowSize.x * 0.97f;
        }
        std::uniform_real_distribution<float> dX(minX, maxX);
        std::uniform_real_distribution<float> dY(
            m_windowSize.y * 0.08f, m_windowSize.y * 0.45f);
        std::uniform_real_distribution<float> dSpd(350.f, 600.f);
        std::uniform_int_distribution<int> dC(0, 4);

        sf::Color palette[] = {
            sf::Color(255, 220, 60),
            sf::Color(255, 180, 30),
            sf::Color(255, 255, 120),
            sf::Color(255, 200, 80),
            sf::Color(255, 240, 150)
        };

        Firework fw;
        float x = dX(m_rng);
        fw.risePos = { x, static_cast<float>(m_windowSize.y) + 5.f };
        fw.origin = fw.risePos;
        fw.targetY = dY(m_rng);
        fw.riseSpeed = dSpd(m_rng);
        fw.color = palette[dC(m_rng)];
        fw.exploded = false;
        fw.age = 0.f;
        fw.trailTimer = 0.f;

        m_fireworks.push_back(fw);
    }

    void explodeFirework(Firework& fw)
    {
        std::uniform_real_distribution<float> dAngle(0.f, 6.2832f);
        std::uniform_real_distribution<float> dSpeed(40.f, 200.f);
        std::uniform_real_distribution<float> dRadius(1.f, 3.5f);
        std::uniform_real_distribution<float> dLife(0.8f, 2.5f);
        std::uniform_real_distribution<float> dDrag(0.92f, 0.98f);
        std::uniform_int_distribution<int> dVar(-30, 30);

        int count = 60 + static_cast<int>(m_rng() % 40);

        for (int i = 0; i < count; ++i)
        {
            float angle = dAngle(m_rng);
            float speed = dSpeed(m_rng);

            FireworkParticle p;
            p.pos = fw.risePos;
            p.vel = { std::cos(angle) * speed, std::sin(angle) * speed };
            p.radius = dRadius(m_rng);
            p.life = dLife(m_rng);
            p.maxLife = p.life;
            p.drag = dDrag(m_rng);

            int rv = dVar(m_rng);
            int gv = dVar(m_rng);
            p.color = {
                static_cast<uint8_t>(std::clamp(static_cast<int>(fw.color.r) + rv, 100, 255)),
                static_cast<uint8_t>(std::clamp(static_cast<int>(fw.color.g) + gv, 50, 255)),
                static_cast<uint8_t>(std::clamp(static_cast<int>(fw.color.b) + rv / 2, 0, 255)),
                255
            };

            fw.particles.push_back(p);
        }

        fw.exploded = true;
    }

    void updateFireworks(float dt)
    {
        for (auto& fw : m_fireworks)
        {
            fw.age += dt;

            if (!fw.exploded)
            {
                fw.risePos.y -= fw.riseSpeed * dt;

                fw.trailTimer -= dt;
                if (fw.trailTimer <= 0.f)
                {
                    std::uniform_real_distribution<float> dOff(-3.f, 3.f);
                    FireworkParticle trail;
                    trail.pos = { fw.risePos.x + dOff(m_rng), fw.risePos.y };
                    trail.vel = { dOff(m_rng) * 2.f, 20.f + dOff(m_rng) * 5.f };
                    trail.radius = 1.5f;
                    trail.life = 0.3f;
                    trail.maxLife = 0.3f;
                    trail.drag = 0.95f;
                    trail.color = { 255, 220, 100, 255 };
                    fw.particles.push_back(trail);
                    fw.trailTimer = 0.02f;
                }

                if (fw.risePos.y <= fw.targetY)
                    explodeFirework(fw);
            }

            for (auto& p : fw.particles)
            {
                p.vel.x *= p.drag;
                p.vel.y *= p.drag;
                p.vel.y += 30.f * dt;
                p.pos += p.vel * dt;
                p.life -= dt;
            }

            fw.particles.erase(
                std::remove_if(fw.particles.begin(), fw.particles.end(),
                    [](const FireworkParticle& p) { return p.life <= 0.f; }),
                fw.particles.end());
        }

        m_fireworks.erase(
            std::remove_if(m_fireworks.begin(), m_fireworks.end(),
                [](const Firework& fw) {
                    return fw.exploded && fw.particles.empty();
                }),
            m_fireworks.end());
    }

    void drawFireworks(sf::RenderTarget& target) const
    {
        for (const auto& fw : m_fireworks)
        {
            if (!fw.exploded)
            {
                sf::CircleShape rocket(3.f);
                rocket.setOrigin({ 3.f, 3.f });
                rocket.setPosition(fw.risePos);
                rocket.setFillColor({ 255, 255, 200, 255 });
                target.draw(rocket);

                sf::CircleShape rGlow(8.f);
                rGlow.setOrigin({ 8.f, 8.f });
                rGlow.setPosition(fw.risePos);
                rGlow.setFillColor({ fw.color.r, fw.color.g, fw.color.b, 60 });
                target.draw(rGlow);
            }

            for (const auto& p : fw.particles)
            {
                float lifeRatio = std::clamp(p.life / p.maxLife, 0.f, 1.f);

                if (p.radius > 1.5f && lifeRatio > 0.3f)
                {
                    sf::CircleShape glow(p.radius * 3.f);
                    glow.setOrigin({ p.radius * 3.f, p.radius * 3.f });
                    glow.setPosition(p.pos);
                    uint8_t ga = static_cast<uint8_t>(20 * lifeRatio);
                    glow.setFillColor({ p.color.r, p.color.g, p.color.b, ga });
                    target.draw(glow);
                }

                float r = p.radius * lifeRatio;
                sf::CircleShape dot(r);
                dot.setOrigin({ r, r });
                dot.setPosition(p.pos);
                uint8_t a = static_cast<uint8_t>(255 * lifeRatio * lifeRatio);
                dot.setFillColor({ p.color.r, p.color.g, p.color.b, a });
                target.draw(dot);
            }
        }
    }

    // ---- Particules etoiles de fond -------------------------
    void initSparks(int count)
    {
        std::mt19937 rng(777);
        std::uniform_real_distribution<float> dX(0.f, static_cast<float>(m_windowSize.x));
        std::uniform_real_distribution<float> dY(0.f, static_cast<float>(m_windowSize.y));
        std::uniform_real_distribution<float> dR(0.5f, 2.8f);
        std::uniform_real_distribution<float> dL(3.f, 8.f);
        std::uniform_real_distribution<float> dSx(-8.f, 8.f);
        std::uniform_real_distribution<float> dSy(-25.f, -5.f);
        std::uniform_real_distribution<float> dTw(2.f, 8.f);
        std::uniform_int_distribution<int>    dC(0, 3);

        sf::Color palette[] = {
            sf::Color(255, 220, 80),
            sf::Color(255, 200, 50),
            sf::Color(255, 255, 150),
            sf::Color(200, 180, 255)
        };

        for (int i = 0; i < count; ++i)
        {
            SparkParticle p;
            p.pos = { dX(rng), dY(rng) };
            p.vel = { dSx(rng), dSy(rng) };
            p.radius = dR(rng);
            p.life = dL(rng);
            p.maxLife = p.life;
            p.alpha = 0.5f;
            p.twinkleSpeed = dTw(rng);
            p.color = palette[dC(rng)];
            m_sparks.push_back(p);
        }
    }

    void updateSparks(float dt)
    {
        float w = static_cast<float>(m_windowSize.x);
        float h = static_cast<float>(m_windowSize.y);

        for (auto& p : m_sparks)
        {
            p.pos += p.vel * dt;
            p.life -= dt;

            float twinkle = std::sin(m_time * p.twinkleSpeed) * 0.5f + 0.5f;
            p.alpha = 0.2f + 0.7f * twinkle;

            if (p.pos.y < -10.f)
            {
                p.pos.y = h + 10.f;
                p.life = p.maxLife;
            }
            if (p.pos.x < -10.f)    p.pos.x = w + 10.f;
            if (p.pos.x > w + 10.f) p.pos.x = -10.f;
        }
    }

    void drawSparks(sf::RenderTarget& target) const
    {
        for (const auto& p : m_sparks)
        {
            float lifeRatio = std::clamp(p.life / p.maxLife, 0.f, 1.f);
            float a = p.alpha * lifeRatio * m_fadeIn;

            if (p.radius > 1.5f)
            {
                float len = p.radius * 2.5f;
                float thin = 0.8f;
                uint8_t sa = static_cast<uint8_t>(
                    std::clamp(a * 200.f, 0.f, 255.f));

                sf::RectangleShape vLine{ sf::Vector2f{ thin, len } };
                vLine.setOrigin({ thin / 2.f, len / 2.f });
                vLine.setPosition(p.pos);
                vLine.setFillColor({ p.color.r, p.color.g, p.color.b, sa });
                target.draw(vLine);

                sf::RectangleShape hLine{ sf::Vector2f{ len, thin } };
                hLine.setOrigin({ len / 2.f, thin / 2.f });
                hLine.setPosition(p.pos);
                hLine.setFillColor({ p.color.r, p.color.g, p.color.b, sa });
                target.draw(hLine);

                sf::CircleShape core(p.radius * 0.6f);
                core.setOrigin({ p.radius * 0.6f, p.radius * 0.6f });
                core.setPosition(p.pos);
                core.setFillColor({ 255, 255, 220,
                    static_cast<uint8_t>(std::clamp(a * 255.f, 0.f, 255.f)) });
                target.draw(core);
            }
            else
            {
                sf::CircleShape dot(p.radius);
                dot.setOrigin({ p.radius, p.radius });
                dot.setPosition(p.pos);
                dot.setFillColor({ p.color.r, p.color.g, p.color.b,
                    static_cast<uint8_t>(std::clamp(a * 255.f, 0.f, 255.f)) });
                target.draw(dot);
            }
        }
    }

    const sf::Font& m_font;
    sf::Vector2u                   m_windowSize;
    sf::Text                       m_title;
    sf::Text                       m_subtitle;
    sf::Text                       m_continueText;
    sf::Text                       m_menuText;
    sf::Text                       m_hint;
    sf::RectangleShape             m_line;
    sf::RectangleShape             m_line2;
    sf::RectangleShape             m_continueBox;
    sf::RectangleShape             m_menuBox;
    std::vector<sf::Text>          m_titleGlow;
    std::vector<SparkParticle>     m_sparks;
    std::vector<Firework>          m_fireworks;
    std::mt19937                   m_rng;
    float                          m_time = 0.f;
    float                          m_fadeIn = 0.f;
    float                          m_nextFireworkTimer = 0.3f;
    int                            m_selected = 0;
    static constexpr int           m_glowLayers = 5;
};