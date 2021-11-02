#include <iostream>

#include "DynamicAllocation.h"

// Source: https://stackoverflow.com/questions/9927856/how-to-use-operator-new-to-count-number-of-times-of-dynamic-memory-allocation

unsigned long long DynamicAllocation::numAllocations = 0;

void* operator new(std::size_t size) {
    DynamicAllocation::numAllocations++;
  void *p = malloc(size);
  if(!p) throw std::bad_alloc();
  return p;
}

void* operator new  [](std::size_t size) {
    DynamicAllocation::numAllocations++;
  void *p = malloc(size);
  if(!p) throw std::bad_alloc();
  return p;
}

void* operator new  [](std::size_t size, const std::nothrow_t&) noexcept {
  DynamicAllocation::numAllocations++;
  return malloc(size);
}
void* operator new   (std::size_t size, const std::nothrow_t&) noexcept {
  DynamicAllocation::numAllocations++;
  return malloc(size);
}


void operator delete(void* ptr) throw() { free(ptr); }
void operator delete (void* ptr, const std::nothrow_t&) noexcept { free(ptr); }
void operator delete[](void* ptr) throw() { free(ptr); }
void operator delete[](void* ptr, const std::nothrow_t&) noexcept { free(ptr); }