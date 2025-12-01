#include "AlgoritmoEvolutivo.h"
#include "Config.h"
#include <cstdlib> 
#include <algorithm> 
#include <iostream>
#include <cmath>

bool compararCarros(const Carro& a, const Carro& b) {
    return a.fitness > b.fitness;
}

double AlgoritmoEvolutivo::ValorAleatorio(double min, double max) {
    return min + static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (max - min)));
}

AlgoritmoEvolutivo::AlgoritmoEvolutivo() {
    srand(time(NULL)); 
    m_streakDoMelhor = 0;
    m_melhorFitnessAnterior = 0.0;
}

void AlgoritmoEvolutivo::Reiniciar(){
    Iniciar();

    historico_fitness.clear();
    historico_tempo_melhor.clear();
    geracao_atual = 1;
    m_streakDoMelhor = 0;
    m_melhorFitnessAnterior = 0.0;
}

void AlgoritmoEvolutivo::Iniciar() {
    geracao_atual = 1;
    populacao.resize(TAMANHO_POPULACAO);
    for (int i = 0; i < TAMANHO_POPULACAO; ++i) {
        populacao[i].genoma.potencia_motor = ValorAleatorio(620.0, 1050.0);
        populacao[i].genoma.tipo_pneu_inicial = rand() % 3 + 1;
        populacao[i].genoma.peso_piloto = ValorAleatorio(54.0, 78.0);
        populacao[i].genoma.tamanho_tanque = ValorAleatorio(54.0, 130.0);
        populacao[i].genoma.estrategia_pitstop_pneu = ValorAleatorio(0.1, 0.5);
        populacao[i].genoma.estrategia_pitstop_combustivel = ValorAleatorio(0.1, 0.3);
    }
    melhor_de_todos = populacao[0]; 
    populacao_anterior = populacao;
}

void AlgoritmoEvolutivo::Avaliar() {
    double melhor_fitness_geracao = -1.0;
    for (const auto& carro : populacao) {
        if (carro.fitness > melhor_fitness_geracao) {
            melhor_fitness_geracao = carro.fitness;
        }
    }
}

void AlgoritmoEvolutivo::SelecaoCrossoverMutacao() {
    // 1. Ordena a população e calcula o fitness médio
    std::sort(populacao.begin(), populacao.end(), compararCarros);
    double soma_fitness = 0;
    for(const auto& c : populacao) {
        soma_fitness += c.fitness;
    }
    double avg_fit = soma_fitness / populacao.size();
    double best_fit = populacao[0].fitness;
    
    // 'delta_normalizado' vai de 0 (estagnado) a 1 (espalhado)
    double delta_normalizado = 0.0;
    if (best_fit > 0) {
        delta_normalizado = (best_fit - avg_fit) / best_fit;
    }
    
    // 2. Verifica estagnação do VENCEDOR - vencer várias vezes consecutivas
    Carro& novo_vencedor = populacao[0];
    if (std::abs(novo_vencedor.fitness - m_melhorFitnessAnterior) < 1e-5 && novo_vencedor.fitness > 0) {
        m_streakDoMelhor++;
    } else {
        m_streakDoMelhor = 1;
    }
    m_melhorFitnessAnterior = novo_vencedor.fitness;

    // Atualiza o campeão global
    if (novo_vencedor.fitness > melhor_de_todos.fitness) {
        melhor_de_todos = novo_vencedor;
    } else {
        populacao[0] = melhor_de_todos; // Garante que o melhor sobreviva
    }

    // 3. Cria a nova população
    std::vector<Carro> nova_populacao;

    // 4. Mantém o Top 1 (Elitismo)
    nova_populacao.push_back(melhor_de_todos);

    // 5. HIPERMUTAÇÃO NO VENCEDOR - se está no topo há mais de 2 gerações, sofrerá mutação
    if (m_streakDoMelhor > 2) {
        double magnitude_hyper = pow(1.5, m_streakDoMelhor - 2); 
        std::cout << "Hipermutacao no vencedor! Streak: " << m_streakDoMelhor << " Nivel: " << magnitude_hyper << std::endl;
        nova_populacao[0].Mutar(magnitude_hyper); // Aplica a mutação no vencedor
    }

    // 6. Pega os pais (Top 2 ao 5)
    std::vector<Carro> pais;
    for (int i = 1; i <= 4; ++i) {
        pais.push_back(populacao[i]);
    }
    
    double magnitude_pequena_pais = 0.1; 
    for (int i = 0; i < pais.size(); ++i) {
        pais[i].Mutar(magnitude_pequena_pais);
        nova_populacao.push_back(pais[i]);
    }
    
    // 7. MUTAÇÃO ADAPTATIVA NOS FILHOS - se a população está estagnada (delta baixo), a mutação é ALTA.
    double magnitude_adaptativa = (1.0 - delta_normalizado) * 5.0 + 1.0; 
    if (magnitude_adaptativa > 10.0) magnitude_adaptativa = 10.0;
    
    // 8. Cria os 15 filhos restantes
    for (int i = 5; i < TAMANHO_POPULACAO; ++i) {
        Carro& pai1 = pais[rand() % pais.size()];
        Carro& pai2 = pais[rand() % pais.size()];
        Carro filho;

        // Crossover Ponderado
        double peso1 = pai1.fitness;
        double peso2 = pai2.fitness;
        double peso_total = peso1 + peso2;
        if (peso_total < 1e-5) { peso1 = 1.0; peso2 = 1.0; peso_total = 2.0; }

        filho.genoma.potencia_motor = (pai1.genoma.potencia_motor * peso1 + pai2.genoma.potencia_motor * peso2) / peso_total;
        filho.genoma.peso_piloto = (pai1.genoma.peso_piloto * peso1 + pai2.genoma.peso_piloto * peso2) / peso_total;
        filho.genoma.tamanho_tanque = (pai1.genoma.tamanho_tanque * peso1 + pai2.genoma.tamanho_tanque * peso2) / peso_total;
        filho.genoma.estrategia_pitstop_pneu = (pai1.genoma.estrategia_pitstop_pneu * peso1 + pai2.genoma.estrategia_pitstop_pneu * peso2) / peso_total;
        filho.genoma.estrategia_pitstop_combustivel = (pai1.genoma.estrategia_pitstop_combustivel * peso1 + pai2.genoma.estrategia_pitstop_combustivel * peso2) / peso_total;

        if (ValorAleatorio(0.0, peso_total) < peso1) {
            filho.genoma.tipo_pneu_inicial = pai1.genoma.tipo_pneu_inicial;
        } else {
            filho.genoma.tipo_pneu_inicial = pai2.genoma.tipo_pneu_inicial;
        }

        // Aplica a mutação adaptativa nos filhos
        filho.Mutar(magnitude_adaptativa);
        
        nova_populacao.push_back(filho);
    }
    
    populacao = nova_populacao;
}

void AlgoritmoEvolutivo::ProximaGeracao() {
    Avaliar();                 
    populacao_anterior = populacao;

    Carro* melhor_da_rodada = &populacao[0];
    for(auto& c : populacao) {
        if(c.fitness > melhor_da_rodada->fitness) {
            melhor_da_rodada = &c;
        }
    }

    SelecaoCrossoverMutacao();

    if (melhor_de_todos.fitness > 0) {
        historico_fitness.push_back(melhor_de_todos.fitness);
    }

    if (melhor_de_todos.terminou_corrida) {
        historico_tempo_melhor.push_back(melhor_de_todos.tempo_de_corrida);
    } else {
        historico_tempo_melhor.push_back(50.0);
    }

    geracao_atual++;
}