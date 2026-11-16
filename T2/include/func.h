/**
 * 
 * @file  func.h
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
 * @brief Cria um arquivo de dados binário a partir de um arquivo csv.
 * 
 * @param[in] arquivoEntrada Nome do arquivo csv que contém os registros a serem armazenados.
 * @param[in] arquivoSaida Nome do arquivo de dados no qual os registros serão inseridos.
 * 
 */
void func8(char* arquivoEntrada, char* arquivoSaida);

/**
 * @brief Cria um arquivo de índices a partir de um arquivo de dados.
 * 
 * @param[in] arquivoEntrada Nome do arquivo de dados.
 * @param[in] arquivoIndice Nome do arquivo de índice.
 * @param[in] func9Mode Determina se a chamada deverá ou não escrever o valor binário do arquivo de Índice.
 * 
 */
void func9(char* arquivoEntrada, char* arquivoIndice, int func9Mode);

/**
 * @brief Printa todos os registros de um arquivo de dados que não estejam logicamente removidos.
 * 
 * @param[in] arquivoEntrada Nome do arquivo de dados.
 * 
 */
void func10(char* arquivoEntrada);

/**
 * @brief Printa todos os registros de um arquivo de dados que atenda aos critérios de busca informados. 
 *        Busca é realizada no arquivo de dados ou de índice, a depender dos critérios fornecidos.
 * 
 * @param[in] arquivoEntrada Nome do arquivo de dados.
 * @param[in] arquivoIndice Nome do arquivo de índices.
 * @param[in] numPares Número de pares tipo de dado e valor respectivo que deverão ser verificados em cada registro.
 * @param[in] args Pares de tipos de dados e valores respectivos a serem buscados no arquivo.
 * 
 */
void func11(char* arquivoEntrada, char* arquivoIndice, int numPares, argsBusca* args);

/**
 * @brief Executa a remoção lógica de registros que atendam aos critérios de busca especificados.
 * 
 * @param[in] arquivoEntrada Nome do arquivo de dados.
 * @param[in] arquivoIndices Nome do arquivo de índices.
 * @param[in] numPares Número de pares tipo de dado e valor respectivo que deverão ser verificados em cada registro.
 * @param[in] args Pares de tipos de dados e valores respectivos a serem buscados no arquivo.
 * 
 */
void func12(char* arquivoEntrada, char* arquivoIndices, int numPares, argsBusca* args);

/**
 * @brief Insere novos registros em um arquivo de dados, utilizando a abordagem WorstFit.
 * 
 * @param[in] arquivoEntrada Nome do arquivo de dados.
 * @param[in] arquivoIndice Nome do arquivo de índices.
 * @param[in] numRegistros Número de registros a serem inseridos.
 * 
 */
void func13(char* arquivoEntrada, char* arquivoIndice, int numRegistros);

/**
 * @brief Atualiza registros que atendam aos critérios de busca especificados em um arquivo de dados.
 * 
 * @param[in] arquivoEntrada Nome do arquivo de dados.
 * @param[in] arquivoIndices Nome do arquivo de índices.
 * @param[in] args Pares de tipos de dados e valores respectivos a serem buscados no arquivo.
 * @param[in] dataAtt Informações a serem inseridas nos registros de dados que atenderem aos critérios de busca, em caso de update.
 * @param[in] numParesBusca Número de pares tipo de dado e valor respectivo que deverão ser verificados em cada registro.
 * @param[in] numParesAtt Número de pares tipo de dado e valor respectivo que serão inseridos nos registros que atendem aos 
 *                        critérios de busca.
 */
void func14(char* arquivoEntrada, char* arquivoIndices, argsBusca* args, argsBusca* dataAtt, int numParesBusca, int numParesAtt);

#endif