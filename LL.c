#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prototypes.h"

LLNode *NEWNode(Directory *directory)
{
    LLNode *x = (LLNode *)malloc(sizeof(LLNode));
    x->directory = directory;
    x->next = NULL;
    x->prev = NULL;
    return x;
}

LLNode *insertLast(LLNode *head, Directory *directory)
{
    LLNode *x;
    if (head == NULL)
        return NEWNode(directory);
    for (x = head; x->next != NULL; x = x->next)
        ;
    x->next = NEWNode(directory);
    return head;
}

LLNode *deleteLL(LLNode *head, char *text)
{
    LLNode *t, *prev;
    for (t = head, prev = NULL; t != NULL;
         prev = t, t = t->next)
    {
        if (strcmp(t->directory->path, text) == 0)
        {
            if (t == head)
                head = t->next;
            else
                prev->next = t->next;
            free(t);
            break;
        }
    }
    return head;
}

void freeLL(LLNode *head)
{
    LLNode *aux;
    if (head == NULL)
        return;
    aux = head->next;
    free(head);
    freeLL(aux);
}