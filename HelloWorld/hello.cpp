#include "XPPConfig.h"

#include <stdio.h>
#include <string.h>
#include "XPLMDisplay.h"
#include "XPLMGraphics.h"
#include "XPLMUtilities.h"

#include <string>
using namespace std;


/* I need to know how many configs I'm working with */
#define NUM_CONFIGS 2

/* the two configs I want to read from file -
 * you could set the defaults here or elsewhere */
int config_hello_times = 1;
string config_hello_text = "not set by user";

/*
 * Global Variables.  We will store our single window globally.  We also record
 * whether the mouse is down from our mouse handler.  The drawing handler looks
 * at this information and draws the appropriate display.
 *
 */

XPLMWindowID	gWindow = NULL;

void MyDrawWindowCallback(
                                   XPLMWindowID         inWindowID,
                                   void *               inRefcon);

void MyHandleKeyCallback(
                                   XPLMWindowID         inWindowID,
                                   char                 inKey,
                                   XPLMKeyFlags         inFlags,
                                   char                 inVirtualKey,
                                   void *               inRefcon,
                                   int                  losingFocus);

int MyHandleMouseClickCallback(
                                   XPLMWindowID         inWindowID,
                                   int                  x,
                                   int                  y,
                                   XPLMMouseStatus      inMouse,
                                   void *               inRefcon);


/*
 * XPluginStart
 *
 * Our start routine registers our window and does any other initialization we
 * must do.
 *
 */
PLUGIN_API int XPluginStart(
						char *		outName,
						char *		outSig,
						char *		outDesc)
{
    /* initiize the array of configuration items */
    XPPCItem *configs = initConfig( NUM_CONFIGS );

    XPLMDebugString( "hello world\n" );

    /* set up what I want from the file -
     * at a minimum, this is type, key,
     * and a reference to variable to hold the result */
    
    configs[0].type = XPPC_TYPE_STRING;
    configs[0].key  = "Hello Text";
    configs[0].ref  = &config_hello_text;

    configs[1].type = XPPC_TYPE_INT;
    configs[1].key  = "Hello Count";
    configs[1].ref  = &config_hello_times;
    
    /* set some reasonable boundaries for this one */
    
    configs[1].min  = 1;
    configs[1].max  = 4;
    
    /* now parse the file */
    parseConfigFile( "Resources/plugins/HelloWorld/hello.conf", configs, NUM_CONFIGS );

    /* First we must fill in the passed in buffers to describe our
	 * plugin to the plugin-system. */

	strcpy(outName, "HelloWorld");
	strcpy(outSig, "xplanesdk.examples.helloworld");
	strcpy(outDesc, "A plugin that makes a window.");

	/* Now we create a window.  We pass in a rectangle in left, top,
	 * right, bottom screen coordinates.  We pass in three callbacks. */

	gWindow = XPLMCreateWindow(
					50, 600, 300, 200,			/* Area of the window. */
					1,							/* Start visible. */
					MyDrawWindowCallback,		/* Callbacks */
					MyHandleKeyCallback,
					MyHandleMouseClickCallback,
					NULL);						/* Refcon - not used. */

	/* We must return 1 to indicate successful initialization, otherwise we
	 * will not be called back again. */

	return 1;
}

/*
 * XPluginStop
 *
 * Our cleanup routine deallocates our window.
 *
 */
PLUGIN_API void	XPluginStop(void)
{
	XPLMDestroyWindow(gWindow);
}

/*
 * XPluginDisable
 *
 * We do not need to do anything when we are disabled, but we must provide the handler.
 *
 */
PLUGIN_API void XPluginDisable(void)
{
}

/*
 * XPluginEnable.
 *
 * We don't do any enable-specific initialization, but we must return 1 to indicate
 * that we may be enabled at this time.
 *
 */
PLUGIN_API int XPluginEnable(void)
{
	return 1;
}

/*
 * XPluginReceiveMessage
 *
 * We don't have to do anything in our receive message handler, but we must provide one.
 *
 */
PLUGIN_API void XPluginReceiveMessage(
					XPLMPluginID	inFromWho,
					long			inMessage,
					void *			inParam)
{
}

/*
 * MyDrawingWindowCallback
 *
 * This callback does the work of drawing our window once per sim cycle each time
 * it is needed.  It dynamically changes the text depending on the saved mouse
 * status.  Note that we don't have to tell X-Plane to redraw us when our text
 * changes; we are redrawn by the sim continuously.
 *
 */
void MyDrawWindowCallback(
                                   XPLMWindowID         inWindowID,
                                   void *               inRefcon)
{
	int		left, top, right, bottom, i;
	float	color[] = { 1.0, 1.0, 1.0 }; 	/* RGB White */
    char   *buffer = new char[ config_hello_text.length() + 1 ];

	/* First we get the location of the window passed in to us. */
	XPLMGetWindowGeometry(inWindowID, &left, &top, &right, &bottom);

	/* We now use an XPLMGraphics routine to draw a translucent dark
	 * rectangle that is our window's shape. */
	XPLMDrawTranslucentDarkBox(left, top, right, bottom);

	/* Finally we draw the text into the window, also using XPLMGraphics
	 * routines.  The NULL indicates no word wrapping. */
	strcpy( buffer, config_hello_text.c_str() );

    for( i=0; i<config_hello_times; i++ ) {
        XPLMDrawString(color, left + 5, top - (20 * (i+1) ),
            buffer, NULL, xplmFont_Basic);
    }

}

/*
 * MyHandleKeyCallback
 *
 * Our key handling callback does nothing in this plugin.  This is ok;
 * we simply don't use keyboard input.
 *
 */
void MyHandleKeyCallback(
                                   XPLMWindowID         inWindowID,
                                   char                 inKey,
                                   XPLMKeyFlags         inFlags,
                                   char                 inVirtualKey,
                                   void *               inRefcon,
                                   int                  losingFocus)
{
}

/*
 * MyHandleMouseClickCallback
 *
 * Our mouse click callback toggles the status of our mouse variable
 * as the mouse is clicked.  We then update our text on the next sim
 * cycle.
 *
 */
int MyHandleMouseClickCallback(
                                   XPLMWindowID         inWindowID,
                                   int                  x,
                                   int                  y,
                                   XPLMMouseStatus      inMouse,
                                   void *               inRefcon)
{
	return 1;
}
