#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node *link;
struct Node
{
    int id;
    link l, r;
    int height;
};

link max(link Node)
{
    while (Node != NULL && Node->r != NULL)
        Node = Node->r;
    return Node;
}

link NEW(int id, link l, link r)
{
    link x = (link)malloc(sizeof(struct Node));
    x->id = id;
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

link insertR(link h, int id)
{
    if (h == NULL)
        return NEW(id, NULL, NULL);
    if (id < h->id)
        h->l = insertR(h->l, id);
    else
        h->r = insertR(h->r, id);
    h = AVLbalance(h);
    return h;
}

link deleteR(link h, int id)
{
    if (h == NULL)
        return h;
    else if (id < h->id)
        h->l = deleteR(h->l, id);
    else if (id > h->id)
        h->r = deleteR(h->r, id);
    else
    {
        if (h->l != NULL && h->r != NULL)
        {
            link aux = max(h->l);
            int x;
            x = h->id;
            h->id = aux->id;
            aux->id = x;

            h->l = deleteR(h->l, aux->id);
        }
        else
        {
            link aux = h;
            if (h->l == NULL && h->r == NULL)
                h = NULL;
            else if (h->l == NULL)
                h = h->r;
            else
                h = h->l;
            /*deleteDirectory(aux->directory);*/
            free(aux);
        }
    }
    h = AVLbalance(h);
    return h;
}

void visit(link h)
{
    printf("%d\n", h->id);
}

void traverse(link h)
{
    if (h == NULL)
        return;
    traverse(h->l);
    visit(h);
    traverse(h->r);
}

int main()
{
    link treeHead;
    treeHead = NEW(20, NULL, NULL);
    treeHead = insertR(treeHead, 10);
    treeHead = insertR(treeHead, 30);
    treeHead = insertR(treeHead, 25);
    treeHead = insertR(treeHead, 5);
    treeHead = insertR(treeHead, 15);
    treeHead = insertR(treeHead, 1);
    treeHead = insertR(treeHead, 16);
    traverse(treeHead);
    return 0;
}