#include "../../include/tools.h"

void contaTam(dataReg* data)
{
    data->tamanhoRegistro = 21 + (int)strlen(data->nomePops) + (int)strlen(data->nomePais);
}

void free_data_reg(dataReg* data) {

    if (data->nomePops != NULL) {

        free(data->nomePops);
        data->nomePops = NULL;
    }

    if (data->nomePais != NULL) {

        free(data->nomePais);
        data->nomePais = NULL;
    }
}

void consume_trash(FILE* arquivoEntrada, int amount) {
    char trash;

    for (int i = 0; i < amount; i++) {

        fread(&trash, sizeof(char), 1, arquivoEntrada);
    }
}

void fill_trash(FILE* arquivoEntrada, size_t ammount) {
    char trash = '$';

    for (size_t i = 0; i < ammount; i++) {

        fwrite(&trash, sizeof(char), 1, arquivoEntrada);
    }

}

void data_write(const dataReg* data, FILE* arquivoSaida)
{
    // Escrita dos registros fixos
    fwrite(&data->removido, sizeof(char), 1, arquivoSaida);
    fwrite(&data->tamanhoRegistro, sizeof(int), 1, arquivoSaida);
    fwrite(&data->encadeamentoLista, sizeof(long long int), 1, arquivoSaida);
    fwrite(&data->idPoPs, sizeof(int), 1, arquivoSaida);
    fwrite(&data->siglaPais, sizeof(char), 2, arquivoSaida);

    // Escrita dos registros variáveis
    fwrite(data->nomePops, sizeof(char), strlen(data->nomePops), arquivoSaida);
    fwrite("|", sizeof(char), 1, arquivoSaida);
    fwrite(data->nomePais, sizeof(char), strlen(data->nomePais), arquivoSaida);
    fwrite("|", sizeof(char), 1, arquivoSaida);

    int bytes_written = 19 + strlen(data->nomePops) + strlen(data->nomePais) + 2;

    char trash = '$';
    while (bytes_written < data->tamanhoRegistro) {
        fwrite(&trash, sizeof(char), 1, arquivoSaida);
        bytes_written++;
    }
}

void data_write_rem(const dataReg* data, FILE* arquivoSaida)
{
    fwrite(&data->removido, sizeof(char), 1, arquivoSaida);
    fwrite(&data->tamanhoRegistro, sizeof(int), 1, arquivoSaida);
    fwrite(&data->encadeamentoLista, sizeof(long long int), 1, arquivoSaida);

    for(int i = 0; i < data->tamanhoRegistro - 13; i++)
        fwrite("$", sizeof(char), 1, arquivoSaida);
}

void read_single_var_field(FILE* arquivoEntrada, char** field, int* total_read_bytes) {
    char byte;
    int mem_alloc = 16;
    int i = 0;

    // Aloca tamanho inicial dinâmico
    *field = (char*) malloc(mem_alloc * sizeof(char));

    // Le byte a byte até o teórico fim do arquivo.
    while (fread(&byte, sizeof(char), 1, arquivoEntrada) == 1) {
        (*total_read_bytes)++;
        
        if (byte == '|') break;
        
        (*field)[i] = byte;
        i++;

        // Se passar da capacidade, realoca o dobro
        if (i >= mem_alloc) {
            mem_alloc *= 2;
            *field = (char*) realloc(*field, mem_alloc * sizeof(char));
        }
    }
    
    // Transforma em string válida
    (*field)[i] = '\0';
}

void read_all_var_fields(dataReg* data, FILE* arquivoEntrada, int alr_bytes_readed) {
    
    int bytes_readed = alr_bytes_readed;

    // percorre nomePops
    read_single_var_field(arquivoEntrada, &data->nomePops, &bytes_readed);

    // percorre nomePais
    read_single_var_field(arquivoEntrada, &data->nomePais, &bytes_readed);

    // consome o lixo restante
    consume_trash(arquivoEntrada, data->tamanhoRegistro - bytes_readed);
}

int data_read(dataReg* data, FILE* arquivoEntrada)
{
    data->nomePops = NULL;
    data->nomePais = NULL;

    if(fread(&data->removido, sizeof(char), 1, arquivoEntrada) != 1)
        return 0;

    fread(&data->tamanhoRegistro, sizeof(int), 1, arquivoEntrada);
    fread(&data->encadeamentoLista, sizeof(long long int), 1, arquivoEntrada);
    fread(&data->idPoPs, sizeof(int), 1, arquivoEntrada);
    fread(&data->siglaPais, sizeof(char), 2, arquivoEntrada);

    if(data->removido == '1'){

        consume_trash(arquivoEntrada, data->tamanhoRegistro - 19);
        return 1;
    }
    
    read_all_var_fields(data, arquivoEntrada, 19);

    return 1;
}

