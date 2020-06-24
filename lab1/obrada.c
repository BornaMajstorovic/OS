#include <stdio.h>
#include <signal.h>

#define N 6    /* broj razina proriteta */

int OZNAKA_CEKANJA[N];
int PRIORITET[N];
int TEKUCI_PRIORITET;


int sig[]={SIGBUS,SIGTERM,SIGUSR1,SIGUSR2, SIGINT};

void zabrani_prekidanje(){
    int i;
    for(i=0; i<5; i++) sighold(sig[i]);
}
void dozvoli_prekidanje(){
    int i;
    for(i=0; i<5; i++) sigrelse(sig[i]);
}

void obrada_signala(int i){
    /* obrada se simulira trošenjem vremena,
     obrada traje 5 sekundi, ispis treba biti svake sekunde */
    
    int j;
    if(i == 1) {
        printf("-  P  -  -  -  -\n");
        for(j = 0; j < 5 ; j++) {
            printf("-  %d  -  -  -  -\n",j + 1);
            sleep(1);
        }
        printf("-  K -  -  -  -\n");
        
    }else if(i == 2) {
        printf("-  -  P  -  -  -\n");
        for(j = 0; j < 5; j++) {
            printf("-  -  %d  -  -  -\n",j + 1);
            sleep(1);
        }
        printf("-  -  K  -  -  -\n");
        
    }else if(i == 3) {
        printf("-  -  -  P  -  -\n");
        for(j = 0; j < 5; j++) {
            printf("-  -  -  %d  -  -\n",j + 1);
            sleep(1);
        }
        printf("-  -  -  K  -  -\n");
        
    }else if(i == 4) {
        printf("-  -  -  -  P  -\n");
        for(j = 0; j < 5; j++) {
            printf("-  -  -  -  %d  -\n",j + 1);
            sleep(1);
        }
        printf("-  -  -  -  K  -\n");
        
    }else if(i == 5) {
        printf("-  -  -  -  -  P\n");
        for(j = 0; j < 5; j++) {
            printf("-  -  -  -  -  %d\n",j + 1);
            sleep(1);
        }
        printf("-  -  -  -  -  K\n");
    }
    
}
void prekidna_rutina(int sig){
    //   int n=-1;
    //   zabrani_prekidanje();
    //   switch(sig){
    //      case SIGUSR1:
    //         n=1;
    //         printf("- X - - - -\n");
    //         break;
    //      case SIGUSR2:
    //         n=2;
    //         printf("- - X - - -\n");
    //         break;
    //      ...
    //   }
    //   OZNAKA_CEKANJA[n]=1;
    //   ponavljaj{
    //      ...
    //      dozvoli_prekidanje();
    //      obrada_prekida(n);
    //      zabrani_prekidanje();
    //      ...
    //   }
    //   dozvoli_prekidanje();
    
    int n, x, i;
    
    zabrani_prekidanje();
    
    switch(sig){
        case SIGBUS:
            n = 1;
            printf("-  X  -  -  -  -\n");
            break;
        case SIGTERM:
            n = 2;
            printf("-  -  X  -  -  -\n");
            break;
        case SIGUSR1:
            n = 3;
            printf("-  -  -  X  -  -\n");
            break;
        case SIGUSR2:
            n = 4;
            printf("-  -  -  -  X  -\n");
            break;
        case SIGINT:
            n = 5;
            printf("-  -  -  -  -  X\n");
            break;
            
    }
    
    OZNAKA_CEKANJA[n] += 1;
    
    do{
        x = 0;
        for(i = TEKUCI_PRIORITET + 1; i < N; i++){
            if(OZNAKA_CEKANJA[i] > 0)
                x = i;
        }
        
        if (x>0){
            OZNAKA_CEKANJA[x] = 0;
            PRIORITET[x] = TEKUCI_PRIORITET;
            TEKUCI_PRIORITET = x;
            dozvoli_prekidanje();
            obrada_signala(x);
            zabrani_prekidanje();
            TEKUCI_PRIORITET = PRIORITET[x];
            PRIORITET[x] = 0;
        }
    }while(x > 0);
    
    dozvoli_prekidanje();
}



int main ( void )
{
    
    sigset(SIGBUS, prekidna_rutina);
    sigset(SIGTERM, prekidna_rutina);
    sigset(SIGUSR1, prekidna_rutina);
    sigset(SIGUSR2, prekidna_rutina);
    sigset(SIGINT, prekidna_rutina);
    
    printf("Proces obrade prekida, PID=%ld\n", getpid());
    
    printf("G 1 2 3 4 5\n");
    printf("-----------\n");
    printf("1 - - - - -\n");
    printf("2 - - - - -\n");
    printf("3 - - - - -\n");
    printf("4 - - - - -\n");
    printf("GP S1 S2 S3 S4 S5\n");
    printf("-----------------\n");
    
    
    for(int i = 0; i < 44; i++){
        printf("%d  -  -  -  -  -\n", i );
        sleep(1);
    }
    
    printf ("Zavrsio osnovni program\n");
    
    return 0;
}

