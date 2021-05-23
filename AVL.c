/*
 * File:  AVL.c
 * Author:  Diogo Cardoso 99209
 * Description: File where the AVL struct related functions are defined.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prototypes.h"


AVLNode* newAVLNode(Directory *directory, AVLNode *l, AVLNode *r){
    AVLNode *newNode = (AVLNode *)malloc(sizeof(AVLNode));
    newNode->directory = directory;
    newNode->l = l;
    newNode->r = r;
    newNode->height = 1;
    return newNode;
}

int height(AVLNode *node){
    if (node == NULL)
        return 0;
    return node->height;
}

AVLNode* max(AVLNode *node)
{
    while (node != NULL && node->r != NULL)
        node = node->r;
    return node;
}

AVLNode* rotL(AVLNode *node){
    int nodeleft, noderight, auxleft, auxright;
    AVLNode * aux = node->r;
    node->r = aux->l;
    aux->l = node;
    nodeleft = height(node->l);
    noderight = height(node->r);
    node->height = nodeleft > noderight ? nodeleft + 1 : noderight + 1;
    auxleft = height(aux->l);
    auxright = height(aux->r);
    aux->height = auxleft > auxright ? auxleft + 1 : auxright + 1;
    return aux;
}

AVLNode* rotR(AVLNode *node){
    int nodeleft, noderight, auxleft, auxright;
    AVLNode* aux = node->l;
    node->l = aux->r;
    aux->r = node;
    nodeleft = height(node->l);
    noderight = height(node->r);
    node->height = nodeleft > noderight ? nodeleft + 1 : noderight + 1;
    auxleft = height(aux->l);
    auxright = height(aux->r);
    aux->height = auxleft > auxright ? auxleft + 1 : auxright + 1;
    return aux;
}

AVLNode* rotLR(AVLNode *node){
    if (node == NULL)
        return node;
    node->l = rotL(node->l);
    return rotR(node);
}
AVLNode* rotRL(AVLNode *node){
    if (node == NULL)
        return node;
    node->r = rotR(node->r);
    return rotL(node);
}
int balance(AVLNode *node){
    if (node == NULL)
        return 0;
    return height(node->l) - height(node->r);
}

AVLNode* AVLbalance(AVLNode *node){
    int balanceFactor, nodeleft, noderight;
    if (node == NULL)
        return node;
    balanceFactor = balance(node);
    if (balanceFactor > 1){ 
        if (balance(node->l) >= 0)
            node = rotR(node);
        else
            node = rotLR(node);
    }
    else if (balanceFactor < -1){ 
        if (balance(node->r) <= 0)
            node = rotL(node);
        else
            node = rotRL(node);
    }
    else{
        nodeleft = height(node->l);
        noderight = height(node->r);
        node->height = nodeleft > noderight ? nodeleft + 1 : noderight + 1;
    }
    return node;
}

AVLNode* insertR(AVLNode *node, Directory *directory){
    if (node == NULL)
        return newAVLNode(directory, NULL, NULL);
    if (strcmp(directory->path, node->directory->path) < 0)
        node->l = insertR(node->l, directory);
    else if (strcmp(directory->path, node->directory->path) > 0)
        node->r = insertR(node->r, directory);
    node = AVLbalance(node);
    return node;
}

AVLNode* deleteR(AVLNode *node, Directory *directory){
    if (node == NULL)
        return node;
    else if (strcmp(directory->path, node->directory->path) < 0)
        node->l = deleteR(node->l, directory);
    else if (strcmp(directory->path, node->directory->path) > 0)
        node->r = deleteR(node->r, directory);
    else{
        if (node->l != NULL && node->r != NULL){
            AVLNode *node_aux = max(node->l);
            Directory *dir_aux;
            dir_aux = node->directory;
            node->directory = node_aux->directory;
            node_aux->directory = dir_aux;
            node->l = deleteR(node->l, node_aux->directory);
        }
        else{
            AVLNode * node_aux = node;
            if (node->l == NULL && node->r == NULL)
                node = NULL;
            else if (node->l == NULL)
                node = node->r;
            else
                node = node->l;
            free(node_aux);
        }
    }
    node = AVLbalance(node);
    return node;
}

void visitABC(Directory *directory, int i)
{
    char *folder = directory->path;
    folder += i + 1;
    printf("%s\n", folder);
}

void traverseABC(AVLNode *node, int i){
    if (node == NULL)
        return;
    traverseABC(node->l, i);
    visitABC(node->directory, i);
    traverseABC(node->r, i);
}

void freeAVL(AVLNode *head){
    AVLNode *aux,*aux2;
    if (head == NULL)
        return;
    aux = head->l;
    aux2 = head->r;
    free(head);
    freeAVL(aux);
    freeAVL(aux2);
}