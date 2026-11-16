#include "../../include/func.h"
#include "../../include/tools.h"
#include "../../include/fornecidas.h"

void func14(char* arquivoEntrada, char* arquivoIndices, argsBusca* args, argsBusca* argsAtt, int numParesBusca, int numParesAtt)
{
    headerReg header;
    dataReg data;

    FILE* fEntrada = fopen(arquivoEntrada, "r+b");

    if(!write_func_init(arquivoEntrada, &fEntrada, &header))
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    header.status = '0';
    fseek(fEntrada, 0, SEEK_SET);
    header_write(&header, fEntrada);

    int valorIdPoPs = -1;
    int search_type = determinaTipoBusca(args, numParesBusca, &valorIdPoPs);

    if(search_type == INDEX_SEARCH) {
        // busca por index
        if(!index_search(valorIdPoPs, arquivoIndices, fEntrada, &data, &header, numParesBusca, numParesAtt, args, argsAtt, UPDATE_MODE))
            printf("Registro inexistente.\n");
    }

    if(search_type == PARAMETER_SEARCH) {
        // busca por parâmetro
        if(!complete_parameter_search(&data, &header, fEntrada, numParesBusca, numParesAtt, args, argsAtt, UPDATE_MODE))
            printf("Registro inexistente.\n");
    }

    fseek(fEntrada, 0, SEEK_SET);
    header.status = '1';
    header_write(&header, fEntrada);

    fclose(fEntrada);

    func9(arquivoEntrada, arquivoIndices, APENAS_CRIA);
    
    return;
}