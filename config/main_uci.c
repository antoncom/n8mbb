#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include "uci.h"
#include "uloop.h"

// Variables and Function definition

#define UCI_LOOKUP_COMPLETE 1

typedef struct
{
    char designation[32];
    char reduction[32];
    char description[128];
} row;

row motor[6];

int ForeachList();
void print_table();
const char *get_config(char *conf_path);


int main()
{
    ForeachList();
    print_table();

    return 0;
}

// Get config from config file /etc/config/device
// use "uci show" to get all config settings in console
// or use "cat /etc/config/device"

// conf_path example value: device.AxisZ0.reduction
const char *get_config(char *conf_path)
{
    struct  uci_ptr ptr;
    struct  uci_context *c = uci_alloc_context();
    if(!c) return 0;

    char *path = (char*)malloc(sizeof(conf_path) * (strlen(conf_path) + 1));
    static char buffer[128];
    strcpy(path, conf_path);

    // read the config if exists
    if ((uci_lookup_ptr(c, &ptr, path, true) != UCI_OK) ||
        (ptr.o==NULL || ptr.o->v.string==NULL)) {
        uci_free_context(c);
        return 0;
    }

    // get value and free context
    if(ptr.flags & UCI_LOOKUP_COMPLETE)
            strcpy(buffer, ptr.o->v.string);
    uci_free_context(c);

    free(path);
    return buffer;
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
