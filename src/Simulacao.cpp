#include "Simulacao.h"
#include "Config.h"

Simulacao::Simulacao(std::vector<Carro>* pop) : populacao(pop) {}

void Simulacao::IniciarCorrida() {
    for (int i = 0; i < populacao->size(); ++i) {
        // Espalha os carros na linha de largada
        (*populacao)[i].ResetarEstado({100.0f, 100.0f + i * 20.0f});
    }
    rodando = true;
    corrida_terminou = false;
}

void Simulacao::Atualizar(float dt) {
    if (!rodando || corrida_terminou) return;

    bool todos_terminaram = true;
    for (auto& carro : *populacao) {
        carro.Atualizar(dt);
        if (!carro.terminou_corrida) {
            todos_terminaram = false;
        }
    }
    
    if (todos_terminaram) {
        rodando = false;
        corrida_terminou = true;
        CalcularFitnessFinal();
    }
}

void Simulacao::CalcularFitnessFinal() {
     for (auto& carro : *populacao) {
        // Fórmula de Fitness
        carro.fitness = (1.0 / carro.tempo_de_corrida) * 1000.0;
        carro.fitness -= carro.batidas * PENALIDADE_BATIDA;
        if (carro.fitness < 0) carro.fitness = 0;
    }
}