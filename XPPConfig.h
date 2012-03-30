#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <string>
using namespace std;

#define XPPC_TYPE_BOOLEAN   1
#define XPPC_TYPE_INT       2
#define XPPC_TYPE_FLOAT     3
#define XPPC_TYPE_CSTRING   4
#define XPPC_TYPE_STRINGOBJ 5

typedef struct XPPCItem {
    int type;
    string key;
    void *ref;
    
    bool  check_value;
    float lower_bound;
    float upper_bound;
    int   min_str_len;
    int   max_str_len;
};

XPPCItem *initConfig( int );
int parseConfigFile( char *, XPPCItem[], int );

