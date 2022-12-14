all: PingPongMPI

PingPongMPI: PingPongMPI.c chrono.c
	mpicc PingPongMPI.c -o PingPongMPI