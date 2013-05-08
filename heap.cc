// heap.cc 
// 12/10/12

#include "heap.h"

Heap::Heap()
{
  MAX = 256;
  data = new Hnodep[MAX];
  size = 0;
}

Heap::~Heap()
{
  delete [] data;
}

bool isLessThan(const Hnodep & p1, const Hnodep & p2)
{
  return p1->count < p2->count;
}

size_t parent(size_t k) { return (k-1) / 2; }
size_t leftChild(size_t k) { return 2*k + 1; }
size_t rightChild(size_t k) { return 2*k + 2; }

void swap(Hnodep & a, Hnodep & b)
{
  Hnodep temp = a;
  a = b;
  b = temp;
}

void Heap::_reheap_up(size_t current)
{
  while (current != 0 and
         isLessThan(data[current], data[parent(current)])) {
    swap(data[current], data[parent(current)]);
    current = parent(current);
  }
}

void Heap::_reheap_down(size_t current)
{
  if (leftChild(current) >= size)
    return;
  size_t child = leftChild(current);
  if (rightChild(current) < size and isLessThan(data[rightChild(current)],
                                                data[child]))
    child = rightChild(current);
  if (isLessThan(data[child], data[current])) {
    swap(data[current], data[child]);
    _reheap_down(child);
  }
}

void Heap::add(const Hnodep & item)
{
  data[size] = item;
  _reheap_up(size);
  size++;
}

Hnodep Heap::remove() 
{
  Hnodep answer = data[0];
  data[0] = data[size-1];
  size--;
  _reheap_down(0);
  return answer;
}

size_t Heap::getSize()
{
  return size;
}
