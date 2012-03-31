#include "XPPConfig.h"

#define ASSERT_EQUAL \
    tests_total++; \
 \
    if( a == b ) { \
        tests_passed++; \
        printf( "ok" ); \
    } \
    else { \
        tests_failed++; \
        printf( "failed" ); \
    } \
    printf( "\n" ); \
    if( XPPCError() ) { \
        printf( "error was %s", XPPCLastError() );\
    }

void assertInt( int, int );
void assertFloat( float, float );
void assertBool( bool, bool );
void assertString( string, string );
void assertCString( char *, char * );

void testInts( void );
void testFloats( void );
void testBools( void );
void testStrings( void );
void testCStrings( void );

int tests_total  = 0;
int tests_passed = 0;
int tests_failed = 0;

void assertInt( int a, int b ) {
    printf( "test %d = %d ", a, b );
    ASSERT_EQUAL
}

void assertFloat( float a, float b ) {
    printf( "test %f = %f ", a, b );
    ASSERT_EQUAL
}

void assertBool( bool a, bool b ) {
    printf( "test %d = %d ", a, b );
    ASSERT_EQUAL
}

void assertString( string a, string b ) {
    printf( "test %s = %s ", a.c_str(), b.c_str() );
    ASSERT_EQUAL
}

void assertCString( char *a, char *b ) {
    printf( "test %s = %s ", a, b );
    /* this needs to be handled specially */
    tests_total++;
    if( strcmp( a, b ) == 0 ) {
        printf( "ok" );
        tests_passed++;
    }
    else {
        printf( "failed" );
        tests_failed++;
    }
    printf( "\n" ); 
}

void testInts( void ) {
    int num_configs = 4; 
    XPPCItem *configs = initConfig( num_configs );

    /* set defaults */
    int one     = 1, 
        two     = 2,
        three   = 3, 
        four    = 4;

/***
pos int
10

pos int2
583

neg int
-3

neg int2
-7382
*/
    /* we'll tests several times with a few configurations:
     *  1. all on default as possible, should all have the
     *  results from the file
     *
     *  2. all configed to be too high - should all be defaults
     *
     *  3. all configed to be too low - should all be defaults
     *
     *  4. test all configs at lower limit equal or above by one
     *
     *  5. test all configs at upper limit equal or below by one
     */

    /* 1 all defaults, all results should be per the config file */
    configs[0].type = XPPC_TYPE_INT;
    configs[0].key  = "pos int2";
    configs[0].ref  = &two;

    configs[1].type = XPPC_TYPE_INT;
    configs[1].key  = "pos int";
    configs[1].ref  = &one;

    configs[2].type = XPPC_TYPE_INT;
    configs[2].key  = "neg int";
    configs[2].ref  = &three;

    configs[3].type = XPPC_TYPE_INT;
    configs[3].key  = "neg int2";
    configs[3].ref  = &four;

    if( !parseConfigFile( "./test_conf.txt", configs, num_configs ) ) {
        printf( "%s\n", XPPCLastError() );
        return;
    }

    printf( "\ntesting ints with default settings...\n" );
    assertInt( one, 10 );
    assertInt( two, 583 );
    assertInt( three, -3 );
    assertInt( four, -7382 );
    
    /* reset and do test 3:
     * 3. all configs too low, expected default values */
    configs = initConfig( num_configs );

    /* set defaults */
    one     = 1;
    two     = 2;
    three   = 3; 
    four    = 4;
    
    configs[0].type = XPPC_TYPE_INT;
    configs[0].key  = "pos int2";
    configs[0].ref  = &two;
    configs[0].min = 32757;

    configs[1].type = XPPC_TYPE_INT;
    configs[1].key  = "pos int";
    configs[1].ref  = &one;
    configs[1].min = 32757;

    configs[2].type = XPPC_TYPE_INT;
    configs[2].key  = "neg int";
    configs[2].ref  = &three;
    configs[2].min = 32757;

    configs[3].type = XPPC_TYPE_INT;
    configs[3].key  = "neg int2";
    configs[3].ref  = &four;
    configs[3].min = 32757;

    parseConfigFile( "./test_conf.txt", configs, num_configs );

    printf( "\ntesting ints with lower bound set to 32767...\n" );
    assertInt( one, 1 );
    assertInt( two, 2 );
    assertInt( three, 3 );
    assertInt( four, 4 );
    
    /* reset and do test 4:
     * 4. testing lower bound equal or above by one */
    configs = initConfig( num_configs );

    /* set defaults */
    one     = 1;
    two     = 2;
    three   = 3; 
    four    = 4;
/***
pos int
10

pos int2
583

neg int
-3

neg int2
-7382
*/
    
    configs[0].type = XPPC_TYPE_INT;
    configs[0].key  = "pos int2";
    configs[0].ref  = &two;
    configs[0].min = 583;

    configs[1].type = XPPC_TYPE_INT;
    configs[1].key  = "pos int";
    configs[1].ref  = &one;
    configs[1].min = 11;

    configs[2].type = XPPC_TYPE_INT;
    configs[2].key  = "neg int";
    configs[2].ref  = &three;
    configs[2].min = -3;

    configs[3].type = XPPC_TYPE_INT;
    configs[3].key  = "neg int2";
    configs[3].ref  = &four;
    configs[3].min = -7381;

    parseConfigFile( "./test_conf.txt", configs, num_configs );

    printf( "\ntesting ints with lower bound equal or one greater...\n" );
    assertInt( one, 1 );
    assertInt( two, 583 );
    assertInt( three, -3 );
    assertInt( four, 4 );
    
    /* reset and do test 5:
     * 5. upper is equal or below by one */
    configs = initConfig( num_configs );

    /* set defaults */
    one     = 1;
    two     = 2;
    three   = 3; 
    four    = 4;
/***
pos int
10

pos int2
583

neg int
-3

neg int2
-7382
*/
    
    configs[0].type = XPPC_TYPE_INT;
    configs[0].key  = "pos int2";
    configs[0].ref  = &two;
    configs[0].max = 582;

    configs[1].type = XPPC_TYPE_INT;
    configs[1].key  = "pos int";
    configs[1].ref  = &one;
    configs[1].max = 10;

    configs[2].type = XPPC_TYPE_INT;
    configs[2].key  = "neg int";
    configs[2].ref  = &three;
    configs[2].max = -4;

    configs[3].type = XPPC_TYPE_INT;
    configs[3].key  = "neg int2";
    configs[3].ref  = &four;
    configs[3].max = -7382;

    parseConfigFile( "./test_conf.txt", configs, num_configs );

    printf( "\ntesting ints with upper bound equal or one under...\n" );
    assertInt( one, 10 );
    assertInt( two, 2 );
    assertInt( three, 3 );
    assertInt( four, -7382 );
}

