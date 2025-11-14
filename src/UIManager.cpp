#include "UIManager.h"
#include "Config.h"
#include "Simulacao.h" 
#include <iostream>
#include <string>
#include <algorithm> 
#include <iomanip> 
#include <sstream> 

bool compararTempoCarros(const Carro& a, const Carro& b) {
    if (a.terminou_corrida && b.terminou_corrida) {
        return a.tempo_de_corrida < b.tempo_de_corrida;
    }
    if (a.terminou_corrida) return true;
    if (b.terminou_corrida) return false;
    return a.fitness > b.fitness;
}

UIManager::UIManager(float largura_painel) : m_largura(largura_painel) {}

void UIManager::Inicializar() {
    if (!m_fonte.loadFromFile("assets/font.ttf")) {
        std::cerr << "Erro ao carregar a fonte!" << std::endl;
    }

    float x_painel = LARGURA_JANELA - m_largura;

    m_painelFundo.setSize(sf::Vector2f(m_largura, ALTURA_JANELA));
    m_painelFundo.setPosition(x_painel, 0);
    m_painelFundo.setFillColor(sf::Color(70, 70, 70));

    // Botão RUN
    m_botaoRun.setSize(sf::Vector2f(m_largura / 2.f - 30, 40));  
    m_botaoRun.setPosition(x_painel + 20, 20);
    m_botaoRun.setFillColor(sf::Color(0, 150, 0));
    m_textoBotao.setFont(m_fonte);
    m_textoBotao.setString("Iniciar");
    m_textoBotao.setCharacterSize(18); 
    m_textoBotao.setPosition(x_painel + 45, 30); // Posição para "Iniciar"

    // Botão Reiniciar
    m_botaoReiniciar.setSize(sf::Vector2f(m_largura / 2.f - 30, 40));
    m_botaoReiniciar.setPosition(x_painel + m_largura / 2.f + 10, 20);
    m_botaoReiniciar.setFillColor(sf::Color(180, 100, 0)); // Laranja
    m_textoReiniciar.setFont(m_fonte);
    m_textoReiniciar.setString("Reiniciar");
    m_textoReiniciar.setCharacterSize(18);
    m_textoReiniciar.setPosition(x_painel + m_largura / 2.f + 30, 30);

    float y_botoes_clima = 70.f; 
    float w_botao_clima = (m_largura - 60) / 3.f; 

    // Botão SOL
    m_botaoSol.setSize(sf::Vector2f(w_botao_clima, 30)); 
    m_botaoSol.setPosition(x_painel + 20, y_botoes_clima);
    m_textoSol.setFont(m_fonte);
    m_textoSol.setString("Sol");
    m_textoSol.setCharacterSize(16);
    m_textoSol.setPosition(x_painel + 20 + w_botao_clima/2.f - 15, y_botoes_clima + 5);
    
    // Botão CHUVA
    m_botaoChuva.setSize(sf::Vector2f(w_botao_clima, 30)); 
    m_botaoChuva.setPosition(x_painel + 30 + w_botao_clima, y_botoes_clima);
    m_textoChuva.setFont(m_fonte);
    m_textoChuva.setString("Chuva");
    m_textoChuva.setCharacterSize(16);
    m_textoChuva.setPosition(x_painel + 30 + w_botao_clima + w_botao_clima/2.f - 25, y_botoes_clima + 5);

    // Botão ALEATÓRIO
    m_botaoAleatorio.setSize(sf::Vector2f(w_botao_clima, 30)); 
    m_botaoAleatorio.setPosition(x_painel + 40 + w_botao_clima * 2, y_botoes_clima);
    m_textoAleatorio.setFont(m_fonte);
    m_textoAleatorio.setString("Aleatorio");
    m_textoAleatorio.setCharacterSize(16);
    m_textoAleatorio.setPosition(x_painel + 40 + w_botao_clima*2 + w_botao_clima/2.f - 40, y_botoes_clima + 5);

    // --- Stats e Gráfico de Fitness ---
    m_textoMelhorFitness.setFont(m_fonte);
    m_textoMelhorFitness.setCharacterSize(16);
    m_textoMelhorFitness.setPosition(x_painel + 20, 110); 
    
    float y_grafico_fit = 140.f; 
    float h_grafico = 150.f; 
    m_bordaGraficoFitness.setSize(sf::Vector2f(m_largura - 40, h_grafico));
    m_bordaGraficoFitness.setPosition(x_painel + 20, y_grafico_fit);
    m_bordaGraficoFitness.setFillColor(sf::Color::Transparent);
    m_bordaGraficoFitness.setOutlineColor(sf::Color(150, 150, 150));
    m_bordaGraficoFitness.setOutlineThickness(1.f);
    
    m_graficoFitness.setPrimitiveType(sf::LinesStrip);
    m_graficoFitnessLabelY_Max.setFont(m_fonte);
    m_graficoFitnessLabelY_Max.setCharacterSize(12);
    m_graficoFitnessLabelY_Max.setPosition(x_painel + 25, y_grafico_fit + 5);
    m_graficoFitnessLabelY_Min.setFont(m_fonte);
    m_graficoFitnessLabelY_Min.setCharacterSize(12);
    m_graficoFitnessLabelY_Min.setPosition(x_painel + 25, y_grafico_fit + h_grafico - 20);
    m_graficoFitnessLabelX_Gen.setFont(m_fonte);
    m_graficoFitnessLabelX_Gen.setCharacterSize(12);
    m_graficoFitnessLabelX_Gen.setPosition(x_painel + 20, y_grafico_fit + h_grafico + 5);

    // --- Placar ---
    m_placarTitulo.setFont(m_fonte);
    m_placarTitulo.setString("Placar (Top 5)");
    m_placarTitulo.setCharacterSize(18);
    m_placarTitulo.setPosition(x_painel + 20, 310);
    m_placarTexto.setFont(m_fonte);
    m_placarTexto.setCharacterSize(14);
    m_placarTexto.setPosition(x_painel + 20, 340); 

    // --- Gráfico de Tempo ---
    float y_grafico_tempo = 480.f;
    m_graficoTempoTitulo.setFont(m_fonte);
    m_graficoTempoTitulo.setString("Grafico de Tempo: N/A");
    m_graficoTempoTitulo.setCharacterSize(16);
    m_graficoTempoTitulo.setPosition(x_painel + 20, y_grafico_tempo - 25); 
    m_bordaGraficoTempo.setSize(sf::Vector2f(m_largura - 40, h_grafico));
    m_bordaGraficoTempo.setPosition(x_painel + 20, y_grafico_tempo);
    m_bordaGraficoTempo.setFillColor(sf::Color::Transparent);
    m_bordaGraficoTempo.setOutlineColor(sf::Color(150, 150, 150));
    m_bordaGraficoTempo.setOutlineThickness(1.f);

    m_graficoTempo.setPrimitiveType(sf::LinesStrip);
    m_graficoTempoLabelY_Max.setFont(m_fonte);
    m_graficoTempoLabelY_Max.setCharacterSize(12);
    m_graficoTempoLabelY_Max.setPosition(x_painel + 25, y_grafico_tempo + 5);
    m_graficoTempoLabelY_Min.setFont(m_fonte);
    m_graficoTempoLabelY_Min.setCharacterSize(12);
    m_graficoTempoLabelY_Min.setPosition(x_painel + 25, y_grafico_tempo + h_grafico - 20);
    m_graficoTempoLabelX_Gen.setFont(m_fonte);
    m_graficoTempoLabelX_Gen.setCharacterSize(12);
    m_graficoTempoLabelX_Gen.setPosition(x_painel + 20, y_grafico_tempo + h_grafico + 5);

    // --- Genes do Melhor ---
    m_genesTitulo.setFont(m_fonte);
    m_genesTitulo.setString("Genes do Melhor Carro");
    m_genesTitulo.setCharacterSize(18);
    m_genesTitulo.setPosition(x_painel + 20, 650);
    m_genesTexto.setFont(m_fonte);
    m_genesTexto.setCharacterSize(14);
    m_genesTexto.setPosition(x_painel + 20, 680);
}

