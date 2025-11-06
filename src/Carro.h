#pragma once
#include "Genoma.h"
#include <SFML/Graphics.hpp>
#include "Pista.h"
#include "Config.h"

class Carro {
public:
    Genoma genoma;

    // Estado na simulação
    sf::Vector2f posicao;
    float velocidade;
    double combustivel_atual;
    double desgaste_pneu_atual;
    double fitness;
    int batidas;
    double tempo_de_corrida;
    int volta_atual;
    bool terminou_corrida;
    int m_targetWaypointIndex;

    // Para visualização
    sf::RectangleShape shape;
    sf::Color cor;

    Carro();
    void ResetarEstado(sf::Vector2f pos_inicial);
    void Atualizar(float dt, const Pista& pista, Clima clima_atual); 
    void Desenhar(sf::RenderWindow& window);
};