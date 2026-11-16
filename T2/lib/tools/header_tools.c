#include "../../include/tools.h"

headerReg header_init()
{
    headerReg header;
    header.status = '0';
    header.topoPilha = -1;
    header.proxOff = 0;
    header.nroRegRem = 0;
    header.nroPoPs = 0;
    return header;
}

int read_func_init(char* arquivo, FILE** f, headerReg* header)
{
    *f = fopen(arquivo, "rb");
    if (*f == NULL)
        return 0;

    if (!header_read(header, *f)) {
        fclose(*f);
        *f = NULL;
        return 0;
    }
    return 1;
}

int write_func_init(char* arquivo, FILE** f, headerReg* header)
{
    *f = fopen(arquivo, "r+b");

    if(*f == NULL)
        return 0;

    if(!header_read(header, *f))
    {
        fclose(*f);
        *f = NULL;
        return 0;
    }

    return 1;
}

void header_write(const headerReg* header, FILE* arquivoSaida)
{
    fwrite(&header->status, sizeof(char), 1, arquivoSaida);
    fwrite(&header->topoPilha, sizeof(long long int), 1, arquivoSaida);
    fwrite(&header->proxOff, sizeof(long long int), 1, arquivoSaida);
    fwrite(&header->nroRegRem, sizeof(int), 1, arquivoSaida);
    fwrite(&header->nroPoPs, sizeof(int), 1, arquivoSaida);
}

int header_read(headerReg* header, FILE* arquivoEntrada)
{
    if (fread(&header->status, sizeof(char), 1, arquivoEntrada) != 1 ||
        fread(&header->topoPilha, sizeof(long long int), 1, arquivoEntrada) != 1 ||
        fread(&header->proxOff, sizeof(long long int), 1, arquivoEntrada) != 1 ||
        fread(&header->nroRegRem, sizeof(int), 1, arquivoEntrada) != 1 ||
        fread(&header->nroPoPs, sizeof(int), 1, arquivoEntrada) != 1)
        return 0;
    return header->status == '1';
}
