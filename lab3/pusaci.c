//
//  main.c
//  PUSACI_C
//
//  Created by Borna on 30/04/2020.
//  Copyright © 2020 hr.fer.majstorovic.borna. All rights reserved.
//

#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>


struct zajednicko{
    sem_t stol_prazan, p1, p2, p3, KO;
    int s1, s2;
};
struct zajednicko *z;
int ID;

int sastojci[3] = {1,2,3};
char *sastojciIspis[3] = {"papir", "duhan", "sibice"};




void dealer() {
    srand((unsigned)time(NULL));
    while (1) {
        int sast1, sast2;
        
        //odredi 2 random sastojka
        while(1){
            
            int index1 = rand()%3+1;
            int index2 = rand()%3+1;
            if(index1 != index2) {
                sast1 = index1;
                sast2 = index2;
                break;
            }
        }
        
        sem_wait(&z->KO);
        //stavi sastojke na stol
        z->s1 = sast1;
        z->s2 = sast2;
        
        if((z->s1) == 1) printf("Trgovac stavlja: papir i ");
        else if((z->s1) == 2) printf("Trgovac stavlja: duhan i ");
        else if((z->s1) == 3) printf("Trgovac stavlja: sibice i ");
        
        if((z->s2) == 1) printf("papir\n");
        else if ((z->s2) == 2) printf("duhan\n");
        else if((z->s2) == 3) printf("sibice\n");
        fflush(stdout);
        sleep(1);
        
        
        sem_post(&z->KO);
        sem_post(&z->p1);
        sem_post(&z->p2);
        sem_post(&z->p3);
        sem_wait(&z->stol_prazan);
    }
    
}



void smoker(int p) {
    int r1,r2;
    //odaberi sastojke koje ovaj pusac nema
    if (p == 1) {
        r1 = 2;
        r2 = 3;
    } else if (p == 2) {
        r1 = 1;
        r2 = 3;
    } else if(p == 3){
        r1 = 1;
        r2 = 2;
    }
    
    while (1) {
        
        if (p == 1) {
            sem_wait(&z->p1);
        } else if (p == 2) {
            sem_wait(&z->p2);
        } else if(p == 3){
            sem_wait(&z->p3);
        }
        
        
        sem_wait(&z->KO);
        
        
        if((r1 == (z->s1) && r2 == (z->s2)) || (r1 == (z->s2) && r2 == (z->s1))){ //nasli smo pusaca kojeg trgovac trazi
            
            printf("Pusac %d: uzima sastojke %s i %s \n", p, sastojciIspis[r1-1], sastojciIspis[r2-1]);
            sleep(1);
            z->s1 = 100;
            z->s2 = 100; //kako nebi ponovno uso u isti if
            
            sem_post(&z->KO);
            sem_post(&z->stol_prazan);
            
            
        } else {
            sem_post(&z->KO);
        }
        
    }
    
}

void brisi(int sig){
    shmdt(z);
    shmctl(ID, IPC_RMID, NULL);
    exit(0);
}


int main(int argc, const char * argv[]) {
    
    
    ID = shmget(IPC_PRIVATE, sizeof(struct zajednicko), 0600 );
    if(ID == -1) exit(1);
    
    sigset(SIGINT, brisi);
    
    z = (struct zajednicko*)shmat(ID, NULL, 0);
    
    shmctl(ID, IPC_RMID, NULL);
    
    sem_init(&z->KO, 1, 1);
    sem_init(&z->stol_prazan, 1, 0);
    sem_init(&z->p1, 1, 0);
    sem_init(&z->p2, 1, 0);
    sem_init(&z->p3, 1, 0);
    
    
    printf("pusac 1: ima papir\n");
    printf("pusac 2: ima duhan\n");
    printf("pusac 3: ima sibice\n");
    
    if (fork() == 0) {
        dealer();
        exit(0);
    }
    for (int i = 1; i <=3; i++) {
        if(fork() == 0){
            smoker(i);
            exit(0);
        }
    }
    
    
    for(int i=0; i<=4; i++){
        wait(NULL);
    }
    
    
    
    brisi(0);
    return 0;
}
