/**
 * 
 * @file tools.h
 * @brief Implementação de funções de uso geral.
 * 
 * Este módulo fornece funções, estruturas e labels gerais usadas múltiplas 
 * vezes nas funcionalidades e / ou por mais de uma funcionalidade, como 
 * leitura e escrita de registros. 
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

//Define o tipo de dado a ser armazenado/buscado pelos algoritmos de leitura de entradas e de busca
#define NOME_POPS               1
#define NOME_PAIS               2
#define SIGLA_PAIS              3
#define ID_POPS                 4

//Define se func9 deve escrever o binário ou apenas criar o arquivo de índice
#define APENAS_CRIA             0
#define ESCREVE_BIN             1

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
 * @brief Escreve informações de um registro de dados em um arquivo.
 * 
 * @param[in] data Estrutura de dados de onde os dados serão lidos e passados para o arquivo.
 * @param[in, out] arquivoSaida Arquivo no qual as informações de dados serão escritas.
 * 
 */
void data_write(const dataReg* data, FILE* arquivoSaida);

/**
 * @brief Escreve um registro logicamente removido em um arquivo de saída.
 * 
 * @param[in] data Estrutura de dados de onde os dados serão lidos e passados para o arquivo.
 * @param[in, out] arquivoSaida Arquivo no qual as informações serão escritas.
 * 
 */
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


/**
 * @brief Determina o tipo de busca a ser realizado por algoritmos de busca.
 * 
 * @param[in] nomeCampo Nome do campo a ser buscado.
 * 
 */
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

/**
 * @brief Busca, em um arquivo de dados, por registros que atendam a todos os critérios de busca determinados, e realiza 
 *        o tratamento determinado (print, delete, update)
 * 
 * @param[in] data Estrutura de dados que armazena as informações lidas do arquivo.
 * @param[in] header Cabeçalho do arquivo.
 * @param[in, out] fEntrada Arquivo de dados de interesse.
 * @param[in] numPares Número de pares tipo de dado e valor respectivo que deverão ser verificados em cada registro.
 * @param[in] numAtt Número de pares tipo de dado e valor respectivo que serão inseridos nos registros que atendem aos 
 *                   critérios de busca, em caso de update.
 * @param[in] args Pares de tipos de dados e valores respectivos a serem buscados no arquivo.
 * @param[in] argsAtt Informações a serem inseridas nos registros de dados que atenderem aos critérios de busca, em caso de update.
 * @param[in] mode Flag que determina o tratamento a ser realizado sobre os dados.
 * 
 * @retval 0 Não há nenhum registro que atenda aos critérios.
 * @retval 1 Existe ao menos um registro que atenda aos critérios.
 */
int complete_parameter_search(dataReg* data, headerReg* header, FILE* fEntrada, int numPares, int numAtt, argsBusca* args, argsBusca* argsAtt, int mode);

/**
 * @brief Abre o arquivo e lê o seu cabeçalho em funções de leitura.
 * 
 * @param[in] arquivo Nome do arquivo a ser aberto.
 * @param[in, out] f Ponteiro no qual o arquivo é armazenado
 * @param[in, out] header Estrutura de dados que armazena o cabeçalho do arquivo.
 * 
 * @retval 0 Caso a abertura do arquivo ou a leitura do cabeçalho falhe
 * @retval 1 Caso ambos obtenham sucesso.
 * 
 */
int read_func_init(char* arquivo, FILE** f, headerReg* header);

/**
 * @brief Abre o arquivo e lê o seu cabeçalho em funções de escrita.
 * 
 * @param[in] arquivo Nome do arquivo a ser aberto.
 * @param[in, out] f Ponteiro no qual o arquivo é armazenado
 * @param[in, out] header Estrutura de dados que armazena o cabeçalho do arquivo.
 * 
 * @retval 0 Caso a abertura do arquivo ou a leitura do cabeçalho falhe
 * @retval 1 Caso ambos obtenham sucesso.
 * 
 */
int write_func_init(char* arquivo, FILE** f, headerReg* header);

/**
 * @brief Verifica se todos os critérios de busca são atendidos por um registro de dados em específico.
 * 
 * @param[in] data Registro de dados de interesse.
 * @param[in] numPares Quantidade de pares tipo de dado e valor respectivo que serão verificados no registro de dados.
 * @param[in] args Estrutura de dados que armazena os pares de tipo de dado e valor respectivo em si.
 * 
 * @retval 0 Não atende a todos os critérios simultaneamente.
 * @retval 1 Atende a todos os critérios simultaneamente.
 */
int validate_all_parameters(dataReg* data, int numPares, argsBusca* args);

/**
 * @brief Libera um registro de dados
 * 
 * @param[in] data Registro que será liberado.
 * 
 */
void free_data_reg(dataReg* data);

/**
 * @brief Consome o lixo de registros de dados que possuem lixo.
 * 
 * @param[in, out] arquivoEntrada Arquivo que contém o registro de interesse.
 * @param[in] amount Quantidade de lixo a ser consumida.
 * 
 */
void consume_trash(FILE* arquivoEntrada, int amount);

/**
 * @brief 
 */
int qsort_aux_cmp(const void* a, const void* b);

/**
 * @brief Escreve um registro de dados em um arquivo de índice
 * 
 * @param[in] index Estrutura de dados que armazena as informações a serem escritas diretamente no arquivo.
 * @param[in] data Estrutura de dados da qual as informações de index serão extraídas.
 * @param[in, out] fIndice Arquivo de índice.
 * @param[in] bOffset Byte offset do dado original que é indexado pela entrada atual no arquivo de índice.
 * 
 */
void index_write(indexReg* index, dataReg* data, FILE* fIndice, int bOffset);

