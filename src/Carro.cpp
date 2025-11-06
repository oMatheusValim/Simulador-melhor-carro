#include "Carro.h"
#include "Config.h"
#include <cmath> // Para atan2 e sqrt

Carro::Carro() {
    shape.setSize(sf::Vector2f(20.0f, 10.0f));
    shape.setOrigin(10.0f, 5.0f); // Define a origem no centro do carro
    cor = sf::Color(rand() % 255, rand() % 255, rand() % 255);
    shape.setFillColor(cor);
    ResetarEstado({100.0f, 100.0f});
}

void Carro::ResetarEstado(sf::Vector2f pos_inicial) {
    posicao = pos_inicial;
    velocidade = 0.0f;
    combustivel_atual = genoma.tamanho_tanque;
    desgaste_pneu_atual = 0.0;
    fitness = 0.0;
    batidas = 0;
    tempo_de_corrida = 0.0;
    volta_atual = 0;
    terminou_corrida = false;
    m_targetWaypointIndex = 1; 
    shape.setRotation(0.f);
}

void Carro::Atualizar(float dt, const Pista& pista, Clima clima_atual) { 
    if (terminou_corrida) return;

    // --- LÓGICA DE DIREÇÃO ---
    sf::Vector2f target = pista.m_waypoints[m_targetWaypointIndex];
    sf::Vector2f direcao = target - posicao;
    float distancia = std::sqrt(direcao.x * direcao.x + direcao.y * direcao.y);

    // Se chegou perto do waypoint, mira o próximo
    if (distancia < 25.0f) { // 25 pixels de "acerto"
        m_targetWaypointIndex++;
        if (m_targetWaypointIndex >= pista.m_waypoints.size()) {
            m_targetWaypointIndex = 0;
            volta_atual++;
            if (volta_atual >= NUM_VOLTAS) {
                terminou_corrida = true;

                // O fitness é calculado NO MOMENTO em que o carro termina.
                fitness = (1.0 / tempo_de_corrida) * 1000.0; 
                fitness -= batidas * PENALIDADE_BATIDA;
                if (fitness < 0) fitness = 0;
                return;
            }
        }
    }

    // --- FÍSICA SIMPLIFICADA ---
    float atrito_pneu = 1.0f;
    int pneu = genoma.tipo_pneu_inicial; // 1=Sol, 2=Chuva, 3=Híbrido

    if (clima_atual == Clima::CHUVA) {
        if (pneu == 1) atrito_pneu = 0.3f; // Pneu de Sol na Chuva (Ruim)
        if (pneu == 2) atrito_pneu = 1.0f; // Pneu de Chuva na Chuva (Ótimo)
        if (pneu == 3) atrito_pneu = 0.8f; // Pneu Híbrido na Chuva (Bom)
    } else { // Clima é SOL
        if (pneu == 1) atrito_pneu = 1.0f; // Pneu de Sol no Sol (Ótimo)
        if (pneu == 2) atrito_pneu = 0.6f; // Pneu de Chuva no Sol (Ruim, gasta mais)
        if (pneu == 3) atrito_pneu = 0.8f; // Pneu Híbrido no Sol (Bom)
    }

    // Aceleração
    double aceleracao = genoma.potencia_motor / (genoma.peso_piloto + combustivel_atual);
    velocidade += aceleracao * dt * 5.0 * atrito_pneu; 
    
    // Limita a velocidade máxima
    float vel_max_base = 200.f;
    if (velocidade > vel_max_base * atrito_pneu) velocidade = vel_max_base * atrito_pneu;

    // Normaliza a direção
    direcao /= distancia;
    
    // Atualiza a posição
    posicao += direcao * velocidade * dt;

    // Gira o carro para olhar para a direção do movimento
    float angulo = atan2(direcao.y, direcao.x) * 180.f / 3.14159f;
    shape.setRotation(angulo);
    
    // Consumir combustível, etc.
    combustivel_atual -= genoma.potencia_motor * 0.001 * dt;
    if (combustivel_atual <= 0) terminou_corrida = true; // Morreu

    tempo_de_corrida += dt;
    shape.setPosition(posicao);
}

void Carro::Desenhar(sf::RenderWindow& window) {
    window.draw(shape);
}