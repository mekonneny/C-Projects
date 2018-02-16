//Yonathan Mekonnen
//mekonneny
//V00714698
//this file implements the "malloc", "free", "callac" and "realloc" functionalities.

  #include <assert.h>
  #include <string.h>
  #include <sys/types.h>
  #include <unistd.h>
  #include <stdio.h>

#include "assignment2-support.h"

  #define META_SIZE sizeof(struct block_meta)
  void *global_base = NULL;

  struct block_meta *findBestFit(size_t size)
  {
    struct block_meta *findNode = NULL;
    struct block_meta *x = (struct block_meta*)global_base;
    while (x != NULL)
    {
      if ((x->free != 0) && (x->size >= size))
      {
        if (findNode == NULL)
      {
        findNode = x;

      }
      else if (x->size < findNode->size)
      {
        findNode = x;
      }
    }
    x = x->next;
  }
  return findNode;
  }

  void divideBlock(struct block_meta* beginingBlock, size_t dataAmount)
  {
    struct block_meta *secondBlock = (struct block_meta*)((char*)beginingBlock + META_SIZE + dataAmount);
    secondBlock->size = beginingBlock->size -dataAmount - META_SIZE;
    secondBlock->next = beginingBlock-> next;
    secondBlock->free = 1;

    beginingBlock->size = dataAmount;
    beginingBlock->free = 0;
    beginingBlock->next = secondBlock;
  }

  size_t align(size_t size) {
  while ( size % 8 != 0)
  {
    size++;
  }
}

  struct block_meta *find_free_block(struct block_meta **last, size_t size) {
    struct block_meta *current = global_base;
    while (current && !(current->free&& current->size >= size)) {
      current = current->next;
    }
    return current;
  }

  struct block_meta *request_space(struct block_meta* last, size_t size) {
    struct block_meta *block;
    block = sbrk(0);
    void *request = sbrk(size + META_SIZE);
    assert((void*)block == request);
    if (request ==(void*) -1) {
      return NULL;
    }
    if(last) {
      last->next->prev = block;
      last = block->next;
    }
    block->size = size;
    block->next = NULL;
    block->prev = last;
    block->free = 0;
    block->magic = 0x12345678;
    return block;
  }

  void *malloc(size_t size){
    struct block_meta *block;
    //TODO

    if (size <= 0){
      return NULL;
    }
    if (!global_base) {
    block = request_space(NULL, size);
  if (!block){
    return NULL;
  }
  global_base = block;
  } else {
    struct block_meta *last = global_base;
    block = find_free_block(&last, size);
    if (!block) {
      block = request_space(last, size);
      if (!block) {
        return NULL;
      }
    } else {
      //TODO:
      block -> free = 0;
      block -> magic = 0x77777777;
    }
  }
  return(block +1);
  }

  struct block_meta *get_block_ptr (void *ptr) {
    return (struct block_meta*)ptr - 1;
  }

  void memory_leaks() {
    int memLeak = 0;
    struct block_meta* block = global_base;
    while (block) {
      memLeak = memLeak + META_SIZE;
      if (block-> free) {
        memLeak = memLeak + block->size;
      }
      block = block->next;
    }
    printf("Full Leak: %d bytes \n", memLeak);
  }
  void showHeap(struct block_meta* block)
  {

  	printf("Block address: %u\n", block);
  	printf("Size of block: %d\n", block->size);


}




  void free (void *ptr) {
    if (!ptr) {
      return;
    }
    //TODO
    struct block_meta* block_ptr = get_block_ptr(ptr);
    assert(block_ptr->free == 0);
    assert(block_ptr ->magic == 0x77777777 || block_ptr->magic == 0x12345678);
    block_ptr->free =1;
    block_ptr-> magic = 0x55555555;
  }

  void *realloc(void *ptr, size_t size) {
    if (!ptr) {

      return malloc(size);
    }
    struct block_meta* block_ptr = get_block_ptr(ptr);
    if (block_ptr->size >= size)
    {

      return ptr;
    }

    void *new_ptr;
    new_ptr = malloc(size);
    if (!new_ptr) {
      return NULL; // TODO:
    }
    memcpy(new_ptr, ptr, block_ptr->size);
    free(ptr);
    return new_ptr;
    }

  void *calloc(size_t nelem, size_t elsize) {
    size_t size = nelem * elsize; // TODO:
    void *ptr = malloc(size);
    memset (ptr, 0, size);
    return ptr;

  }
