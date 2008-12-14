#define PURPLE_PLUGINS

#include <X11/Xlib.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <gdk/gdkx.h>
#include <glib.h>
#include <gtkutils.h>

#include <stdio.h>

#include "notify.h"
#include "plugin.h"
#include "version.h"

static void menuhide_attach(PurpleConversation *conv);
static void keygrabber_init(GtkWidget *widget);
static gboolean event_filter(gpointer event_data);
static char* keycode_to_str(int keycode);

static PidginConversation *gtkconv;

static gboolean
plugin_load(PurplePlugin *plugin) {
	void *conv_handle = purple_conversations_get_handle();
    GList *convs;

    for (convs = purple_get_conversations(); convs != NULL; convs = convs->next)
	menuhide_attach((PurpleConversation *)convs->data);

    purple_signal_connect(conv_handle, "conversation-created",
	plugin, PURPLE_CALLBACK(menuhide_attach), NULL);
   

    return TRUE;
}

static void
menuhide_attach(PurpleConversation *conv)
{
	gtkconv = PIDGIN_CONVERSATION(conv);
	
	gtk_widget_hide(gtkconv->win->menu.menubar);
	keygrabber_init(gtkconv->win->window);
}

static GdkFilterReturn
gdk_filter(GdkXEvent *xevent,
	GdkEvent *event,
	gpointer data)
{
	gboolean result = event_filter(xevent);
	if (result == TRUE)
		return GDK_FILTER_CONTINUE;
	else
		return GDK_FILTER_REMOVE;
}

static gboolean
event_filter(gpointer event_data)
{
	XKeyEvent *keyevent = (XKeyEvent*)event_data;

	if (((XEvent*)keyevent)->type != KeyPress)
		return TRUE;

	if (keyevent->state & ControlMask) {
		char *keyname = keycode_to_str(keyevent->keycode);
		if (strcmp(keyname, "i") == 0) {
			//printf("userinfo\n");
			gtk_menu_item_activate((GtkMenuItem*)gtkconv->win->menu.get_info);
		} else if (strcmp(keyname, "n") == 0) {
			gtk_notebook_next_page((GtkNotebook*)gtkconv->win->notebook);
		} else if (strcmp(keyname, "p") == 0) {
			gtk_notebook_prev_page((GtkNotebook*)gtkconv->win->notebook);
		} else {
			return TRUE;
		}

		return FALSE;
	}

	return TRUE;
}

static char*
keycode_to_str(int keycode)
{
	KeyCode key = (KeyCode)keycode;
	KeySym sym;
	Display *display;
	char *keyname = NULL;

	display = GDK_DISPLAY();
	sym = XKeycodeToKeysym(display, key, 0);
	if (sym != NoSymbol)
		keyname = XKeysymToString(sym);
	
	return keyname;
}

static void
keygrabber_init(GtkWidget *widget)
{
	GdkWindow* root;

	root = gtk_widget_get_toplevel(GTK_WIDGET(widget))->window;

	gdk_window_add_filter(root, gdk_filter, NULL);
}

static PurplePluginInfo info = {
    PURPLE_PLUGIN_MAGIC,
    PURPLE_MAJOR_VERSION,
    PURPLE_MINOR_VERSION,
    PURPLE_PLUGIN_STANDARD,
    NULL,
    0,
    NULL,
    PURPLE_PRIORITY_DEFAULT,

    "gtk-menuhide",
    "Menu Hide",
    "0.1",

    "A plugin to hide totally uncool menu.",          
    "The summay said it all.",          
    "Roman Bogorodskiy <bogorodskiy@gmail.com>",
    "http://novel.evilcoder.org",
    
    plugin_load,                   
    NULL,                          
    NULL,                          
                                   
    NULL,                          
    NULL,                          
    NULL,                        
    NULL,                   
    NULL,                          
    NULL,                          
    NULL,                          
    NULL                           
};                               
    
static void                        
init_plugin(PurplePlugin *plugin)
{                                  
}

PURPLE_INIT_PLUGIN(hello_world, init_plugin, info)

