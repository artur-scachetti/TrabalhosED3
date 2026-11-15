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

/** Refere-se ao número de bytes de registros fixos presentes em um registro de dados
 *  encadeamentoLista +  idPoPs + siglaPais 
 *      8             +    4    +     2       =  14
 */ 
#define FIXED_BYTES_PER_REG       14

/** Refere-se ao número de bytes presentes em um registro de header
 *  idPoPs + byteOffset
 *    4    +     8      =  12
 */
#define BYTES_PER_IDX_REG       12

/** Refere-se ao número de bytes presentes em um registro de header
 *  status + topoPilha + proxOff + nroRegRem + nroPares
 *    1    +     4     +   4     +     4     +    4      =  17
 */
#define BYTES_PER_HEADER        25

// Define os macros para decidir os tipos de busca
#define INDEX_SEARCH            0
#define PARAMETER_SEARCH        1

// Define a ação a ser tomada pelos algoritmos de pesquisa
#define PRINT_MODE              0
#define DELETE_MODE             1
#define UPDATE_MODE             2

/**
 * @brief Estrutura que define os campos de um registro de cabeçalho.
 */
typedef struct 
{
    char status; /**< Indica se o arquivo está consistente ('1') ou não ('0') */
    long long int topoPilha; /**< RRN do maior registro removido (-1 caso não haja) */
    long long int proxOff;  /**< Próximo RRN disponível */
    int nroRegRem; /**< Número de registros marcados como removidos */
    int nroPoPs; /**< Número de pares idPoPs e idPoPsConectado (número de registros distintos) */

} headerReg;

/**
 * @brief Estrutura que define os campos de um registro de dados.
 */
typedef struct 
{
    char removido; /**< Indica se o registro está removido ('1') ou não ('0') */
    int tamanhoRegistro; /**< Número de bytes do registro */
    long long int encadeamentoLista; /**< Byteoffset do próximo registro removido */
    int idPoPs; /**< ID do PoPs */
    char siglaPais[2]; /**< Sigla do país */
    char* nomePops; /**< Nome do Pops */
    char* nomePais; /**< Nome do pais */
} dataReg;

/**
 * @brief Estrutura que define os campos de um registro de cabeçalho do arquivo de índice primário
 */
typedef struct
{
    char status; /**< Indica se o registro está removido ('1') ou não ('0')  */
} indexHeader;

/**
 * @brief Estrutura que define os campos de um registro de dados do arquivo de índice primário
 */
typedef struct
{
    int idPops; /**< ID do PoPs */
    long long int byteOffset; /**< byteoffset do registro */
} indexReg;

/**
 * @brief Estrutura feita armazenar argumentos necessários para busca específica.
 */
typedef struct 
{
    char nomesCampo[100]; /**< Nome do parâmetro do qual se quer fazer a busca */
    char* valoresCampo; /**< O valor procurado no parâmetro especificado */

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
 * @brief Inicializa um registro de dados, a partir de 4 strings.
 * 
 * @param[in] idPops String do idPops
 * @param[in] idPopsConectado String do idPops conectado
 * @param[in] velocidade String da velocidade
 * @param[in] unidadeMedida String da unidade de medida
 * 
 * @return Um registro de dados data, com os valores inseridos, substituindo os campos inteiros nulos por -1, 
 * e os campos de string nulos por $. Inicializa removido como 0 e o campo de encadeamento como -1.
 * 
 */
dataReg data_Cria(char* idPoPs, char* idPoPsConectado, char* nomePops, char* nomePais);

/**
 * @brief Conta e atualiza o tamanho de um registro
 * 
 * @param[in] data registro a ser analizado
 */
void contaTam(dataReg* data);

/**
 * @brief Atualiza um campo de um registro de dados
 * 
 * @param[in] data Registro a ser atualizado
 * @param[in] nomeCampo Campo a ser atualizado
 * @param[in] valorCampo Valor a ser atribuído ao campo
 * 
 */
void data_Atualiza(dataReg* data, char* nomeCampo, char*valorCampo);

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
int header_read(headerReg* header, FILE* arquivoEntrada);

/**
 * @brief Escreve informações nos campos de um registro de dados.
 * 
 * @param[in] data Estrutura de dados de onde os dados serão lidos e passados para o arquivo.
 * @param[in, out] arquivoSaida Arquivo para qual as informações de dados serão escritas.
 * 
 */
void data_write(const dataReg* data, FILE* arquivoSaida);

void data_write_rem(const dataReg* data, FILE* arquivoSaida);

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

int get_search_mode(char* nomeCampo);

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

int complete_parameter_search(dataReg* data, headerReg* header, FILE* fEntrada, int numPares, int numAtt, argsBusca* args, argsBusca* argsAtt, int mode);

void parameter_delete(dataReg* data, int modoBusca, char* valorBuscado);

/**
 * @brief Verifica se o campo possui o valor buscado
 * 
 * @param[in] data Estrutura de dados com as informações a serem buscadas.
 * @param[in] modoBusca Define em qual dos campos deve-se fazer a verificação.
 * @param[in] valorBuscado Valor que se procura no campo definido. 
 * 
 * @retval 0 Valor não encontrado no campo.
 * @retval 1 Valor encontrado.
 * 
 */

 int read_func_init(char* arquivo, FILE** f, headerReg* header);

int validate_all_parameters(dataReg* data, int numPares, argsBusca* args);

 void free_data_reg(dataReg* data);

 void consume_trash(FILE* arquivoEntrada, int amount);

 int qsort_aux_cmp(const void* a, const void* b);

 void index_write(indexReg* index, dataReg* data, FILE* fIndice, int bOffset);

 void delete_reg(dataReg* data, headerReg* header, FILE* fEntrada, long long int original_bOffset);

 int index_search(int target_id, char* arquivoIndices, FILE* fEntrada, dataReg* data, headerReg* header, int numPares, int numAtt, argsBusca* args, argsBusca* argsAtt, int mode); 

argsBusca* determinaArgsBusca(int numPares);

void readSingleInput(dataReg* data, int modo);

void readDataInput(dataReg* data);

void writeDataWorst(dataReg* data, headerReg* header, FILE* arquivoSaida);

#endif