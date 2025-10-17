#include "Carro.h"
#include "Config.h"

Carro::Carro() {
    shape.setSize(sf::Vector2f(20.0f, 10.0f));
    shape.setOrigin(10.0f, 5.0f);
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
}

void Carro::Atualizar(float dt) {
    if (terminou_corrida) return;

    // --- FÍSICA SUPER SIMPLIFICADA (AQUI VOCÊ IMPLEMENTA A LÓGICA) ---
    // Aceleração baseada na potência e peso
    double aceleracao = genoma.potencia_motor / (genoma.peso_piloto + combustivel_atual);
    velocidade += aceleracao * dt;

    // Mover para frente
    posicao.x += velocidade * dt;

    // Consumir combustível
    combustivel_atual -= genoma.potencia_motor * 0.001 * dt;

    // Simular fim da volta
    if (posicao.x > LARGURA_JANELA - 200) {
        posicao.x = 100;
        volta_atual++;
    }

    if (volta_atual >= NUM_VOLTAS) {
        terminou_corrida = true;
    }
    
    tempo_de_corrida += dt;
    shape.setPosition(posicao);
}

void Carro::Desenhar(sf::RenderWindow& window) {
    window.draw(shape);
}