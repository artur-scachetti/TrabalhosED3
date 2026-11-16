#include "include/func.h"
#include "include/tools.h"
#include "include/fornecidas.h"

int main()
{
    // Identifica a operação que o usuário quer realizar
    int cmd = 0;
    scanf("%d", &cmd);
    switch(cmd)
    {
        case(8):
        {
            char arquivoEntrada[100];
            char arquivoSaida[100];

            scanf("%s %s", arquivoEntrada, arquivoSaida);

            func8(arquivoEntrada, arquivoSaida);

            break;
        }
        case(9):
        {
            char arquivoEntrada[100];
            char arquivoIndice[100];

            scanf("%s %s", arquivoEntrada, arquivoIndice);

            func9(arquivoEntrada, arquivoIndice, ESCREVE_BIN);

            break;
        }
        case(10):
        {
            char arquivoEntrada[100];

            scanf("%s", arquivoEntrada);

            func10(arquivoEntrada);
            
            break;
        }
        case(11):
        {
            char arquivoEntrada[100];
            char arquivoIndices[100];
            int numConjuntos = 0;

            scanf("%s %s %d", arquivoEntrada, arquivoIndices, &numConjuntos);

            // Percorre o número dado de conjunto de pares Campo/Valor
            for(int i = 0; i < numConjuntos; i++)
            {
                int numPares;
                scanf("%d", &numPares);

                // Extrai os Pares Campo/Valor para usar como argumentos de busca
                argsBusca* args = determinaArgsBusca(numPares);

                func11(arquivoEntrada, arquivoIndices, numPares, args);

                freeArgsBusca(args, numPares);

                if(i < numConjuntos)
                    printf("\n");
            }
            break;
        }
        case(12):
        {
            char arquivoEntrada[100];
            char arquivoIndices[100];
            int numConjuntos = 0;

            scanf("%s %s %d", arquivoEntrada, arquivoIndices, &numConjuntos);

            // Percorre o número dado de conjunto de pares Campo/Valor
            for(int i = 0; i < numConjuntos; i++)
            {
                int numPares;
                scanf("%d", &numPares);

                // Extrai os Pares Campo/Valor para usar como argumentos de busca
                argsBusca* args = determinaArgsBusca(numPares);

                func12(arquivoEntrada, arquivoIndices, numPares, args);

                freeArgsBusca(args, numPares);
            }

            BinarioNaTela(arquivoEntrada);
            BinarioNaTela(arquivoIndices);
            break;
        }
        case(13):
        {
            char arquivoEntrada[100];
            char arquivoIndices[100];
            int numRegistros = 0;

            scanf("%s %s %d", arquivoEntrada, arquivoIndices, &numRegistros);

            func13(arquivoEntrada, arquivoIndices, numRegistros);

            break;
        }
        case(14):
        {
            char arquivoEntrada[100];
            char arquivoIndices[100];
            int numAtt = 0;

            scanf("%s %s %d", arquivoEntrada, arquivoIndices, &numAtt);

            // Percorre os Pares de Campo/Valor a serem atualizados
            for(int i = 0; i < numAtt; i++)
            {
                int numParesBusca;
                int numParesAtt;
                scanf("%d", &numParesBusca);
                
                // Extrai os Pares Campo/Valor para usar como argumentos de busca
                argsBusca* args = determinaArgsBusca(numParesBusca);

                scanf("%d", &numParesAtt);

                // Extrai os Pares Campo/Valor para usar como argumentos de update
                argsBusca* argsAtt = determinaArgsBusca(numParesAtt);

                func14(arquivoEntrada, arquivoIndices, args, argsAtt, numParesBusca, numParesAtt);

                freeArgsBusca(args, numParesBusca);
                freeArgsBusca(argsAtt, numParesAtt);
            }
            BinarioNaTela(arquivoEntrada);
            BinarioNaTela(arquivoIndices);
        }
    }
}