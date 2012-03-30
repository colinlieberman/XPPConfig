#include "XPPConfig.h"

#include <map>

#include <fstream>
#include <math.h>

using namespace std;

#define STATE_FIND_KEY 0
#define STATE_HAVE_KEY 1

/* helper function for trimming space from the ends of lines */
string rtrim( string );
string rtrim( string str ) {
    size_t endpos = str.find_last_not_of(" \t\n");
    if( string::npos != endpos )
    {
        str = str.substr( 0, endpos+1 );
    }

    return str;
}

XPPCItem *initConfig( int num_configs ) {
    XPPCItem *configs = NULL;
    int i;

    configs = new XPPCItem[ num_configs ];

    /* initialize all to reasonable default state */ 
    for( i=0; i<num_configs; i++ ) {
        configs[i].type        = XPPC_TYPE_FLOAT;
        configs[i].ref         = NULL;
        configs[i].key         = "";
        configs[i].check_value = true;
        configs[i].lower_bound = -65535;
        configs[i].upper_bound = 65535;
        configs[i].min_str_len = 0;
        configs[i].max_str_len = 255;
    }

    return configs;
}

/* read config file and write to configs */
int parseConfigFile( char *config_file_path, XPPCItem *configs, int num_configs ) {

    string line;
    ifstream pref_file;
    int i, state = STATE_FIND_KEY;

    map<string, XPPCItem> config_map;
    map<string, XPPCItem>::iterator it; 

    /* temp holder for use when processing config file */
    XPPCItem cur_item;
    void *cur_ref;

    /* temp values for holding while testing */
    int tmpi;
    float tmpf;

    pref_file.open( config_file_path );

    if( ! pref_file ) {
        fprintf( stderr, "unable to open file %s; using defaults", config_file_path );
        return 0;
    }

    /* after opening the file, map input array for lookup */
    for( i=0; i<num_configs; i++ ) {
        config_map[ configs[i].key ] = configs[i];
    }

    while( !pref_file.eof() )
    {
        getline( pref_file, line );

        line = rtrim( line );
        if( line == "" ) {
            continue;
        }

        switch( state ) {
            case STATE_FIND_KEY:
                it = config_map.find( line );
                if( it != config_map.end() ) {
                    /* cur line matches a config key */
                    cur_item = it->second;
                    state = STATE_HAVE_KEY;
                }
                break;

            case STATE_HAVE_KEY:
                /* always assume the next line is the value we want */
                state = STATE_FIND_KEY;
                
                cur_ref = cur_item.ref;
                switch( cur_item.type ) {
                    case XPPC_TYPE_INT:
                        tmpi = atoi( line.c_str() );
                        /* test its value if requested */
                        if( cur_item.check_value ) {
                            if( tmpi < (int)ceil( cur_item.lower_bound  )
                                 || tmpi > (int)floor( cur_item.upper_bound ) ) {
                            
                                continue;
                            }
                        }
                        *(int *)cur_ref = tmpi;
                        break;

                    case XPPC_TYPE_FLOAT:
                        tmpf = atof( line.c_str() );
                        /* test the value if rquested */
                        if( cur_item.check_value ) {
                            if( tmpf < cur_item.lower_bound  
                                 || tmpf > cur_item.upper_bound ) {
                            
                                continue;
                            }
                        }
                        *(float *)cur_ref = tmpf;
                }
        }
    }

    pref_file.close();

    return 1;
} 
