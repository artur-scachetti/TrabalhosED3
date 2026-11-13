#include "../../include/func.h"
#include "../../include/tools.h"
#include "../../include/fornecidas.h"

void func12(char* arquivoEntrada, char* arquivoIndices, int numPares, argsBusca* args) {

    FILE* fEntrada = NULL;
    headerReg header;
    dataReg data;

    if(!read_func_init(arquivoEntrada, &fEntrada, &header)) {

        printf("Falha no processamento do arquivo.\n");
        return;
    }

    int search_type = -1;
    int valorIdPoPs = -1;

    for(int i = 0; i < numPares; i++) {

        if(strcmp(args[i].nomesCampo, "idPoPs") == 0) {

            search_type = INDEX_SEARCH;
            valorIdPoPs = atoi(args[i].valoresCampo);

            break;
        }

        else
            search_type = PARAMETER_SEARCH;
    }

    if(search_type == INDEX_SEARCH) {
        // busca por index
        index_search(valorIdPoPs, arquivoIndices, fEntrada, &data, &header, numPares, args, DELETE_MODE);
    }

    if(search_type == PARAMETER_SEARCH) {
        // busca por parâmetro
        complete_parameter_search(&data, &header, fEntrada, numPares, args, DELETE_MODE); 
    }

    return;
}