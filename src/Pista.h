#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Pista {
public:
    std::vector<sf::Vector2f> m_waypoints;
    sf::VertexArray m_visualTrack;

    sf::Texture m_backgroundTexture;
    sf::Sprite m_backgroundSprite;

    Pista();
    void Desenhar(sf::RenderWindow& window);
};