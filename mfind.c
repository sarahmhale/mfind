#include "mfind.h"

static int NUMTHREADS_WAITING;
static char * NAME;
static int NRTHR = 0;
static char * TYPE;
static bool RUNNING = true;

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t lock_cond = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;


char * concat_path(char * path, char * current_file){
    char * str3 = (char *) malloc(1 + strlen(path)+ strlen(current_file));
    strcpy(str3, path);
    strcat(str3, current_file);
    return str3;
}

void add_to_queue(char * path){
    char * str = concat_path(path, "/");
    pthread_mutex_lock(&lock);
    enqueue(str);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&lock);
    free(str); 
}

char * check_file_type(char * path){
    struct stat file_info;
    if(lstat(path, &file_info)==-1){
        fprintf(stderr,"no such %s : ", path);
        perror("");
    }else{
        switch (file_info.st_mode & S_IFMT) {
            case S_IFREG:  
                return("f");
            case S_IFDIR:
                return("d");
            case S_IFLNK: 
                return("l");
        }
    }
}

bool is_match(char * current_name,char * path){
    if(!strcmp(current_name, NAME)){
        if(TYPE != 0){
            if(!strcmp(TYPE, check_file_type(path))){
                printf("%s\n",path);
                free(path);
                return true;   
            }
        }else{
            printf("%s\n",path);
            free(path);
            return true;  
        }
    }
    return false;
}

void open_directory(int nr_reads){
    struct dirent *p_dirent;
    DIR *p_dir;

    pthread_mutex_lock(&lock);
    char * path = dequeue();
    pthread_mutex_unlock(&lock);

    if(path != NULL){
        p_dir = opendir (path);
        if(p_dir == NULL){
            fprintf(stderr, "%s :", path);
            perror("");
        }else{
            while ((p_dirent = readdir(p_dir)) != NULL) {
                char * new_path = concat_path(path,p_dirent->d_name);
                if(!is_match(p_dirent->d_name, new_path)){
                    if(strcmp(p_dirent->d_name, ".") && strcmp(p_dirent->d_name, "..")){
                        if(!strcmp(check_file_type(new_path),"d")){
                            add_to_queue(new_path);
                        }
                    }
                    free(new_path); 
                }
            }
            closedir (p_dir);
        } 
        free(path);
    } 
}

void * traverse_files(){
    int nr_reads = 0;
    while(RUNNING){
        if(!is_empty()){
            nr_reads++;
            open_directory(nr_reads);  
        }
        else{
            pthread_mutex_lock( &lock_cond);
            NUMTHREADS_WAITING++;

            if(NUMTHREADS_WAITING >= NRTHR){
                RUNNING = false;
                pthread_cond_broadcast(&cond);
                pthread_mutex_unlock( &lock_cond);
                break;  
            }

            pthread_cond_wait(&cond, &lock_cond);
            NUMTHREADS_WAITING--;
            pthread_mutex_unlock( &lock_cond);
        }
    }
    printf("Threads: %d Reads %d\n",(unsigned int)pthread_self(),nr_reads );
    return NULL;
}

bool is_last_character_backslash(char * str){
    if(str[strlen(str) - 1] != '/'){
        return false;
    }else{
        return true;
    }
}

void read_input_args(int argc , char **argv,int t_flag, int p_flag){
    int start_index = 1;
    if( t_flag == 1 && p_flag == 1){
        start_index = 5;
    }else if((t_flag == 1 && p_flag == 0)|| (t_flag == 0 && p_flag == 1)){
        start_index = 3; 
    }

    for(int i = start_index; i < argc-1; i++){
        if(is_last_character_backslash(argv[i])){
           enqueue( argv[i]);
        }else{
            char * str = concat_path(argv[i],"/");
            enqueue(str);
            free(str);
        }   
    }
    NAME = argv[argc-1];
}

int main(int argc, char *argv[]){
    NUMTHREADS_WAITING = 0;
    int t_flag = 0;
    int p_flag = 0;
    char * cvalue = NULL;
    int c;
    
    while ((c = getopt (argc, argv, "t:p:")) != -1){
        switch (c){
            case 't':
                t_flag = 1;
                TYPE = optarg;
                break;
            case 'p':
                p_flag = 1;
                NRTHR = atoi(optarg);
                break;
            default:
                break;
        }
    }
    read_input_args(argc, argv,t_flag,p_flag);

    pthread_t threads[NRTHR];
    for(int i = 0; i < NRTHR-1; i++){
        if(pthread_create(&(threads[i]), NULL, traverse_files, NULL) != 0){
            perror("");
        }
    }

    traverse_files();

    for(int i = 0; i < NRTHR-1; i++){
        pthread_join(threads[i],NULL);
    }
}

