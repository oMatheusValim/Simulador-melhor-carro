#include "Carro.h"
#include "Config.h"
#include <cmath> 

double Carro::ValorAleatorio(double min, double max) {
    return min + static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (max - min)));
}


// Magnitude: 1.0 = normal, 0.1 = pequena, 5.0 = grande
void Carro::Mutar(double magnitude) {
    
    // Se a magnitude for -1, é um "pit stop" forçado
    if (magnitude == -1) {
        // Troca o pneu atual
        if (genoma.tipo_pneu_inicial == 1) { // Era Sol
            genoma.tipo_pneu_inicial = 2; // Vira Chuva
        } else if (genoma.tipo_pneu_inicial == 2) { // Era Chuva
            genoma.tipo_pneu_inicial = 1; // Vira Sol
        }
        
        // penalidade de tempo pelo pit stop
        tempo_de_corrida += 3.0; 
        return; 
    }

    // mutação da potência do motor
    double range_potencia = 1050.0 - 620.0;
    double variacao = ValorAleatorio(-range_potencia, range_potencia) * 0.1 * magnitude; 
    genoma.potencia_motor += variacao; 
    if (genoma.potencia_motor < 620.0) genoma.potencia_motor = 620.0;
    if (genoma.potencia_motor > 1050.0) genoma.potencia_motor = 1050.0;

    // mutação do tamanho do tanque
    double range_tanque = 130.0 - 54.0;
    double variacao_tanque = ValorAleatorio(-range_tanque, range_tanque) * 0.1 * magnitude;
    genoma.tamanho_tanque += variacao_tanque;
    if (genoma.tamanho_tanque < 54.0) genoma.tamanho_tanque = 54.0;
    if (genoma.tamanho_tanque > 130.0) genoma.tamanho_tanque = 130.0;

    // mutação do peso do piloto
    double range_piloto = 78.0 - 54.0;
    double variacao_piloto = ValorAleatorio(-range_piloto, range_piloto) * 0.1 * magnitude;
    genoma.peso_piloto += variacao_piloto; 
    if (genoma.peso_piloto < 54.0) genoma.peso_piloto = 54.0;
    if (genoma.peso_piloto > 78.0) genoma.peso_piloto = 78.0;

    // mutação da estratégia de combustível
    double range_estrategia_combustivel = 1.0 - 0.0;
    double variacao_estrategia_combustivel = ValorAleatorio(-range_estrategia_combustivel, range_estrategia_combustivel) * 0.1 * magnitude;
    genoma.estrategia_pitstop_combustivel += variacao_estrategia_combustivel; 
    if (genoma.estrategia_pitstop_combustivel < 0.0) genoma.estrategia_pitstop_combustivel = 0.0;
    if (genoma.estrategia_pitstop_combustivel > 1.0) genoma.estrategia_pitstop_combustivel = 1.0;

    // mutação da estragégia de desgaste do pneu
    double range_desgaste_pneu = 1.0 - 0.0;
    double variacao_desgaste_pneu= ValorAleatorio(-range_desgaste_pneu, range_desgaste_pneu) * 0.1 * magnitude;
    genoma.estrategia_pitstop_pneu+= variacao_desgaste_pneu; 
    if (genoma.estrategia_pitstop_pneu < 0.0) genoma.estrategia_pitstop_pneu = 0.0;
    if (genoma.estrategia_pitstop_pneu > 1.0) genoma.estrategia_pitstop_pneu = 1.0;
}

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
    m_targetWaypointIndex = 1; 
    shape.setRotation(0.f);

    m_state = CarState::RACING;
    m_pitStopTimer = 0.f;
}

