#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../lib/libubox/uloop.h"

#include "../rule/rules.h"
#include "variables.h"


struct Source *create_source(char *model, char *array[])
{
  Source *src;
  src = (Source *)malloc(sizeof(Source));

  if(strcmp(model, "uci") == 0)
  {
    strcpy(src->type, "uci");

    strcpy(src->x.package, array[0]);
    strcpy(src->x.section, array[1]);
    strcpy(src->x.option, array[2]);
    strcpy(src->x.uci_response, "");

    // simple hash analogue
    // to find in cache quickly

    strcpy(src->hash, src->x.package);
    strcat(src->hash, src->x.section);
    strcat(src->hash, src->x.option);

  }


  else if(strcmp(model, "ubus") == 0)
  {
    strcpy(src->type, "ubus");
    strcpy(src->x.name, array[0]);
    strcpy(src->x.method, array[1]);
    strcpy(src->x.param, array[2]);
    strcpy(src->x.ubus_response, "");

    strcpy(src->hash, src->x.name);
    strcat(src->hash, src->x.method);
    strcat(src->hash, src->x.param);
  }

  else if(strcmp(model, "constant") == 0)
  {
      ;
  }
  return src;
}

struct Variable *create_variable(Variable *v, char *name, Source *src, Rule *rule)
{
  Variable *var;
  var = (Variable *)malloc(sizeof(Variable));
  var->source = *src;
  var->next = v;
  strcpy(var->name, name);
  rule->stack_of_vars = var;
  return var;
}
