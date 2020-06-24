#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

long A;
int n,m;

void *povecajA() {
    for (int i = 0; i < m; i++) A++;
    return NULL;
}

int main (int argc, char *argv[]) { 
    
    if (argc !=3) {
        printf("Ne ispravan broj argumenata\n");
        exit(0);
    }
    
    A = 0;
    n = atoi(argv[1]);
    m = atoi(argv[2]);
    pthread_t *dretve;
    dretve = (pthread_t*) malloc (n * sizeof(pthread_t));
    
    if(dretve == NULL) printf("alokacija nije uspjela\n");
    
    for (int i = 0; i < n; i++){
        if (pthread_create(dretve + i, NULL, povecajA, NULL)){
            exit(0);
        }
    }
    
    for (int i = 0; i < n; i++) {
        pthread_join(*(dretve + i),NULL);
    }
    
    printf ("%ld\n", A);
    return 0;
    
}
