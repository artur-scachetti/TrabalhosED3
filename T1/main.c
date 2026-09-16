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
        case(1):
        {
            char arquivoEntradaCSV[100];
            char arquivoSaidaBin[100];

            scanf("%s %s", arquivoEntradaCSV, arquivoSaidaBin);

            func1(arquivoEntradaCSV, arquivoSaidaBin);

            break;
        }
        case(2):
        {
            char arquivoEntradaBin[100];

            scanf("%s", arquivoEntradaBin);

            func2(arquivoEntradaBin);

            break;
        }
        case(3):
        {
            char arquivoEntradaBin[100];
            int numConjuntos = 0;

            // A variável "numConjuntos" indica quantas buscas com critério devem ser executadas
            scanf("%s %d", arquivoEntradaBin, &numConjuntos);

            // Para isso, deve-se realizar a obtenção de parâmetros conforme indicado por "numConjuntos",
            // levando em conta que não se sabe quantos pares parâmetro-valor o programa está esperando.
            for(int i = 0; i < numConjuntos; i++)
            {
                // Assim, para cada conjunto, primeiro obtém-se o número de pares esperados, para que
                // então a variável do tipo "argsBusca" seja criada do tamanho apropriado
                int numPares;
                scanf("%d", &numPares);

                argsBusca args[numPares];
                
                for(int j = 0; j < numPares; j++)
                {
                    // Percorre-se todos os pares, armazenando tanto o nome do parâmetro buscado, quanto
                    // o valor nele buscado.
                    scanf("%s", args[j].nomesCampo);

                    if(strcmp(args[j].nomesCampo, "unidadeMedida") == 0){

                        ScanQuoteString(args[j].valoresCampo);
                    }
                    else scanf("%s", args[j].valoresCampo);

                }

                func3(arquivoEntradaBin, numPares, args);
            }
        
            break;
        }
        case(4):
        {
            char arquivoEntradaBin[100];
            int RRN = 0;

            scanf("%s %d", arquivoEntradaBin, &RRN);
            func4(arquivoEntradaBin, RRN);

            break;
        }
        case(5):
        {
            char arquivoEntradaBin[100];
            int numRem = 0;

            scanf("%s %d", arquivoEntradaBin, &numRem);

            func5(arquivoEntradaBin, numRem);
            break;
        }
            break;
        
        case(6):
        {

        }
            break;

        case(7):
        {

        }
            break;
            
            
    }
}