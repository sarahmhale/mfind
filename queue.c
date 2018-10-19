#include "queue.h"

/* this implematation is taken from
 * http://www.cprogrammingnotes.com/question/dynamic-queue.html,
 * small edits made by me.
 */
struct node *front = NULL;
struct node *rear = NULL;

node * enqueue(char * item)
{   
    struct node *nptr = malloc(sizeof(node));
    nptr->data = malloc(strlen(item)+1);
    strcpy(nptr->data, item);
    nptr->next = NULL;
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


    return front;
}

bool is_empty(){
    if( front == NULL){
        return true;
    }
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
        printf("\n\nqueue is emptysssss \n");
        return NULL;
    }
    else
    {
        struct node *temp;
        temp = front;
        front = front->next;
        char * start = temp->data;
        free(temp);
        return start;
    }
}