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
        configs[i].min         = -65535;
        configs[i].max         = 65535;
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
    int     tmpi;
    float   tmpf;

    pref_file.open( config_file_path );

    if( ! pref_file ) {
        fprintf( stderr, "can't read file %s", config_file_path );
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
                            if( tmpi < (int)ceil( cur_item.min  )
                                 || tmpi > (int)floor( cur_item.max ) ) {
                            
                                continue;
                            }
                        }
                        *(int *)cur_ref = tmpi;
                        break;

                    case XPPC_TYPE_FLOAT:
                        tmpf = atof( line.c_str() );
                        /* test the value if rquested */
                        if( cur_item.check_value ) {
                            if( tmpf < cur_item.min  
                                 || tmpf > cur_item.max ) {
                            
                                continue;
                            }
                        }
                        *(float *)cur_ref = tmpf;
                        break;

                    case XPPC_TYPE_BOOLEAN:
                        tmpi = atoi( line.c_str() );
                        /* test the value if rquested */
                        /* i think for bools, if it's not zero or one
                         * we can assume opperator error */
                        if( cur_item.check_value ) {
                            if( tmpi < 0 
                                 || tmpi > 1 ) {
                            
                                continue;
                            }
                        }
                        *(bool *)cur_ref = tmpi;
                        break;
                    
                    /* do the length checking for string types together */
                    case XPPC_TYPE_CSTRING:
                    case XPPC_TYPE_STRING:
                        /* test the value if rquested */
                        if( cur_item.check_value ) {
                            /* min string length is 0 */
                            if( line.length() < ( cur_item.min < 0 ? 0 : (int)ceil( cur_item.min ) )
                                 || line.length() > (int)floor( cur_item.max ) ) {
                            
                                continue;
                            }
                        }
                        if( cur_item.type == XPPC_TYPE_STRING ) {
                            *(string *)cur_ref = line;
                        }
                        else {
                            /* todo: figure out how to handle c-strings */
                        }
                        break;
                }
        }
    }

    pref_file.close();

    return 1;
} 
