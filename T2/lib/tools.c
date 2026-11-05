#include "tools.h"
#include "fornecidas.h"

headerReg header_init()
{
    headerReg header;

    header.status = '0';
    header.topoPilha = -1;
    header.proxOff = 0;
    header.nroRegRem = 0;
    header.nroPares = 0;

    return header;
}

void header_write(const headerReg* header, FILE* arquivoSaida)
{
    fwrite(&header->status, sizeof(char), 1, arquivoSaida);
    fwrite(&header->topoPilha, sizeof(int), 1, arquivoSaida);
    fwrite(&header->proxOff, sizeof(int), 1, arquivoSaida);
    fwrite(&header->nroRegRem, sizeof(int), 1, arquivoSaida);
    fwrite(&header->nroPares, sizeof(int), 1, arquivoSaida);
}

void header_read(headerReg* header, FILE* arquivoEntrada)
{
    fread(&header->status, sizeof(char), 1, arquivoEntrada);
    fread(&header->topoPilha, sizeof(int), 1, arquivoEntrada);
    fread(&header->proxOff, sizeof(int), 1, arquivoEntrada);
    fread(&header->nroRegRem, sizeof(int), 1, arquivoEntrada);
    fread(&header->nroPares, sizeof(int), 1, arquivoEntrada);

}

void data_write(const dataReg* data, FILE* arquivoSaida)
{
    fwrite(&data->removido, sizeof(char), 1, arquivoSaida);
    fwrite(&data->tamanhoRegistro, sizeof(int), 1, arquivoSaida);
    fwrite(&data->encadeamentoLista, sizeof(int), 2, arquivoSaida);
    fwrite(&data->idPoPs, sizeof(int), 1, arquivoSaida);
    fwrite(&data->siglaPais, sizeof(char), 2, arquivoSaida);
    fwrite(data->nomePops, sizeof(char), strlen(data->nomePops), arquivoSaida);
    fwrite("|", sizeof(char), 1, arquivoSaida);
    fwrite(data->nomePais, sizeof(char), strlen(data->nomePais), arquivoSaida);
    fwrite("|", sizeof(char), 1, arquivoSaida);
}

void data_write_rem(const dataReg* data, FILE* arquivoSaida)
{
    fwrite(&data->removido, sizeof(char), 1, arquivoSaida);
    fwrite(&data->tamanhoRegistro, sizeof(int), 1, arquivoSaida);
    fwrite(&data->encadeamentoLista, sizeof(int), 1, arquivoSaida);

    for(int i = 0; i < data->tamanhoRegistro - 13; i++)
        fwrite("$", sizeof(char), 1, arquivoSaida);
}

int data_read(dataReg* data, FILE* arquivoEntrada)
{
    if(fread(&data->removido, sizeof(char), 1, arquivoEntrada) != 1)
        return 0;

    fread(&data->tamanhoRegistro, sizeof(int), 1, arquivoEntrada);
    fread(&data->encadeamentoLista, sizeof(int), 1, arquivoEntrada);
    fread(&data->idPoPs, sizeof(int), 1, arquivoEntrada);
    fread(&data->siglaPais, 2, 1, arquivoEntrada);
    fwrite(&data->nomePais, sizeof(data->nomePais), 1, arquivoEntrada);
    fwrite(&data->nomePops, sizeof(data->nomePops), 1, arquivoEntrada);

    return 1;
}

void contaTam(dataReg* data)
{
    data->tamanhoRegistro = sizeof(char)
                          + sizeof(int)
                          + sizeof(long long)
                          + sizeof(int)
                          + 2 * sizeof(char)
                          + strlen(data->nomePops) + 1
                          + strlen(data->nomePais) + 1;
}

dataReg data_Cria(char* idPoPs, char* siglaPais, char* nomePops, char* nomePais)
{
    dataReg data;

    data.removido = '0';
    data.encadeamentoLista = -1;

    data.idPoPs = atoi(idPoPs);

    if(strcmp(siglaPais, "NULO") == 0)
        strcpy(data.siglaPais, "$$");
    else
        strcpy(data.siglaPais, siglaPais);

    if(strcmp(nomePops, "NULO") == 0)
        data.nomePops[0] = '\0';
    else
        strcpy(data.nomePops, nomePops);

    if(strcmp(nomePais, "NULO") == 0)
        data.nomePais[0] = '\0';
    else
        strcpy(data.nomePais, nomePais);

    contaTam(&data);

    return data;
}

