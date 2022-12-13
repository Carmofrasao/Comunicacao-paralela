#include <stdio.h>
#include <string.h>

int nmsg;       // o número total de mensagens
int tmsg;       // o tamanho de cada mensagem
int nproc;      // o número de processos MPI
int par;        // opcional

int main(int argc, char *argv[]){
	long i;

	if (argc < 4){
		printf("usage: %s <nmsg> <tmsg> <nproc> (<-bl> OU <-nbl>)\n",
			   argv[0]);
		return 0;
	}
	else{
        nmsg = atoi(argv[1]);
		if (nmsg % 2 != 0){
			printf("usage: %s <nmsg> <tmsg> <nproc> (<-bl> OU <-nbl>)\n",
				   argv[0]);
			printf("<nmsg> deve ser par\n");
			return 0;
		}
		tmsg = atoi(argv[2]);
		if (tmsg % 8 != 0){
			printf("usage: %s <nmsg> <tmsg> <nproc> (<-bl> OU <-nbl>)\n",
				   argv[0]);
			printf("<tmsg> deve ser multiplo de 8\n");
			return 0;
		}
        nproc = atoi(argv[3]);
        if (nproc != 2){
            printf("usage: %s <nmsg> <tmsg> <nproc> (-bl OU -nbl)\n",
				   argv[0]);
			printf("<nproc> deve ser 2\n");
			return 0;
        }
        if(argc == 5){
            if(strcmp(argv[4], "-bl") == 0)
                par = 1;
            else if (strcmp(argv[4], "-nbl") == 0)
                par = 2;
        }
	}
}