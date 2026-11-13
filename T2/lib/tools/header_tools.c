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

int read_func_init(char* arquivo, FILE** f, const headerReg* header) {

    int status_ok = 1;

    // Verifica se o arquivo de entrada existe
    if((f = fopen(arquivo, "rb")) == NULL) {

        status_ok = 0;
        return status_ok;
    }

    // Verifica se o cabeçalho está consistente
    if(header_read(&header, f) == 0) {

        fclose(f);
        status_ok = 0;
        return status_ok;
    }
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
    fread(&header->status, sizeof(char), 1, arquivoEntrada);
    fread(&header->topoPilha, sizeof(long long int), 1, arquivoEntrada);
    fread(&header->proxOff, sizeof(long long int), 1, arquivoEntrada);
    fread(&header->nroRegRem, sizeof(int), 1, arquivoEntrada);
    fread(&header->nroPoPs, sizeof(int), 1, arquivoEntrada);

    if(&header->status == '0')
        return 0;

    else
        return 1;
}