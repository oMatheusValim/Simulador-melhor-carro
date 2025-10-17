#pragma once
#include "Genoma.h"
#include <SFML/Graphics.hpp>

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

    // Para visualização
    sf::RectangleShape shape;
    sf::Color cor;

    Carro();
    void ResetarEstado(sf::Vector2f pos_inicial);
    void Atualizar(float dt); // dt = delta time
    void Desenhar(sf::RenderWindow& window);
};