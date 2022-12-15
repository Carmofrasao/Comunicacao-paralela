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
	int numtasks, rank, dest, source, rc, count, tag = 1;
	long int mensagem[ni];
	MPI_Status Stat;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &processId);

	long int i;
	if(processId == 0){
		for(i = 1; i <= ni; i++)
			mensagem[i-1] = i;
	}
	else {
		for(int h = 0; i < ni; i++){
			i++;
			mensagem[h] = i;
		}
	}
	
	if(par == 1){
		if ( processId == 0 ) {
			dest = 1;
			source = 1;
			for(int i = 1; i <= ni; i++){
				rc = MPI_Send(&mensagem[i], ni, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
				rc = MPI_Recv(&mensagem[i], ni, MPI_CHAR, source, tag, MPI_COMM_WORLD, &Stat);
			}
		}
		else if ( processId == 1 ) {
			dest = 0;
			source = 0;
			for(int i = 1; i <= ni; i++){
				rc = MPI_Recv(&mensagem[i], ni, MPI_CHAR, source, tag, MPI_COMM_WORLD, &Stat);
				rc = MPI_Send(&mensagem[i], ni, MPI_CHAR, dest, tag, MPI_COMM_WORLD);
			}
		}
	}
	for(int i = 0; i < ni; i++){
		printf("%ld ", mensagem[i]);
	}
	printf("\n");

	MPI_Finalize( );
	return 0;
}