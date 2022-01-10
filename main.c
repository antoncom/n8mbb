#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/libuci/uci.h"
#include "lib/libubox/uloop.h"

#include "variable/variables.h"
#include "rule/rules.h"

// Variables and Function definition

#define UCI_LOOKUP_COMPLETE 1
/*
extern void add_to_cache(Variable *v, Module *m);
extern int total_items_in_cache(Cache *p);
extern int is_in_cache(Variable *var, Module *module);
extern void load_var(Variable *var, Module *module);
extern struct Variable *create_variable(Variable *v, char *name, Source *src, Rule *rule);

extern struct uloop_timeout timeout;
extern void timer_creator(void);
*/
int ForeachList();
void print_table();
void timer_creator(void);



int main()
{
    //ForeachList();
    //print_table();

    uloop_init();
    timer_creator();
    uloop_run();

    return 0;
}
