#pragma once

struct Genoma {
    double potencia_motor = 75.0; // 
    int tipo_pneu_inicial = 1; // 1: Sol, 2: Chuva, 3: Híbrido
    double peso_piloto = 75.0;
    double tamanho_tanque = 70.0;
    double estrategia_pitstop_pneu = 0.4;
    double estrategia_pitstop_combustivel = 0.2;
};