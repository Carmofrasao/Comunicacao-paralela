#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "chrono.c"

long nmsg;       // o número total de mensagens
long tmsg;       // o tamanho de cada mensagem
int nproc;      // o número de processos MPI
int par;        // opcional
int processId; 	// rank dos processos
int ni;			// tamanho do vetor contendo as mensagens

chronometer_t pingPongTime;

// #define DEBUG 1

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
	MPI_Status Stat;

	long int *inmsg = (long int*)calloc(ni, sizeof(long int));
	long int *outmsg = (long int*)calloc(ni, sizeof(long int));

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

	#if DEBUG == 1
		printf("Processo: %d, in\n", processId);
		for(int i = 0; i < ni; i++){
			printf("%ld ", inmsg[i]);
		}
		printf("\n");
	#endif

	MPI_Barrier(MPI_COMM_WORLD);

	if(processId == 0){
		chrono_reset(&pingPongTime);
		chrono_start(&pingPongTime);
	}

	if(par == 1){
		int dest, source, rc, tag = 1;
		if ( processId == 0 ) {
			dest = 1;
			source = 1;
			for(int m = 0; m < nmsg/2; m++)
				for(int i = 0; i < ni; i++){
					rc = MPI_Send(&inmsg[i], 1, MPI_LONG, dest, tag, MPI_COMM_WORLD);
					rc = MPI_Recv(&outmsg[i], 1, MPI_LONG, source, tag, MPI_COMM_WORLD, &Stat);
				}
		}
		else if ( processId == 1 ) {
			dest = 0;
			source = 0;
			for(int m = 0; m < nmsg/2; m++)
				for(int i = 0; i < ni; i++){
					rc = MPI_Recv(&outmsg[i], 1, MPI_LONG, source, tag, MPI_COMM_WORLD, &Stat);
					rc = MPI_Send(&inmsg[i], 1, MPI_LONG, dest, tag, MPI_COMM_WORLD);	
				}
		}
	}
	else if(par == 2){
		int next, prev, tag1=1, tag2=2;
		MPI_Request reqs[ni];
		MPI_Status stats[ni];

		if ( processId == 0 ) {
			prev = 1;
			next = 1;
			for(int m = 0; m < nmsg/2; m++)
				for(int i = 0; i < ni; i++){
					MPI_Isend(&inmsg[i], 1, MPI_LONG, next, tag2, MPI_COMM_WORLD, &reqs[1]);
					MPI_Irecv(&outmsg[i], 1, MPI_LONG, prev, tag1, MPI_COMM_WORLD, &reqs[0]);
					MPI_Waitall(2, reqs, stats);
				}
		}
		else if ( processId == 1 ) {
			prev = 0;
			next = 0;
			for(int m = 0; m < nmsg/2; m++)
				for(int i = 0; i < ni; i++){
					MPI_Isend(&inmsg[i], 1, MPI_LONG, next, tag1, MPI_COMM_WORLD, &reqs[1]);
					MPI_Irecv(&outmsg[i], 1, MPI_LONG, prev, tag2, MPI_COMM_WORLD, &reqs[0]);
					MPI_Waitall(2, reqs, stats);
				}
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);

	if(processId == 0){
		chrono_stop(&pingPongTime);
		chrono_reportTime(&pingPongTime, "pingPongTime");

		// calcular e imprimir a VAZAO (numero de operacoes/s)
		double total_time_in_seconds = (double)chrono_gettotal(&pingPongTime) /
									((double)1000 * 1000 * 1000);
	    double total_time_in_micro = (double)chrono_gettotal(&pingPongTime) /
									((double)1000);
		printf("total_time_in_seconds: %lf s\n", total_time_in_seconds);
		printf("Latencia: %lf us/nmsg\n", (total_time_in_micro / nmsg)/2);
		double MBPS = ((double)(nmsg*tmsg) / ((double)total_time_in_seconds*1000*1000));
		printf("Throughput: %lf MB/s\n", MBPS);
	}
	
	#if DEBUG == 1
		printf("Processo: %d, out\n", processId);
		for(int i = 0; i < ni; i++){
			printf("%ld ", outmsg[i]);
		}
		printf("\n");
	#endif

	free(inmsg);
	free(outmsg);

	MPI_Finalize( );
	return 0;
}