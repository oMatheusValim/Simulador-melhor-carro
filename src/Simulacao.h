#pragma once
#include <vector>
#include "Carro.h"

class Simulacao {
public:
    std::vector<Carro>* populacao;
    bool rodando = false;
    bool corrida_terminou = false;

    Simulacao(std::vector<Carro>* pop);
    void IniciarCorrida();
    void Atualizar(float dt);
    void CalcularFitnessFinal();
};