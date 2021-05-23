/*
 * File: filesystem.c
 * Author: Diogo Cardoso 99209
 * Description: A program which recreates a file system with a default set
 * of commands.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prototypes.h"
#include "constants.h"


int main(){
    int run = 1;
    char command[7];
    Directory *root = setupRoot();
    while (run == 1){
        scanf("%s", command);
        if (strcmp(command, "help") == 0)
            printHelp();

        else if (strcmp(command, "quit") == 0){
            run = 0;
            deleteDir(root);
            free(root);
        }    
        else if (strcmp(command, "set") == 0)
            setDir(root);

        else if (strcmp(command, "print") == 0)
            printEverything(root);

        else if (strcmp(command, "find") == 0)
            findValue(root);

        else if (strcmp(command, "list") == 0)
            listChildren(root);

        else if (strcmp(command, "search") == 0)
            search(root);

        else if (strcmp(command, "delete") == 0)
            delete(root);
    }
    return 0;
}

Directory* setupRoot(){
    Directory* root = (Directory *)malloc(sizeof(Directory));
    root->value = NULL;
    root->children123 = NULL;
    root->childrenABC = NULL;
    root->path = NULL;
    return root;
}

void printHelp(){
    puts(HELP_DSCP
        QUIT_DSCP   
        SET_DSCP 
        PRINT_DSCP 
        FIND_DSCP 
        LIST_DSCP
        SEARCH_DSCP 
        DELETE_DSCP);
}

void setDir(Directory *root){
    char *token;
    char buffer[65535 + 1];
    char buffer_aux[65535 + 1];
    char *buffer_ptr = buffer_aux;
    Directory *h;
    Directory *newDir;
    getchar();
    scanf("%s", buffer);
    token = strtok(buffer, "/");
    h = root;
    while (token != NULL){
        *buffer_ptr = '/';
        ++buffer_ptr;
        strcpy(buffer_ptr, token);
        buffer_ptr += strlen(token);
        *buffer_ptr = '\0';
        if (h->childrenABC != NULL && findFolder(h->childrenABC, buffer_aux) != NULL)
            h = findFolder(h->childrenABC, buffer_aux);
        else{
            newDir = (Directory *)malloc(sizeof(Directory));
            newDir->path = (char *)malloc(strlen(buffer_aux) + 1);
            strcpy(newDir->path, buffer_aux);
            newDir->childrenABC = NULL;
            newDir->children123 = NULL;
            newDir->value = NULL;
            newDir->parent = h;
            h->childrenABC = insertR(h->childrenABC, newDir);
            h->children123 = insertLast(h->children123, newDir);
            h = newDir;
        }
        token = strtok(NULL, "/");
    }
    getchar();
    fgets(buffer, 65536, stdin);
    if (h->value != NULL)
        free(h->value);
    h->value = (char *)malloc(strlen(buffer) + 1);
    strcpy(h->value, buffer);
}

Directory* findFolder(AVLNode *h, char *name){
    if (h == NULL)
        return NULL;
    else if (strcmp(name, h->directory->path) == 0)
        return h->directory;
    else if (strcmp(name, h->directory->path) < 0)
        return findFolder(h->l, name);
    else
        return findFolder(h->r, name);
}

void printEverything(Directory *root){
    LLNode *aux;
    for (aux = root->children123; aux != NULL; aux = aux->next){
        if (aux->directory->value != NULL){
            printf("%s ", aux->directory->path);
            printf("%s", aux->directory->value);
        }
        printEverything(aux->directory);
    }
}

void findValue(Directory *root){
    char *token;
    char buffer[65535 + 1];
    char buffer_aux[65535 + 1];
    char *buffer_ptr = buffer_aux;
    Directory *h;
    getchar();
    scanf("%s", buffer);
    token = strtok(buffer, "/");
    h = root;
    while (token != NULL){
        *buffer_ptr = '/';
        ++buffer_ptr;
        strcpy(buffer_ptr, token);
        buffer_ptr += strlen(token);
        *buffer_ptr = '\0';
        if (h->childrenABC != NULL && findFolder(h->childrenABC, buffer_aux) != NULL)
            h = findFolder(h->childrenABC, buffer_aux);
        else{
            printf("not found\n");
            return;
        }
        token = strtok(NULL, "/");
    }
    if (h->value != NULL)
        printf("%s", h->value);
    else
        printf("no data\n");
}

void listChildren(Directory *root){
    char *token;
    char buffer[65535 + 1];
    char buffer_aux[65535 + 1];
    char *buffer_ptr = buffer_aux;
    Directory *h;
    buffer_aux[0] = '\0';
    token = NULL;
    if (scanf("%*[ ]%s", buffer))
        token = strtok(buffer, "/");
    h = root;
    while (token != NULL){
        *buffer_ptr = '/';
        ++buffer_ptr;
        strcpy(buffer_ptr, token);
        buffer_ptr += strlen(token);
        *buffer_ptr = '\0';
        if (h->childrenABC != NULL && findFolder(h->childrenABC, buffer_aux) != NULL)
            h = findFolder(h->childrenABC, buffer_aux);
        else{
            printf("not found\n");
            return;
        }
        token = strtok(NULL, "/");
    }
    traverseABC(h->childrenABC, strlen(buffer_aux));
}

int searchR(Directory *directory, char *keyword){
    LLNode *h;
    for (h = directory->children123; h != NULL; h = h->next){
        if (h->directory->value != NULL && !strcmp(h->directory->value, keyword)){
            printf("%s\n", h->directory->path);
            return 1;
        }
        if (searchR(h->directory, keyword))
            return 1;
    }
    return 0;
}

void search(Directory *directory){
    char *keyword;
    char buffer[65535 + 1];
    getchar();
    fgets(buffer, 65536, stdin);
    keyword = (char *)malloc(strlen(buffer) + 1);
    strcpy(keyword, buffer);
    if (searchR(directory, keyword)){
        free(keyword);
        return;
    }
    printf("not found\n");
    free(keyword);
}

void deleteDir(Directory *directory){
    LLNode *h;
    for (h = directory->children123; h != NULL; h = h->next){
        if (h->directory->children123 != NULL)
            deleteDir(h->directory);
        free(h->directory->path);
        free(h->directory->value);
        free(h->directory);
    }
    if (directory->childrenABC != NULL){
        freeAVL(directory->childrenABC);
        freeLL(directory->children123);
    }
}

void delete (Directory *root){
    char *token;
    char buffer[65535 + 1];
    char buffer_aux[65535 + 1];
    char *buffer_ptr = buffer_aux;
    Directory *h;
    LLNode *aux;
    buffer_aux[0] = '\0';
    token = NULL;
    if (scanf("%*[ ]%s", buffer))
        token = strtok(buffer, "/");
    h = root;
    while (token != NULL)
    {
        *buffer_ptr = '/';
        ++buffer_ptr;
        strcpy(buffer_ptr, token);
        buffer_ptr += strlen(token);
        *buffer_ptr = '\0';
        if (h->childrenABC != NULL && findFolder(h->childrenABC, buffer_aux) != NULL)
            h = findFolder(h->childrenABC, buffer_aux);
        else{
            printf("not found\n");
            return;
        }
        token = strtok(NULL, "/");
    }
    if (h == root){
        for (aux = h->children123; aux != NULL; aux = aux->next){
            deleteDir(aux->directory);
            free(aux->directory->value);
            free(aux->directory->path);
            free(aux->directory);
        }
        if (h->childrenABC != NULL){
            freeAVL(h->childrenABC);
            h->childrenABC = NULL;
            freeLL(h->children123);
            h->children123 = NULL;
        }
    }
    else{
        deleteDir(h);
        h->parent->childrenABC = deleteR(h->parent->childrenABC, h);
        h->parent->children123 = deleteLL(h->parent->children123, h);
        free(h->value);
        free(h->path);
        free(h);
    }
}