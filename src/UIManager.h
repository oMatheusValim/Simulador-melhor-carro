#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Config.h"
#include "Carro.h"

class UIManager {
public:
    UIManager(float largura_painel);
    void Inicializar();
    void Atualizar(const std::vector<double>& historico_fitness, 
                   const std::vector<double>& historico_tempo, 
                   const Carro& melhor_carro, 
                   const std::vector<Carro>& populacao,
                   bool simulacao_rodando);
    void TratarEvento(const sf::Event& event, bool& sim_deve_alternar, bool& sim_deve_reiniciar);
    void Desenhar(sf::RenderWindow& window);

private:
    sf::RectangleShape m_painelFundo;
    sf::Font m_fonte;
    float m_largura;

    // Botões
    sf::RectangleShape m_botaoRun;
    sf::Text m_textoBotao;
    sf::RectangleShape m_botaoReiniciar;
    sf::Text m_textoReiniciar; 
    sf::RectangleShape m_botaoSol;
    sf::Text m_textoSol;
    sf::RectangleShape m_botaoChuva;
    sf::Text m_textoChuva;
    sf::RectangleShape m_botaoAleatorio;
    sf::Text m_textoAleatorio;

    // Stats
    sf::Text m_textoMelhorFitness;
    sf::Text m_placarTitulo;
    sf::Text m_placarTexto;

    // Gráfico de Fitness
    sf::VertexArray m_graficoFitness;
    sf::RectangleShape m_bordaGraficoFitness;
    sf::Text m_graficoFitnessLabelY_Max;
    sf::Text m_graficoFitnessLabelY_Min;
    sf::Text m_graficoFitnessLabelX_Gen;

    // Gráfico de Tempo
    sf::VertexArray m_graficoTempo;
    sf::RectangleShape m_bordaGraficoTempo;
    sf::Text m_graficoTempoLabelY_Max;
    sf::Text m_graficoTempoLabelY_Min;
    sf::Text m_graficoTempoLabelX_Gen;
    sf::Text m_graficoTempoTitulo;
    
    // Genes do Melhor
    sf::Text m_genesTitulo;
    sf::Text m_genesTexto;
};