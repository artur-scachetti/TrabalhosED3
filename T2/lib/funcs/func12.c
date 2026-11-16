#include "../../include/func.h"
#include "../../include/tools.h"
#include "../../include/fornecidas.h"

void func12(char* arquivoEntrada, char* arquivoIndices, int numPares, argsBusca* args) {

    headerReg header;
    dataReg data;

    FILE* fEntrada = NULL;

    // Verifica a inicialização correta dos arquivos utilizados e do registro de cabeçalho
    if(!write_func_init(arquivoEntrada, &fEntrada, &header))
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Marca o arquivo de dados como inconsistente
    header.status = '0';
    fseek(fEntrada, 0, SEEK_SET);
    header_write(&header, fEntrada);

    int valorIdPoPs = -1;

    // Determina se deve ser realizada uma busca por Indices ou uma por Parâmetros
    int search_type = determinaTipoBusca(args, numPares, &valorIdPoPs);

    if(search_type == INDEX_SEARCH) {
        // busca por index (em delete mode)
        index_search(valorIdPoPs, arquivoIndices, fEntrada, &data, &header, numPares, 0, args, NULL, DELETE_MODE);
    }

    if(search_type == PARAMETER_SEARCH) {
        // busca por parâmetro (em delete mode)
        complete_parameter_search(&data, &header, fEntrada, numPares, 0, args, NULL, DELETE_MODE);
    }

    // Marca o arquivo de dados como consistente
    fseek(fEntrada, 0, SEEK_SET);
    header.status = '1';
    header_write(&header, fEntrada);

    fclose(fEntrada);

    // Apenas cria o arquivo de índices ordenados
    func9(arquivoEntrada, arquivoIndices, APENAS_CRIA);

    return;
}