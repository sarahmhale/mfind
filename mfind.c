#include "mfind.h"

static int NUMTHREADS_WAITING;
static char *NAME;
static int NRTHR = 0;
static char *TYPE;
static bool RUNNING = true;
static pthread_mutex_t lock_cond = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

/* Concat two strings and allocated memory.
 * Parameters: Two char * to be concated
 * Returns:  the concated string.
 */
char *concat_path(char *path, char *current_file)
{
    char *str3 = (char *)malloc(1 + strlen(path) + strlen(current_file));
    strcpy(str3, path);
    strcat(str3, current_file);
    return str3;
}

/* Add a path to the queue. 
 */
void add_to_queue(char *path)
{
    char *str = concat_path(path, "/");
    pthread_mutex_lock(&lock_cond);
    enqueue(str);
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&lock_cond);
    free(str);
}

/* Checks which type the file is.
 * Return: "f" if file
 *         "d" if directory
 *         "l" if link
 */
char *check_file_type(char *path)
{
    struct stat file_info;
    if (lstat(path, &file_info) == -1)
    {
        fprintf(stderr, "no such %s: \n", path);
        return NULL;
    }
    else
    {
        switch (file_info.st_mode & S_IFMT)
        {
        case S_IFREG:
            return ("f");
        case S_IFDIR:
            return ("d");
        case S_IFLNK:
            return ("l");
        default:
            return NULL;
        }
    }
}

/* Checks if the file is a match to the file searched for
 * Return: True if a match
 *         False if not a match
 */
bool is_match(char *current_name, char *path)
{
    if (!strcmp(current_name, NAME))
    {
        if (TYPE != 0)
        {
            if (!strcmp(TYPE, check_file_type(path)))
            {
                printf("%s\n", path);
                return true;
            }
        }
        else
        {
            printf("%s\n", path);
            return true;
        }
    }
    return false;
}

/* Loops through all files in directory.
 * Checks if match
 * if a file is a directory it will be added to queue.    
 */
void read_dir(DIR *p_dir, char *path)
{
    struct dirent *p_dirent;
    while ((p_dirent = readdir(p_dir)) != NULL)
    {
        char *new_path = concat_path(path, p_dirent->d_name);
        is_match(p_dirent->d_name, new_path);
        if (strcmp(p_dirent->d_name, ".") && strcmp(p_dirent->d_name, ".."))
        {
            if (!strcmp(check_file_type(new_path), "d"))
            {
                add_to_queue(new_path);
            }
        }
        free(new_path);
    }
}

/* Takes the first element of the queue and opens the directory
 * Then uses read_dir.
 */
void open_directory(int nr_reads)
{

    DIR *p_dir;

    pthread_mutex_lock(&lock_cond);
    char *path = dequeue();
    pthread_mutex_unlock(&lock_cond);

    if (path != NULL)
    {
        p_dir = opendir(path);
        if (p_dir == NULL)
        {
            fprintf(stderr, "%s : ", path);
            perror("opendir error: ");
        }
        else
        {
            read_dir(p_dir, path);
        }
        closedir(p_dir);
        free(path);
    }
}

/* Controls if the threads should open a directory or wait.
 */
void *traverse_files()
{
    int nr_reads = 0;
    while (RUNNING)
    {
        if (!is_empty())
        {
            nr_reads++;
            open_directory(nr_reads);
        }
        else
        {
            pthread_mutex_lock(&lock_cond);
            NUMTHREADS_WAITING++;

            if (NUMTHREADS_WAITING >= NRTHR)
            {
                RUNNING = false;
                pthread_cond_broadcast(&cond);
                pthread_mutex_unlock(&lock_cond);
                break;
            }

            pthread_cond_wait(&cond, &lock_cond);
            NUMTHREADS_WAITING--;
            pthread_mutex_unlock(&lock_cond);
        }
    }
    printf("Threads: %d Reads %d\n", (unsigned int)pthread_self(), nr_reads);
    return NULL;
}

/* Check if last character is /
 * Return: True if the last character is /
 *         False if the last character is not /
 */
bool is_last_character_backslash(char *str)
{
    if (str[strlen(str) - 1] != '/')
    {
        return false;
    }
    else
    {
        return true;
    }
}

/* Check if start path is match 
 */
void start_path()
{
    pthread_mutex_lock(&lock_cond);
    char *path = dequeue();
    pthread_mutex_unlock(&lock_cond);

    if (strstr(path, NAME) != NULL)
    {
        is_match(NAME, path);
    }

    pthread_mutex_lock(&lock_cond);
    enqueue(path);
    pthread_mutex_unlock(&lock_cond);
    free(path);
}

/* Creates threads, runs traverese files and wait for the threads to
 * be done.
 */
void run_threads()
{
    pthread_t threads[NRTHR];
    for (int i = 0; i < NRTHR - 1; i++)
    {
        if (pthread_create(&(threads[i]), NULL, traverse_files, NULL) != 0)
        {
            perror("pthread_create error: ");
        }
    }

    traverse_files();

    for (int i = 0; i < NRTHR - 1; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("pthread_join error: ");
        }
    }
}

/* enqueues input arguments that are not flags and sets the name of the 
 * goal.
 */
void enqueue_input_files(char **argv, int optind, int argc)
{
    char **arguments = argv + optind;

    for (int i = 0; i < (argc - optind - 1); i++)
    {
        char *str = concat_path(arguments[i], "/");
        enqueue(str);
        free(str);
    }
    NAME = arguments[(argc - optind - 1)];
}

bool type(char *optarg)
{

    if (strcmp(optarg, "d") && strcmp(optarg, "f") && strcmp(optarg, "l"))
    {
        fprintf(stderr, "Type for -t is wrong\n");
        return false;
    }
    TYPE = optarg;
    return true;
}

bool p_threads(char *optarg)
{
    for (int i = 0; i < strlen(optarg); i++)
    {
        if (!isdigit(optarg[i]))
        {
            fprintf(stderr, "-p is not followed by a number.\n");
            return false;
        }
    }
    NRTHR = atoi(optarg);
    return true;
}

bool flags(int argc, char **argv)
{
    int c;

    while ((c = getopt(argc, argv, "t:p:")) != -1)
    {
        switch (c)
        {
        case 't':
            if (type(optarg))
            {
                break;
            }
            return false;
        case 'p':
            if (p_threads(optarg))
            {
                break;
            }
            return false;
        default:
            fprintf(stderr, "Invalid flag\n");
            return false;
        }
    }
    return true;
}

/* Reads the flags and checks so that arguments are right
 * Return: 1 if something is wrong
 *         0 if everyting is right
 */
bool read_input(int argc, char **argv)
{

    if (flags(argc, argv))
    {
        enqueue_input_files(argv, optind, argc);
        return true;
    }

    return false;
}

int main(int argc, char *argv[])
{
    NUMTHREADS_WAITING = 0;
    int c;

    if (argc < 3)
    {
        fprintf(stderr, "To few arguments\n");
        return 1;
    }
    if (read_input(argc, argv) == false)
    {
        return 1;
    }

    start_path();
    run_threads();
}
