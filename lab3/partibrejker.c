//
//  main.c
//  PARTIBREJKERI_C
//
//  Created by Borna on 30/04/2020.
//  Copyright © 2020 hr.fer.majstorovic.borna. All rights reserved.
//

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include <sys/syscall.h>



pthread_mutex_t monitor;
pthread_cond_t studos;
pthread_cond_t brejker;

int brojStudenataUSobi = 0;
int brejkerUSobi = 0; // 0 nije 1 je

int N = 4; //broj studenata fiksno mjenjati


void *student(void * K){
    
    int stud =*((int*)K);
    
    srand((unsigned)time(NULL));
    int x;
    x = rand()%400 + 100;
    usleep(x*1000);
    
    for(int i = 0; i < 3; i++){
        
        pthread_mutex_lock(&monitor);
        while(brejkerUSobi == 1)
            pthread_cond_wait(&studos, &monitor);
        pthread_mutex_unlock(&monitor);
        
        pthread_mutex_lock(&monitor);
        brojStudenataUSobi++;
        if(brojStudenataUSobi > 3) pthread_cond_signal(&brejker);
        printf("Student %d je usao u sobu\n", stud);
        pthread_mutex_unlock(&monitor);
        
        int y = rand()%1000 + 1000;
        usleep(y*1000);
        
        pthread_mutex_lock(&monitor);
        brojStudenataUSobi--;
        
        if(brojStudenataUSobi == 0) { //salji signal brejkeri da smije izac
            pthread_cond_signal(&brejker);
        }
        printf("Student %d je izasao iz sobe\n", stud);
        
        pthread_mutex_unlock(&monitor);
        
        usleep(y*1000);
    }
    pthread_mutex_lock(&monitor);
    N--;
    pthread_mutex_unlock(&monitor);
    
    return NULL;
}

void *partibrejker(){
    
    while (N>0) {
        srand((unsigned)time(NULL));
        int x;
        x = rand()%900 + 100;
        usleep(x*1000);
        
        pthread_mutex_lock(&monitor);
        while (brojStudenataUSobi < 3) {
            pthread_cond_wait(&brejker, &monitor);
        }
        if (brojStudenataUSobi >=3 && brejkerUSobi == 0) {
            pthread_cond_signal(&brejker);
            
            brejkerUSobi = 1;
            printf("Partibrejker je uso u sobu \n");
            pthread_cond_signal(&studos);
        }
        pthread_mutex_unlock(&monitor);
        
        pthread_mutex_lock(&monitor);
        while (brojStudenataUSobi != 0) {
            pthread_cond_wait(&brejker, &monitor);
        }
        if (brojStudenataUSobi == 0) {
            printf("Partibrejker izlazi iz sobe \n");
            brejkerUSobi = 0;
            pthread_cond_signal(&studos);//vrati studentima mogucnot ulaska
        }
        pthread_cond_signal(&brejker);
        pthread_mutex_unlock(&monitor);
        
    }
    
    return NULL;
}

void brisi(){
    pthread_mutex_destroy(&monitor);
    pthread_cond_destroy(&studos);
    pthread_cond_destroy(&brejker);
    exit(0);
}

int main(int argc, const char * argv[]) {
    pthread_t dretvaStudent[N];
    pthread_t dretvaBrejker;
    
    sigset(SIGINT, brisi);
    
    pthread_mutex_init(&monitor, NULL);
    pthread_cond_init(&studos, NULL);
    pthread_cond_init(&brejker, NULL);
    
    for (int i = 1; i <= N; i++) {
        pthread_create(&dretvaStudent[i], NULL, student, &i);
        sleep(1);
    }
    pthread_create(&dretvaBrejker, NULL, partibrejker(), NULL);
    
    for (int i = 1; i <= N; i++) {
        pthread_join(dretvaStudent[i], NULL);
    }
    pthread_join(dretvaBrejker, NULL);
    
    brisi();
    
    return 0;
}
