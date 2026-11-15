#include "../../include/func.h"
#include "../../include/tools.h"
#include "../../include/fornecidas.h"

void func13(char* arquivoEntrada, char* arquivoIndice, int numRegistros)
{
    FILE* fEntrada = NULL;

    headerReg header;
    dataReg data;

    if(!(fEntrada = fopen(arquivoEntrada, "r+b")))
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    header_read(&header, fEntrada);
    header.status = '0';
    fseek(fEntrada, 0, SEEK_SET);
    header_write(&header, fEntrada);

    for(int i = 0; i < numRegistros; i++)
    {
        readDataInput(&data);
        writeDataWorst(&data, &header, fEntrada);
        free_data_reg(&data);
    }

    fseek(fEntrada, 0, SEEK_SET);
    header.status = '1';
    header_write(&header, fEntrada);

    fclose(fEntrada);

    func9(arquivoEntrada, arquivoIndice, 0);

    BinarioNaTela(arquivoEntrada);
    BinarioNaTela(arquivoIndice);
    return;
}