#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>


/* this implematation is inspired by
 * http://www.cprogrammingnotes.com/question/dynamic-queue.html,
 * I have edited it so that it fits my needs.
 */

typedef struct node
{
    char * data;
    struct node *next;
}node;

node * enqueue(char *);
char * dequeue();
bool is_empty();

