#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include "cache.h"
#include "cachelab.h"

typedef struct {
  int hit;
  int miss;
  int evict;
} tTuple;

tTuple cache_simu(char op, unsigned long int target_addr, int set, int way, int block, cache_line** cache); 
tTuple process_file(int s, int e, int b, int v_flag, char* filename, cache_line** cache); 

int main(int argc, char** argv)
{
    int vflag = 0, hflag = 0, index, c;
    int s_val = 0, e_val = 0, b_val = 0;
    char* filename = 0;
    extern char* optarg;
    // printf("Mark 1\n");

    opterr = 0;
    while ((c = getopt(argc, argv, "vhs:E:b:t:")) != -1) {
      switch(c)
      {
        case 'v':
          vflag = 1;
          break;
        case 'h':
          hflag = 1;
          break;
        case 's':
          // printf("optarg = %s\n", optarg);
          s_val = atoi(optarg);
          break;
        case 'E':
          e_val = atoi(optarg);
          break;
        case 'b':
          b_val = atoi(optarg);
          break;
        case 't':
          filename = optarg;
          break;
        case '?':
          if (optopt == 's' || optopt == 'E' || optopt == 'b' || optopt == 't') {
            printf("Option -%c requires an argument.\n", optopt);
          }
          return 1;
        default:
          abort();
      }
    }

    int set = pow(2, s_val);
    int way = e_val;
    cache_line** cache = (cache_line**) calloc(set, sizeof(cache_line*));
    for (int i = 0; i < set; i++) {
      cache[i] = (cache_line*) calloc(way, sizeof(cache_line));
    }
    // printf("Mark 1\n");
    tTuple result = process_file(s_val, e_val, b_val, vflag, filename, cache);

    // printSummary(0, 0, 0);

    printSummary(result.hit, result.miss, result.evict);
    for (int i = 0; i < set; i++) {
      free(cache[i]);
    }
    free(cache);
    return 0;
}

tTuple process_file(int s, int e, int b, int v_flag, char* filename, cache_line** cache) {
  FILE* fpIn;
  fpIn = fopen(filename, "r");
  
  /*
  // Initialize cache
  int set = pow(2, s), block = pow(2, b);
  cache_line** cache = (cache_line**) calloc(set, sizeof(cache_line*));
  for (int i = 0; i < set; i++) {
    cache[i] = (cache_line*) calloc(e, sizeof(cache_line));
  }
  */

  char read_line[50];
  char op;
  unsigned long int addr;
  int byte;
  tTuple ans;
  ans.hit = 0;
  ans.miss = 0; 
  ans.evict = 0;
  while (fgets(read_line, 50, fpIn) != NULL) {
    // printf("%s, miss or hit\n", read_line);
    sscanf(read_line, " %c %lx, %d", &op, &addr, &byte);
    // printf("read opcode = %c, address = %ulx, byte = %d, ", op, addr, byte);
    if (op == 'I') {
      continue;
    }
    tTuple res = cache_simu(op, addr, s, e, b, cache);
    ans.hit += res.hit;
    ans.miss += res.miss;
    ans.evict += res.evict;
    if (v_flag) {
      printf("%c %lx,%d ", op, addr, byte);
      if (res.miss == 1)
        printf("miss ");
      if (res.evict == 1)
        printf("eviction ");
      if (res.hit == 1)
        printf("hit ");
      if (res.hit == 2)
        printf("hit hit ");
      printf("\n");
    }
  }
  fclose(fpIn);
  // printf("hits:%d misses:%d evictions:%d\n", ans.hit, ans.miss, ans.evict);
  return ans;
}


tTuple cache_simu(char op, unsigned long int target_addr, int s, int way, int b, cache_line** cache) {
  int idx = (target_addr >> b) & ~(~0 << s);
  int target_tag = target_addr >> (b + s);

  // printf("idx = %x, target_tag = %x, ", idx, target_tag);
  int set = pow(2, s);
  tTuple ans = {0};
  int max_lru = 0;
  int max_lru_idx;
  int empty_idx = -1;
  int found_lru = -1;
  int found = 0;

  // If opcode = 'M', set initial hit to 1 because it will be at least 1
  if (op == 'M') {
    ans.hit = 1;
  }
  
  // First roll, trying to find the tag, meanwhile, keep track of the max_lru_idx
  for (int i = 0; i < way; i++) {
    if (found == 0) {
      if (cache[idx][i].valid == 1 && cache[idx][i].tag == target_tag) {
        ans.hit += 1;
        found = 1;
        found_lru = cache[idx][i].lru;
        cache[idx][i].lru = -1;
        break;
      }
      else if (cache[idx][i].valid == 1 && cache[idx][i].tag != target_tag) {
        if (cache[idx][i].lru >= max_lru) {
          max_lru = cache[idx][i].lru;
          max_lru_idx = i;
        }
      }
      else if (cache[idx][i].valid == 0) {
         if (empty_idx == -1) {
           empty_idx = i;
         }
       }
    }
  }

  // If the tag is found in the set, roll the set again to update each lru
  if (found == 1) {
    for (int i = 0; i < way; i++) {
      if (cache[idx][i].valid == 1 && cache[idx][i].lru < found_lru) {
        cache[idx][i].lru++;
      }
    }
  }

  // If the tag is not found in the set and there is invalid line in the set, put new data in invalid line, validate that line, and increment lru of all valid lines by 1
  else if (found == 0 && empty_idx != -1) {
    // printf("Mark 1\n");
    ans.miss += 1;
    for (int i = 0; i < way; i++) {
      cache[idx][i].lru++;
    }
    cache[idx][empty_idx].lru = 0;
    cache[idx][empty_idx].tag = target_tag;
    cache[idx][empty_idx].valid = 1;
    // printf("\n cache set %x, line %x, new tag = %x, new valid = %d, new lru = %d\n", idx, empty_idx, cache[idx][empty_idx].tag, cache[idx][empty_idx].valid, cache[idx][empty_idx].lru);
  } 

  // If the tag is not found, there is no invalid line in the set, evict max_lru_idx line and put new data in it, increment lru of all other lines
  else if (found == 0 && empty_idx == -1) {
    ans.miss += 1;
    ans.evict += 1;
    empty_idx = max_lru_idx;
    for (int i = 0; i < way; i++) {
      if (cache[idx][i].valid == 1) {
        cache[idx][i].lru++;
      }
    }
    cache[idx][empty_idx].lru = 0;
    cache[idx][empty_idx].tag = target_tag;
  }
  return ans;
}


















