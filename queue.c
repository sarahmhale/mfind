#include "queue.h"

/* this implematation is inspired by
 * http://www.cprogrammingnotes.com/question/dynamic-queue.html,
 * I have edited it so that it fits my needs.
 */
struct node *front;
struct node *rear;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/* Adds a node to the end of the list and
 * sets the data of the node to the inparameter of the 
 * function. Returns the node.
 */
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

/* Checks if the queue is empty and returns 
 * a boolean.
 */
bool is_empty(){
    pthread_mutex_lock(&lock);
    if( front == NULL){
        pthread_mutex_unlock(&lock);
        return true;
    }
    pthread_mutex_unlock(&lock);
    return false;
    
}

/* Removes the first element in the queue and
 * return the data that the node contains.
 */
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