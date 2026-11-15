#include "../../include/func.h"
#include "../../include/tools.h"
#include "../../include/fornecidas.h"

void func9(char* arquivoEntrada, char* arquivoIndice, int p) {

    FILE* fEntrada = NULL;
    FILE* fIndice = NULL;

    headerReg header;
    dataReg data;

    if(!read_func_init(arquivoEntrada, &fEntrada, &header) || !(fIndice = fopen(arquivoIndice, "wb"))) {
        if (fEntrada != NULL) fclose(fEntrada);

        printf("Falha no processamento do arquivo.\n");
        return;
    }

    indexHeader idx_header;
    idx_header.status = '0';
    fwrite(&idx_header.status, sizeof(char), 1, fIndice);

    size_t capacity;

    if (header.nroPoPs > 0) {
        capacity = (size_t)header.nroPoPs;
    } else {
        capacity = 16;
    }

    indexReg* idxVect = malloc(capacity * sizeof(indexReg));
    if (idxVect == NULL) {
        fclose(fEntrada);
        fclose(fIndice);
        return;
    }

    int i = 0;
    long long int currentOffset = BYTES_PER_HEADER;

    while(data_read(&data, fEntrada)) {

        if(data.removido == '0') {
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

            idxVect[i].idPops = data.idPoPs;
            idxVect[i].byteOffset = currentOffset;

            i++;
        }

        currentOffset = ftell(fEntrada);

        free_data_reg(&data);
    }   

    fclose(fEntrada);

    qsort(idxVect, i, sizeof(indexReg), qsort_aux_cmp);

    for(int write_loop = 0; write_loop < i; write_loop++) {
        fwrite(&idxVect[write_loop].idPops, sizeof(int), 1, fIndice);
        fwrite(&idxVect[write_loop].byteOffset, sizeof(long long int), 1, fIndice);
    }

    free(idxVect);

    fseek(fIndice, 0, SEEK_SET);
    idx_header.status = '1';
    fwrite(&idx_header.status, sizeof(char), 1, fIndice);

    fclose(fIndice);

    if(p == 1)
        BinarioNaTela(arquivoIndice);

    return;
}