#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../lib/libuci/uci.h"
#include "../lib/libubox/uloop.h"

#include "../variable/variables.h"
#include "rules.h"

struct uloop_timeout timeout;
void timer_creator(void);
int ForeachList();
void print_table();
struct uci_option *get_uci_value(char package_name[], char section_name[], char option_name[]);


struct Module *create_module(char *title)
{
  Module *module;
  module = (Module *)malloc(sizeof(Module));
  strcpy(module->title, title);
  module->stack_of_rules = NULL;
  module->cache = NULL;
  return module;
}

Module *module_rules;


struct Cache *create_cache_item(Cache *c, Variable *v)
{
  Cache *cache;
  cache = (Cache *)malloc(sizeof(Cache));
  cache->source = v->source;
  cache->value = v->input;
  cache->next = c;
  return cache;
}



void add_to_cache(Variable *v, Module *m)
{
  Cache *cache;
  // If cache is empty
  if(m->cache == NULL)
  {
    cache = NULL;
    cache = create_cache_item(cache, v);
    m->cache = cache;
  }
  else
  {
    cache = m->cache;
    cache = create_cache_item(cache, v);
    m->cache = cache;
  }

}

int total_items_in_cache(Cache *p){
    int t = 0;
    // пока не конец стека
    while (p != NULL) {
      // продвижение по списку
      p = p->next;
      t++;
    }
    return t;
}

int is_in_cache(Variable *var, Module *module)
{
  Cache *cache;
  cache = module->cache;
  while(cache != NULL)
  {
    if(strcmp(var->source.hash, cache->source.hash) == 0)
        return 1;
    cache = cache->next;
  }
  return 0;
}

void clear_cache(Cache *c){
    int t = 0;
    Cache *cache;
    while (c != NULL) {
      cache = c->next;
      free(c);
      c = cache;

      t++;
    }
    printf("Cleared in cache: %d\n", t);

}

bool is_float(const char *s) {
  if (s == NULL) {
    return false;
  }
  char *endptr;
  (float) strtod(s, &endptr);
  if (s == endptr) {
    return false; // no conversion;
  }
  // Look at trailing text
  while (isspace((unsigned char ) *endptr))
    endptr++;
  return *endptr == '\0';
}



struct Rule *create_rule(Rule *r, char *title, Module *module)
{
  Rule *rule;
  rule = (Rule *)malloc(sizeof(Rule));
  rule->next = r;
  rule->stack_of_vars = NULL;
  strcpy(rule->title, title);

  module->stack_of_rules = rule;
  return rule;
}


// Loads initial data to variable 'subtotal' field.
void load_var(Variable *var, Module *module)
{
  char response[256];
  long int result_i;
  struct uci_option *pUciOption;

  if(strcmp(var->source.type, "uci") == 0)
  {
    // TODO get from UCI code here
    // and place to cache
    pUciOption = get_uci_value(var->source.x.package, var->source.x.section, var->source.x.option);
    strcpy(response, pUciOption->v.string);

    strcpy(var->source.x.uci_response, response);
  }
  else if(strcmp(var->source.type, "ubus") == 0)
  {

    // TODO get from UBUS code here
    // and place to cache

    strcpy(response, "-500.10");
    strcpy(var->source.x.ubus_response, response);
  }

  else
  {
    // for constants
    strcpy(response, var->input.x.string);
    var->input.type = STRING;
  }


  if(strlen(response) == 0)
  {
    var->input.x.string = strcpy(var->input.x.string, "");
    var->input.type = STRING;
  }
  else // convert to REAL or INTEGER or STRING
  {
      if(strcmp(response, "0") == 0)
      {
          var->input.x.integer = 0;
          var->input.type = INTEGER;
      }
      else
      {
          char *endp;
          result_i = (long int) strtol(response, &endp, 10);
          if(strlen(endp) == 0)
          {
              var->input.x.integer = result_i;
              var->input.type = INTEGER;
          }
          else
          {
              if(is_float(response))
              {
                var->input.x.real = (float) strtod(response, NULL);
                var->input.type = REAL;
              }
              else
              {
                  var->input.x.string = response;
                  var->input.type = STRING;
              }
          }
      }

    // Check if the same value already loaded
    if(is_in_cache(var, module) == 0)
    {
      add_to_cache(var, module);
    }
  }



  switch(var->input.type) {
    case STRING:
        printf("STRING\n");
        printf("\nVariable [%s] was loaded. Input: %s\n", var->name, var->input.x.string);
        break;
    case REAL:
        printf("REAL\n");
        printf("\nVariable [%s] was loaded. Input: %f\n", var->name, var->input.x.real);
        break;
    case INTEGER:
        printf("INTEGER\n");
        printf("\nVariable [%s] was loaded. Input: %ld\n", var->name, var->input.x.integer);
        break;
    default:
        printf("--- type not defined -- \n");
  }


}

