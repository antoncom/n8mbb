#ifndef VARIABLES_H
#define VARIABLES_H 1
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../lib/libubox/uloop.h"

//#include "../rule/rules.h"
//#include "uci.h"

#define STRUCTURE_LOADED 1

typedef struct Source
{
  char type[10];
  char hash[64];
  union {
    struct {
      char name[64];
      char method[64];
      char param[64];
      char ubus_response[256];
    };
    struct {
      char package[64];
      char section[64];
      char option[64];
      char uci_response[64];
    };
  } x;
} Source;

typedef enum { INTEGER, STRING, REAL } Type;

// Good explanation of Union:
// https://stackoverflow.com/questions/724617/examples-of-union-in-c
typedef struct Value
{
  Type type;
  union
  {
    char *string;
    long int integer;
    float real;
  } x;
} Value;

typedef struct Variable
{
  char name[64];
  struct Source source;
  struct Value input;
  struct Value subtotal;
  struct Value output;
  struct Variable *next;
} Variable;


typedef struct
{
    char designation[32];
    char reduction[32];
    char description[128];
} row_motor_table;

row_motor_table motor[6];

//struct Source *create_source(char *model, char *array[]);
//struct Variable *create_variable(Variable *v, char *name, Source *src, Rule *rule);

#endif /* VARIABLES_H */
