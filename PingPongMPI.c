#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int nmsg;       // o número total de mensagens
int tmsg;       // o tamanho de cada mensagem
int nproc;      // o número de processos MPI
int par;        // opcional
int processId; 	// rank dos processos
int ni;			// tamanho do vetor contendo as mensagens

int main(int argc, char *argv[]){

	par = 1;

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
            if (strcmp(argv[4], "-nbl") == 0)
                par = 2;
        }
	}

	ni = tmsg/8;
	int dest, source, rc, tag = 1;
	MPI_Status Stat;

	long int *inmsg = calloc(ni, sizeof(long int));
	long int *outmsg = calloc(ni, sizeof(long int));

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &processId);

	if(processId == 0){
		for(long int i = 1; i <= ni; i++)
			inmsg[i-1] = i;
	}
	else {
		long int i = ni + 1;
		for(long int h = 0; h < ni; h++){
			inmsg[h] = i;
			i++;
		}
	}

	printf("Processo: %d, in\n", processId);
	for(int i = 0; i < ni; i++){
		printf("%ld ", inmsg[i]);
	}
	printf("\n");
	
	if(par == 1){
		if ( processId == 0 ) {
			dest = 1;
			source = 1;
			for(int i = 1; i <= ni; i++){
				rc = MPI_Send(&inmsg[i], ni, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
				rc = MPI_Recv(&outmsg[i], ni, MPI_CHAR, source, tag, MPI_COMM_WORLD, &Stat);
			}
		}
		else if ( processId == 1 ) {
			dest = 0;
			source = 0;
			for(int i = 1; i <= ni; i++){
				rc = MPI_Recv(&inmsg[i], ni, MPI_CHAR, source, tag, MPI_COMM_WORLD, &Stat);
				rc = MPI_Send(&outmsg[i], ni, MPI_CHAR, dest, tag, MPI_COMM_WORLD);	
			}
		}
	}
	else if(par == 2){

	}
	
	printf("Processo: %d, out\n", processId);
	for(int i = 0; i < ni; i++){
		printf("%ld ", outmsg[i]);
	}
	printf("\n");

	free(inmsg);
	free(outmsg);

	MPI_Finalize( );
	return 0;
}