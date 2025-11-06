#include "AlgoritmoEvolutivo.h"
#include "Config.h"
#include <cstdlib> 
#include <algorithm> 
#include <iostream>
#include <cmath>

// Função auxiliar para ordenar carros por fitness
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

    // Limpa os históricos 
    historico_fitness.clear();
    historico_tempo_melhor.clear();
    
    // Reseta os contadores
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
}

void AlgoritmoEvolutivo::Avaliar() {
    // Calcula o fitness da população atual.
    double melhor_fitness_geracao = -1.0;
    for (const auto& carro : populacao) {
        if (carro.fitness > melhor_fitness_geracao) {
            melhor_fitness_geracao = carro.fitness;
        }
    }
}

// Magnitude: 1.0 = normal, 0.1 = pequena, 5.0 = grande
void AlgoritmoEvolutivo::Mutar(Carro& individuo, double magnitude) {

    // mutação da potência do motor - a média da f1 é 1050 cavalos e da f2 é 620
    double range_potencia = 1050.0 - 620.0;
    double variacao = ValorAleatorio(-range_potencia, range_potencia) * 0.1 * magnitude;
    individuo.genoma.potencia_motor += variacao;
    
    // Garante limites
    if (individuo.genoma.potencia_motor < 620.0) individuo.genoma.potencia_motor = 620.0;
    if (individuo.genoma.potencia_motor > 1050.0) individuo.genoma.potencia_motor = 1050.0;
    
    // mutação do tamanho do tanque - por regras, pode até ~130 litros na f1 e 65 litros na f2
    double range_tanque = 130.0 - 54.0;
    double variacao_tanque = ValorAleatorio(-range_tanque, range_tanque) * 0.1 * magnitude;
    individuo.genoma.tamanho_tanque += variacao_tanque;

    if (individuo.genoma.tamanho_tanque < 54.0) individuo.genoma.tamanho_tanque = 54.0;
    if (individuo.genoma.tamanho_tanque > 130.0) individuo.genoma.tamanho_tanque = 130.0;
    

    // mutação do peso do piloto - o piloto mais leve da f1 de 2024 tinha 54 kg e o mais pesado tinha 78
    double range_piloto = 78.0 - 54.0;
    double variacao_piloto = ValorAleatorio(-range_piloto, range_piloto) * 0.1 * magnitude;
    individuo.genoma.peso_piloto += variacao_piloto;
    
    if (individuo.genoma.peso_piloto < 54.0) individuo.genoma.peso_piloto = 54.0;
    if (individuo.genoma.peso_piloto > 78.0) individuo.genoma.peso_piloto = 78.0;
}

void AlgoritmoEvolutivo::SelecaoCrossoverMutacao() {
    
    // 1. Ordena a população
    std::sort(populacao.begin(), populacao.end(), compararCarros);

    // 2. Atualiza o melhor de todos e verifica estagnação
    Carro& novo_vencedor = populacao[0];
    
    // Compara o fitness do novo vencedor com o da geração passada
    if (std::abs(novo_vencedor.fitness - m_melhorFitnessAnterior) < 1e-5 && novo_vencedor.fitness > 0) {
        m_streakDoMelhor++;
    } else {
        m_streakDoMelhor = 1; // Reset se o fitness for diferente
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

    // --- HIPERMUTAÇÃO NO VENCEDOR (Sua ideia!) ---
    if (m_streakDoMelhor > 2) {
        // Magnitude exponencial: 1.5^(1), 1.5^(2), ...
        double magnitude_hyper = pow(1.5, m_streakDoMelhor - 2); 
        std::cout << "Hipermutacao no vencedor! Nivel: " << magnitude_hyper << std::endl;
        Mutar(nova_populacao[0], magnitude_hyper);
    }

    // 5. Pega os pais (Top 2 ao 5)
    std::vector<Carro> pais;
    for (int i = 1; i <= 4; ++i) {
        pais.push_back(populacao[i]);
    }
    
    // 6. Aplica MUTAÇÃO PEQUENA nos pais (para explorar)
    double magnitude_pequena = 0.1; 
    for (int i = 0; i < pais.size(); ++i) {
        Mutar(pais[i], magnitude_pequena);
        nova_populacao.push_back(pais[i]); // Adiciona os pais mutados
    }
    
    // 7. Cria os 15 filhos restantes
    double magnitude_grande_filhos = 2.0; // Mutação padrão para novos filhos
    for (int i = 5; i < TAMANHO_POPULACAO; ++i) {
        Carro& pai1 = pais[rand() % pais.size()];
        Carro& pai2 = pais[rand() % pais.size()];
        
        Carro filho;

        // --- CROSSOVER PONDERADO ---
        double peso1 = pai1.fitness;
        double peso2 = pai2.fitness;
        double peso_total = peso1 + peso2;
        if (peso_total < 1e-5) { peso1 = 1.0; peso2 = 1.0; peso_total = 2.0; }

        // Genes do tipo double (média ponderada)
        filho.genoma.potencia_motor = (pai1.genoma.potencia_motor * peso1 + pai2.genoma.potencia_motor * peso2) / peso_total;
        filho.genoma.peso_piloto = (pai1.genoma.peso_piloto * peso1 + pai2.genoma.peso_piloto * peso2) / peso_total;
        filho.genoma.tamanho_tanque = (pai1.genoma.tamanho_tanque * peso1 + pai2.genoma.tamanho_tanque * peso2) / peso_total;
        filho.genoma.estrategia_pitstop_pneu = (pai1.genoma.estrategia_pitstop_pneu * peso1 + pai2.genoma.estrategia_pitstop_pneu * peso2) / peso_total;
        filho.genoma.estrategia_pitstop_combustivel = (pai1.genoma.estrategia_pitstop_combustivel * peso1 + pai2.genoma.estrategia_pitstop_combustivel * peso2) / peso_total;

        // --- CORREÇÃO DO BUG ---
        // Genes do tipo int (sorteio ponderado)
        if (ValorAleatorio(0.0, peso_total) < peso1) {
            filho.genoma.tipo_pneu_inicial = pai1.genoma.tipo_pneu_inicial;
        } else {
            filho.genoma.tipo_pneu_inicial = pai2.genoma.tipo_pneu_inicial;
        }

        // 7. Aplica MUTAÇÃO GRANDE nos filhos
        Mutar(filho, magnitude_grande_filhos);
        
        nova_populacao.push_back(filho);
    }
    
    populacao = nova_populacao; // Substitui a população antiga
}

void AlgoritmoEvolutivo::ProximaGeracao() {
    Avaliar();                 // 1. Calcula o fitness de todos
    SelecaoCrossoverMutacao(); // 2. Encontra o novo 'melhor_de_todos'
    geracao_atual++;

    // 3. Salva o melhor fitness DEPOIS da seleção,
    // garantindo que o gráfico só suba ou se mantenha.
    if (melhor_de_todos.fitness > 0) {
        historico_fitness.push_back(melhor_de_todos.fitness);
    }

    // Salva o tempo do melhor, se ele terminou a corrida
    if (melhor_de_todos.terminou_corrida) {
        historico_tempo_melhor.push_back(melhor_de_todos.tempo_de_corrida);
    } else {
        // Se ele não terminou, adiciona um tempo "punitivo" para o gráfico não ficar estranho
        historico_tempo_melhor.push_back(50.0);
    }
}