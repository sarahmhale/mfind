#include "mfind.h"
#include "queue.h"
int NUMTHREADS_EXECUTING;
char * NAME;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t cond_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;


char * concat_path(char * path, char * current_file){
    char * str3 = (char *) malloc(1 + strlen(path)+ strlen(current_file));
    strcpy(str3, path);
    strcat(str3, current_file);
    return str3;
}

void check_file_type(char * path){
    struct stat file_info;

    if(lstat(path, &file_info)==-1){
        fprintf(stderr,"no such %s : ", path);
        perror("");
    }else{ 
        if(S_ISDIR(file_info.st_mode) && !S_ISLNK(file_info.st_mode)){
            char * str = concat_path(path, "/");
            enqueue(str);
            free(str);
        }
    }
    free(path);
}

void open_directory(int nr_reads){
    struct dirent *p_dirent;
    DIR *p_dir;

    //pthread_mutex_lock(&lock);
    char * path = dequeue();
    //pthread_mutex_unlock(&lock);

    p_dir = opendir (path);

    if (p_dir == NULL) {
        printf("could not open dir");   
    }else{
        while ((p_dirent = readdir(p_dir)) != NULL) {
            char * new_path = concat_path(path,p_dirent->d_name);
            
            if(!strcmp(p_dirent->d_name, NAME)){
                printf("%s\n",new_path);
                free(new_path);
                
            }else{
                if(strcmp(p_dirent->d_name, ".") && strcmp(p_dirent->d_name, "..")){
                    check_file_type(new_path);
                }else{
                    free(new_path);
                }
            }
          
        }
        closedir (p_dir);
    } 
    pthread_cond_signal( &cond );
    free(path);
}

void * traverse_files(){
    int nr_reads = 0;
    printf("Running: %d\n",(unsigned int)pthread_self() );

    //should NUMTHREADS_EXECUTING > 0 || !is_empty()
    while(NUMTHREADS_EXECUTING > 0 && !is_empty()){
        printf("NUMTHREADS %d: ", NUMTHREADS_EXECUTING);
        //pthread_mutex_lock( &lock);
        if(is_empty() == true){
            printf("Waiting on condition variable cond1\n"); 
           // pthread_cond_wait(&cond, &lock); 

        }else{
            nr_reads++;
            printf("Running: %d Reads %d\n",(unsigned int)pthread_self(),nr_reads );
            open_directory(nr_reads);
           // pthread_cond_signal(&cond); 
        }
       // pthread_mutex_unlock( &lock);
    }
    

    printf("Threads: %d Reads %d\n",(unsigned int)pthread_self(),nr_reads );
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
    NAME = argv[argc-1];
}




int main(int argc, char *argv[]){
    pthread_mutex_init(&lock, NULL);
    NUMTHREADS_EXECUTING = 1;
    int nr_of_threads = 2;
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
                NUMTHREADS_EXECUTING = nrthr;
                break;
            default:
               
                break;
        }
    }
    read_input_args(argc, argv,t_flag,p_flag);
    pthread_t threads[nrthr];
    
    for(int i = 0; i < nrthr-1; i++){
        if(pthread_create(&(threads[i]), NULL, &traverse_files, NULL)!= 0){
            perror("");
        }
    }
    traverse_files();

    for(int i = 0; i < nrthr-1; i++){
        pthread_join(threads[i],NULL);
    }
}