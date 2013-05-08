// heap.h
// 12/10/12

#ifndef HEAP_H
#define HEAP_H

#include "hnode.h"


class Heap {
  
 public:
  Heap();
  ~Heap();
  void add(const Hnodep & item);
  Hnodep remove();
  size_t getSize();

 private:
  void _reheap_up(size_t current);
  void _reheap_down(size_t current);
  size_t MAX;
  Hnodep *data;
  size_t size;

};

#endif
