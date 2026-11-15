#include "../../include/func.h"
#include "../../include/tools.h"
#include "../../include/fornecidas.h"

void func11(char* arquivoEntrada, char* arquivoIndice, int numPares, argsBusca* args) 
{
    FILE* fEntrada = NULL;

    headerReg header;
    dataReg data;

    if(!read_func_init(arquivoEntrada, &fEntrada, &header)) {

        printf("Falha no processamento do arquivo.\n");
        return;
    }
     
    int search_type = -1;
    int valorIdPoPS = -1;

    for(int i = 0; i < numPares; i++) {

        if(strcmp(args[i].nomesCampo, "idPoPs") == 0) {

            search_type = INDEX_SEARCH;
            valorIdPoPS = atoi(args[i].valoresCampo);

            break;
        }

        else
            search_type = PARAMETER_SEARCH;
    }

    if(search_type == INDEX_SEARCH) {
        // busca por index
        if(!index_search(valorIdPoPS, arquivoIndice, fEntrada, &data, &header, numPares, 0,  args, NULL, PRINT_MODE))
            printf("Registro inexistente.\n");
    }

    if(search_type == PARAMETER_SEARCH) {
        // busca por parâmetro
        if(!complete_parameter_search(&data, &header, fEntrada, numPares, 0, args, NULL, PRINT_MODE))
            printf("Registro inexistente.\n"); 

    }
    
    fclose(fEntrada);
    return;
}       