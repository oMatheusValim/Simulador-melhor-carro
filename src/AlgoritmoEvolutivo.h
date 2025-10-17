#pragma once
#include <vector>
#include "Carro.h"

class AlgoritmoEvolutivo {
public:
    std::vector<Carro> populacao;
    int geracao_atual = 0;
    Carro melhor_de_todos;
    std::vector<double> historico_fitness;

    AlgoritmoEvolutivo();
    void Iniciar();
    void ProximaGeracao();

private:
    void Avaliar();
    void SelecaoCrossoverMutacao();
    double ValorAleatorio(double min, double max);
};