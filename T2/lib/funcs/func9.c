#include "../../include/func.h"
#include "../../include/tools.h"
#include "../../include/fornecidas.h"

void func9(char* arquivoEntrada, char* arquivoIndice) {

    FILE* fEntrada = NULL;
    FILE* fIndice = NULL;

    headerReg header;
    dataReg data;

    if(!(fIndice = fopen(arquivoIndice, "wb")) || !read_func_init(arquivoEntrada, &fEntrada, &header)) {

        printf("Falha no processamento do arquivo.\n");
        return;
    }

    indexHeader idx_header;
    idx_header.status = '0';
    fwrite(&idx_header.status, sizeof(char), 1, fIndice);

    indexReg index;
    indexReg* idxVect = malloc(header.nroPoPs * sizeof(indexReg));

    int i = 0;
    long long int currentOffset = BYTES_PER_HEADER;

    while(data_read(&data, fEntrada)) {

        if(!(data.removido)) {

            idxVect[i].idPops = data.idPoPs;
            idxVect[i].byteOffset = currentOffset;

            i++;
        }

        currentOffset += data.tamanhoRegistro;

        free_data_reg(&data);
    }

    fclose(fEntrada);

    qsort(idxVect, i, sizeof(indexReg), qsort_aux_cmp);

    for(int write_loop = 0; write_loop < i; write_loop++) {

        fwrite(&idxVect[i].idPops, sizeof(int), 1, fIndice);
        fwrite(&idxVect[i].byteOffset, sizeof(long long int), 1, fIndice);
    }

    free(idxVect);

    fseek(fIndice, 0, SEEK_SET);
    idx_header.status = '1';
    fwrite(&idx_header.status, sizeof(char), 1, fIndice);

    fclose(fIndice);

    return;
}