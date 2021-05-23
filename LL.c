/*
 * File:  LL.c
 * Author:  Diogo Cardoso 99209
 * Description: File where the Linked list struct related functions are defined.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prototypes.h"

LLNode* newLLNode(Directory *directory){
    LLNode *newNode = (LLNode*)malloc(sizeof(LLNode));
    newNode->directory = directory;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

LLNode* insertLast(LLNode *head, Directory *directory){
    LLNode *aux;
    if (head == NULL)
        return newLLNode(directory);
    for (aux = head; aux->next != NULL; aux = aux->next)
        ;
    aux->next = newLLNode(directory);
    return head;
}

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

void freeLL(LLNode *head){
    LLNode *aux;
    if (head == NULL)
        return;
    aux = head->next;
    free(head);
    freeLL(aux);
}