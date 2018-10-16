#include "mfind.h"
#include "queue.h"

void * print_hello(){
    printf("hello world\n");
    return NULL;
}



int main(int argc, char *argv[]){
    int nr_of_threads = 2;

    //TODO: create function
    pthread_t threads[nr_of_threads];
    for(int i = 0; i < nr_of_threads-1; i++){
        if(pthread_create(&(threads[i]), NULL, &print_hello, NULL)!= 0){
            perror("");
        }else{ 
            printf("created string");
        }
    }

    enqueue("hej");
    enqueue("bacon");
    printf("%s\n", dequeue());
    printf("%s\n", dequeue());
    
    // for(int i = 0; i < nr_of_threads-1; i++){
    //     pthread_join(threads[i],NULL);
    // }

}