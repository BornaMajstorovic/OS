#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/time.h>

int pid = 0;
int sig[]={SIGBUS,SIGTERM,SIGUSR1,SIGUSR2};

void prekidna_rutina(int sig){
    kill(pid,SIGKILL);
    exit(0);
}


int main(int argc, char *argv[]){
    pid = atoi(argv[1]);
    int t;
    sigset(SIGINT, prekidna_rutina);
    srand((unsigned) time(NULL));
    
    while(1){
        t = rand() % (4+1);
        printf("Saljem %d \n",t + 1);
        kill(pid,sig[t]);
        sleep(rand()%(3)+3);
    }
    
    return 0;
}



