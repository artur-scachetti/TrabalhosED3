#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BYTES_PER_REG   13

typedef struct {

    char status;
    int topoPilha;
    int proxRRN;
    int nroRegRem;
    int nroPares;

} headerReg;

typedef struct {

    char removido;
    int encadeamentoPilha;
    int idPoPs;
    int idPoPsConectado;
    int velocidade;
    char unidadeMedida;

} dataReg;

typedef struct {

    char nomesCampo[100];
    char valoresCampo[100];

} argsBusca;

headerReg header_init();
void header_write(headerReg* header, FILE* arquivoSaida);
void header_read(headerReg* header, FILE* arquivoEntrada);

void data_write(dataReg* data, FILE* arquivoSaida);
int data_read(dataReg* data, FILE* arquivoEntrada);

void read_reg_csv(char* buffer, dataReg *data, FILE* arquivoSaida);
int print_reg(dataReg* data);

int parameter_search(dataReg* data, int modoBusca, char* valorBuscado);

#endif