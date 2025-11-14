#include "Simulacao.h"
#include "Config.h"
#include <cmath>

Clima Simulacao::s_climaEscolhido = Clima::SOL;

Simulacao::Simulacao(std::vector<Carro>* pop) : populacao(pop) {
    rodando = false;
    corrida_terminou = true; 
}

void Simulacao::IniciarCorrida() {
    // lógica do clima
    if (s_climaEscolhido == Clima::ALEATORIO) {
        // Começa com Sol ou Chuva aleatoriamente
        m_climaDaCorrida = (rand() % 2 == 0) ? Clima::SOL : Clima::CHUVA;
        // Muda de clima a cada 10-20 segundos
        m_tempoTrocaClima = 10.0f + (rand() % 10);
    } else {
        // Clima fixo
        m_climaDaCorrida = s_climaEscolhido;
        m_tempoTrocaClima = -1.f; // -1 significa que não muda
    }

    for (int i = 0; i < populacao->size(); ++i) {
        (*populacao)[i].ResetarEstado(m_pista.m_waypoints[0]); 
    }
    rodando = true;
    corrida_terminou = false;
}

void Simulacao::Atualizar(float dt) {
    if (!rodando || corrida_terminou) return;

    // --- LÓGICA DE CLIMA DINÂMICO ---
    if (m_tempoTrocaClima > 0) { // Se for -1, nunca muda
        m_tempoTrocaClima -= dt;
        if (m_tempoTrocaClima <= 0) {
            // Inverte o clima
            m_climaDaCorrida = (m_climaDaCorrida == Clima::SOL) ? Clima::CHUVA : Clima::SOL;
            // Define o próximo tempo de troca
            m_tempoTrocaClima = 10.0f + (rand() % 10);

            // mutação em tempo real
            for(auto& carro : *populacao) {
                // Se o carro não é híbrido (3) e não terminou
                if (carro.genoma.tipo_pneu_inicial != 3 && !carro.terminou_corrida) {
                    // 70% de chance de ser forçado a ir para o "pit stop"
                    if(rand() % 10 < 7) {
                        carro.Mutar(-1); // -1 é o código para "pit stop"
                    }
                }
            }
        }
    }

    bool todos_terminaram = true;
    for (auto& carro : *populacao) {
        if (!carro.terminou_corrida) { 
            carro.Atualizar(dt, m_pista, m_climaDaCorrida);
            if (!carro.terminou_corrida) {
                todos_terminaram = false;
            }
        }
    }

    /*// Detecção de colisões
    for (int i = 0; i < populacao->size(); ++i) {
        if ((*populacao)[i].terminou_corrida) continue; // Não checa carros que terminaram

        for (int j = i + 1; j < populacao->size(); ++j) {
            if ((*populacao)[j].terminou_corrida) continue;
            
            // Pega as posições
            sf::Vector2f pos1 = (*populacao)[i].posicao;
            sf::Vector2f pos2 = (*populacao)[j].posicao;
            
            // Calcula a distância
            float dx = pos1.x - pos2.x;
            float dy = pos1.y - pos2.y;
            float distancia = std::sqrt(dx*dx + dy*dy);
            
            // Se a distância for menor que o tamanho do carro (ex: 15 pixels)
            if (distancia < 15.0f) {
                (*populacao)[i].batidas++;
                (*populacao)[j].batidas++;
                
                // Penalidade de velocidade (simples)
                (*populacao)[i].velocidade *= 0.5f;
                (*populacao)[j].velocidade *= 0.5f;
            }
        }
    }*/
    
    if (todos_terminaram) {
        rodando = false;
        corrida_terminou = true;
    }
}

void Simulacao::PararCorrida() {
    rodando = false;
    corrida_terminou = true;

    // Reseta os carros para a posição inicial
    for (auto& carro : *populacao) {
        carro.ResetarEstado(m_pista.m_waypoints[0]);
    }
}