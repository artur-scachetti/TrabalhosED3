#include "func.h"
#include "tools.h"

int main(){

    int cmd;
    scanf("%d", &cmd);

    switch(cmd){

        case(1):

            char arquivoEntradaCSV[100];
            char arquivoSaidaBin[100];

            scanf("%s %s", arquivoEntradaCSV, arquivoSaidaBin);

            func1(arquivoEntradaCSV, arquivoSaidaBin);

            break;
        
        case(2):

            char arquivoEntradaBin[100];

            scanf("%s", arquivoEntradaBin);

            func2(arquivoEntradaBin);

            break;
        
        case(3):

            char arquivoEntradaBin[100];
            
            int numConjuntos;

            scanf("%s %d", arquivoEntradaBin, &numConjuntos);

            for(int i = 0; i < numConjuntos; i++){

                int numPares;
                scanf("%d", &numPares);

                argsBusca args[numPares];

                for(int j = 0; j < numPares; j++){

                    scanf("%s %s", args[j].nomesCampo, args[j].valoresCampo);
                }

                func3(arquivoEntradaBin, numPares, args);
            }

            break;

        case(4):

            char arquivoEntradaBin[100];

            int RRN;

            scanf("%s %d", arquivoEntradaBin, &RRN);

            func4(arquivoEntradaBin, RRN);

            break;
    }
}