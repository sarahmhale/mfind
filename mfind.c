#include "mfind.h"
#include "queue.h"

char * name;

char * concat_path(char * path, char * current_file){
    char * str3 = (char *) malloc(1 + strlen(path)+ strlen(current_file));
    strcpy(str3, path);
    strcat(str3, current_file);
    return str3;
}

void check_file_type(char * path){
    struct stat file_info;
    printf("path in check file type %s\n", path);
    if(lstat(path, &file_info)==-1){
        perror("");
    }else{ 
        if(S_ISDIR(file_info.st_mode) && !S_ISLNK(file_info.st_mode)){
            enqueue(concat_path(path, "/"));
        }
    }
}

void open_directory(){
    struct dirent *p_dirent;
    DIR *p_dir;
    char * path = dequeue();
    printf("path %s\n", path);
    p_dir = opendir (path);
    printf("path %s\n", path);
    if (p_dir == NULL) {
        printf("could not open dir");   
    }else{
        while ((p_dirent = readdir(p_dir)) != NULL) {

            if(!strcmp(p_dirent->d_name, name)){
                printf("is goal\n");
            }else{
                char * new_path = concat_path(path,p_dirent->d_name);

                if(strcmp(p_dirent->d_name, ".") && strcmp(p_dirent->d_name, "..")){
                    check_file_type(new_path);
                }
            }
        }
        closedir (p_dir);
    } 
}

void * traverse_files(){
    while(!is_empty()){
        open_directory();
    }
   return NULL;
}


void read_input_args(int argc , char **argv,int t_flag, int p_flag){
    int start_index = 1;
    if( t_flag == 1 && p_flag == 1){
        start_index = 5;
    }else if((t_flag == 1 && p_flag == 0)|| (t_flag == 0 && p_flag == 1)){
        start_index = 3; 
    }
    for(int i = start_index; i < argc-1; i++){
        enqueue( argv[i]);
    }
    name = argv[argc-1];
}


int main(int argc, char *argv[]){
    int nr_of_threads = 2;
    name = "file.txt";
    int t_flag = 0;
    int p_flag = 0;
    char * type = 0;
    int nrthr = 0;
    char * cvalue = NULL;
    int c;

    while ((c = getopt (argc, argv, "t:p:")) != -1){
        switch (c){
            case 't':
                t_flag = 1;
                type = optarg;
                break;
            case 'p':
                p_flag = 1;
                nrthr = atoi(optarg);
                break;
            default:
                break;
        }
    }
    read_input_args(argc, argv,t_flag,p_flag);



    // pthread_t threads[nr_of_threads];

    //traverse_files();
    // for(int i = 0; i < nr_of_threads-1; i++){
    //     if(pthread_create(&(threads[i]), NULL, &traverse_files, NULL)!= 0){
    //         perror("");
    //     }
   // }

    // for(int i = 0; i < nr_of_threads-1; i++){
    //     pthread_join(threads[i],NULL);
    // }

}