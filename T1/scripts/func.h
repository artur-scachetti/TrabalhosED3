/**
 * 
 * @file func.h
 * @brief Implementação das funcionalidades do trabalho.
 * 
 * Este módulo fornece as funcionalidades específicas solicitadas
 * nas especificações do trabalho, seus nomes correspondem exatamente
 * ao descrito no trabalho ("func1" corresponde à funcionalidade 1 do
 * documento).
 * 
 */

#ifndef FUNC_H
#define FUNC_H

#include "tools.h"

/**
 * @brief Cria um arquivo binário de dados a partir de um arquivo .csv.
 * 
 * Essa função inicializa um cabeçalho padrão, escreve ele em um arquivo binário recém criado, lê as 
 * informações dos registros de dados contidas em um arquivo .csv, extrai essas informações e escreve-as
 * no arquivo binário. No final, expõe o binário do arquivo de saída na tela.
 * 
 * @param[in] arquivoEntrada Trata-se do arquivo .csv que contém as informações a serem passadas para o
 *                           arquivo binário.
 * @param[out] arquivoSaida Arquivo binário que receberá o cabeçalho padrão e os dados retirados do 
 *                          arquivo .csv.
 * 
 */
void func1(char* arquivoEntrada, char* arquivoSaida);

/**
 * @brief Printa todos os registros de dados presentes no arquivo binário.
 * 
 * Essa função percorre o arquivo binário, ignora o registro de cabeçalho e então lê os registros
 * de dados, printando-os na formatação específica: 'idPoPs idPoPsConectado velocidade "unidadeMedida"'.
 * Existe o tratamento para valores nulos, sendo transpostos como "NULO", tanto para velocidade quanto
 * para unidade de medida.
 * 
 * @param[in] arquivoEntrada Arquivo binário do qual serão extraídas as informações dos registros.
 * 
 */
void func2(char* arquivoEntrada);

/**
 * @brief Printa registros que satisfazem crtérios de busca.
 * 
 * Essa função recebe da main.c os critérios (pares Parâmetro/Valor) de busca, percorre o arquivo
 * binário, comparando em cada registro as informações obtidas com os critérios de busca. A função
 * utiliza uma flag interna que printa os registros enquanto ela estiver ativa (enquanto todos os 
 * critério forem atendidos), mas que não o faz se ao menos um dos pares Parâmetro/Valor não for 
 * atendido (flag desativada).
 * 
 * @param[in] arquivoEntrada Arquivo binário do qual serão extraídas as informações dos registros.
 * @param[in] numPares Indica quantos pares Parâmetro/Valor, ou seja, critérios estão sendo utilizados
 *                     na busca.
 * @param[in] args Estrutura que carrega tanto os Parâmetros quanto os Valores nele buscados.
 *  
 */
void func3(char* arquivoEntrada, int numPares, argsBusca* args);

/**
 * @brief Printa um registro acessado diretamente.
 * 
 * Essa função acessa diretamente um registro por meio das informações de tamanho (bytes) de registro e 
 * do número de RRN. O bytte offset exato é encontrado por meio da fórmula matemática:
 * byte offset = tamanho do registro x RRN
 * 
 * @param[in] arquivoEntrada Arquivo binário que acessado e do qual serão retirados os dados do registro.
 * @param[in] RRN Número do registro o qual se quer recuperar / printar.
 * 
 */
void func4(char* arquivoEntrada, int RRN);

#endif