dataReg data_Cria(char* idPoPs, char* siglaPais, char* nomePops, char* nomePais)
{
    dataReg data;
    data.removido = '0';
    data.encadeamentoLista = -1;
    data.idPoPs = atoi(idPoPs);

    if(strcmp(siglaPais, "NULO") == 0) 
        memcpy(data.siglaPais, "$$", 2);
    else 
        memcpy(data.siglaPais, siglaPais, 2);

    if(strcmp(nomePops, "NULO") == 0) {

        data.nomePops = (char*) malloc(1 * sizeof(char));
        data.nomePops[0] = '\0';
    } 
    else {

        data.nomePops = (char*) malloc((strlen(nomePops) + 1) * sizeof(char));
        strcpy(data.nomePops, nomePops);
    }

    if(strcmp(nomePais, "NULO") == 0) {

        data.nomePais = (char*) malloc(1 * sizeof(char));
        data.nomePais[0] = '\0';
    } 
    else {

        data.nomePais = (char*) malloc((strlen(nomePais) + 1) * sizeof(char));
        strcpy(data.nomePais, nomePais);
    }

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
            memcpy(data->siglaPais, "$$", 2);
        else
            memcpy(data->siglaPais, valorCampo, 2);
    }

    else if(strcmp(nomeCampo, "nomePoPs") == 0)
    {
        free(data->nomePops);

        if(strcmp(valorCampo, "NULO") == 0) {

            data->nomePops = (char*) malloc(1 * sizeof(char));
            data->nomePops[0] = '\0';
        } 
        
        else {

            data->nomePops = (char*) malloc((strlen(valorCampo) + 1) * sizeof(char));
            strcpy(data->nomePops, valorCampo);
        }
    }

    else if(strcmp(nomeCampo, "nomePais") == 0)
    {
        free(data->nomePais);

        if(strcmp(valorCampo, "NULO") == 0) {

            data->nomePais = (char*) malloc(1 * sizeof(char));
            data->nomePais[0] = '\0';
        } 
        
        else {

            data->nomePais = (char*) malloc((strlen(valorCampo) + 1) * sizeof(char));
            strcpy(data->nomePais, valorCampo);
        }
    }
    contaTam(data);
}

void read_reg_csv(char* buffer, dataReg* data)
{
    char* cursor = buffer;
    char* inicioCampo = buffer;

    data->removido = '0';
    data->encadeamentoLista = -1;

    for(int campo_atual = 0; campo_atual < 5; campo_atual++)
    {
        cursor = strchr(inicioCampo, ',');

        if(cursor != NULL)
        {
            *cursor = '\0';
        }

        inicioCampo[strcspn(inicioCampo, "\r\n")] = '\0';

        int campo_vazio = (inicioCampo[0] == '\0');

        switch(campo_atual)
        {
            case 0:
                break;

            case 1:
                if(!campo_vazio)
                    data->idPoPs = atoi(inicioCampo);
                else
                    data->idPoPs = -1;
                break;

            case 2:
                data->nomePops = malloc(strlen(inicioCampo) + 1);

                if(data->nomePops == NULL)
                    exit(EXIT_FAILURE);

                strcpy(data->nomePops, inicioCampo);
                break;

            case 3:
                data->nomePais = malloc(strlen(inicioCampo) + 1);

                if(data->nomePais == NULL)
                    exit(EXIT_FAILURE);

                strcpy(data->nomePais, inicioCampo);
                break;

            case 4:
                if(!campo_vazio)
                    memcpy(data->siglaPais, inicioCampo, 2);
                else
                    memcpy(data->siglaPais, "$$", 2);
                break;
        }

        if(cursor == NULL)
            break;

        inicioCampo = cursor + 1;
    }

    contaTam(data);
}

int print_reg(dataReg* data)
{
    int registro_existente = 0;

    if(data->removido == '0') 
    {
        printf("%d ", data->idPoPs);

        // Realiza o tratamento de erros, tanto para registros fixos
        // quanto para os variáveis.
        if (data->nomePops[0] == '\0') 
            printf("NULO ");
        else 
            printf("%s ", data->nomePops);
        
        if (data->nomePais[0] == '\0') 
            printf("NULO ");
        else 
            printf("%s ", data->nomePais);

        if (strncmp(data->siglaPais, "$$", 2) == 0) 
            printf("NULO\n");
        else 
            printf("%.2s\n", data->siglaPais);

         registro_existente = 1;
    }

    return registro_existente;
}

