/*
cache.h structure of cache that used to simulate caches
*/

#ifndef CACHE_H
#define CACHE_H
typedef struct {
  int valid;
  int tag;
  int lru;
} cache_line;


#endif /* CACHE_H */

