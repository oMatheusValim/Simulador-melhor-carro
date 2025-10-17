#include "AlgoritmoEvolutivo.h"
#include "Config.h"
#include <cstdlib> // rand
#include <algorithm> // max_element

// Função auxiliar para valores aleatórios
double ValorAleatorio(double min, double max) {
    return min + static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (max - min)));
}

AlgoritmoEvolutivo::AlgoritmoEvolutivo() {
    srand(time(NULL)); // Seed para números aleatórios
}

void AlgoritmoEvolutivo::Iniciar() {
    geracao_atual = 1;
    populacao.resize(TAMANHO_POPULACAO);
    for (int i = 0; i < TAMANHO_POPULACAO; ++i) {
        populacao[i].genoma.potencia_motor = ValorAleatorio(50.0, 100.0);
        populacao[i].genoma.tipo_pneu_inicial = rand() % 3 + 1;
        populacao[i].genoma.peso_piloto = ValorAleatorio(60.0, 90.0);
        populacao[i].genoma.tamanho_tanque = ValorAleatorio(40.0, 100.0);
        populacao[i].genoma.estrategia_pitstop_pneu = ValorAleatorio(0.1, 0.5);
        populacao[i].genoma.estrategia_pitstop_combustivel = ValorAleatorio(0.1, 0.3);
    }
    melhor_de_todos = populacao[0]; // Inicializa com o primeiro
}

void AlgoritmoEvolutivo::Avaliar() {
    // A avaliação é feita pela simulação, que atualiza o fitness de cada carro.
    // Aqui, apenas registramos o melhor da geração atual.
    double melhor_fitness_geracao = -1.0;
    for (const auto& carro : populacao) {
        if (carro.fitness > melhor_fitness_geracao) {
            melhor_fitness_geracao = carro.fitness;
        }
    }
    historico_fitness.push_back(melhor_fitness_geracao);
}

void AlgoritmoEvolutivo::SelecaoCrossoverMutacao() {
    // --- SELEÇÃO (Elitismo) ---
    int max_i = 0;
    for (int i = 1; i < TAMANHO_POPULACAO; ++i) {
        if (populacao[i].fitness > populacao[max_i].fitness) {
            max_i = i;
        }
    }

    if (populacao[max_i].fitness > melhor_de_todos.fitness) {
        melhor_de_todos = populacao[max_i];
    }

    // --- REARRANJO, CROSSOVER E MUTAÇÃO ---
    std::vector<Carro> nova_populacao;
    nova_populacao.push_back(melhor_de_todos); [cite_start]// Regra de Ouro: Nunca perder o melhor! [cite: 27]

    for (int i = 1; i < TAMANHO_POPULACAO; ++i) {
        Carro filho;
        [cite_start]// Crossover por média com o melhor [cite: 41, 143]
        filho.genoma.potencia_motor = (populacao[i].genoma.potencia_motor + melhor_de_todos.genoma.potencia_motor) / 2.0;
        filho.genoma.peso_piloto = (populacao[i].genoma.peso_piloto + melhor_de_todos.genoma.peso_piloto) / 2.0;
        filho.genoma.tamanho_tanque = (populacao[i].genoma.tamanho_tanque + melhor_de_todos.genoma.tamanho_tanque) / 2.0;
        //... outros genes

        [cite_start]// Mutação: modificar um pouco o gene [cite: 69]
        if (ValorAleatorio(0, 1) < TAXA_MUTACAO) {
            double range = 100.0 - 50.0;
            double variacao = ValorAleatorio(-range, range) * 0.1; // 10% da variação total
            filho.genoma.potencia_motor += variacao;
            // Garantir que não saia dos limites
            if (filho.genoma.potencia_motor < 50.0) filho.genoma.potencia_motor = 50.0;
            if (filho.genoma.potencia_motor > 100.0) filho.genoma.potencia_motor = 100.0;
        }
        // ... aplicar mutação para outros genes ...
        
        nova_populacao.push_back(filho);
    }
    populacao = nova_populacao;
}


void AlgoritmoEvolutivo::ProximaGeracao() {
    Avaliar();
    SelecaoCrossoverMutacao();
    geracao_atual++;
}