void UIManager::TratarEvento(const sf::Event& event, bool& sim_deve_alternar, bool& sim_deve_reiniciar) { 
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f posMouse(event.mouseButton.x, event.mouseButton.y);
            
            if (m_botaoRun.getGlobalBounds().contains(posMouse)) {
                sim_deve_alternar = true;
            } 
            else if (m_botaoReiniciar.getGlobalBounds().contains(posMouse)) {
                sim_deve_reiniciar = true;
            }
            else if (m_botaoSol.getGlobalBounds().contains(posMouse)) {
                Simulacao::s_climaEscolhido = Clima::SOL; 
            } 
            else if (m_botaoChuva.getGlobalBounds().contains(posMouse)) {
                Simulacao::s_climaEscolhido = Clima::CHUVA; 
            }
            else if (m_botaoAleatorio.getGlobalBounds().contains(posMouse)) {
                Simulacao::s_climaEscolhido = Clima::ALEATORIO; 
            }
        }
    }
}

void UIManager::Atualizar(const std::vector<double>& historico_fitness, 
                        const std::vector<double>& historico_tempo,
                        const Carro& melhor_carro, 
                        const std::vector<Carro>& populacao,
                        bool simulacao_rodando){
    
    float x_painel = LARGURA_JANELA - m_largura;

    // Botão de iniciar e pausar 
    if (simulacao_rodando) {
        m_textoBotao.setString("Pausar");
        m_botaoRun.setFillColor(sf::Color(200, 0, 0)); // Vermelho
        m_textoBotao.setPosition(x_painel + 50, 30); // Posição para "Pausar"
    } else {
        m_textoBotao.setString("Iniciar");
        m_botaoRun.setFillColor(sf::Color(0, 150, 0)); // Verde
        m_textoBotao.setPosition(x_painel + 45, 30); // Posição para "Iniciar"
    }

    // Clima
    Clima clima = Simulacao::s_climaEscolhido;
    m_botaoSol.setFillColor(clima == Clima::SOL ? sf::Color(255, 215, 0) : sf::Color(100, 100, 100));
    m_botaoChuva.setFillColor(clima == Clima::CHUVA ? sf::Color(0, 100, 255) : sf::Color(100, 100, 100));
    m_botaoAleatorio.setFillColor(clima == Clima::ALEATORIO ? sf::Color(150, 0, 255) : sf::Color(100, 100, 100));

    // Fitness
    std::stringstream ss_fit;
    ss_fit << std::fixed << std::setprecision(2) << melhor_carro.fitness;
    m_textoMelhorFitness.setString("Melhor Fitness: " + ss_fit.str());
    
    // Lógica do gráfico fitness
    m_graficoFitness.clear();
    double max_fit = 0, min_fit = 0;
    if (!historico_fitness.empty()) {
        max_fit = historico_fitness[0]; min_fit = historico_fitness[0];
        for (double fit : historico_fitness) {
            if (fit > max_fit) max_fit = fit;
            if (fit < min_fit) min_fit = fit;
        }
    }

    double range_fit = max_fit - min_fit;
    if (range_fit < 1e-5) range_fit = 1.0; 
    double max_fit_padded = max_fit + range_fit * 0.1; 
    double min_fit_padded = min_fit - range_fit * 0.1;
    if (min_fit_padded < 0) min_fit_padded = 0; 

    float x_base_fit = LARGURA_JANELA - m_largura + 20;
    float y_base_fit = 140.f + 150.f; 
    float w_fit = m_largura - 40;
    float h_fit = 150.f - 10; 

    for (size_t i = 0; i < historico_fitness.size(); ++i) {
        float x_norm = (historico_fitness.size() == 1) ? 0.0f : static_cast<float>(i) / (historico_fitness.size() - 1);
        float y_norm = (max_fit_padded == min_fit_padded) ? 0.0f : (historico_fitness[i] - min_fit_padded) / (max_fit_padded - min_fit_padded);
        float x = x_base_fit + x_norm * w_fit;
        float y = y_base_fit - y_norm * h_fit; 
        m_graficoFitness.append(sf::Vertex(sf::Vector2f(x, y), sf::Color::White));
    }
    
    std::stringstream ss_max_fit, ss_min_fit, ss_gen_fit;
    ss_max_fit << std::fixed << std::setprecision(1) << max_fit_padded;
    ss_min_fit << std::fixed << std::setprecision(1) << min_fit_padded;
    ss_gen_fit << "Geracao: " << historico_fitness.size();
    m_graficoFitnessLabelY_Max.setString(ss_max_fit.str());
    m_graficoFitnessLabelY_Min.setString(ss_min_fit.str());
    m_graficoFitnessLabelX_Gen.setString(ss_gen_fit.str());

    // Lógica do placar
    std::vector<Carro> copia_pop = populacao;
    std::sort(copia_pop.begin(), copia_pop.end(), compararTempoCarros);
    std::stringstream ss_placar;
    for (int i = 0; i < 5; ++i) {
        if (i >= copia_pop.size()) break;
        ss_placar << (i + 1) << ". ";
        if (copia_pop[i].terminou_corrida) {
            ss_placar << std::fixed << std::setprecision(2) << copia_pop[i].tempo_de_corrida << "s";
        } else {
            ss_placar << "Nao terminou";
        }
        ss_placar << " (Fit: " << std::fixed << std::setprecision(2) << copia_pop[i].fitness << ")\n";
    }
    m_placarTexto.setString(ss_placar.str());

    // Lógica do gráfico de tempo 
    m_graficoTempo.clear();
    double max_time = 0, min_time = 0;
    if (!historico_tempo.empty()) {
        max_time = historico_tempo[0]; min_time = historico_tempo[0];
        for (double time : historico_tempo) {
            if (time > max_time) max_time = time;
            if (time < min_time) min_time = time;
        }
    }

    std::stringstream ss_titulo_tempo;
    ss_titulo_tempo << "Grafico de Tempo (Melhor: " << std::fixed << std::setprecision(2) << min_time << "s)";
    m_graficoTempoTitulo.setString(ss_titulo_tempo.str());

    double range_time = max_time - min_time;
    if (range_time < 1e-5) range_time = 1.0;
    double max_time_padded = max_time + range_time * 0.1;
    double min_time_padded = min_time - range_time * 0.1;
    if (min_time_padded < 0) min_time_padded = 0;

    float x_base_t = LARGURA_JANELA - m_largura + 20;
    float y_base_t = 480.f + 150.f; 
    float w_t = m_largura - 40;
    float h_t = 150.f - 10; 

    for (size_t i = 0; i < historico_tempo.size(); ++i) {
        float x_norm = (historico_tempo.size() == 1) ? 0.0f : static_cast<float>(i) / (historico_tempo.size() - 1);
        float y_norm = (max_time_padded == min_time_padded) ? 0.0f : (historico_tempo[i] - min_time_padded) / (max_time_padded - min_time_padded);
        float x = x_base_t + x_norm * w_t;
        float y = y_base_t - y_norm * h_t;
        m_graficoTempo.append(sf::Vertex(sf::Vector2f(x, y), sf::Color::Cyan));
    }

    std::stringstream ss_max_t, ss_min_t, ss_gen_t;
    ss_max_t << std::fixed << std::setprecision(1) << max_time_padded << "s";
    ss_min_t << std::fixed << std::setprecision(1) << min_time_padded << "s";
    ss_gen_t << "Geracao: " << historico_tempo.size();
    m_graficoTempoLabelY_Max.setString(ss_max_t.str());
    m_graficoTempoLabelY_Min.setString(ss_min_t.str());
    m_graficoTempoLabelX_Gen.setString(ss_gen_t.str());

    // lógica dos genes 
    std::stringstream ss_genes;
    ss_genes << std::fixed << std::setprecision(2);
    ss_genes << "Potencia: " << melhor_carro.genoma.potencia_motor << "\n";
    ss_genes << "Peso Piloto: " << melhor_carro.genoma.peso_piloto << " kg\n";
    ss_genes << "Tanque: " << melhor_carro.genoma.tamanho_tanque << " L\n";

    // Converte o int do pneu para string
    std::string tipo_pneu_str;
    switch (melhor_carro.genoma.tipo_pneu_inicial) {
        case 1: tipo_pneu_str = "Sol"; break;
        case 2: tipo_pneu_str = "Chuva"; break;
        case 3: tipo_pneu_str = "Hibrido"; break;
        default: tipo_pneu_str = "N/A"; break;
    }
    ss_genes << "Tipo de pneu: " << tipo_pneu_str << "\n";

    ss_genes << "Desgaste aceitavel do pneu: " << melhor_carro.genoma.estrategia_pitstop_pneu * 100 << "%\n";
    ss_genes << "Gasolina segura: " << melhor_carro.genoma.estrategia_pitstop_combustivel * 100 << "%\n";
    m_genesTexto.setString(ss_genes.str());
}

