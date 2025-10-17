#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

#include "Config.h"
#include "AlgoritmoEvolutivo.h"
#include "Simulacao.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(LARGURA_JANELA, ALTURA_JANELA), TITULO_JANELA);
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    AlgoritmoEvolutivo ag;
    ag.Iniciar();

    Simulacao sim(&ag.populacao);

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        float dt = deltaClock.restart().asSeconds();
        ImGui::SFML::Update(window, sf::seconds(dt));

        // --- Interface ImGui ---
        ImGui::Begin("Painel de Controle");
        if (ImGui::Button("Iniciar Corrida / Próxima Geração")) {
            if (sim.corrida_terminou) {
                ag.ProximaGeracao();
            }
            sim.IniciarCorrida();
        }
        ImGui::Text("Geração: %d", ag.geracao_atual);
        ImGui::Text("Melhor Fitness Global: %.2f", ag.melhor_de_todos.fitness);
        
        ImGui::Separator();
        ImGui::Text("Genoma do Melhor:");
        Genoma& melhor_g = ag.melhor_de_todos.genoma;
        ImGui::Text("  Potencia: %.2f", melhor_g.potencia_motor);
        ImGui::Text("  Peso Piloto: %.2f", melhor_g.peso_piloto);
        ImGui::Text("  Tanque: %.2f L", melhor_g.tamanho_tanque);

        // Gráfico simples de fitness
        if (!ag.historico_fitness.empty()) {
             ImGui::PlotLines("Melhor Fitness por Geração", ag.historico_fitness.data(), ag.historico_fitness.size());
        }

        ImGui::End();

        // --- Lógica da Simulação ---
        sim.Atualizar(dt);

        // --- Desenho ---
        window.clear(sf::Color(50, 50, 50)); // Fundo cinza escuro
        for (auto& carro : ag.populacao) {
            carro.Desenhar(window);
        }
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}