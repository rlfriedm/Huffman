// hnode.h
// Rachel Friedman
// 12/10/12

#include <cstdlib>

struct Hnode {
  char data;
  size_t count;
  Hnode *left, *right;
  Hnode(char data, size_t count, Hnode * left=NULL, Hnode *right=NULL);
  Hnode(char data, Hnode * left=NULL, Hnode * right=NULL);
  Hnode(Hnode * left=NULL, Hnode * right=NULL);
};

typedef Hnode * Hnodep;



