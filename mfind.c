#include "mfind.h"
#include "queue.h"


void * print_hello(){
    printf("hello world\n");
    return NULL;
}

void enqueue_startfolders(int size, char * buf[size]){
    for(int i = 0; i < size; i++){
        enqueue(buf[i]);
    }
}

void * traverse_files(){
    printf("start\n");
    struct dirent *p_dirent;
    DIR *p_dir;
    p_dir = opendir (dequeue());
   
    if (p_dir == NULL) {
        printf("could not open dir");   
    }else{
        printf("hello\n");
        while ((p_dirent = readdir(p_dir)) != NULL) {
            printf ("[%s]\n", p_dirent->d_name);
        }
        closedir (p_dir);
    } 
    return NULL;
}

// void dequeue_all(int size,char buf[size]){
    
// }

int main(int argc, char *argv[]){
    int nr_of_threads = 2;

    //just for now
    char * buf[2] = {"test","test"};

    enqueue_startfolders(2,buf);

    pthread_t threads[nr_of_threads];

    traverse_files();
    for(int i = 0; i < nr_of_threads-1; i++){
        if(pthread_create(&(threads[i]), NULL, &traverse_files, NULL)!= 0){
            perror("");
        }
    }

    for(int i = 0; i < nr_of_threads-1; i++){
        pthread_join(threads[i],NULL);
    }

}