void delete_reg(dataReg* data, headerReg* header, FILE* fEntrada, long long int original_bOffset) {

    data->removido = '1';

    long long int prev_bOffset = -1;
    long long int current_bOffset = header->topoPilha;

    dataReg rem_data;

    while(current_bOffset != -1) {

        fseek(fEntrada, current_bOffset, SEEK_SET);
        data_read(&rem_data, fEntrada);

        if(data->tamanhoRegistro > rem_data.tamanhoRegistro) {

            free_data_reg(&rem_data);
            break;
        }

        prev_bOffset = current_bOffset;
        current_bOffset = rem_data.encadeamentoLista;
        free_data_reg(&rem_data);

    }

    data->encadeamentoLista = current_bOffset;

    fseek(fEntrada, original_bOffset, SEEK_SET);
    data_write_rem(data, fEntrada);

    if(prev_bOffset == -1)
        header->topoPilha = original_bOffset;

    else {

        fseek(fEntrada, prev_bOffset, SEEK_SET);
        data_read(&rem_data, fEntrada);

        rem_data.encadeamentoLista = original_bOffset;

        fseek(fEntrada, prev_bOffset, SEEK_SET);
        data_write_rem(&rem_data, fEntrada);
        free_data_reg(&rem_data);
    }

    header->nroRegRem++;
    header->nroPoPs--;

    return;

}

int get_search_mode(char* nomeCampo) 
{   
    if (strcmp(nomeCampo, "siglaPais") == 0) return 1;
    if (strcmp(nomeCampo, "nomePoPs") == 0) return 2;
    if (strcmp(nomeCampo, "nomePais") == 0) return 3;

    if (strcmp(nomeCampo, "idPoPs") == 0) return 13;

    return 0;
}

int parameter_search(dataReg* data, int search_mode, char* valorBuscado) 
{
    if(search_mode == 0)
        return 0;

    int null = (strcmp(valorBuscado, "NULO") == 0);

    switch (search_mode) 
    {
    
        case 1: // siglaPais
            if (null) 
                return (strncmp(data->siglaPais, "$$", 2) == 0);

            return (strncmp(data->siglaPais, valorBuscado, 2) == 0);

        case 2: // nomePops
            if (null) 
                return (data->nomePops[0] == '\0');

            return (strcmp(data->nomePops, valorBuscado) == 0);

        case 3: // nomePais
            if (null) 
                return (data->nomePais[0] == '\0');
                
            return (strcmp(data->nomePais, valorBuscado) == 0);

        case 13: // idPoPs (já passado pela index search)
            if (null) 
                return (data->idPoPs == -1); 

            return (data->idPoPs == atoi(valorBuscado));

        default:
            return 0;
    }
}

int validate_all_parameters(dataReg* data, int numPares, argsBusca* args) 
{
    for (int i = 0; i < numPares; i++) {

        int search_mode = get_search_mode(args[i].nomesCampo);
        
        if (search_mode != 0 && !parameter_search(data, search_mode, args[i].valoresCampo)) {
            return 0; 
        }
    }
    return 1; 
}

int complete_parameter_search(dataReg* data, headerReg* header, FILE* fEntrada, int numPares, int numAtt, argsBusca* args, argsBusca* argsAtt, int mode) {
    int found_field = 0;

    // loop externo que lê sequencialmente todos os registros
    while (data_read(data, fEntrada) != 0) {

        long long int original_bOffset = ftell(fEntrada) - data->tamanhoRegistro;
        
        
        if (data->removido == '1') {
            free_data_reg(data);
            continue;
        }

        // loop interno de verificação de todos os pares de parâmetros de busca
        if (validate_all_parameters(data, numPares, args)) {
            
            if (mode == PRINT_MODE) {
                
                print_reg(data);

                found_field = 1;
            }

            if (mode == DELETE_MODE) {

                delete_reg(data, header, fEntrada, original_bOffset);
                fseek(fEntrada, original_bOffset + data->tamanhoRegistro, SEEK_SET);

                found_field = 1;
            }

            if (mode == UPDATE_MODE)
            {
                long long int proximoOffset = ftell(fEntrada);
                int tamOriginal = data->tamanhoRegistro;
                for(int i = 0; i < numAtt; i++)
                {
                    data_Atualiza(data, argsAtt[i].nomesCampo, argsAtt[i].valoresCampo);
                }
                if(data->tamanhoRegistro <= tamOriginal)
                {
                    data->tamanhoRegistro = tamOriginal;
                    data->removido = '0';
                    data->encadeamentoLista = -1;

                    fseek(fEntrada, original_bOffset, SEEK_SET);
                    data_write(data, fEntrada);
                }
                else
                {
                    int tamanhoNovo = data->tamanhoRegistro;
                    data->tamanhoRegistro = tamOriginal;

                    delete_reg(data, header, fEntrada, original_bOffset);

                    data->tamanhoRegistro = tamanhoNovo;
                    data->removido = '0';
                    data->encadeamentoLista = -1;

                    writeDataWorst(data, header, fEntrada);
                }
                found_field = 1;
                fseek(fEntrada, proximoOffset, SEEK_SET);
            }
        }

        free_data_reg(data);
    }

    return found_field;
}

