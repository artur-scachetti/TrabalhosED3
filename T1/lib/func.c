#include "../include/func.h"
#include "../include/tools.h"
#include "../include/fornecidas.h"

void func1(char* arquivoEntrada, char* arquivoSaida)
{
    FILE* fEntrada = NULL;
    FILE* fSaida = NULL;
    if ((fEntrada = fopen(arquivoEntrada, "r")) != NULL && (fSaida = fopen(arquivoSaida, "wb")) != NULL) 
    {
        char buffer[2000];

        headerReg header = header_init();
        header_write(&header, fSaida);

        fgets(buffer, sizeof(buffer), fEntrada);

        dataReg data;
        
        int registros = 0;
        
        // Percorre-se o arquivo .csv inteiro, armazenando-o na string 'buffer'.
        while(fgets(buffer, sizeof(buffer), fEntrada) != NULL) 
        {
            read_reg_csv(buffer, &data);
            data_write(&data, fSaida);
            registros++;
        }
        fseek(fSaida, 0, SEEK_SET);

        // Em caso de sucesso, marca o arquivo como consistente e atualiza o 
        // número de registros presentes.
        header.status = '1';
        header.nroPares = registros;
        header_write(&header, fSaida);
        fclose(fEntrada);
        fclose(fSaida);
        BinarioNaTela(arquivoSaida);
    }
    else 
        if(fEntrada == NULL || fSaida == NULL) printf("Falha no processamento do arquivo.\n");

    return;
}

void func2(char* arquivoEntrada)
{
    FILE* fEntrada = NULL;
    headerReg header;

    int registro_existente = 0;

    if ((fEntrada = fopen(arquivoEntrada, "rb")) != NULL) 
    {
        header_read(&header, fEntrada);

        if (header.status == '0') 
        {
        printf("Falha no processamento do arquivo.\n");

        fclose(fEntrada);
        return;
        }
        
        dataReg data;
        while(data_read(&data, fEntrada)) 
        {
            registro_existente = print_reg(&data);
        }

        if(registro_existente == 0)
            printf("Registro inexistente.\n");

        fclose(fEntrada);
    }
    else 
        printf("Falha no processamento do arquivo.");

    return;
}

void func3(char* arquivoEntrada, int numPares, argsBusca* args) 
{
    FILE* fEntrada = NULL;
    headerReg header;
    dataReg data;

    if ((fEntrada = fopen(arquivoEntrada, "rb")) != NULL) 
    {
        header_read(&header, fEntrada);
        if (header.status == '0') 
        {
            printf("Falha no processamento do arquivo.\n");
            fclose(fEntrada);
            return;
        }

        int encontrou = 0;

        while (data_read(&data, fEntrada) != 0) 
        {
            if (data.removido == '0') 
            {
                int sucesso = 1;

                // Realiza a busca no registro para TODOS os critérios.
                // Apenas se NENHUM falhar que o registro analisado é printado.
                for (int i = 0; i < numPares; i++) 
                {
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

                    else 
                    {
                        printf("Falha no processamento do arquivo.\n");
                        fclose(fEntrada);
                        return;
                    }
                    
                    // Basta que um dos critérios falhem para que este bloco seja executado
                    // o que quebra o loop com resultado de falha.
                    if (!parameter_search(&data, modoBusca, valorBusca)) 
                    {
                        sucesso = 0;
                        break;
                    }
                }

                // Apenas executado se todos os critérios forem satisfeitos.
                if (sucesso == 1) 
                {
                    print_reg(&data);
                    encontrou = 1;
                }
            }
        }

        if (encontrou == 0) 
            printf("Registro Inexistente.\n");

        fclose(fEntrada);

    } else 
        printf("Falha no processamento do arquivo.\n");

    printf("\n");
}

