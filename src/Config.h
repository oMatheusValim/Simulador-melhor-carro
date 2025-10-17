#pragma once

// window configuration
const int LARGURA_JANELA = 1600; // window_wight
const int ALTURA_JANELA = 900; // window_height
const char* TITULO_JANELA = "Simulador Evolutivo F1"; //window_title

// Configurações do Algoritmo Evolutivo - evolutive algoritm configuration
const int TAMANHO_POPULACAO = 20; //population_size
const double TAXA_MUTACAO = 0.05; // 5% mutation_rate

// Configurações da Simulação - simulation configuration
const int NUM_VOLTAS = 3; // number_laps
const double PENALIDADE_BATIDA = 50.0; // penality_hit
const double TEMPO_PIT_STOP = 5.0; // seconds pitstop_time