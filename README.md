XPPConfig: A configuration engine for X-Plane Plugins
===========

by Colin Lieberman

The goal of this project is to provide a simple, convenient, generic way to parse a config file for an X-Plane plugin, and providing the config file format in a way that's robust and reasonably user-proof.

To build:
--------

1. Get the code: `$ git clone git://github.com/colinlieberman/XPPConfig.git XPPConfig`
1. Eiter make a Makefile via QTCreator or symlink one of the provided ones
1. build with `make`
1. `make test`
1. That's all she wrote.

To use in your project
--------------------

1. build the library - see above
1. link the library when building your project (see the included Hello World project)
1. use the API

XPPConfig API
------------

Please let me know of any suggestions you have to make the API simpler.

See the tests in test.cpp and the included HelloWorld sample project for examples.

XPPConfig.h exposes the following functions:

* `XPPCItem *XPPCInit( int num_configs )`
* `int XPPCParseConfigFile( char *config_file_path, XPPCItem[] configs, int num_configs )`
* `const char *XPPCLastError( void )`
* `bool XPPCError( void )`

### XPPCInit ###
This function creates the array of config items (see below) and IMPORTANTLY initialized them with default data. If you don't call this, it's entirely possible that Bad Things will happen.

It takes one argument, and integer, for the number of configurations you'll use.

It returns an array of XPPConfigItem structs that you then set up for parsing your config file.

#### XPPConfigItem ####

An XPPConfigItem has the following properties:

* int type - one of `XPPC_TYPE_INT`, `XPPC_TYPE_FLOAT`, `XPPC_TYPE_STRING`, etc (see XPPConfig.h for others)
* string key - the name of the key in the config file. Keys must be on their own line, with values alone on the immediately follwoing line.
* void *ref - this is a *pointer* to the variable you want to hold the value for the given key 
* bool  check_value - whether or not to use min and max (below) to check values before setting *ref. If check_value is true and the value is outside the bounds given, *ref is not set. 
* float min - the lowest allowed value; the shortest allowed length for strings.
* float max - the greatest allowed value; the longest allowed length for strings.

### XPPCParseConfigFile ###

Read the config file and set the values pointed to by the XPPConfigItem array. Arguments are the path to the config, the array returned by XPPCInit(), and the number of items in the array.

*Before* calling this function, you'll want to set up the returned XXPConfigItems so we can know how to parse the file. See test.cpp and the included HelloWorld project for examples.

### XPPCLastError ###

Returns a cstring describing the last error encountered by the config parser. It returns a cstring so you can pass it directly to XPLMDebugString. The string it returns is prefixed with "XPPConfig: " so it's easy to grep Log.txt for errors.

### XPPCError ###

Returns a boolean indicating whether or not the configuration parser is aware of any errors. 
