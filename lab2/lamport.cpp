//
//  main.cpp
//  lamport
//
//  Created by Borna on 04/04/2020.
//  Copyright © 2020 Borna. All rights reserved.
//
#include <iostream>
#include <algorithm>
#include <vector>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdatomic.h>

using std::cout;
using std::cin;
using std::max_element;
using std::endl;
using std::vector;

atomic_int broj[100000000], ulaz[100000000];
int id, n, m;
atomic_int *a = 0;

void KOulaz(int i){
    ulaz[i] = 1;
    broj[i] = *max_element(broj, broj + n) + 1;
    ulaz[i] = 0;
    
    for (int j = 0; j < n; j++) {
        while (ulaz[j] != 0);
        while(broj[j] != 0 && (broj[j] < broj[i] || (broj[j] == broj[i] && j < i)));
        
    }
}

void KOizlaz(int i){
    broj[i] = 0;
}

void brisi(int signal){
    shmdt(ulaz);
    shmdt(broj);
    shmdt(a);
    shmctl(id, IPC_RMID, nullptr);
    exit(0);
}


void proces(int pid) {
    KOulaz(pid);
    for (int i = 0; i < m; i++) {
        *a+=1;
    }
    KOizlaz(pid);
}


int main(int argc, const char * argv[]) {
    if (argc != 3) {
        cout << "Krivi broj argumenata" << endl;
        exit(0);
    }
    
    n = atoi(argv[1]);
    m = atoi(argv[2]);
    
    
    id = shmget(IPC_PRIVATE, sizeof(int), 0600);
    
    if (id == -1) {
        exit(1);
    }
    
    
    a = (atomic_int *)shmat(id, nullptr, 0);
    
    sigset(SIGINT, brisi);
    int pid;
    
    for (int i = 0; i < n; i++) {
        broj[i] = 0;
        ulaz[i] = 0;
        switch (pid = fork()) {
            case -1:
                cout<< "Ne moze se stvoriti novi proces" <<endl;
                exit(1);
            case 0:
                proces(i);
                exit(0);
            default:
                break;
        }
    }
    
    for (int i = 0; i < n ; i++) {
        wait(NULL);
    }
    
    cout <<"A = "<< *a << endl;
    
    brisi(0);
    return 0;
}
