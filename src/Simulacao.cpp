#include "Simulacao.h"
#include "Config.h"
#include <cmath>

Clima Simulacao::s_climaEscolhido = Clima::SOL;

Simulacao::Simulacao(std::vector<Carro>* pop) : populacao(pop) {
    rodando = false;
    corrida_terminou = true; 
    m_primeiroTerminou = false;
    m_cronometroFimCorrida = 10.0f;
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
        m_tempoTrocaClima = -1.f;
    }

    for (int i = 0; i < populacao->size(); ++i) {
        (*populacao)[i].ResetarEstado(m_pista.m_waypoints[0]); 
    }
    rodando = true;
    corrida_terminou = false;

    m_primeiroTerminou = false;
    m_carrosTerminaramCount = 0;
    m_cronometroFimCorrida = 10.0f;
}

void Simulacao::Atualizar(float dt) {
    if (!rodando || corrida_terminou) return;

    // --- 1. LÓGICA DE CLIMA DINÂMICO ---
    if (m_tempoTrocaClima > 0) {
        m_tempoTrocaClima -= dt;
        if (m_tempoTrocaClima <= 0) {
            // Inverte o clima
            m_climaDaCorrida = (m_climaDaCorrida == Clima::SOL) ? Clima::CHUVA : Clima::SOL;
            // Define o próximo tempo de troca (entre 10s e 20s)
            m_tempoTrocaClima = 10.0f + (rand() % 10);

            // Mutação em tempo real (Pit Stop forçado)
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

    // --- 2. ATUALIZAR CARROS E CONTAR TERMINADOS ---
    bool todos_terminaram = true;
    int contagem_terminados_total = 0; // Conta quantos já acabaram

    for (auto& carro : *populacao) {
        if (!carro.terminou_corrida) { 
            carro.Atualizar(dt, m_pista, m_climaDaCorrida);
            
            // Verifica de novo se terminou após a atualização
            if (!carro.terminou_corrida) { 
                todos_terminaram = false;
            }
        }
        
        // Se o carro já terminou (neste frame ou antes), conta ele
        if (carro.terminou_corrida) {
            contagem_terminados_total++;
        }
    }
    
    // Atualiza a variável da classe
    m_carrosTerminaramCount = contagem_terminados_total;

    // --- 3. LÓGICA DO CRONÔMETRO (TOP 5) ---
    // Se 5 ou mais carros terminaram E o cronômetro ainda não foi ativado
    if (contagem_terminados_total >= 5 && !m_primeiroTerminou) {
        m_primeiroTerminou = true; // Ativa o gatilho do cronômetro
        m_cronometroFimCorrida = 10.0f;
    }

    // Se o gatilho está ativado, faz a contagem regressiva
    if (m_primeiroTerminou) {
        m_cronometroFimCorrida -= dt;
    }

    // --- 4. VERIFICA FIM DA CORRIDA ---
    // A corrida acaba se todos chegarem OU se o tempo de tolerância acabar
    if (todos_terminaram || (m_primeiroTerminou && m_cronometroFimCorrida <= 0)) {
        rodando = false;
        corrida_terminou = true;

    }
}

void Simulacao::PararCorrida() {
    rodando = false;
    corrida_terminou = true;

    m_primeiroTerminou = false;
    m_cronometroFimCorrida = 5.0f;

    m_carrosTerminaramCount = 0;
    // Reseta os carros para a posição inicial
    for (auto& carro : *populacao) {
        carro.ResetarEstado(m_pista.m_waypoints[0]);
    }
}