void Carro::Atualizar(float dt, const Pista& pista, Clima clima_atual) { 
    if (terminou_corrida) return;
    
    /// Lógica da máquina de estados

    // 1. parando no pitstop
    if (m_state == CarState::PITTING) {
        tempo_de_corrida += dt; 
        m_pitStopTimer -= dt;   

        if (m_pitStopTimer <= 0) {
            combustivel_atual = genoma.tamanho_tanque; 
            desgaste_pneu_atual = 0.0; 
            m_state = CarState::RACING;

            int escolha = rand() % 100;
            
            if (clima_atual == Clima::CHUVA) {
                // Na chuva: 70% chance de Pneu Chuva (2), 30% Híbrido (3)
                if (escolha < 70) genoma.tipo_pneu_inicial = 2;
                else genoma.tipo_pneu_inicial = 3;
            } else {
                // No sol: 70% chance de Pneu Sol (1), 30% Híbrido (3)
                if (escolha < 70) genoma.tipo_pneu_inicial = 1;
                else genoma.tipo_pneu_inicial = 3;
            }

            m_state = CarState::RACING;
            
            // Põe o carro fisicamente no waypoint de merge (Índice 1)
            posicao = pista.m_waypoints[1]; 
            // Define o próximo alvo como o waypoint DEPOIS do merge (Índice 2)
            m_targetWaypointIndex = 2; 
        }
        return;
    }

    // Lógica de física e desgaste (calculada para todos os estados de movimento) 
    float atrito_pneu = 1.0f;
    int pneu = genoma.tipo_pneu_inicial; 
    if (clima_atual == Clima::CHUVA) {
        if (pneu == 1) atrito_pneu = 0.3f; 
        if (pneu == 2) atrito_pneu = 1.0f; 
        if (pneu == 3) atrito_pneu = 0.8f; 
    } else { 
        if (pneu == 1) atrito_pneu = 1.0f; 
        if (pneu == 2) atrito_pneu = 0.6f; 
        if (pneu == 3) atrito_pneu = 0.8f; 
    }
    desgaste_pneu_atual += (1.0 / atrito_pneu) * 0.005 * dt;

    // lógica de movimento
    sf::Vector2f target;
    
    // Define o alvo com base no estado
    if (m_state == CarState::RACING) {
        target = pista.m_waypoints[m_targetWaypointIndex];
    } else if (m_state == CarState::ENTERING_PIT) {
        target = pista.m_pitWaypoints[m_targetWaypointIndex];
    }
    
    sf::Vector2f direcao = target - posicao;
    float distancia = std::sqrt(direcao.x * direcao.x + direcao.y * direcao.y);

    // lógica de navegação
    if (distancia < 25.0f) { // Se chegou no waypoint alvo
        m_targetWaypointIndex++; // Mira o próximo
        
        if (m_state == CarState::RACING) {
            if (m_targetWaypointIndex >= pista.m_waypoints.size()) {
                m_targetWaypointIndex = 0; 
                volta_atual++;
                if (volta_atual >= NUM_VOLTAS) {
                    terminou_corrida = true;
                    // Cálculo do Fitness
                    double fitness_tempo = (1.0 / tempo_de_corrida) * 1000.0;
                    double fitness_combustivel = (combustivel_atual / genoma.tamanho_tanque) * 100.0;
                    fitness = fitness_tempo + fitness_combustivel;
                    if (fitness < 0) fitness = 0;
                    return;
                }
            }

            const int PIT_ENTRANCE_INDEX = 36; 

            if (m_targetWaypointIndex == PIT_ENTRANCE_INDEX) {
                if (volta_atual < NUM_VOLTAS - 1) 
                { 
                    // Checa os "limiares"
                    bool needsFuel = (combustivel_atual / genoma.tamanho_tanque) < genoma.estrategia_pitstop_combustivel;
                    bool needsTires = (desgaste_pneu_atual > genoma.estrategia_pitstop_pneu);

                    // Decide entrar no pit
                    if ((needsFuel || needsTires) && genoma.tipo_pneu_inicial != 3) {
                        m_state = CarState::ENTERING_PIT; 
                        m_targetWaypointIndex = 0; 
                    }
                }
            }
        
        // entrando no pit 
        } else if (m_state == CarState::ENTERING_PIT) {
            if (m_targetWaypointIndex >= pista.m_pitWaypoints.size()) {
                // Chegou na garagem (último waypoint do pit)
                m_state = CarState::PITTING;
                m_pitStopTimer = 2.0f; // 2.5 segundos de parada
                velocidade = 0.f;
                return;
            }
        }
    }

    /// física com resistência do ar
    
    // Calcula a Força de Aceleração
    double forca_motor = (genoma.potencia_motor / (genoma.peso_piloto + combustivel_atual)) * 200.0;

    // combina o atrito do tipo de pneu com o estado de desgaste
    double fator_tracao = atrito_pneu * (1.0 - desgaste_pneu_atual);

    // Calcula a Resistência do Ar (Drag)
    double resistencia_ar = (velocidade * velocidade) * 0.005;
    
    // Aplica a Resultante - a força do motor é multiplicada pela tração que o pneu consegue aplicar no chão
    velocidade += (forca_motor * fator_tracao - resistencia_ar) * dt;

    if (velocidade < 0) velocidade = 0;

    direcao /= distancia;
    posicao += direcao * velocidade * dt;
    float angulo = atan2(direcao.y, direcao.x) * 180.f / 3.14159f;
    shape.setRotation(angulo);
    
    // Consumir combustível (Potência alta gasta mais)
    combustivel_atual -= genoma.potencia_motor * 0.001 * dt;
    
    if (combustivel_atual <= 0 || desgaste_pneu_atual >= 1.0) {
        terminou_corrida = true; 
    }
    tempo_de_corrida += dt;
    shape.setPosition(posicao);
}

void Carro::Desenhar(sf::RenderWindow& window) {
    window.draw(shape);
}