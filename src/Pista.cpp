#include "Pista.h"
#include "Config.h" // Para LARGURA_JANELA
#include <iostream>

Pista::Pista() {
    if (!m_backgroundTexture.loadFromFile("assets/img_pista.png")) {
        std::cerr << "Erro: Nao foi possivel carregar a imagem da pista!" << std::endl;
    }
    m_backgroundSprite.setTexture(m_backgroundTexture);
    
    float areaSimulacaoX = LARGURA_JANELA - 300; 
    float escala = areaSimulacaoX / m_backgroundTexture.getSize().x;
    m_backgroundSprite.setScale(escala, escala);

    // Waypoints da pista principal
    m_waypoints.push_back(sf::Vector2f(974.f, 556.f));
    m_waypoints.push_back(sf::Vector2f(530.f, 556.f));
    m_waypoints.push_back(sf::Vector2f(237.f, 554.f));
    m_waypoints.push_back(sf::Vector2f(193.f, 523.f));
    m_waypoints.push_back(sf::Vector2f(190.f, 445.f));
    m_waypoints.push_back(sf::Vector2f(63.f, 400.f));
    m_waypoints.push_back(sf::Vector2f(30.f, 325.f));
    m_waypoints.push_back(sf::Vector2f(57.f, 251.f));
    m_waypoints.push_back(sf::Vector2f(125.f, 213.f));
    m_waypoints.push_back(sf::Vector2f(400.f, 213.f));
    m_waypoints.push_back(sf::Vector2f(422.f, 253.f));
    m_waypoints.push_back(sf::Vector2f(409.f, 295.f));
    m_waypoints.push_back(sf::Vector2f(358.f, 323.f));
    m_waypoints.push_back(sf::Vector2f(285.f, 327.f));
    m_waypoints.push_back(sf::Vector2f(219.f, 327.f));
    m_waypoints.push_back(sf::Vector2f(184.f, 357.f));
    m_waypoints.push_back(sf::Vector2f(194.f, 400.f));
    m_waypoints.push_back(sf::Vector2f(238.f, 429.f));
    m_waypoints.push_back(sf::Vector2f(332.f, 477.f));
    m_waypoints.push_back(sf::Vector2f(469.f, 481.f));
    m_waypoints.push_back(sf::Vector2f(494.f, 460.f));
    m_waypoints.push_back(sf::Vector2f(518.f, 374.f));
    m_waypoints.push_back(sf::Vector2f(642.f, 233.f));
    m_waypoints.push_back(sf::Vector2f(686.f, 233.f));
    m_waypoints.push_back(sf::Vector2f(1053.f, 449.f));
    m_waypoints.push_back(sf::Vector2f(1081.f, 423.f));
    m_waypoints.push_back(sf::Vector2f(1048.f, 349.f));
    m_waypoints.push_back(sf::Vector2f(994.f, 321.f));
    m_waypoints.push_back(sf::Vector2f(965.f, 288.f));
    m_waypoints.push_back(sf::Vector2f(981.f, 247.f));
    m_waypoints.push_back(sf::Vector2f(1017.f, 223.f));
    m_waypoints.push_back(sf::Vector2f(1176.f, 274.f));
    m_waypoints.push_back(sf::Vector2f(1212.f, 340.f));
    m_waypoints.push_back(sf::Vector2f(1207.f, 483.f));
    m_waypoints.push_back(sf::Vector2f(1173.f, 528.f));
    m_waypoints.push_back(sf::Vector2f(1138.f, 548.f));
    m_waypoints.push_back(sf::Vector2f(1038.f, 552.f));
    m_waypoints.push_back(sf::Vector2f(980.f, 553.f));

    // Waypoints do pitstop
    m_pitWaypoints.push_back(sf::Vector2f(1038.f, 552.f)); // mesmo ponto da pista principal, é o ultimo antes de mudar o caminho
    m_pitWaypoints.push_back(sf::Vector2f(1015.f, 522.f));
    m_pitWaypoints.push_back(sf::Vector2f(998.f, 515.f));
    m_pitWaypoints.push_back(sf::Vector2f(991.f, 515.f));
    m_pitWaypoints.push_back(sf::Vector2f(985.f, 508.f));
    m_pitWaypoints.push_back(sf::Vector2f(959.f, 505.f));
    m_pitWaypoints.push_back(sf::Vector2f(931.f, 506.f));
    m_pitWaypoints.push_back(sf::Vector2f(920.f, 506.f));
    m_pitWaypoints.push_back(sf::Vector2f(903.f, 504.f));
    m_pitWaypoints.push_back(sf::Vector2f(845.f, 504.f));
    m_pitWaypoints.push_back(sf::Vector2f(815.f, 505.f));
    m_pitWaypoints.push_back(sf::Vector2f(768.f, 507.f));
    m_pitWaypoints.push_back(sf::Vector2f(730.f, 505.f)); // onde vai parar os 2.5 segundos
    m_pitWaypoints.push_back(sf::Vector2f(675.f, 505.f));
    m_pitWaypoints.push_back(sf::Vector2f(603.f, 506.f));
    m_pitWaypoints.push_back(sf::Vector2f(576.f, 513.f));
    m_pitWaypoints.push_back(sf::Vector2f(568.f, 515.f));
    m_pitWaypoints.push_back(sf::Vector2f(557.f, 520.f));
    m_pitWaypoints.push_back(sf::Vector2f(544.f, 532.f));
    m_pitWaypoints.push_back(sf::Vector2f(537.f, 543.f));
    m_pitWaypoints.push_back(sf::Vector2f(530.f, 556.f)); // mesmo ponto da pista principal, é onde volta à pista principal

    m_visualTrack.setPrimitiveType(sf::LinesStrip);
    for (const auto& point : m_waypoints) {
        m_visualTrack.append(sf::Vertex(point, sf::Color(255, 255, 255, 100)));
    }
    // Fecha o loop visualmente (só se houver pontos)
    if (!m_waypoints.empty()) {
        m_visualTrack.append(sf::Vertex(m_waypoints[0], sf::Color(255, 255, 255, 100)));
    }

    m_visualPitTrack.setPrimitiveType(sf::LinesStrip);
    for (const auto& point : m_pitWaypoints) {
        m_visualPitTrack.append(sf::Vertex(point, sf::Color::Yellow)); // Pit em amarelo
    }
}

void Pista::Desenhar(sf::RenderWindow& window) {
    // Desenha o fundo primeiro
    window.draw(m_backgroundSprite); 
    
    // Desenha a linha de waypoints por cima (para debug)
    window.draw(m_visualTrack); 
}