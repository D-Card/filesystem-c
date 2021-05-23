#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prototypes.h"
#include "constants.h"

/* read the command and runs appropriate functions */
int main(){
    int run = 1;
    char command[7];
    Directory *root = setupRoot();
    while (run == 1){
        scanf("%s", command);
        if (strcmp(command, HELP) == 0)
            printHelp();

        else if (strcmp(command, QUIT) == 0){
            run = 0;
            deleteDir(root);
            free(root);
        }    
        else if (strcmp(command, SET) == 0)
            setDir(root);

        else if (strcmp(command, PRINT) == 0)
            printEverything(root);

        else if (strcmp(command, FIND) == 0)
            findValue(root);

        else if (strcmp(command, LIST) == 0)
            listChildren(root);

        else if (strcmp(command, SEARCH) == 0)
            search(root);

        else if (strcmp(command, DELETE) == 0)
            delete(root);
    }
    return 0;
}

/* sets up root struct when starting the program */
Directory* setupRoot(){
    Directory* root = (Directory *)malloc(sizeof(Directory));
    root->value = NULL;
    root->children123 = NULL;
    root->childrenABC = NULL;
    root->path = NULL;
    return root;
}

/* prints help text */
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

/* ran when "set" command is invoked. creates directory */
void setDir(Directory *root){
    char *token;
    char buffer[MAX_CMD_SIZE + 1];
    char buffer_aux[MAX_CMD_SIZE + 1];
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
        /* check if directory already exists */
        if (h->childrenABC != NULL && findFolder(h->childrenABC, buffer_aux) != NULL)
            h = findFolder(h->childrenABC, buffer_aux);
        else{
            /* create new directory */
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
    fgets(buffer, MAX_CMD_SIZE + 1, stdin);
    if (h->value != NULL)
        free(h->value);
    h->value = (char *)malloc(strlen(buffer) + 1);
    strcpy(h->value, buffer);
}

/* auxiliary function, recursively tries to find the folder which path == name */
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

/* ran when "print" command is invoked. prints all paths */
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

/* ran when "find" command is invoked. prints the value of a specified path */
void findValue(Directory *root){
    Directory* dir;
    dir = getDir(root);
    if (dir != NULL){
        if (dir->value != NULL)
            printf("%s", dir->value);
        else
            printf(NO_DATA);
    }
}

/* ran when "list"command is invoked. lists all children of a specified directory */
void listChildren(Directory *root){
    char *token;
    char buffer[MAX_CMD_SIZE + 1];
    char buffer_aux[MAX_CMD_SIZE + 1];
    char *buffer_ptr = buffer_aux;
    Directory *h;
    buffer_aux[0] = '\0';
    token = NULL;
    if (scanf("%*[ ]%s", buffer))
        token = strtok(buffer, "/");
    h = root;
    /* get to the specified path */
    while (token != NULL){
        *buffer_ptr = '/';
        ++buffer_ptr;
        strcpy(buffer_ptr, token);
        buffer_ptr += strlen(token);
        *buffer_ptr = '\0';
        if (h->childrenABC != NULL && findFolder(h->childrenABC, buffer_aux) != NULL)
            h = findFolder(h->childrenABC, buffer_aux);
        else{
            printf(NOT_FOUND);
            return;
        }
        token = strtok(NULL, "/");
    }
    traverse(h->childrenABC, strlen(buffer_aux));
}

/* auxiliary function. recursively searches for a path which value is keyword */
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

/* ran when "search" command is invoked. prints path with specified value */
void search(Directory *directory){
    char *keyword;
    char buffer[MAX_CMD_SIZE + 1];
    getchar();
    fgets(buffer, MAX_CMD_SIZE + 1, stdin);
    keyword = (char *)malloc(strlen(buffer) + 1);
    strcpy(keyword, buffer);
    if (searchR(directory, keyword)){
        free(keyword);
        return;
    }
    printf(NOT_FOUND);
    free(keyword);
}

/* auxiliary function. recursively deletes a directory and its children */
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

/* ran when "delete" command is invoked. deletes a directory and all its children */
void delete (Directory *root){
    LLNode *aux;
    Directory* dir;
    dir = getDir(root);
    if (dir != NULL){
        /* root directory is special because we can´t delete the root struct */
        if (dir == root){
            for (aux = dir->children123; aux != NULL; aux = aux->next){
                deleteDir(aux->directory);
                free(aux->directory->value);
                free(aux->directory->path);
                free(aux->directory);
            }
            if (dir->childrenABC != NULL){
                freeAVL(dir->childrenABC);
                dir->childrenABC = NULL;
                freeLL(dir->children123);
                dir->children123 = NULL;
            }
        }
        else{
            deleteDir(dir);
            dir->parent->childrenABC = deleteR(dir->parent->childrenABC, dir);
            dir->parent->children123 = deleteLL(dir->parent->children123, dir);
            free(dir->value);
            free(dir->path);
            free(dir);
        }
    }
}

/* auxiliary function. reads from stdin and returns specified directory */
Directory* getDir(Directory* root){
    char *token;
    char buffer[MAX_CMD_SIZE + 1];
    char buffer_aux[MAX_CMD_SIZE + 1];
    char *buffer_ptr = buffer_aux;
    Directory *h;
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
            printf(NOT_FOUND);
            return NULL;
        }
        token = strtok(NULL, "/");
    }
    return h;
}