void func4(char* arquivoEntrada, int RRN)
{
    FILE* fEntrada = NULL;
    dataReg data;

    int registro_existente = 0;

    // Fórmula matemática que usa o tamanho do registro e o
    // número do registro que se quer recuperar.
    int byteOffset = RRN * BYTES_PER_REG;

    if((fEntrada = fopen(arquivoEntrada, "rb")) != 0)
    {
        if(fseek(fEntrada, byteOffset, SEEK_SET) != 0)
        {
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

void func5(char* arquivoEntrada, int numRem) 
{
    FILE* fEntrada = NULL;
    headerReg header;

    if ((fEntrada = fopen(arquivoEntrada, "r+b")) != NULL) 
    {
        header_read(&header, fEntrada);
        if (header.status == '0') 
        {
            printf("Falha no processamento do arquivo.\n");
            fclose(fEntrada);
            return;
        }

        header.status = 0;
        fseek(fEntrada, 0, SEEK_SET);
        header_write(&header, fEntrada);
    
        for(int i = 0; i < numRem; i++)
        {
            int numPares = 0;
            scanf("%d", &numPares);

            argsBusca args[numPares];

            for(int j = 0; j < numPares; j++)
                scanf("%s %s", args[j].nomesCampo, args[j].valoresCampo);
            
            fseek(fEntrada, 17, SEEK_SET);

            dataReg data;
            int RRN = 0;

            while(data_read(&data, fEntrada) != 0) 
            {
                if(data.removido == '0')
                {
                    int sucesso = 1;
                    for(int j = 0; j < numPares; j++)
                    {
                        int modoBusca = 0;
                        if (strcmp(args[j].nomesCampo, "idPoPs") == 0) 
                            modoBusca = 1;

                        else if (strcmp(args[j].nomesCampo, "idPoPsConectado") == 0) 
                            modoBusca = 2;

                        else if (strcmp(args[j].nomesCampo, "velocidade") == 0) 
                            modoBusca = 3;

                        else if (strcmp(args[j].nomesCampo, "unidadeMedida") == 0) 
                            modoBusca = 4;

                        if (!parameter_search(&data, modoBusca, args[j].valoresCampo))
                        {
                            sucesso = 0;
                            break;
                        }
                    }

                    if(sucesso == 1)
                    {
                        data.removido = '1';
                        data.encadeamentoPilha = header.topoPilha;
                        header.topoPilha = RRN;
                        header.nroRegRem++;
                        
                        fseek(fEntrada, -18, SEEK_CUR);
                        data_write_rem(&data, fEntrada);
                    }
                }
                RRN++;
            }
        }

        fseek(fEntrada, 0, SEEK_SET);
        header.status = '1';
        header_write(&header, fEntrada);
        fclose(fEntrada);

        BinarioNaTela(arquivoEntrada);
    }
    else
        printf("Falha no processamento do arquivo.\n");
}
        
void func6(char* arquivoEntrada, int numEntradas)
{
    FILE* fEntrada = NULL;
    headerReg header;

    if ((fEntrada = fopen(arquivoEntrada, "r+b")) != NULL) 
    {
        header_read(&header, fEntrada);
        if (header.status == '0') 
        {
            printf("Falha no processamento do arquivo.\n");
            fclose(fEntrada);
            return;
        }
        header.status = '0';
        fseek(fEntrada, 0, SEEK_SET);
        header_write(&header, fEntrada);

        for(int i = 0; i < numEntradas; i++)
        {
            char idPops[50], idPopsConectado[50], velocidade[50], unidadeMedida[50];

            scanf("%s %s %s", idPops, idPopsConectado, velocidade);
            ScanQuoteString(unidadeMedida);

            dataReg data = data_Cria(idPops, idPopsConectado, velocidade, unidadeMedida);

            int RRN = 0;

            if(header.nroRegRem == 0)
            {
                RRN = header.proxRRN;
                fseek(fEntrada, 17 + 18*RRN, SEEK_SET);
                data_write(&data, fEntrada);
                header.proxRRN++;
            }
            else if(header.nroRegRem >= 1)
            {
                dataReg data2;
                RRN = header.topoPilha;

                fseek(fEntrada, 17 + 18*RRN, SEEK_SET);
                data_read(&data2, fEntrada);

                header.topoPilha = data2.encadeamentoPilha;
                header.nroRegRem--;

                fseek(fEntrada, 17 + 18*RRN, SEEK_SET);
                data_write(&data, fEntrada);
            }
        }
        
        fseek(fEntrada, 0, SEEK_SET);
        header.status = '1';
        header_write(&header, fEntrada);
        fclose(fEntrada);
        BinarioNaTela(arquivoEntrada);
    }
    else
        printf("Falha no processamento do arquivo.\n");
}

void func7(char* arquivoEntrada, int numAtt)
{
    FILE* fEntrada = NULL;
    headerReg header;

    if ((fEntrada = fopen(arquivoEntrada, "r+b")) != NULL) 
    {
        header_read(&header, fEntrada);
        if (header.status == '0') 
        {
            printf("Falha no processamento do arquivo.\n");
            fclose(fEntrada);
            return;
        }
        header.status = '0';
        fseek(fEntrada, 0, SEEK_SET);
        header_write(&header, fEntrada);

        for(int i = 0; i < numAtt; i++)
        {
            int numParesBusca = 0;
            scanf("%d", &numParesBusca);

            argsBusca argsBusc[numParesBusca];

            for(int j = 0; j < numParesBusca; j++)
            {   
                scanf("%s", argsBusc[j].nomesCampo);
                
                if(strcmp(argsBusc[j].nomesCampo, "unidadeMedida") == 0)
                    ScanQuoteString(argsBusc[j].valoresCampo);
                else
                    scanf("%s", argsBusc[j].valoresCampo);
            }

            int numParesAtt = 0;
            
            scanf("%d", &numParesAtt);

            argsBusca argsAtt[numParesAtt];

            for(int j = 0; j < numParesAtt; j++)
            {
                scanf("%s", argsAtt[j].nomesCampo);
                if(strcmp(argsAtt[j].nomesCampo, "unidadeMedida") == 0)
                    ScanQuoteString(argsAtt[j].valoresCampo);
                else
                    scanf("%s", argsAtt[j].valoresCampo);
            }
            fseek(fEntrada, 17, SEEK_SET);

            dataReg data;

            while(data_read(&data, fEntrada) != 0) 
            {
                if(data.removido == '0')
                {
                    int sucesso = 1;
                    for(int j = 0; j < numParesBusca; j++)
                    {
                        int modoBusca = 0;
                        if (strcmp(argsBusc[j].nomesCampo, "idPoPs") == 0) 
                            modoBusca = 1;

                        else if (strcmp(argsBusc[j].nomesCampo, "idPoPsConectado") == 0) 
                            modoBusca = 2;

                        else if (strcmp(argsBusc[j].nomesCampo, "velocidade") == 0) 
                            modoBusca = 3;

                        else if (strcmp(argsBusc[j].nomesCampo, "unidadeMedida") == 0) 
                            modoBusca = 4;

                        if (!parameter_search(&data, modoBusca, argsBusc[j].valoresCampo))
                        {
                            sucesso = 0;
                            break;
                        }
                    }

                    if(sucesso == 1)
                    {
                        for(int k = 0; k < numParesAtt; k++)
                        {
                            data_Atualiza(&data, argsAtt[k].nomesCampo, argsAtt[k].valoresCampo);
                        }       
                        fseek(fEntrada, -18, SEEK_CUR);
                        data_write(&data, fEntrada);

                        fseek(fEntrada, 0, SEEK_CUR);
                    }
                }
            }
        }

        fseek(fEntrada, 0, SEEK_SET);
        header.status = '1';
        header_write(&header, fEntrada);
        fclose(fEntrada);

        BinarioNaTela(arquivoEntrada);
    }
    else
        printf("Falha no processamento do arquivo.\n");
}
