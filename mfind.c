#include "mfind.h"
#include "queue.h"

node * list = NULL;


void * print_hello(){
    printf("hello world\n");
    return NULL;
}

pthread_t create_threads(int nr_of_threads){
    pthread_t threads[nr_of_threads];
    for(int i = 0; i < nr_of_threads-1; i++){
        if(pthread_create(&(threads[i]), NULL, &print_hello, NULL)!= 0){
            perror("");
        }else{
            printf("created string");
        }
    }
    return threads;
}

int main(int argc, char *argv[]){
    int nr_of_threads = 2;
    pthread_t threads = create_threads(nr_of_threads);
    
    // for(int i = 0; i < nr_of_threads-1; i++){
    //     pthread_join(threads[i],NULL);
    // }

}