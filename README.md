# SisOp - Trabalho 2

Implementação do trabalho 2 para a disciplina de Sistemas Operacionais / UnB

Autor: Diogo César Ferreira

Link para o repositório: https://github.com/Aendur/SisOp-T2/tree/main

Este branch é referente à implementação usando **threads**.
Para acesar a implementação com _IPC_, acesse https://github.com/Aendur/SisOp-T2/tree/IPC

## Compilação
Para compilar, executar o comando 
```
$ make
```
ou
```
$ make main
```
O executável será criado no caminho `bin/main`.

## Pre-requisitos
Este programa apresenta como pre-requisitos adicionais as seguintes bibliotecas:

Simple DirectMedia Layer
- `libsdl2-2.0-0`
- `libsdl2-dev`

## Configuração da partida
Os processos do servidor e do cliente utilizam os parâmetros definidos respectivamente nos arquivos `settingsr.ini`. A especificação de como configurar esses
arquivos pode ser lida diretamente nos arquivos fornecidos.


## Dados adicionais
O programa foi testado utilizando-se o compilador/sistema operacional `g++ (Debian 11.2.0-16) 11.2.0` executando no WSL2.


