#include "Simulacao.h"
#include "Config.h"

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
        }
    }

    bool todos_terminaram = true;
    for (auto& carro : *populacao) {
        carro.Atualizar(dt, m_pista, m_climaDaCorrida);
        if (!carro.terminou_corrida) {
            todos_terminaram = false;
        }
    }
    
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