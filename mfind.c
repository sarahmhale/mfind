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

char * concat_path(char * path, char * current_file){
    char * str3 = (char *) malloc(1 + strlen(path)+ strlen(current_file) );
    strcpy(str3, path);
    strcat(str3, current_file);
    return str3;
}

char * get_directory(char * path){
    char *last = strrchr(path, '/');
    if (last != NULL) {
        return last+1;
    }
    return NULL;
}

void check_file_type(char * path){
    struct stat file_info;
    if(lstat(path, &file_info)==-1){
        perror("");
    }else{ 
        if(S_ISDIR(file_info.st_mode)){
            printf("directory %s\n", path);
            enqueue(path);
        }else if(S_ISREG(file_info.st_mode)){
            //TODO: check if they are the same
        }
    }
}

void open_directory(){
    struct dirent *p_dirent;
    DIR *p_dir;
    char * path = dequeue();
    p_dir = opendir (path);

    if (p_dir == NULL) {
        printf("could not open dir");   
    }else{
        while ((p_dirent = readdir(p_dir)) != NULL) {
            char * new_path = concat_path(path,p_dirent->d_name);
            check_file_type(new_path);
        }
        closedir (p_dir);
    } 
}

void * traverse_files(){
   open_directory();
   return NULL;
}


int main(int argc, char *argv[]){
    int nr_of_threads = 2;

   
    char * buf[2] = {"test/"};

    enqueue_startfolders(2,buf);

    pthread_t threads[nr_of_threads];

    traverse_files();
    // for(int i = 0; i < nr_of_threads-1; i++){
    //     if(pthread_create(&(threads[i]), NULL, &traverse_files, NULL)!= 0){
    //         perror("");
    //     }
   // }

    // for(int i = 0; i < nr_of_threads-1; i++){
    //     pthread_join(threads[i],NULL);
    // }

}