#include "XPPConfig.h"

#define NUM_CONFIGS 4

int main( void ) {
    XPPCItem *configs = initConfig( NUM_CONFIGS );

    /* set defaults */
    int one     = 1, 
        two     = 2,
        three   = 3, 
        four    = 4;

    float fone  = 0,
          ftwo  = 0;

    configs[0].type = XPPC_TYPE_INT;
    configs[0].key  = "neg int";
    configs[0].ref  = &two;
    configs[0].check_value = true;
    configs[0].lower_bound = 0;
    configs[0].upper_bound = 100;
    
    configs[1].type = XPPC_TYPE_INT;
    configs[1].key  = "pos int";
    configs[1].ref  = &one;

    configs[2].key = "pos float";
    configs[2].ref = &fone;

    configs[3].key = "neg float";
    configs[3].ref = &ftwo;

    parseConfigFile( "./test_conf.txt", configs, NUM_CONFIGS );

    printf( "one: %d\ntwo: %d\nthree: %d\nfour: %d\n", one, two, three, four );
    printf( "fone: %f\nftwo: %f\n", fone, ftwo );

    return 0;
}
