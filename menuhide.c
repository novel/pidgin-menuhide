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
static void statusbar_create(GtkWidget* widget);

// function to change mode
static void set_mode(gboolean mode);

static PidginConversation *gtkconv;

/* Current mode
 * TRUE goes for 'INSERT'
 * FALSE goes for 'COMMAND'
 */
static gboolean current_mode = TRUE;
GtkWidget *modeline;
static gboolean label_created = FALSE;

static void
set_mode(gboolean mode)
{

	printf("here\n");

	current_mode = mode;

	if (current_mode == TRUE) {
		printf("-- INSERT --\n");
		gtk_label_set_text(modeline, "-- INSERT --");
	} else {
		gtk_label_set_text(modeline, "-- COMMAND --");
	}
}	

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
statusbar_create(GtkWidget* widget)
{
	GList *children;
	GList *child;

	if (label_created == TRUE)
		return;

	children = gtk_container_get_children (GTK_CONTAINER (widget));

	for (child = children; child; child = child->next) {
		GtkWidget *cur;

		cur = (GtkWidget *)child->data;		
		printf("%s\n", gtk_widget_get_name(cur));
		
	        modeline = gtk_label_new("");
		gtk_box_pack_end (GTK_BOX (cur), modeline, FALSE, TRUE, 0);
		gtk_misc_set_alignment(modeline, 0, 0);

		gtk_widget_show (modeline);

		set_mode(TRUE);
	}

	label_created = TRUE;
}

static void
menuhide_attach(PurpleConversation *conv)
{
	gtkconv = PIDGIN_CONVERSATION(conv);
	
	gtk_widget_hide(gtkconv->win->menu.menubar);
	keygrabber_init(gtkconv->win->window);
	statusbar_create(gtkconv->win->notebook);
	set_mode(TRUE);
	//	gtk_widget_show(
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

	char *keyname = keycode_to_str(keyevent->keycode);

	if (current_mode == TRUE) {
		if (strcmp(keyname, "Escape") == 0) {
			set_mode(FALSE);
			return FALSE;
		}
	} else {
		if (strcmp(keyname, "i") == 0) {
			set_mode(TRUE);		
			return FALSE;
		}
	}
//	if (strcmp(keyname, "Escape")

	if (keyevent->state & ControlMask) {
//		char *keyname = keycode_to_str(keyevent->keycode);

		printf("KEYNAME = %s\n", keyname);

		if (strcmp(keyname, "i") == 0) {
			//printf("userinfo\n");
			gtk_menu_item_activate((GtkMenuItem*)gtkconv->win->menu.get_info);
		} else if (strcmp(keyname, "n") == 0) {
			gtk_notebook_next_page((GtkNotebook*)gtkconv->win->notebook);
		} else if (strcmp(keyname, "p") == 0) {
			gtk_notebook_prev_page((GtkNotebook*)gtkconv->win->notebook);
		} else if (strcmp(keyname, "w") == 0) {
			//gtk_notebook_remove_page(
			//		(GtkNotebook*)gtkconv->win->notebook,
			//	  	gtk_notebook_get_current_page((GtkNotebook*)gtkconv->win->notebook));
//			gtk_widget_activate(gtkconv->close);
			purple_conversation_destroy(gtkconv->active_conv);
		} else {
			printf("KEYNAME = %s\n", keyname);
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

