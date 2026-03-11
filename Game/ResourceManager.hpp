#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <stdexcept>

class ResourceManager
{
public:
    static ResourceManager& getInstance()
    {
        static ResourceManager instance;
        return instance;
    }

    // Charge une texture si pas déjà chargée, retourne une référence
    sf::Texture& loadTexture(const std::string& id, const std::string& path)
    {
        auto it = textures.find(id);
        if (it != textures.end())
            return it->second;

        sf::Texture tex(path);
        textures.emplace(id, std::move(tex));
        return textures.at(id);
    }

    sf::Texture& getTexture(const std::string& id)
    {
        auto it = textures.find(id);
        if (it == textures.end())
            throw std::runtime_error("Texture non trouvée : " + id);
        return it->second;
    }

    void clear() { textures.clear(); }

private:
    ResourceManager() = default;
    std::map<std::string, sf::Texture> textures;
};