void UIManager::Desenhar(sf::RenderWindow& window) {
    window.draw(m_painelFundo);
    window.draw(m_botaoRun);
    window.draw(m_textoBotao);
    window.draw(m_botaoSol);
    window.draw(m_textoSol);
    window.draw(m_botaoChuva);
    window.draw(m_textoChuva);
    window.draw(m_botaoAleatorio);
    window.draw(m_textoAleatorio);

    // Fitness
    window.draw(m_textoMelhorFitness);
    window.draw(m_bordaGraficoFitness);
    window.draw(m_graficoFitness);
    window.draw(m_graficoFitnessLabelY_Max);
    window.draw(m_graficoFitnessLabelY_Min);
    window.draw(m_graficoFitnessLabelX_Gen);
    
    // Placar
    window.draw(m_placarTitulo);
    window.draw(m_placarTexto);

    // Tempo 
    window.draw(m_bordaGraficoTempo);
    window.draw(m_graficoTempo);
    window.draw(m_graficoTempoLabelY_Max);
    window.draw(m_graficoTempoLabelY_Min);
    window.draw(m_graficoTempoLabelX_Gen);

    // Genes 
    window.draw(m_genesTitulo);
    window.draw(m_genesTexto);

    // Reiniciar e pausar (CORRIGIDO)
    window.draw(m_botaoReiniciar);
    window.draw(m_textoReiniciar);
}