#pragma once
#include <vector>
#include "Carro.h"

class AlgoritmoEvolutivo {
public:
    std::vector<Carro> populacao;
    int geracao_atual = 0;
    Carro melhor_de_todos;
    std::vector<double> historico_fitness;
    std::vector<double> historico_tempo_melhor;

    int m_streakDoMelhor; // Conta quantas gerações o melhor não muda
    double m_melhorFitnessAnterior; // Guarda o fitness da geração passada
    
    AlgoritmoEvolutivo();
    void Reiniciar();
    void Iniciar();
    void ProximaGeracao();

private:
    void Avaliar();
    void SelecaoCrossoverMutacao();
    double ValorAleatorio(double min, double max);
    //void Mutar(Carro& individuo, double magnitude);
};