/**
 * @brief Realiza a remoção lógica de um registro em um arquivo de dados.
 * 
 * @param[in] data Registro a ser removido.
 * @param[in, out] header Cabeçalho do arquivo de dados.
 * @param[in, out] fEntrada Arquivo de dados de interesse.
 * @param[in] original_bOffset Byte offset original do registro no arquivo.
 * 
 */
void delete_reg(dataReg* data, headerReg* header, FILE* fEntrada, long long int original_bOffset);

/**
 * @brief Realiza busca por índice em um arquivo de índices, e realiza o tratamento especificado no arquivo de dados indexado.
 * 
 * @param[in] target_id Indice alvo.
 * @param[in] arquivoIndices Nome do arquivo de indices.
 * @param[in, out] fEntrada Arquivo de dados indexado.
 * @param[in, out] header Cabeçalho do arquivo de dados.
 * @param[in] numPares Número de pares tipo de dado e valor respectivo que deverão ser verificados em cada registro.
 * @param[in] numAtt Número de pares tipo de dado e valor respectivo que serão inseridos nos registros que atendem aos critérios 
 *                   de busca, em caso de update.
 * @param[in] args Pares de tipos de dados e valores respectivos a serem buscados no arquivo.
 * @param[in] argsAtt Informações a serem inseridas nos registros de dados que atenderem aos critérios de busca, em caso de update.
 * @param[in] mode Flag que determina o tratamento a ser realizado sobre os dados.
 * 
 * @retval 0 Não há nenhum registro que atenda aos critérios.
 * @retval 1 Existe ao menos um registro que atenda aos critérios.
 * 
 */
int index_search(int target_id, char* arquivoIndices, FILE* fEntrada, dataReg* data, headerReg* header, int numPares, int numAtt, argsBusca* args, argsBusca* argsAtt, int mode); 

/**
 * @brief Coleta e armazena argumentos de busca inseridos no terminal.
 * 
 * @param[in] numPares Número de pares tipo de dado e valor respectivo que deverão ser lidos.
 * 
 * @return Um vetor de estruturas argsBusca, que contém todas os pares.
 * 
 */
argsBusca* determinaArgsBusca(int numPares);

/**
 * @brief Armazena um dado lido por input no campo respectivo de um registro de dados.
 * 
 * @param[in, out] data Registro de dados de interesse.
 * @param[in] modo Determina em qual campo do registro o dado será inserido.
 * 
 */
void readSingleInput(dataReg* data, int modo);

/**
 * @brief Lê um registro de dados completo inserido por terminal.
 * 
 * @param[out] data Estrutura de dados que irá armazenar as informações lidas.
 * 
 */
void readDataInput(dataReg* data);

/**
 * @brief Escreve um registro de dados em um arquivo de dados utilizando o método WorstFit
 * 
 * @param[in] data Estrutura de dados que armazena as informações a serem inseridas.
 * @param[in, out] header Cabeçalho do arquivo de dados.
 * @param[in, out] arquivoSaida Arquivo de dados de interesse.
 * 
 */
void writeDataWorst(dataReg* data, headerReg* header, FILE* arquivoSaida);

/**
 * @brief Atualiza determinados campos de um registro de dados e reposiciona este campo no arquivo caso sua localização original 
 *        não possua mais espaço suficiente. 
 *       
 * @param[in] data Estrutura de dados que armazena os conteúdos do registro de interesse
 * @param[in, out] header Cabeçalho do arquivo de dados.
 * @param[in, out] fEntrada Arquivo de dados em si.
 * @param[in] offset Byte offset original do registro.
 * @param[in] numAtt Número de pares tipo de dado e valor respectivo que serão inseridos no registro
 * @param[in] argsAtt Pares de tipo de dado e valor respectivo em si.
 * 
*/
void atualizarRegistro(dataReg* data, headerReg* header, FILE* fEntrada, long long int offset, int numAtt, argsBusca* argsAtt);

/**
 * @brief Lê um único dado do tipo string inserido no terminal
 * 
 * @return String contendo a informação lida.
 * 
 */
char* readSingleData(void);

/**
 * @brief Determina se busca será realizada no arquivo de indice ou no arquivo de dados.
 * 
 * @param[in] args Pares de tipo de dado e valor respectivo a serem analizados para determinação do tipo de busca.
 * @param[in] numPares Número de pares tipo de dado e valor respectivo.
 * @param[in] valorIdPops Armazena o índice alvo caso a busca seja no arquivo de índice.
 * 
 * @retval INDEX_SEARCH Busca no arquivo de índices.
 * @retval PARAMETER_SEARCH Busca no arquivo de dados.
 * 
 */
int determinaTipoBusca(argsBusca* args, int numPares, int* valorIdPoPs);

/**
 * @brief Recebe e regulzariza uma string a ser inserida em um campo nome de um registro de dados.
 * 
 * @param[in, out] nome Ponteiro para o campo de interesse do registro.
 * @param[in] valor Nome a ser armazenado no campo.
 * 
 */
void atribuirNome(char** nome, char* valor);

/**
 * @brief Recebe e regulariza uma string a ser inserida no campo siglaPais de um registro de dados.
 * 
 * @param[in, out] siglaPais Campo do registro de dados.
 * @param[in] valor Sigla a ser inserida.
 * 
 */
void atribuirSigla(char* siglaPais, char* valor);

/**
 * @brief Libera a memória de um ponteiro de argsBusca.
 * 
 * @param[in] args Ponteiro a ser liberado.
 * @param[in] numPares Quantidade de duplas tipo de dado e valor respectivo armazenadas.
 * 
 */
void freeArgsBusca(argsBusca* args, int numPares);

#endif