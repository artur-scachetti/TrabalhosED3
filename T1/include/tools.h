/**
 * 
 * @file tools.h
 * @brief Implementação de funções de uso geral.
 * 
 * Este módulo fornece funções gerais usadas múltiplas vezes nas funcionalidades
 * e / ou por mais de uma funcionalidade, como leitura e escrita de registros.
 * Funciona basicamente como uma "biblioteca padrão" da manipulação de arquivos.
 * 
 */

#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Refere-se ao número de bytes presentes em um registro de dados
 *  removido + tamanhoPilha + idPoPs + idPoPsConectado + velocidade + unidadeMedida
 *      1    +       4      +    4   +         4       +      4     +       1       =  18 (17 pois conta-se o 0!)
 */ 
#define BYTES_PER_REG   17

/**
 * @brief Estrutura que define os campos de um registro de cabeçalho.
 */
typedef struct {

    char status; /**< Indica se o arquivo está consistente ('1') ou não ('0') */
    int topoPilha; /**< RRN do registro removido mais recente (-1 caso não haja) */
    int proxRRN;  /**< Próximo RRN disponível */
    int nroRegRem; /**< Número de registros marcados como removidos */
    int nroPares; /**< Número de pares idPoPs e idPoPsConectado (número de registros distintos) */

} headerReg;

/**
 * @brief Estrutura que define os campos de um registro de dados.
 */
typedef struct {

    char removido; /**< Indica se o registro está removido ('1') ou não ('0') */
    int encadeamentoPilha; /**< RRN do próximo registro removido */
    int idPoPs; /**< ID do PoPs */
    int idPoPsConectado; /**< ID do PoPs Conectado */
    int velocidade; /**< Velocidade de transmissão */
    char unidadeMedida; /**< Um byte que indica a unidade utilizada*/

} dataReg;

/**
 * @brief Estrutura feita armazenar argumentos necessários para busca específica.
 */
typedef struct {

    char nomesCampo[100]; /**< Nome do parâmetro do qual se quer fazer a busca */
    char valoresCampo[100]; /**< O valor procurado no parâmetro especificado */

} argsBusca;

/**
 * @brief Inicializa um registro de cabeçalho.
 * 
 * @return Um cabeçalho com valores padrões (status = '0', topoPilha = -1, resto dos argumentos = 0).
 * 
 * @warning O nroPares é inicializado por padrão como 0, mas deve ser atualizado depois após processo
 *          de read / write!
 */
headerReg header_init();

/**
 * @brief Escreve informações nos campos de um registro de cabeçalho.
 * 
 * @param[in] header Estrutura de cabeçalho de onde os dados serão lidos e passados para o arquivo.
 * @param[in, out] arquivoSaida Arquivo para qual as informações de cabeçalho serão escritas.
 * 
 */
void header_write(const headerReg* header, FILE* arquivoSaida);

/**
 * @brief Lê as informações dos campos de um registro de cabeçalho.
 * 
 * @param[out] header Estrutura de cabeçalho preenchida pelos dados lidos do arquivo, para
 *                   que possam ser vistas pelo usuário. 
 * @param[in] arquivoEntrada Arquivo do qual as informações de cabeçalho serão lidas.
 * 
 */
void header_read(headerReg* header, FILE* arquivoEntrada);

/**
 * @brief Escreve informações nos campos de um registro de dados.
 * 
 * @param[in] data Estrutura de dados de onde os dados serão lidos e passados para o arquivo.
 * @param[in, out] arquivoSaida Arquivo para qual as informações de dados serão escritas.
 * 
 */
void data_write(const dataReg* data, FILE* arquivoSaida);

/**
 * @brief Lê as informações dos campos de um registro de dados.
 * 
 * @param[out] data Estrutura de dados preenchida pelas informações lidas do arquivo, para
 *                   que possam ser vistas pelo usuário. 
 * @param[in] arquivoEntrada Arquivo do qual as informações de daods serão lidas.
 * 
 * @retval 0 Registro logicamente removido.
 * @retval 1 Sucesso. 
 * 
 */
int data_read(dataReg* data, FILE* arquivoEntrada);

/**
 * @brief Lê os campos de um registro de dados do arquivo .csv.
 * 
 * Essa função percorre a linha do .csv com dois "cursores". Um deles, a cada
 * iteração do loop, aponta para o próximo ';' e o transforma em '\0', enquanto
 * o outro aponta para o byte imediatamente após esse, sendo o início do campo de dados.
 * Assim, formam-se substrings contendo apenas a informação dos campos para serem
 * processadas.
 * 
 * @param[in, out] buffer Buffer que contém o registro inteiro armazenado sem tratamento
 *                   diretamente da linha do .csv.
 * @param[out] data Estrutura que contém os campos do registro de dados que se quer 
 *                 armazenar os dados na linha .csv lida.
 * 
 * @warning Esta função funciona para o formato de organização do .csv específico para
 *          este trabalho -> "idPoPs;idPoPsConectado;velocidade;unidadeMedida"
 */
void read_reg_csv(char* buffer, dataReg *data);

/**
 * @brief Printa as informações de um registro de dados na tela.
 * 
 * @param[in] data Estrutura de dados com as informações a serem printadas. 
 * 
 * @retval 0 Registro logicamente removido.
 * @retval 1 Sucesso. 
 * 
 */
int print_reg(dataReg* data);

/**
 * @brief Verifica se o campo possui o valor buscado.
 * 
 * @param[in] data Estrutura de dados com as informações a serem buscadas.
 * @param[in] modoBusca Define em qual dos campos deve-se fazer a verificação.
 * @param[in] valorBuscado Valor que se procura no campo definido. 
 * 
 * @retval 0 Valor não encontrado no campo.
 * @retval 1 Valor encontrado.
 * 
 */
int parameter_search(dataReg* data, int modoBusca, char* valorBuscado);

#endif