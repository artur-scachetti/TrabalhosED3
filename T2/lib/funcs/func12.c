#include "../../include/func.h"
#include "../../include/tools.h"
#include "../../include/fornecidas.h"

void func12(char* arquivoEntrada, char* arquivoIndices, int numPares, argsBusca* args) {

    headerReg header;
    dataReg data;

    FILE* fEntrada = NULL;

    if(!write_func_init(arquivoEntrada, &fEntrada, &header))
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    header.status = '0';
    fseek(fEntrada, 0, SEEK_SET);
    header_write(&header, fEntrada);

    int valorIdPoPs = -1;
    int search_type = determinaTipoBusca(args, numPares, &valorIdPoPs);

    if(search_type == INDEX_SEARCH) {
        // busca por index
        index_search(valorIdPoPs, arquivoIndices, fEntrada, &data, &header, numPares, 0, args, NULL, DELETE_MODE);
    }

    if(search_type == PARAMETER_SEARCH) {
        // busca por parâmetro
        complete_parameter_search(&data, &header, fEntrada, numPares, 0, args, NULL, DELETE_MODE);
    }

    fseek(fEntrada, 0, SEEK_SET);
    header.status = '1';
    header_write(&header, fEntrada);

    fclose(fEntrada);

    func9(arquivoEntrada, arquivoIndices, APENAS_CRIA);

    return;
}