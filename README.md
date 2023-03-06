# Broadcast em cluster

Trabalho feito para a matéria de programação paralela, no curso de Ciencia da Computação, da Universidade Federal do Paraná.

Autor:
Anderson Frasão

O algoritmo PingPongMPI.c tem como propósito executar dois processos em paralelo, sendo que esses dois processos se comunicam entre si, 
com mensagens formadas por vetores de long.

Para ultilizar o programa, execute o comando make no terminal (dentro da pasta em que o código esta) e execute um desses comandos:

> ./PingPongMPI

ou

> mpirun -np 2 ./PingPongMPI < nmsg > < tmsg > < nproc > (< -bl > OU < -nbl >)

sendo: 

* np: numero de processos (2 pois são só duas maquinas)

* nmsg: numero de mensagens a serem enviadas
  
* tmmsg: tamanho das mensagens a serem enviadas 
  
* nproc: numero de pocessos (sim, é redundante, culpa do professor) 
  
* bl: bloqueante (padrão)

* nbl: não bloqueante
