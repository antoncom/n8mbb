#ifndef RULES_H
#define RULES_H 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../lib/libubox/uloop.h"

#include "../variable/variables.h"

#define STRUCTURE_LOADED 1

typedef struct Cache {
  struct Source source;
  struct Value value;
  struct Cache *next;
} Cache;

typedef struct Rule
{
  char title[128];
  struct Rule *next;
  struct Variable *stack_of_vars;
} Rule;

typedef struct Module
{
  char title[128];
  struct Rule *stack_of_rules;
  struct Cache *cache;

} Module;


#define UCI_LOOKUP_COMPLETE 1

extern Module *module_rules;
extern Cache global_cache;



/*
struct uloop_timeout timeout;
extern void timer_creator(void);
extern int ForeachList();
extern  void print_table();
*/

#endif /* RULES_H */
