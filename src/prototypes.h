/* Structs */
typedef struct AVLNode AVLNode;
typedef struct Directory Directory;
typedef struct LLNode LLNode;

struct Directory{
    char *path;
    char *value;
    Directory *parent;
    AVLNode *childrenABC;
    LLNode *children123;
};

struct LLNode{
    Directory *directory;
    LLNode *next, *prev;
};

struct AVLNode{
    Directory *directory;
    AVLNode *l, *r;
    int height;
};

/* AVL prototypes */
AVLNode* insertR(AVLNode*, Directory*);
AVLNode* deleteR(AVLNode*, Directory*);
void traverse(AVLNode*, int);
void freeAVL(AVLNode*);

/* Linked list prototypes */
LLNode* insertLast(LLNode*, Directory*);
LLNode* deleteLL(LLNode*, Directory*);
void freeLL(LLNode*);

/* Main file prototypes */
Directory* setupRoot();
Directory* findFolder(AVLNode*, char*);
void setDir(Directory*);
void listChildren(Directory*);
void findValue(Directory*);
void printEverything(Directory*);
void printHelp();
void deleteDir(Directory*);
void delete (Directory*);
int searchR(Directory*, char*);
void search(Directory*);
Directory* getDir(Directory*);