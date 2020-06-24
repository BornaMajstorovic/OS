#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>

int pauza = 0;
long int broj = 10000000001;
long int zadnji = 10000000001;

int periodicki_ispis(int sig){
    printf("Zadnji prosti broj je : %d \n", zadnji);
}

int postavi_pauzu(int sig){
    pauza = 1 - pauza;
    return pauza;
}

void prekini(int sig){
    printf("Zadnji prosti broj je: %d \n", zadnji);
    printf("Prekid programa");
    exit(0);
}

int prost ( unsigned long n ) {
    unsigned long i, max;
    
    if ( ( n & 1 ) == 0 ) /* je li paran? */
        return 0;
    
    max = sqrt ( n );
    for ( i = 3; i <= max; i += 2 )
        if ( ( n % i ) == 0 )
            return 0;
    
    return 1; /* broj je prost! */
}

int main(void){
    
    sigset(SIGINT, postavi_pauzu);
    sigset(SIGTERM, prekini);
    sigset(SIGALRM, periodicki_ispis);
    
    // definiranje periodickog slanja signala
    struct itimerval t;
    t.it_value.tv_sec = 5;
    t.it_value.tv_usec = 500000;
    t.it_interval.tv_sec = 5;
    t.it_interval.tv_usec= 500000;
    
    setitimer (ITIMER_REAL, &t, NULL);
    
    printf("PID : %d\n", getpid());
    printf("Brojanje prostih brojeva: \n");
    
    int zastavica = 0;
    
    while(1){
        zastavica = prost(broj);
        if(zastavica){
            zadnji = broj;
        }
        broj++;
        while(pauza){
            pause();
        }
    }
    
    return 0;
}



