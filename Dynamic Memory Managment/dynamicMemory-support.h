//Yonathan Mekonnen
//mekonneny
//V00714698
//this file implements the "malloc", "free", "callac" and "realloc" functionalities.

#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

struct block_meta {
  size_t size;
  struct block_meta *next;
  struct block_meta *prev;

  int free;
  int magic; // debugging only TODO: remove this in non-debug mode
};

struct block_meta *findBestFit(size_t size);

void divideBlock(struct block_meta* beginingBlock, size_t dataAmount);

size_t align(size_t size);

struct block_meta *find_free_block(struct block_meta **last, size_t size);

struct block_meta *request_space(struct block_meta* last, size_t size);

void *malloc(size_t size);

void memory_leaks();

void free (void *ptr);

void *realloc(void *ptr, size_t size);

void *calloc(size_t nelem, size_t elsize);