void data_Atualiza(dataReg* data, char* nomeCampo, char* valorCampo)
{
    if(strcmp(nomeCampo, "idPoPs") == 0)
    {
        if(strcmp(valorCampo, "NULO") != 0)
            data->idPoPs = atoi(valorCampo);
    }

    else if(strcmp(nomeCampo, "siglaPais") == 0)
    {
        if(strcmp(valorCampo, "NULO") == 0)
            strcpy(data->siglaPais, "$$");
        else
            strcpy(data->siglaPais, valorCampo);
    }

    else if(strcmp(nomeCampo, "nomePops") == 0)
    {
        if(strcmp(valorCampo, "NULO") == 0)
            data->nomePops[0] = '\0';
        else
            strcpy(data->nomePops, valorCampo);
    }

    else if(strcmp(nomeCampo, "nomePais") == 0)
    {
        if(strcmp(valorCampo, "NULO") == 0)
            data->nomePais[0] = '\0';
        else
            strcpy(data->nomePais, valorCampo);
    }

    contaTam(data);
}

void read_reg_csv(char* buffer, dataReg* data)
{
    char* cursor = buffer;
    char* inicioCampo = buffer;

    data->removido = '0';
    data->encadeamentoLista = -1;

    for(int campo_atual = 0; campo_atual < 4; campo_atual++)
    {
        cursor = strchr(inicioCampo, ',');

        if(cursor != NULL)
        {
            *cursor = '\0';
            cursor++;
        }

        if(inicioCampo != NULL)
            inicioCampo[strcspn(inicioCampo, "\r\n")] = '\0';

        int campo_vazio = (inicioCampo == NULL || inicioCampo[0] == '\0');

        switch(campo_atual)
        {
            case 0:
                if(!campo_vazio)
                    data->idPoPs = atoi(inicioCampo);
                else
                    data->idPoPs = -1;
                break;

            case 1:
                if(!campo_vazio)
                {
                    strncpy(data->siglaPais, inicioCampo, 2);
                    data->siglaPais[2] = '\0';
                }
                else
                    strcpy(data->siglaPais, "$$");
                break;

            case 2:
                if(!campo_vazio)
                    strcpy(data->nomePops, inicioCampo);
                else
                    data->nomePops[0] = '\0';
                break;

            case 3:
                if(!campo_vazio)
                    strcpy(data->nomePais, inicioCampo);
                else
                    data->nomePais[0] = '\0';
                break;
        }

        inicioCampo = cursor;
    }

    contaTam(data);
}

int print_reg(dataReg* data)
{
    int registro_existente = 0;

    if(data->removido == '0') 
    {
        printf("%d %d ", data->idPoPs, data->idPoPsConectado);

        // Realiza o tratamento de erros, tanto para velocidade
        // quanto para unidade de medida.
        if (data->velocidade == -1) 
            printf("NULO ");
        else 
            printf("%d ", data->velocidade);

        if (data->unidadeMedida == '$') 
            printf("NULO\n");
        else 
            printf("\"%c\"\n", data->unidadeMedida);

         registro_existente = 1;
    }

    return registro_existente;
}

int get_search_mode(char* nomeCampo) 
{
    if (strcmp(nomeCampo, "idPoPs") == 0) return 1;
    if (strcmp(nomeCampo, "siglaPais") == 0) return 2;
    if (strcmp(nomeCampo, "nomePops") == 0) return 3;
    if (strcmp(nomeCampo, "nomePais") == 0) return 4;
    return 0;
}

int parameter_search(dataReg* data, int modoBusca, char* valorBuscado) 
{
    int valor;

    switch (modoBusca) 
    {
        // Buscando por idPoPs
        case 1:
            if (strcmp(valorBuscado, "NULO") == 0)
                valor = -1;
            else
                valor = atoi(valorBuscado);
            // Retorna 1 (mantém flag da funcionalidade 3 ativa) caso
            // seja de fato o valor procurado, mas desativa a flag caso
            // contrário.
            return (data->idPoPs == valor);

        // Buscando por idPoPsConectado
        case 2:
            if (strcmp(valorBuscado, "NULO") == 0)
                valor = -1;
            else
                valor = atoi(valorBuscado);

            return (data->idPoPsConectado == valor);

        // Buscando por velocidade
        case 3:
            if (strcmp(valorBuscado, "NULO") == 0)
                valor = -1;
            else
                valor = atoi(valorBuscado);

            return (data->velocidade == valor);

        // Buscando por unidadeMedida
        case 4: 
            char valorChar;

            if (strcmp(valorBuscado, "NULO") == 0 || strcmp(valorBuscado, "") == 0)
                valorChar = '$';
            else
            
                valorChar = valorBuscado[0];

            return (data->unidadeMedida == valorChar);

        default:
            return 0;
    }
}