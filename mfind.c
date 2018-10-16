#include "mfind.h"
#include "queue.h"


void * print_hello(){
    printf("hello world\n");
    return NULL;
}

void enqueue_startfolders(int size,char * buf[size]){
    for(int i = 0; i < size; i++){
        enqueue(buf[i]);
    }
}

void traverse_files(char * path){
    struct dirent *p_dirent;
    DIR *p_dir;
    p_dir = opendir (path);
    
    if (p_dir == NULL) {
        printf("could not open dir");   
    }else{
        while ((p_dirent = readdir(p_dir)) != NULL) {
            printf ("[%s]\n", p_dirent->d_name);
        }
        closedir (p_dir);
    } 
}

// void dequeue_all(int size,char buf[size]){
    
// }

int main(int argc, char *argv[]){
    int nr_of_threads = 2;

    char * buf[2] = {"test","test/hej"};
    enqueue_startfolders(2,buf);

    pthread_t threads[nr_of_threads];
    //TODO: run function first
    traverse_files("test");
    for(int i = 0; i < nr_of_threads-1; i++){
        if(pthread_create(&(threads[i]), NULL, &print_hello, NULL)!= 0){
            perror("");
        }
    }


    printf("%s\n", dequeue());
    printf("%s\n", dequeue());
    
    // for(int i = 0; i < nr_of_threads-1; i++){
    //     pthread_join(threads[i],NULL);
    // }

}