argsBusca* determinaArgsBusca(int numPares)
{
    argsBusca* args = (argsBusca*)malloc(numPares*sizeof(argsBusca));

    if(args == NULL)
        return NULL;

    for(int i = 0; i < numPares; i++)
    {
        scanf("%s", args[i].nomesCampo);
        if(strcmp(args[i].nomesCampo, "idPoPs") == 0)
            scanf("%ms", &args[i].valoresCampo);
        else
        {
            char* aux = NULL;
            size_t capacidade = 4;
            size_t tamanho = 0;
            int c;

            while((c = getchar()) != EOF &&
                (c == ' ' || c == '\n'));

            if(c == '"')
            {
                aux = malloc(capacidade);
                while((c = getchar()) != '"' && c != EOF)
                {
                    if(tamanho + 1 >= capacidade)
                    {
                        capacidade *= 2;
                        aux = realloc(aux, capacidade);
                    }
                    aux[tamanho++] = (char)c;
                }

                if(c == EOF)
                {
                    free(aux);
                    return NULL;
                }
                aux[tamanho] = '\0';
                args[i].valoresCampo = aux;
            }
            else
            {
                args[i].valoresCampo = malloc(5);
                strcpy(args[i].valoresCampo, "NULO");
                for(int j = 0; j < 3; j++)
                    c = getchar();
            }
        }
    }
    return args;
}

void readSingleInput(dataReg* data, int modo)
{
    char* aux = NULL;
    size_t capacidade = 4;
    size_t tamanho = 0;
    int c;

    while((c = getchar()) != EOF &&
                (c == ' ' || c == '\n'));

    if(c == '"')
    {
        aux = malloc(capacidade);
                while((c = getchar()) != '"' && c != EOF)
                {
                    if(tamanho + 1 >= capacidade)
                    {
                        capacidade *= 2;
                        aux = realloc(aux, capacidade);
                    }
                    aux[tamanho++] = (char)c;
                }

                if(c == EOF)
                {
                    free(aux);
                    return;
                }
                aux[tamanho] = '\0';
    }
    else
    {
        aux = malloc(5);
        for(int j = 0; j < 3; j++)
            c = getchar();
        strcpy(aux, "NULO");
    }
    if(modo == 1)
    {
        if(strcmp(aux, "NULO") == 0)
            aux[0] = '\0';

        data->nomePops = aux;
    }
    else if(modo == 2)
    {
        if(strcmp(aux, "NULO") == 0)
            aux[0] = '\0';

        data->nomePais = aux;
    }
    if(modo == 3)
    {
        if(strcmp(aux, "NULO") == 0)
            memcpy(data->siglaPais, "$$", 2);
        else
            memcpy(data->siglaPais, aux, 2);

        free(aux);
    }
    data->removido = '0';
    data->encadeamentoLista = -1;
}

void readDataInput(dataReg* data)
{
    int idPoPs = 0;
    scanf("%d", &idPoPs);
    data->idPoPs = idPoPs;

    readSingleInput(data, 1); //nome PoPs
    readSingleInput(data, 2); //nome Pais
    readSingleInput(data, 3); //Sigla
    contaTam(data);
}

void writeDataWorst(dataReg* data, headerReg* header, FILE* fSaida)
{
    fseek(fSaida, 0, SEEK_SET);

    int reutilizou = 0;

    if(header->nroRegRem > 0 && header->topoPilha != -1)
    {
        long long int offsetMaior = header->topoPilha;
        dataReg dataAux;

        fseek(fSaida, offsetMaior, SEEK_SET);

        if(!data_read(&dataAux, fSaida))
            return;

        if(dataAux.removido == '1' &&
           dataAux.tamanhoRegistro >= data->tamanhoRegistro)
        {
            header->topoPilha = dataAux.encadeamentoLista;
            header->nroRegRem--;

            data->tamanhoRegistro = dataAux.tamanhoRegistro;
            data->removido = '0';
            data->encadeamentoLista = -1;

            fseek(fSaida, offsetMaior, SEEK_SET);
            data_write(data, fSaida);

            reutilizou = 1;
        }

        free_data_reg(&dataAux);
    }

    if(!reutilizou)
    {
        fseek(fSaida, 0, SEEK_END);
        data_write(data, fSaida);
        header->proxOff = ftell(fSaida);
    }
    header->nroPoPs++;
}