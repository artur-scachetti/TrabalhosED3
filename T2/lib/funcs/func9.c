#include "../../include/func.h"
#include "../../include/tools.h"
#include "../../include/fornecidas.h"

void func9(char* arquivoEntrada, char* arquivoIndice, int func9Mode) {

    FILE* fEntrada = NULL;
    FILE* fIndice = NULL;

    headerReg header;
    dataReg data;

    // Verifica a inicialização correta dos arquivos utilizados e do registro de cabeçalho
    if(!read_func_init(arquivoEntrada, &fEntrada, &header) || !(fIndice = fopen(arquivoIndice, "wb"))) {
        if (fEntrada != NULL) fclose(fEntrada);

        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Marca o arquivo de índice como "inconsistente"
    indexHeader idx_header;
    idx_header.status = '0';
    fwrite(&idx_header.status, sizeof(char), 1, fIndice);

    size_t capacity;

    // Guarda o tamanho da quantidade de registros a serem usados
    if (header.nroPoPs > 0) {
        capacity = (size_t)header.nroPoPs;
    } else {
        capacity = 16;
    }

    // Aloca um vetor de registros de índice, da quantidade de registros
    // de dados necessários
    indexReg* idxVect = malloc(capacity * sizeof(indexReg));
    if (idxVect == NULL) {
        fclose(fEntrada);
        fclose(fIndice);
        return;
    }

    int i = 0;
    long long int currentOffset = BYTES_PER_HEADER;

    // Percorre sequencialmente todos os registros do arquivo de dados
    while(data_read(&data, fEntrada)) {


        // Caso não removido
        if(data.removido == '0') {

            // Realoca memória para o dobro da memóra já alocada caso necessário
            if ((size_t)i == capacity) {
                capacity *= 2;
                indexReg* expanded = realloc(idxVect, capacity * sizeof(indexReg));
                if (expanded == NULL) {
                    free_data_reg(&data);
                    free(idxVect);
                    fclose(fEntrada);
                    fclose(fIndice);
                    return;
                }
                idxVect = expanded;
            }

            // Guarda em um dos registro de índice armazenados no vetor
            // as informações referentes ao registro de dados
            idxVect[i].idPops = data.idPoPs;
            idxVect[i].byteOffset = currentOffset;

            i++;
        }

        currentOffset = ftell(fEntrada);

        free_data_reg(&data);
    }   

    fclose(fEntrada);

    // Realiza o quick sort desse vetor de registros de índice
    qsort(idxVect, i, sizeof(indexReg), qsort_aux_cmp);

    // Percorre o loop escrevendo os conteúdos do vetor (que são registros de índice)
    // no arquivo de Índices
    for(int write_loop = 0; write_loop < i; write_loop++) {
        fwrite(&idxVect[write_loop].idPops, sizeof(int), 1, fIndice);
        fwrite(&idxVect[write_loop].byteOffset, sizeof(long long int), 1, fIndice);
    }

    free(idxVect);

    // Marca status do arquivo de índices como consistente
    fseek(fIndice, 0, SEEK_SET);
    idx_header.status = '1';
    fwrite(&idx_header.status, sizeof(char), 1, fIndice);

    fclose(fIndice);

    // Mostra os binários do arquivo de índice
    if(func9Mode == ESCREVE_BIN)
        BinarioNaTela(arquivoIndice);

    return;
}