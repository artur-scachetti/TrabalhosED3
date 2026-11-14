#include "../../include/tools.h"

/** Refere-se ao número de bytes presentes em um registro de header
 *  idPoPs + byteOffset
 *    4    +     8      =  12
 */
#define BYTES_PER_IDX_REG       12

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

int qsort_aux_cmp(const void* a, const void* b) {
    const indexReg* idxA = (const indexReg*)a;
    const indexReg* idxB = (const indexReg*)b;

    if (idxA->idPops < idxB->idPops) return -1;
    if (idxA->idPops > idxB->idPops) return 1;
    return 0;
}

void index_write(indexReg* index, dataReg* data, FILE* fIndice, int bOffset) {

    index->idPops = data->idPoPs;
    index->byteOffset = (long long int) bOffset;

    fwrite(index->idPops, sizeof(int), 1, fIndice);
    fwrite(index->byteOffset, sizeof(long long int), 1, fIndice);
    
}

int bsearch_aux_cmp(const void* target, const void* element) {

    int target_id = *(const int*)target;
    const indexReg* reg = (const indexReg*)element;

    if (target_id < reg->idPops) return -1;
    if (target_id > reg->idPops) return 1;

    return 0;
}

int index_search(int target_id, char* arquivoIndices, FILE* fEntrada, dataReg* data, headerReg* header, int numPares, argsBusca* args, int mode) 
{
    FILE* fIndice = fopen(arquivoIndices, "rb");

    if (!fIndice) 
        return 0;

    char status;
    fread(&status, sizeof(char), 1, fIndice);
    
    if (status == '0') {
        fclose(fIndice);
        return 0; 
    }

    int numRegs = header->nroPoPs;
    indexReg* idxVect = (indexReg*) malloc(numRegs * sizeof(indexReg));
    
    for (int i = 0; i < numRegs; i++) {
        fread(&idxVect[i].idPops, sizeof(int), 1, fIndice);
        fread(&idxVect[i].byteOffset, sizeof(long long int), 1, fIndice);
    }
    fclose(fIndice);

    indexReg* result = (indexReg*) bsearch(&target_id, idxVect, numRegs, sizeof(indexReg), bsearch_aux_cmp);

    int found_field = 0;

    if (result == NULL)
        return 0;

    fseek(fEntrada, result->byteOffset, SEEK_SET);
    data_read(data, fEntrada);

    if (data->removido == '0') {
        
        if (validate_all_parameters(data, numPares, args)) {
            
            if (mode == PRINT_MODE)
                print_reg(data);

            if (mode == DELETE_MODE)
                delete_reg(data, header, fEntrada, result->byteOffset);

            found_field = 1;
        }
    }

    free_data_reg(data);
    free(idxVect);

    return found_field;
}
