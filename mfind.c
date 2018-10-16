#include "mfind.h"
#include "queue.h"


int NR_OF_THREADS = 2;

void * print_hello(){
    printf("hello world\n");
    return NULL;
}


int main(int argc, char *argv[]){

    pthread_t threads[NR_OF_THREADS];
    for(int i = 0; i < NR_OF_THREADS-1; i++){
        if(pthread_create(&(threads[i]), NULL, &print_hello, NULL)!= 0){
            perror("");
        }else{
            printf("created string");
        }
    }
    // for(int i = 0; i < NR_OF_THREADS-1; i++){
    //     pthread_join(threads[i],NULL);
    // }

}