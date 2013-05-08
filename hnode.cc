// hnode.cc
// Rachel Friedman
// 12/10/12

#include "hnode.h"

Hnode::Hnode(char data, size_t count, Hnode * left, Hnode *right) :
  data(data), count(count), left(left), right(right){}
Hnode::Hnode(char data, Hnode * left, Hnode * right) : 
  data(data), count(0), left(left), right(right){}
Hnode::Hnode(Hnode * left, Hnode * right) : 
  data(0), count(0), left(left), right(right){}


