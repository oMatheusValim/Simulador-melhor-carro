# Simulador do Melhor Carro

## 🎥 Demonstração do Projeto

[![Assista ao Vídeo](https://img.youtube.com/vi/7GTCjU1kPnY/0.jpg)](https://youtu.be/7GTCjU1kPnY)

> Caso o vídeo acima não carregue, você pode assistir [diretamente no YouTube](https://youtu.be/7GTCjU1kPnY) ou baixar via [Google Drive](https://drive.google.com/file/d/1OUxzYLVQ7Zd-cjHRWNR1_nlqcq7oCzug/view?usp=drive_link).


## 🎯 Objetivo

Este projeto utiliza um algoritmo genético para otimizar os parâmetros de carros de corrida em um simulador 2D. O objetivo é encontrar a melhor combinação de "genes" (potência, peso, tipo de pneu, estratégia de parada, etc.) para que os carros completem um percurso no menor tempo possível, adaptando-se a diferentes condições, como o clima dinâmico.

Este trabalho foi desenvolvido como projeto final da disciplina SSC0713 - Sistemas Evolutivos Aplicados à Robótica, ministrada pelo Prof. Dr. Eduardo do Valle Simões.

## ✨ Funcionalidades Principais

O simulador implementa um ecossistema complexo de física e evolução:

### 🏎️ Simulação e Física
* **Física de Arrasto (Drag):** A velocidade máxima não é fixa; ela é limitada naturalmente pela resistência do ar, criando um *trade-off* real entre potência do motor e consumo de combustível.
* **Desgaste e Atrito:** Implementação de desgaste progressivo dos pneus e perda de tração. O atrito varia dependendo do tipo de pneu (Sol, Chuva, Híbrido) e do clima atual.
* **Máquina de Estados (FSM):** Os carros possuem inteligência para decidir quando entrar no Pit Stop, navegando entre estados de *Racing*, *Entering Pit* e *Pitting*.
* **Clima Dinâmico:** No modo aleatório, o clima muda durante a corrida, forçando "Pit Stops de Emergência" (mutações em tempo real) para troca de pneus.

### 🧬 Algoritmo Genético Avançado
O sistema utiliza múltiplas estratégias para evitar estagnação (máximos locais):
* **Elitismo:** O melhor indivíduo é preservado, mas monitorado.
* **Hipermutação Exponencial:** Se o vencedor permanecer no topo por muitas gerações consecutivas, ele sofre uma taxa de mutação exponencialmente maior para forçar a rotatividade.
* **Mutação Adaptativa:** A taxa de mutação dos filhos varia automaticamente baseada na diferença entre o melhor fitness e a média da população.
* **Crossover Ponderado:** Pais com melhor desempenho têm maior peso matemático na transmissão de seus genes.

### 🖥️ Interface Gráfica (UI)
* **Painel de Controle em Tempo Real:** Botões para Iniciar, Pausar e Reiniciar a simulação, além de controle total sobre o clima.
* **Gráficos em Tempo Real:**
    * *Fitness:* Visualização da evolução da aptidão ao longo das gerações.
    * *Tempo:* Gráfico de queda do tempo da melhor volta.
* **Visualização de Genes:** Exibição detalhada dos genes do atual campeão, incluindo estratégias de risco (porcentagem de segurança para combustível e pneus).
* **Modo Editor:** Ferramenta interna para mapear novos *waypoints* e desenhar traçados de pista e pit lane clicando na tela.

## 🛠️ Tecnologias Utilizadas

  * **C++ (Padrão 17):** Linguagem principal do projeto.
  * **SFML (v2.6):** Biblioteca de multimídia utilizada para toda a renderização gráfica, janelas e interface do usuário (UI).
  * **CMake:** Utilizado para gerenciar a compilação do projeto de forma multiplataforma.

-----

## 🚀 Como Executar o Projeto

Existem duas maneiras de executar o simulador: compilando a partir do código-fonte (Linux) ou baixando o executável pronto (Windows).

### 🐧 Linux (Para Desenvolvedores/Compilando)

Estas instruções são para sistemas baseados em Ubuntu/Debian e permitem compilar o projeto do zero.

**1. Instalar as Dependências:**
Um script de conveniência é fornecido para instalar todas as ferramentas e bibliotecas necessárias (`g++`, `cmake`, `git` e `libsfml-dev`).

Primeiro, torne o script executável:

```bash
chmod +x setup.sh
```

Depois, execute-o:

```bash
./setup.sh
```

**2. Compilar o Projeto:**
Com as dependências instaladas, use o CMake para compilar o projeto. O `libsfml-dev` será encontrado automaticamente.

```bash
cmake -B build -S . && cmake --build build
```

**3. Executar o Simulador:**
O executável será criado dentro da pasta `build`.

```bash
./build/BEST-CAR-SIMULATOR
```

### 🪟 Windows (Público Geral / Executável)

*(Sugestão: Para que esta seção funcione, você precisaria compilar o projeto no Windows uma vez e enviar o `.exe` e as `.dll` necessárias para a seção "Releases" do seu GitHub.)*

1.  Vá até a seção **[Releases](https://www.google.com/search?q=https://github.com/seu-usuario/Simulador-melhor-carro/releases)** deste repositório.
2.  Baixe o arquivo `Simulador-F1-Windows.zip`.
3.  Descompacte a pasta em qualquer local.
4.  Execute o arquivo `BEST-CAR-SIMULATOR.exe`.