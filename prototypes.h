typedef struct Node *link;
typedef struct Directory Directory;
typedef struct LLNode LLNode;
struct LLNode{
    Directory* directory;
    LLNode* next, * prev;
};

struct Directory{
    char* path;
    char* value;
    link childrenABC;
    LLNode* children123;
};



struct Node
{
    Directory* directory;
    link l, r;
    int height;
};

link insertR(link h, Directory* directory);
link deleteR(link h, Directory* directory);
void traverseABC(link h);

LLNode* insertLast(LLNode* head, Directory* directory);
