#include "func.h"
#include "tools.h"

void func_1(char* arquivoEntrada, char* arquivoSaida){

    FILE* fEntrada;
    FILE* fSaida;

    if ((fEntrada = fopen(arquivoEntrada, "r")) != NULL 
            && (fSaida = fopen(arquivoSaida, "wb")) != NULL) {

        char buffer[2000];

        headerReg header = header_init();
        header_write(&header, fSaida);

        fgets(buffer, sizeof(buffer), fEntrada);

        dataReg data;
        
        int registros = 0;

        while(fgets(*buffer, sizeof(buffer), fEntrada) != NULL) {

            read_reg_csv(buffer, &data, fSaida);
            data_write(&data, fSaida);
            registros++;

        }
        fseek(fSaida, 0, SEEK_SET);

        header.status = '1';
        header.nroPares = registros;
        header_write(&header, fSaida);

        // USAR A FUNÇÂO BINARIO NA TELA QUANDO ELA LIBERAR NO MOODLE !!!!!!!!!!!!!!!!!!!!!!!!
    }
    else {
        if(fEntrada == NULL) printf("Open no arquivo de entrada falhou\n");
        if(fSaida == NULL) printf("Open no arquivo de saida falhou\n");
    }

    fclose(fEntrada);
    fclose(fSaida);

    return;
}

void func2(char* arquivoEntrada){

    FILE* fEntrada;

    headerReg header;

    int registro_existente = 0;

    if ((fEntrada = fopen(arquivoEntrada, "rb")) != NULL) {

        header_read(&header, fEntrada);

        if (header.status == '0') {
        printf("Falha no processamento do arquivo.\n");

        fclose(fEntrada);
        return;
        }
        
        dataReg data;
        while(data_read(&data, fEntrada)) {
                
            registro_existente = print_reg(&data);

        }

        if(registro_existente == 0)
            printf("Registro inexistente.\n");

        fclose(fEntrada);
    }
    else printf("Falha no processamento do arquivo.");

    return;
}

void func3(char* arquivoEntrada, int numPares, argsBusca* args) {

    FILE* fEntrada;
    headerReg header;
    dataReg data;

    if ((fEntrada = fopen(arquivoEntrada, "rb")) != NULL) {

        header_read(&header, fEntrada);
        if (header.status == '0') {
            printf("Falha no processamento do arquivo.\n");
            fclose(fEntrada);
            return;
        }

        int encontrou = 0;

        while (data_read(&data, fEntrada) != 0) {

            if (data.removido == '0') {
                int sucesso = 1;

                for (int i = 0; i < numPares; i++) {

                    char* nomeBusca = args[i].nomesCampo;
                    char* valorBusca = args[i].valoresCampo;

                    int modoBusca = 0;

                    if (strcmp(nomeBusca, "idPoPs") == 0) 
                        modoBusca = 1;

                    else if (strcmp(nomeBusca, "idPoPsConectado") == 0) 
                        modoBusca = 2;

                    else if (strcmp(nomeBusca, "velocidade") == 0) 
                        modoBusca = 3;

                    else if (strcmp(nomeBusca, "unidadeMedida") == 0) 
                        modoBusca = 4;

                    else {
                        printf("Falha no processamento do arquivo.\n");
                        fclose(fEntrada);
                        return;
                    }

                    if (!parameter_search(&data, modoBusca, valorBusca)) {
                        sucesso = 0;
                        break;
                    }
                }

                if (sucesso == 1) {
                    print_reg(&data);
                    encontrou = 1;
                }
            }
        }

        if (encontrou == 0) {
            printf("Registro Inexistente.\n");
        }

        fclose(fEntrada);

    } else {
        printf("Falha no processamento do arquivo.\n");
    }
}

void func4(char* arquivoEntrada, int RRN){

    FILE* fEntrada;

    dataReg data;

    int registro_existente = 0;

    int byteOffset = RRN * BYTES_PER_REG;

    if((fEntrada = fopen(arquivoEntrada, "rb")) != NULL){

        if(fseek(fEntrada, byteOffset, SEEK_SET) != NULL){

            data_read(&data, fEntrada);

            registro_existente = print_reg(&data);
        }

        if(registro_existente == 0)
            printf("Registro inexistente.\n");

        fclose(fEntrada);
    }
    else printf("Falha no processamento do arquivo.");

    return;

}