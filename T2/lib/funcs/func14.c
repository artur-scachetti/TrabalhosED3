#include "../../include/func.h"
#include "../../include/tools.h"
#include "../../include/fornecidas.h"

void func14(char* arquivoEntrada, char* arquivoIndices, argsBusca* args, argsBusca* argsAtt, int numParesBusca, int numParesAtt)
{
    headerReg header;
    dataReg data;

    FILE* fEntrada = fopen(arquivoEntrada, "r+b");

    if(fEntrada == NULL || !header_read(&header, fEntrada))
    {
        if(fEntrada != NULL)
            fclose(fEntrada);

        printf("Falha no processamento do arquivo.\n");
        return;
    }

    header.status = '0';
    fseek(fEntrada, 0, SEEK_SET);
    header_write(&header, fEntrada);

    int search_type = -1;
    int valorIdPoPs = -1;

    for(int i = 0; i < numParesBusca; i++) {

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
        index_search(valorIdPoPs, arquivoIndices, fEntrada, &data, &header, numParesBusca, numParesAtt, args, argsAtt, UPDATE_MODE);
    }

    if(search_type == PARAMETER_SEARCH) {
        // busca por parâmetro
        complete_parameter_search(&data, &header, fEntrada, numParesBusca, numParesAtt, args, argsAtt, UPDATE_MODE); 
    }

    fseek(fEntrada, 0, SEEK_SET);
    header.status = '1';
    header_write(&header, fEntrada);

    fclose(fEntrada);

    func9(arquivoEntrada, arquivoIndices, 0);
    
    return;
}