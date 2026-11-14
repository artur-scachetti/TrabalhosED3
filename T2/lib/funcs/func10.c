#include "../../include/func.h"
#include "../../include/tools.h"
#include "../../include/fornecidas.h"

void func10(char* arquivoEntrada) {

    FILE* fEntrada = NULL;
    headerReg header;
    int registro_existente = 0;

    if(!read_func_init(arquivoEntrada, fEntrada, &header)) {

        printf("Falha no processamento do arquivo.\n");
        return;
    }

    dataReg data;

    // Entra no loop de leitura sequencial do arquivo binário
    while(data_read(&data, fEntrada)){
        
        // Formata e printa as informações resgatadas na 
        // struct de dados
        if(print_reg(&data))
            registro_existente = 1;

    }

    if(registro_existente == 0) 
        printf("Registro inexistente.\n");
    
    fclose(fEntrada);

    return;
}