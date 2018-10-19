#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* this implematation is taken from
 * http://www.cprogrammingnotes.com/question/dynamic-queue.html,
 * small edits made by me.
 */

typedef struct node
{
    char * data;
    struct node *next;
}node;

void display();
node * enqueue(char *);
char * dequeue();
bool is_empty();