void testFloats( void ) {
    int num_configs = 2; 
    XPPCItem *configs = initConfig( num_configs );
/*
pos float
4738.281

neg float
-29.275
*/

    /* set defaults */
    float one = 1,
          two = 2;

    configs[0].key = "pos float";
    configs[0].ref = &one;

    configs[1].key = "neg float";
    configs[1].ref = &two;
    
    parseConfigFile( "./test_conf.txt", configs, num_configs );

    printf( "\ntesting floats with defaults...\n" );
    assertFloat( one, 4738.281 );
    assertFloat( two, -29.275 );
    
    /* reset and do more tests */
    configs = initConfig( num_configs );

    one = 1,
    two = 2;

    configs[0].key = "pos float";
    configs[0].ref = &one;
    configs[0].max = 4738.280;

    configs[1].key = "neg float";
    configs[1].ref = &two;
    configs[1].max = -29.276;
/*
pos float
4738.281

neg float
-29.275
*/
    
    parseConfigFile( "./test_conf.txt", configs, num_configs );

    printf( "\ntesting floats with upper limit .001 under\n" );
    assertFloat( one, 1 );
    assertFloat( two, 2 );
    
    /* reset and do more tests */
    configs = initConfig( num_configs );

    one = 1,
    two = 2;

    configs[0].key = "pos float";
    configs[0].ref = &one;
    configs[0].min = 4738.282;

    configs[1].key = "neg float";
    configs[1].ref = &two;
    configs[1].min = -29.274;
    
    parseConfigFile( "./test_conf.txt", configs, num_configs );

    printf( "\ntesting floats with lower limit .001 over\n" );
    assertFloat( one, 1 );
    assertFloat( two, 2 );
}

