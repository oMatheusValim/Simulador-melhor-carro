#pragma once
#include <vector>
#include "Carro.h"
#include "Pista.h"
#include "Config.h"

class Simulacao {
public:
    std::vector<Carro>* populacao;
    bool rodando = false;
    bool corrida_terminou = false;
    Pista m_pista;

    static Clima s_climaEscolhido;
    Clima m_climaDaCorrida;
    float m_tempoTrocaClima;

    Simulacao(std::vector<Carro>* pop);
    void IniciarCorrida();
    void Atualizar(float dt);
    void PararCorrida();
    void CalcularFitnessFinal();
};