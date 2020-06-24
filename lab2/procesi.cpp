//
//  main.cpp
//  procesi
//
//  Created by Borna on 04/04/2020.
//  Copyright © 2020 Borna. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <csignal>
using namespace std;

int *a ;
int n,m,id;

void proces(int i) {
    for (int j = 0; j < m; j++) {
        *a+=1;
    }
}

void brisi(int sig){
    shmdt((char*) a);
    shmctl(id, IPC_RMID, NULL);
    exit(0);
}

int main(int argc, const char * argv[]) {
    
    if (argc !=3) {
        cout << "Ne ispravan broj argumenata" << endl;
        exit(0);
    }
    
    int  pid;
    
    n = atoi(argv[1]);
    m = atoi(argv[2]);
    
    
    id = shmget(IPC_PRIVATE, sizeof(int*), 0600);
    if (id == -1) {
        exit(1);
    }
    
    a = (int *)shmat(id, nullptr, 0);
    *a = 0;
    sigset(SIGINT, brisi);
    
    for (int i = 0; i < n; i++) {
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
    
    for(int i = 0; i < n; i++) {
        wait(NULL);
    }
    cout <<"A = "<< *a << endl;
    
    brisi(0);
    return 0;
}
