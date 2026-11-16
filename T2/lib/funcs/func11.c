#include "../../include/func.h"
#include "../../include/tools.h"
#include "../../include/fornecidas.h"

void func11(char* arquivoEntrada, char* arquivoIndice, int numPares, argsBusca* args) 
{
    FILE* fEntrada = NULL;

    headerReg header;
    dataReg data;

    // Verifica a inicialização correta dos arquivos utilizados e do registro de cabeçalho
    if(!read_func_init(arquivoEntrada, &fEntrada, &header)) 
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    
    int valorIdPoPs = -1;

    // Determina se deve ser realizada uma busca por Indices ou uma por Parâmetros
    int search_type = determinaTipoBusca(args, numPares, &valorIdPoPs);

    if(search_type == INDEX_SEARCH) {
        // busca por index (em print mode)
        if(!index_search(valorIdPoPs, arquivoIndice, fEntrada, &data, &header, numPares, 0,  args, NULL, PRINT_MODE))
            printf("Registro inexistente.\n");
    }

    if(search_type == PARAMETER_SEARCH) {
        // busca por parâmetros (em print mode)
        if(!complete_parameter_search(&data, &header, fEntrada, numPares, 0, args, NULL, PRINT_MODE))
            printf("Registro inexistente.\n"); 

    }
    
    fclose(fEntrada);
    return;
}       