#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prototypes.h"

Directory *findFolder(link h, char *name)
{
    if (h == NULL)
        return NULL;
    else if (strcmp(name, h->directory->path) == 0)
        return h->directory;
    else if (strcmp(name, h->directory->path) < 0)
        return findFolder(h->l, name);
    else
        return findFolder(h->r, name);
}

void setCommand(Directory *root)
{
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
    while (token != NULL)
    {
        *buffer_ptr = '/';
        ++buffer_ptr;
        strcpy(buffer_ptr, token);
        buffer_ptr += strlen(token);
        *buffer_ptr = '\0';
        if (h->childrenABC != NULL && findFolder(h->childrenABC, buffer_aux) != NULL)
            h = findFolder(h->childrenABC, buffer_aux);
        else
        {
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

void listChildren(Directory *root)
{
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
    while (token != NULL)
    {
        *buffer_ptr = '/';
        ++buffer_ptr;
        strcpy(buffer_ptr, token);
        buffer_ptr += strlen(token);
        *buffer_ptr = '\0';
        if (h->childrenABC != NULL && findFolder(h->childrenABC, buffer_aux) != NULL)
            h = findFolder(h->childrenABC, buffer_aux);
        else
        {
            printf("not found\n");
            return;
        }
        token = strtok(NULL, "/");
    }
    traverseABC(h->childrenABC, strlen(buffer_aux));
}

void findValue(Directory *root)
{
    char *token;
    char buffer[65535 + 1];
    char buffer_aux[65535 + 1];
    char *buffer_ptr = buffer_aux;
    Directory *h;
    getchar();
    scanf("%s", buffer);
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
        else
        {
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

void printEverything(Directory *directory)
{
    LLNode *h;
    /* if (directory == NULL)
        return; */
    for (h = directory->children123; h != NULL; h = h->next)
    {
        if (h->directory->value != NULL)
        {
            printf("%s ", h->directory->path);
            printf("%s", h->directory->value);
        }
        printEverything(h->directory);
    }
}

void printHelp()
{
    puts(
        "help: Imprime os comandos disponíveis.\n"
        "quit: Termina o programa.\n"
        "set: Adiciona ou modifica o valor a armazenar.\n"
        "print: Imprime todos os caminhos e valores.\n"
        "find: Imprime o valor armazenado.\n"
        "list: Lista todos os componentes imediatos de um sub-caminho.\n"
        "search: Procura o caminho dado um valor.\n"
        "delete: Apaga um caminho e todos os subcaminhos.");
}

void deleteDir(Directory *directory)
{
    LLNode *h;
    for (h = directory->children123; h != NULL; h = h->next)
    {
        if (h->directory->children123 != NULL)
            deleteDir(h->directory);
        free(h->directory->path);
        free(h->directory->value);
        free(h->directory);
    }
    if (directory->childrenABC != NULL)
    {
        freeAVL(directory->childrenABC);
        freeLL(directory->children123);
    }
}

void delete (Directory *root)
{
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
        else
        {
            printf("not found\n");
            return;
        }
        token = strtok(NULL, "/");
    }
    if (h == root)
    {
        for (aux = h->children123; aux != NULL; aux = aux->next)
        {
            deleteDir(aux->directory);
            aux->directory->parent->childrenABC = deleteR(aux->directory->parent->childrenABC, aux->directory);
            aux->directory->parent->children123 = deleteLL(aux->directory->parent->children123, aux->directory->path);
            free(aux->directory->value);
            free(aux->directory->path);
            free(aux->directory);
        }
    }
    else

    {
        deleteDir(h);
        h->parent->childrenABC = deleteR(h->parent->childrenABC, h);
        h->parent->children123 = deleteLL(h->parent->children123, h->path);
        free(h->value);
        free(h->path);
        free(h);
    }
}

int main()
{

    int run = 1;
    char command[7];
    Directory *root = (Directory *)malloc(sizeof(Directory));
    root->value = NULL;
    root->children123 = NULL;
    root->childrenABC = NULL;
    root->path = NULL;
    while (run == 1)
    {
        scanf("%s", command);
        if (strcmp(command, "quit") == 0)
        {
            run = 0;
            deleteDir(root);
            free(root);
        }
        else if (strcmp(command, "help") == 0)
            printHelp();

        else if (strcmp(command, "set") == 0)
            setCommand(root);

        else if (strcmp(command, "print") == 0)
            printEverything(root);

        else if (strcmp(command, "find") == 0)
            findValue(root);

        else if (strcmp(command, "list") == 0)
            listChildren(root);

        /* else if (strcmp(command, "search") == 0)
            search();*/

        else if (strcmp(command, "delete") == 0)
            delete (root);
    }
    return 0;
}