void testBools( void ) {
    int num_configs = 6; 
    XPPCItem *configs = initConfig( num_configs );
/*
here's a bool
0

here's another bool
1
*/
    /* for bools we'll test that real bools are read correctly and that
     * ints read as bools fail as expected to the defaults */
    bool one   = true,
         two   = false,
         three = true,
         four  = false,
         five  = true,
         six   = false;
    
    configs[0].type = XPPC_TYPE_BOOLEAN;
    configs[0].key  = "here's a bool";
    configs[0].ref  = &one;

    configs[1].type = XPPC_TYPE_BOOLEAN;
    configs[1].key  = "here's another bool";
    configs[1].ref  = &two;

    configs[2].type = XPPC_TYPE_BOOLEAN;
    configs[2].key  = "neg int";
    configs[2].ref  = &three;

    configs[3].type = XPPC_TYPE_BOOLEAN;
    configs[3].key  = "pos int";
    configs[3].ref  = &four;

    configs[4].type = XPPC_TYPE_BOOLEAN;
    configs[4].key  = "pos int2";
    configs[4].ref  = &five;

    configs[5].type = XPPC_TYPE_BOOLEAN;
    configs[5].key  = "neg int2";
    configs[5].ref  = &six;
    
    parseConfigFile( "./test_conf.txt", configs, num_configs );

    printf( "\ntesting bools....\n" );
    assertBool( one, false );
    assertBool( two, true );
    assertBool( three, true );
    assertBool( four, false );
    assertBool( five, true );
    assertBool( six, false );
}

void testStrings( void ) {
    int num_configs = 5; 
    XPPCItem *configs = initConfig( num_configs );

    /* set defaults */
    string one   = "foo",
           two   = "bar",
           three = "baz",
           four  = "fizz",
           five  = "buzz";
/*
string
Hello World!

string2
!dlroW olleH

string 3
cats

string 4
dogs

string 5
potatoes
*/

    /* defaults */
    configs[0].type = XPPC_TYPE_STRING;
    configs[0].key  = "string";
    configs[0].ref  = &one;

    /* min = strlen */
    configs[1].type = XPPC_TYPE_STRING;
    configs[1].key  = "string2";
    configs[1].ref  = &two;
    configs[1].min  = 9;

    /* min > strlen */
    configs[2].type = XPPC_TYPE_STRING;
    configs[2].key  = "string 3";
    configs[2].ref  = &three;
    configs[2].min  = 5;

    /* max = strlen */
    configs[3].type = XPPC_TYPE_STRING;
    configs[3].key  = "string 4";
    configs[3].ref  = &four;
    configs[3].max  = 4;

    /* max < strlen */
    configs[4].type = XPPC_TYPE_STRING;
    configs[4].key  = "string 5";
    configs[4].ref  = &five;
    configs[4].max  = 7;
    
    parseConfigFile( "./test_conf.txt", configs, num_configs );

    printf( "\ntesting strings....\n" );
    assertString( one, "Hello World!" );
    assertString( two, "!dlroW olleH" );
    assertString( three, "baz" );
    assertString( four, "dogs" );
    assertString( five, "buzz" );
}

void testCStrings( void ) {
    int num_configs = 5; 
    XPPCItem *configs = initConfig( num_configs );

    /* set defaults */
    char *one   = "foo",
         *two   = "bar",
         *three = "baz",
         *four  = "fizz",
         *five  = "buzz";
/*
string
Hello World!

string2
!dlroW olleH

string 3
cats

string 4
dogs

string 5
potatoes
*/

    /* defaults */
    configs[0].type = XPPC_TYPE_CSTRING;
    configs[0].key  = "string";
    configs[0].ref  = &one;

    /* min = strlen */
    configs[1].type = XPPC_TYPE_CSTRING;
    configs[1].key  = "string2";
    configs[1].ref  = &two;
    configs[1].min  = 9;

    /* min > strlen */
    configs[2].type = XPPC_TYPE_CSTRING;
    configs[2].key  = "string 3";
    configs[2].ref  = &three;
    configs[2].min  = 5;

    /* max = strlen */
    configs[3].type = XPPC_TYPE_CSTRING;
    configs[3].key  = "string 4";
    configs[3].ref  = &four;
    configs[3].max  = 4;

    /* max < strlen */
    configs[4].type = XPPC_TYPE_CSTRING;
    configs[4].key  = "string 5";
    configs[4].ref  = &five;
    configs[4].max  = 7;
    
    parseConfigFile( "./test_conf.txt", configs, num_configs );

    printf( "\ntesting cstrings....\n" );
    assertCString( one, "Hello World!" );
    assertCString( two, "!dlroW olleH" );
    assertCString( three, "baz" );
    assertCString( four, "dogs" );
    assertCString( five, "buzz" );
}

int main( void ) {
    string result;

    testInts();
    testFloats();
    testBools();
    testStrings();
    testCStrings(); 

    result = tests_failed 
                ? "FAILURE"
                : "SUCCESS";

    printf( "\ntotal: %d\npassed: %d\nfailed: %d\n", tests_total, tests_passed, tests_failed );
    printf( "\nTest results: %s\n\n", result.c_str() );

    /* exit status 0 if nothing failed */
    return tests_failed;
}