struct Source *create_source(char *model, char *array[]);
struct Variable *create_variable(Variable *v, char *name, Source *src, Rule *rule);


static void timeout_cb(struct uloop_timeout *t)
{
    Rule *rule;
    rule = NULL;
    Source *src1;
    Variable *var0, *var1;
    var0 = NULL;
    //char uci[][2];
    char *uci[] = {"device", "AxisYaw3", "reduction"};

    module_rules = create_module("[module] Nitrogen8M Baseball Main Logic.\n");
    rule = create_rule(rule, "[mode] Fastballs and Split-Fingers.", module_rules);
    src1 = create_source("uci", uci);
    var1 = create_variable(var0, "mDesignation_1", src1, rule);
    load_var(var1, module_rules);


    //load_var(var2, module_rules);
    //load_var(var3, module_rules);

    printf("TOTAL ITEMS IN CACHE: %d\n", total_items_in_cache(module_rules->cache));
    /*
    ForeachList();
    print_table();

*/
    //printf("TOTAL ITEMS IN CACHE: %d\n", total_items_in_cache(module_rules->cache));
    free(var1);

    /*
    free(var3);
    free(rule);
    free(src2);
    free(src1);
    */
    clear_cache(module_rules->cache);


    uloop_timeout_set(t, 1500);
}


void timer_creator(void)
{
 timeout.cb = timeout_cb;
 uloop_timeout_set(&timeout,1500);
}

void print_table()
{
    puts("This is a test of config API, running in C/C++");
    puts("/etc/config/device");
    puts("");
    puts("Motors:");
    puts("|=============|===========|====================================================|");
    puts("| Designation | Reduction |                Description                         |");
    puts("|=============|===========|====================================================|");

    for(int i=0; i<6; i++)
    {
        char dsc[54] = " ";
        strncpy(dsc, motor[i].description, 47);
        strncat(dsc, "...", 3);
        printf("|%12s |%8s   | %47s |\n", motor[i].designation, motor[i].reduction, dsc);
    }
    puts("|-------------|-----------|----------------------------------------------------|");
}

struct uci_option *get_uci_value(char package_name[], char section_name[], char option_name[])
{
    struct uci_context *pUciContext = NULL;		//Define a UCI context
    struct uci_package *pUciPackage = NULL;
    //struct uci_element *pUciElement = NULL;

    pUciContext = uci_alloc_context();			//Apply for a UCI context
    if (UCI_OK != uci_load(pUciContext, package_name, &pUciPackage))		//If opening the UCI file fails, clean up the UCI context
    {
        printf("[%s][%d] Error uci_load!\n", __FILE__, __LINE__);
    	uci_free_context(pUciContext);
    	return NULL;
    }

    struct uci_section *pUciSection = uci_lookup_section(pUciContext, pUciPackage, section_name);
    struct uci_option *pUciOption = uci_lookup_option(pUciContext, pUciSection, option_name);
    return pUciOption;
}

int ForeachList()
{
    struct uci_context *pUciContext = NULL;		//Define a UCI context
    struct uci_package *pUciPackage = NULL;
    struct uci_element *pUciElement = NULL;

    pUciContext = uci_alloc_context();			//Apply for a UCI context
    if (UCI_OK != uci_load(pUciContext, "device", &pUciPackage))		//If opening the UCI file fails, clean up the UCI context
    {
        printf("[%s][%d] Error uci_load!\n", __FILE__, __LINE__);
    	uci_free_context(pUciContext);
    	return -1;
    }

    /* Traverse a package, and loop several times when there are several sections in a package */
    int i=0;
    printf("\nNitrogen8M Baseball project.\n\n");
    uci_foreach_element(&pUciPackage->sections, pUciElement)
    {
        /* Convert an element to section type */
        struct uci_section *pUciSection = uci_to_section(pUciElement);

        /* uci_lookup_option() gets a value of the current section */
        struct uci_option *pUciOptionDesignation = uci_lookup_option(pUciContext, pUciSection, "designated");
        struct uci_option *pUciOptionReduction = uci_lookup_option(pUciContext, pUciSection, "reduction");
        struct uci_option *pUciOptionDescription = uci_lookup_option(pUciContext, pUciSection, "description");

        /* There are two types of Option: UCI_TYPE_STRING and UCI_TYPE_LIST */
        strcpy(motor[i].designation, pUciOptionDesignation->v.string);
        strcpy(motor[i].reduction, pUciOptionReduction->v.string);
        strcpy(motor[i].description, pUciOptionDescription->v.string);
        i++;
    }

    uci_unload(pUciContext, pUciPackage); 	//Release pUciPackage
    uci_free_context(pUciContext);


    return 0;
}
