#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prototypes.h"

link max(link Node)
{
    while (Node != NULL && Node->r != NULL)
        Node = Node->r;
    return Node;
}

link NEW(Directory *directory, link l, link r)
{
    link x = (link)malloc(sizeof(struct Node));
    x->directory = directory;
    x->l = l;
    x->r = r;
    x->height = 1;
    return x;
}

int height(link h)
{
    if (h == NULL)
        return 0;
    return h->height;
}

link rotL(link h)
{
    int hleft, hright, xleft, xright;
    link x = h->r;
    h->r = x->l;
    x->l = h;
    hleft = height(h->l);
    hright = height(h->r);
    h->height = hleft > hright ? hleft + 1 : hright + 1;
    xleft = height(x->l);
    xright = height(x->r);
    x->height = xleft > xright ? xleft + 1 : xright + 1;
    return x;
}

link rotR(link h)
{
    int hleft, hright, xleft, xright;
    link x = h->l;
    h->l = x->r;
    x->r = h;
    hleft = height(h->l);
    hright = height(h->r);
    h->height = hleft > hright ? hleft + 1 : hright + 1;
    xleft = height(x->l);
    xright = height(x->r);
    x->height = xleft > xright ? xleft + 1 : xright + 1;
    return x;
}

link rotLR(link h) /* rotação dupla esquerda direita */
{
    if (h == NULL)
        return h;
    h->l = rotL(h->l);
    return rotR(h);
}
link rotRL(link h) /* rotação dupla direita esquerda */
{
    if (h == NULL)
        return h;
    h->r = rotR(h->r);
    return rotL(h);
}
int balance(link h) /* balance factor*/
{
    if (h == NULL)
        return 0;
    return height(h->l) - height(h->r);
}

link AVLbalance(link h)
{
    int balanceFactor, hleft, hright;
    if (h == NULL)
        return h;
    balanceFactor = balance(h);
    if (balanceFactor > 1)
    { /* mais peso para a esquerda */
        if (balance(h->l) >= 0)
            h = rotR(h);
        else
            h = rotLR(h);
    }
    else if (balanceFactor < -1)
    { /* mais peso para a direita*/
        if (balance(h->r) <= 0)
            h = rotL(h);
        else
            h = rotRL(h);
    }
    else
    {
        hleft = height(h->l);
        hright = height(h->r);
        h->height = hleft > hright ? hleft + 1 : hright + 1;
    }
    return h;
}

link insertR(link h, Directory *directory)
{
    if (h == NULL)
        return NEW(directory, NULL, NULL);
    if (strcmp(directory->path, h->directory->path) < 0)
        h->l = insertR(h->l, directory);
    else if (strcmp(directory->path, h->directory->path) > 0)
        h->r = insertR(h->r, directory);
    h = AVLbalance(h);
    return h;
}

link deleteR(link h, Directory *directory)
{
    if (h == NULL)
        return h;
    else if (strcmp(directory->path, h->directory->path) < 0)
        h->l = deleteR(h->l, directory);
    else if (strcmp(directory->path, h->directory->path) > 0)
        h->r = deleteR(h->r, directory);
    else
    {
        if (h->l != NULL && h->r != NULL)
        {
            link aux = max(h->l);
            Directory *x;
            x = h->directory;
            h->directory = aux->directory;
            aux->directory = x;

            h->l = deleteR(h->l, aux->directory);
        }
        else
        {
            if (h->l == NULL && h->r == NULL)
                h = NULL;
            else if (h->l == NULL)
                h = h->r;
            else
                h = h->l;
        }
    }
    h = AVLbalance(h);
    return h;
}

void visitABC(Directory *directory, int i)
{
    char *folder = directory->path;
    folder += i + 1;
    printf("%s\n", folder);
}

void traverseABC(link h, int i)
{
    if (h == NULL)
        return;
    traverseABC(h->l, i);
    visitABC(h->directory, i);
    traverseABC(h->r, i);
}

void freeAVL(link head)
{
    link aux, aux2;
    if (head == NULL)
        return;
    aux = head->l;
    aux2 = head->r;
    free(head);
    freeAVL(aux);
    freeAVL(aux2);
}