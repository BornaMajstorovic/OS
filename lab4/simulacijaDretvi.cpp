//
//  main.cpp
//  simulacija
//
//  Created by Borna on 14/05/2020.
//  Copyright © 2020 hr.fer.majstorovic.borna. All rights reserved.
//
#include <iostream>
#include <unistd.h>
#include <iomanip>
using namespace std;

int t = 0; //simulacija vremena (trenutno vrijeme)

struct dretva {
    int id; //1, 2, 3, ...
    int p;  //preostalo vrijeme rada
    int prio; //prioritet
    int rasp; //način raspoređivanja (za opcionalni zadatak)
};


#define MAX_DRETVI    5
struct dretva *P[MAX_DRETVI]; //red pripravnih dretvi, P[0] = aktivna dretva

/* podaci o događajima pojave novih poslova - dretvi */
#define DRETVI    6
int nove[DRETVI][5] =
{
    /* trenutak dolaska, id, p, prio, rasp (prio i rasp se koriste ovisno o rasporedivacu) */
    { 1,  3, 5, 3, 1 }, /* rasp = 0 => PRIO+FIFO; 1 => PRIO+RR */
    { 3,  5, 6, 5, 1 },
    { 7,  2, 3, 5, 0 },
    { 12, 1, 5, 3, 0 },
    { 20, 6, 3, 6, 1 },
    { 20, 7, 4, 7, 1 },
};

void ispis_stanja ( int ispisi_zaglavlje ) // 1 na pocetku, 0 inace
{
    int i;
    
    if ( ispisi_zaglavlje ) {
        printf ( "  t    AKT" );
        for ( i = 1; i < MAX_DRETVI; i++ )
            printf ( "     PR%d", i );
        printf ( "\n" );
    }
    
    printf ( "%3d ", t );
    for ( i = 0; i < MAX_DRETVI; i++ )
        if ( P[i] != nullptr )
            printf ( "  %d/%d/%d ",
                    P[i]->id, P[i]->prio, P[i]->p );
        else
            printf ( "  -/-/- " );
    printf ( "\n");
}



int main(int argc, char const *argv[]) {
    int ima = DRETVI, dretvice = 0;
    
    for(int i = 0; i < MAX_DRETVI; i++) {
        P[i] = nullptr;
    }
    
    ispis_stanja(1);
    
    while (ima > 0) {
        
        if (P[0] != nullptr) {
            P[0]->p--;
            if (dretvice > 1) {
                for (int i = 0; i < dretvice - 1; i++) {
                    struct dretva *tmp = new dretva();
                    tmp = P[i];
                    P[i] = P[i+1];
                    P[i+1] = tmp;
                }
            }
        }
        
        
        if(dretvice){
            if (P[dretvice-1]->p == 0) {
                cout<<"Dretva "<<P[dretvice - 1]->id<<" je zavrsila"<<endl;
                P[dretvice-1] = nullptr;
                dretvice--;
                ima--;
            }
        }
        
        
        for (int i = 0; i < DRETVI; i++) {
            if (nove[i][0] == t) {
                P[dretvice] = new dretva();
                P[dretvice]->id = nove[i][1];
                P[dretvice]->p = nove[i][2];
                P[dretvice]->prio = nove[i][3];
                cout<< setw(3) << t <<'\t'<<" nova dretva id="<<P[dretvice]->id<<", p="<<P[dretvice]->p<<", prio="<<P[dretvice]->prio << endl;
                dretvice++;
            }
        }
        
        ispis_stanja(0);
        t++;
    }
    
    for(int i=0; i<MAX_DRETVI; i++){
        delete P[i];
    }
    
    
    return 0;
}

