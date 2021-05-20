#include <stdio.h>
#include <stdlib.h>
#include "prototypes.h"

LLNode* NEWNode(Directory* directory)
{
    LLNode* x = (LLNode*)malloc(sizeof(LLNode));
    x->directory = directory;
    x->next = NULL;
    x->prev = NULL;
    return x;
}

LLNode* insertLast(LLNode* head, Directory* directory){
    LLNode* x;
    if (head == NULL)
        return NEWNode(directory);
    for (x = head; x->next != NULL; x = x->next)
        ;
    x->next = NEWNode(directory);
    return head;
}