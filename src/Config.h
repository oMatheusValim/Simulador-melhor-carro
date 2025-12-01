#pragma once

inline const int LARGURA_JANELA = 1600; 
inline const int ALTURA_JANELA = 900; 
inline const char* TITULO_JANELA = "Simulador Evolutivo F1"; 

// Configurações do Algoritmo Evolutivo 
const int TAMANHO_POPULACAO = 20; 
const double TAXA_MUTACAO = 0.15; 
enum class Clima { SOL, CHUVA, ALEATORIO }; 

// Configurações da Simulação 
const int NUM_VOLTAS = 5; 
const double TEMPO_PIT_STOP = 2.5; 