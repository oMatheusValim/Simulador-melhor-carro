#!/bin/bash
# Script para configurar o ambiente de desenvolvimento C++ para o projeto F1 Evolution

# Interrompe o script se qualquer comando falhar
set -e

echo "--- Atualizando lista de pacotes ---"
sudo apt update

echo "--- INSTALANDO FERRAMENTAS ESSENCIAIS (COMPILADOR C++, GDB, GIT, CMAKE) ---"
sudo apt install -y build-essential gdb cmake git

echo "--- Instalando biblioteca SFML (versão de desenvolvimento) ---"
sudo apt install -y libsfml-dev

echo "--- Verificando versões ---"
g++ --version
cmake --version

echo "--- Configuração concluída! ---"
echo "Extensões do VS Code para instalar: 'C/C++ Extension Pack' e 'CMake Tools'."
echo "Próximos passos:"
echo "1. Compile o projeto com: cmake -B build -S . && cmake --build build"
echo "2. Execute com: ./build/F1_Evolution"