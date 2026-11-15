#include "../../include/func.h"
#include "../../include/tools.h"
#include "../../include/fornecidas.h"

void func8(char* arquivoEntrada, char* arquivoSaida)
{
    FILE* fEntrada = NULL;
    FILE* fSaida = NULL;
    dataReg data;

    if(!(fEntrada = fopen(arquivoEntrada, "r")) || !(fSaida = fopen(arquivoSaida, "wb"))) {
        printf("Falha no processamento do arquivo.\n");
        return;
    }
    headerReg header = header_init();
    header_write(&header, fSaida);

    char buffer[2000];

    int nroPoPs = 0;
    fgets(buffer, sizeof(buffer), fEntrada);

    while(fgets(buffer, sizeof(buffer), fEntrada) != NULL)
    {
        read_reg_csv(buffer, &data);
        data_write(&data, fSaida);
        free_data_reg(&data);
        nroPoPs++;
    }
    int offset = ftell(fSaida);
    fseek(fSaida, 0 , SEEK_SET);

    header.status = '1';
    header.nroPoPs = nroPoPs;
    header.proxOff = offset;
    header_write(&header, fSaida);

    fclose(fEntrada);
    fclose(fSaida);

    BinarioNaTela(arquivoSaida);
}