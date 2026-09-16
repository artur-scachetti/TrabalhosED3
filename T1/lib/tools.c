#include "tools.h"
#include "fornecidas.h"
#include "fornecidas.h"

headerReg header_init()
{
    headerReg header;

    header.status = '0';
    header.topoPilha = -1;
    header.proxRRN = 0;
    header.nroRegRem = 0;
    header.nroPares = 0;

    return header;
}

void header_write(const headerReg* header, FILE* arquivoSaida)
{
    fwrite(&header->status, sizeof(char), 1, arquivoSaida);
    fwrite(&header->topoPilha, sizeof(int), 1, arquivoSaida);
    fwrite(&header->proxRRN, sizeof(int), 1, arquivoSaida);
    fwrite(&header->nroRegRem, sizeof(int), 1, arquivoSaida);
    fwrite(&header->nroPares, sizeof(int), 1, arquivoSaida);
}

void header_read(headerReg* header, FILE* arquivoEntrada)
{
    fread(&header->status, sizeof(char), 1, arquivoEntrada);
    fread(&header->topoPilha, sizeof(int), 1, arquivoEntrada);
    fread(&header->proxRRN, sizeof(int), 1, arquivoEntrada);
    fread(&header->nroRegRem, sizeof(int), 1, arquivoEntrada);
    fread(&header->nroPares, sizeof(int), 1, arquivoEntrada);

}

void data_write(const dataReg* data, FILE* arquivoSaida)
{
    fwrite(&data->removido, sizeof(char), 1, arquivoSaida);
    fwrite(&data->encadeamentoPilha, sizeof(int), 1, arquivoSaida);
    fwrite(&data->idPoPs, sizeof(int), 1, arquivoSaida);
    fwrite(&data->idPoPsConectado, sizeof(int), 1, arquivoSaida);
    fwrite(&data->velocidade, sizeof(int), 1, arquivoSaida);
    fwrite(&data->unidadeMedida, sizeof(char), 1, arquivoSaida);
}

void data_write_rem(const dataReg* data, FILE* arquivoSaida)
{
    fwrite(&data->removido, sizeof(char), 1, arquivoSaida);
    fwrite(&data->encadeamentoPilha, sizeof(int), 1, arquivoSaida);
    fwrite("$$$$$$$$$$$$$", 13*sizeof(char), 1, arquivoSaida);
}

int data_read(dataReg* data, FILE* arquivoEntrada)
{
    if(fread(&data->removido, sizeof(char), 1, arquivoEntrada) != 1)
        return 0;

    fread(&data->encadeamentoPilha, sizeof(int), 1, arquivoEntrada);
    fread(&data->idPoPs, sizeof(int), 1, arquivoEntrada);
    fread(&data->idPoPsConectado, sizeof(int), 1, arquivoEntrada);
    fread(&data->velocidade, sizeof(int), 1, arquivoEntrada);
    fread(&data->unidadeMedida, sizeof(char), 1, arquivoEntrada);

    return 1;
}

dataReg data_Cria(char* idPoPs, char* idPoPsConectado, char* velocidade, char* unidadeMedida)
{
    dataReg data;

    data.removido = '0';
    data.encadeamentoPilha = -1;

    if(strcmp(idPoPs, "NULO") == 0)
        data.idPoPs = -1;
    else  
        data.idPoPs = atoi(idPoPs);

    if(strcmp(idPoPsConectado, "NULO") == 0)
        data.idPoPsConectado = -1;
    else  
        data.idPoPsConectado = atoi(idPoPsConectado);

    if(strcmp(velocidade, "NULO") == 0)
        data.velocidade = -1;
    else  
        data.velocidade = atoi(velocidade);
    
    if (strcmp(unidadeMedida, "") == 0) 
        strcpy(&data.unidadeMedida, "$");
    else 
        data.unidadeMedida = unidadeMedida[0];

    return data;
}

void data_Atualiza(dataReg* data, char* nomeCampo, char* valorCampo)
{
    if(strcmp(nomeCampo, "idPoPs") == 0)
    {
        if(strcmp(valorCampo, "NULO") == 0)
            data->idPoPs = -1;
        else  
            data->idPoPs = atoi(valorCampo);
    }
    if(strcmp(nomeCampo, "idPoPsConectado") == 0)
    {
        if(strcmp(valorCampo, "NULO") == 0)
            data->idPoPsConectado = -1;
        else  
            data->idPoPsConectado = atoi(valorCampo);
    }
    if(strcmp(nomeCampo, "velocidade") == 0)
    {
        if(strcmp(valorCampo, "NULO") == 0)
            data->velocidade = -1;
        else  
            data->velocidade = atoi(valorCampo);
    }
    if(strcmp(nomeCampo, "unidadeMedida") == 0)
    {
        if (strcmp(valorCampo, "") == 0) 
            data->unidadeMedida = '$';
        else 
            data->unidadeMedida = valorCampo[0];
    }
}

void read_reg_csv(char* buffer, dataReg *data)
{
    char *cursor = buffer;
    char *inicioCampo = buffer;

    data->removido = '0'; 
    data->encadeamentoPilha = -1;

    // Avança os cursores 3 vezes (pois são 4 campos a serem lidos).
    // Isso leva em conta que o primeiro campo já é lido na inicialização.
    for(int campo_atual = 0; campo_atual < 4; campo_atual++)
    {
        // Avança um dos cursores até a próxima ocorrência de ';'
        // Substitui o ponto e virgula por '\0' (criação de substring).
        cursor = strchr(cursor, ',');
        if(cursor != NULL)
        {
            *cursor = '\0';
            cursor++;
        }

        int campo_vazio = (inicioCampo == NULL || inicioCampo[0] == '\0' || inicioCampo[0] == '\n' || inicioCampo[0] == '\r');

        switch(campo_atual)
        {
            case(0):
                // Em caso de campo não vazio, transfere o conteúdo
                // da substring para o campo da estrutura de dados.
                if(!campo_vazio)
                    data->idPoPs = atoi(inicioCampo);
                else 
                    data->idPoPs = -1;

                break;

            case(1):
                if(!campo_vazio)
                    data->idPoPsConectado = atoi(inicioCampo);
                else 
                    data->idPoPsConectado = -1;

                break;

            case(2):
                if(!campo_vazio)
                    data->velocidade = atoi(inicioCampo);
                else 
                    data->velocidade = -1;

                break;

            case(3):
                if(!campo_vazio)
                    data->unidadeMedida = inicioCampo[0];
                else 
                    data->unidadeMedida = '$';

                break;
        }

        // Volta a sincronizar os cursores.
        inicioCampo = cursor;
    }
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