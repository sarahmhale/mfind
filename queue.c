#include "queue.h"

/* this implematation is taken from
 * http://www.cprogrammingnotes.com/question/dynamic-queue.html,
 * small edits made by me.
 */
struct node *front;
struct node *rear;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


node * enqueue(char * item)
{   
    struct node *nptr = malloc(sizeof(node));
    nptr->data = malloc(strlen(item)+1);
    strcpy(nptr->data, item);
    nptr->next = NULL;

    pthread_mutex_lock(&lock);
    if (rear == NULL || front == NULL) 
    {
        front = nptr;
        rear = nptr;

    }
    else
    {
        rear->next = nptr;
        rear = rear->next;
    }
    pthread_mutex_unlock(&lock);
    return front;
}

bool is_empty(){
    pthread_mutex_lock(&lock);
    if( front == NULL){
        pthread_mutex_unlock(&lock);
        return true;
    }
    pthread_mutex_unlock(&lock);
    return false;
    
}

void display()
{
    struct node *temp;
    temp = front;
    printf("\n");
    while (temp != NULL)
    {
        printf("%s\t", temp->data);
        temp = temp->next;
    }
}

char * dequeue()
{
   
    if (front == NULL)
    {
        return NULL;
    }
    else
    {
        pthread_mutex_lock(&lock);
        struct node *temp;
        temp = front;
        front = front->next;
        char * start = temp->data;
        free(temp);
        pthread_mutex_unlock(&lock);
        return start;
    }
     
}