#include <SFML/Graphics.hpp>
#include "Config.h"
#include "AlgoritmoEvolutivo.h"
#include "Simulacao.h"
#include "UIManager.h" 
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(LARGURA_JANELA, ALTURA_JANELA), TITULO_JANELA);
    window.setFramerateLimit(60);

    AlgoritmoEvolutivo ag;
    ag.Iniciar();

    Simulacao sim(&ag.populacao);
    
    UIManager ui(300.0f);
    ui.Inicializar();

    // --- VARIÁVEIS DE CONTROLE ---
    bool sim_deve_alternar_pausa = false; // Gatilho do botão Iniciar/Pausar
    bool sim_deve_reiniciar = false;    // Gatilho do botão Reiniciar
    bool simulacao_automatica = false; // Estado atual (rodando ou pausado)
    bool editor_mode = false; // Para o editor de waypoints
    sf::Clock deltaClock;

    while (window.isOpen()) {
        float dt = deltaClock.restart().asSeconds();
        sf::Event event;

        while (window.pollEvent(event)) {
            // Passa ambos os gatilhos para a UI (por referência)
            ui.TratarEvento(event, sim_deve_alternar_pausa, sim_deve_reiniciar); 

            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // --- LÓGICA DO MODO EDITOR ---
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::E) {
                    editor_mode = !editor_mode;
                    std::cout << "--- MODO EDITOR: " << (editor_mode ? "ATIVADO" : "DESATIVADO") << " ---" << std::endl;
                }
            }
            if (editor_mode && event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    std::cout << "m_waypoints.push_back(sf::Vector2f(" 
                              << event.mouseButton.x << ".f, " 
                              << event.mouseButton.y << ".f));" << std::endl;
                }
            }
            // --- FIM DA LÓGICA DO EDITOR ---
        }

        // --- LÓGICA DE CONTROLE (CORRIGIDA) ---
        
        // 1. Checa por REINÍCIO (prioridade máxima)
        if (sim_deve_reiniciar) {
            ag.Reiniciar();              // Reseta o algoritmo
            sim.PararCorrida();          // Para a corrida atual
            simulacao_automatica = false; // Desliga o modo automático
            sim_deve_reiniciar = false;   // Reseta o gatilho
        }

        // 2. Checa por INICIAR/PAUSAR
        if (sim_deve_alternar_pausa) {
            simulacao_automatica = !simulacao_automatica; // Inverte o estado
            
            // Se ESTAMOS LIGANDO a simulação (e ela não estava rodando)
            if (simulacao_automatica && sim.corrida_terminou) {
                sim.IniciarCorrida();
            }
            sim_deve_alternar_pausa = false; // Reseta o gatilho
        }
        
        // 3. Roda a simulação automática (se estiver ligada)
        if (simulacao_automatica) {
            if (sim.rodando) {
                sim.Atualizar(dt);
            }
            if (sim.corrida_terminou) {
                ag.ProximaGeracao();   
                sim.IniciarCorrida();    
            }
        }
        // --- Fim da Lógica de Controle ---

        // Atualiza a UI (passa o estado da simulação)
        ui.Atualizar(ag.historico_fitness, 
                     ag.historico_tempo_melhor, 
                     ag.melhor_de_todos, 
                     ag.populacao,
                     simulacao_automatica); // Passa o estado atual

        // --- Desenho ---
        window.clear(sf::Color(50, 50, 50));
        sim.m_pista.Desenhar(window); 
        
        // --- CORREÇÃO DE EXIBIÇÃO ---
        // Desenha os carros se a simulação estiver rodando OU pausada
        // (Só não desenha se a simulação não foi iniciada)
        if (simulacao_automatica || sim.rodando) {
            for (auto& carro : ag.populacao) {
                carro.Desenhar(window);
            }
        }
        
        ui.Desenhar(window);
        window.display();
    }
    return 0;
}