#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prototypes.h"

/* creates a new node */
LLNode* newLLNode(Directory *directory){
    LLNode *newNode = (LLNode*)malloc(sizeof(LLNode));
    newNode->directory = directory;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

/* inserts a node at the end of the list */
LLNode* insertLast(LLNode *head, Directory *directory){
    LLNode *aux;
    if (head == NULL)
        return newLLNode(directory);
    for (aux = head; aux->next != NULL; aux = aux->next)
        ;
    aux->next = newLLNode(directory);
    return head;
}

/* deletes specified node from list */
LLNode* deleteLL(LLNode *head, Directory* directory){
    LLNode *aux, *prev;
    for (aux = head, prev = NULL; aux != NULL;
         prev = aux, aux = aux->next){
        if (strcmp(aux->directory->path, directory->path) == 0){
            if (aux == head)
                head = aux->next;
            else
                prev->next = aux->next;
            free(aux);
            break;
        }
    }
    return head;
}

/* frees all nodes in LL */
void freeLL(LLNode *head){
    LLNode *aux;
    if (head == NULL)
        return;
    aux = head->next;
    free(head);
    freeLL(aux);
}