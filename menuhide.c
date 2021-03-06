#define PURPLE_PLUGINS

#include <X11/Xlib.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <gdk/gdkx.h>
#include <glib.h>
#include <gtkutils.h>

#include <stdio.h>
#include <string.h>

#include "notify.h"
#include "plugin.h"
#include "version.h"

#include "dgin314.h"

static void menuhide_attach(PurpleConversation *conv);
static void keygrabber_init(GtkWidget *widget, PidginWindow *win);
static gboolean event_filter(gpointer event_data, PidginWindow *win);
static char* keycode_to_str(int keycode);
static void statusbar_create(GtkWidget* widget);

// function to change mode
static void set_mode(gboolean mode);

//static PidginConversation *gtkconv;

/* Current mode
 * TRUE goes for 'INSERT'
 * FALSE goes for 'COMMAND'
 */
static gboolean current_mode = TRUE;
GtkWidget *modeline;
static gboolean label_created = FALSE;

// array to hold pressed keys
GArray *keychain;

// function to process keychain
static void process_keychain(PidginConversation *gtkconv);

static void
process_keychain(PidginConversation *gtkconv)
{
	printf("process_keychain(length = %d)\n", keychain->len);

	if (keychain->len == 1) {
		char *first = g_array_index(keychain, char*, 0);

		/* if shortcut is not supported at the beginning, clear the list */
		if ((strcmp(first, "g") != 0) && (strcmp(first, "d") != 0) && 
				(strcmp(first, "u") != 0)) {
			g_array_remove_range(keychain, 0, 1);
			return;
		}

		/* XXX go upper, remove the unneeded && above */
		if (strcmp(first, "d") == 0) {
			pidgin_close_tab(gtkconv);
			g_array_remove_range(keychain, 0, 1);
		}

		if (strcmp(first, "u") == 0) {
			pidgin_restore_tab(gtkconv);
			g_array_remove_range(keychain, 0, 1);
		}
	}	

	if (keychain->len == 2) {
		char *first, *second;

		first = g_array_index(keychain, char*, 0);
		second = g_array_index(keychain, char*, 1);
		printf("keychain: %s-%s\n", first, second);
		//		g_array_index(keychain, char*, 0),
		//		g_array_index(keychain, char*, 1));
		if (strcmp(first, "g") == 0) {
			if (strcmp(second, "t") == 0) {
				pidgin_go_to_next_tab(gtkconv);
			} else if (strcmp(second, "T") == 0) {
				pidgin_go_to_prev_tab(gtkconv);
			} else if (strcmp(second, "$") == 0) {
				pidgin_go_to_last_tab(gtkconv);
			} else if (strcmp(second, "^") == 0) {
				pidgin_go_to_first_tab(gtkconv);
			}
		}

		g_array_remove_range(keychain, 0, 2);
	}
}

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

	purple_signal_connect(conv_handle, "deleting-conversation", 
			plugin, PURPLE_CALLBACK(pidgin_conv_destroy_handler), NULL);
   
	keychain = g_array_new(FALSE, FALSE, sizeof(char*));

	/* init queue of closed tabs */
	closed_convs = g_queue_new();

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
		
	        modeline = gtk_label_new("XXX");
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
	PidginConversation *gtkconv = PIDGIN_CONVERSATION(conv);
	PidginWindow *win = pidgin_conv_get_window(gtkconv);

	gtk_widget_hide(gtkconv->win->menu.menubar);
	keygrabber_init(gtkconv->win->window, win);
	statusbar_create(gtkconv->win->window);
	set_mode(TRUE);
	//	gtk_widget_show(
}

static GdkFilterReturn
gdk_filter(GdkXEvent *xevent,
	GdkEvent *event,
	gpointer data)
{
	PidginWindow *win = (PidginWindow*)data;
	gboolean result = event_filter(xevent, win);
	if (result == TRUE)
		return GDK_FILTER_CONTINUE;
	else
		return GDK_FILTER_REMOVE;
}

static gboolean
event_filter(gpointer event_data, PidginWindow *win)
{	
	PidginConversation *gtkconv;
       
	gtkconv	= pidgin_conv_window_get_active_gtkconv(win);

	XKeyEvent *keyevent = (XKeyEvent*)event_data;

	if (((XEvent*)keyevent)->type != KeyPress)
		return TRUE;

	char *keyname = keycode_to_str(keyevent->keycode);

	/*
	 * This part handles switching between modes
	 */
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

	/* These keybindings are relevant in all modes */
	if (keyevent->state & ControlMask) {
//		char *keyname = keycode_to_str(keyevent->keycode);

//		printf("KEYNAME = %s\n", keyname);

		if (strcmp(keyname, "i") == 0) {
			//printf("userinfo\n");
			gtk_menu_item_activate((GtkMenuItem*)gtkconv->win->menu.get_info);
		} else if (strcmp(keyname, "n") == 0) {
			gtk_notebook_next_page((GtkNotebook*)gtkconv->win->notebook);
		} else if (strcmp(keyname, "p") == 0) {
			gtk_notebook_prev_page((GtkNotebook*)gtkconv->win->notebook);
		} else if (strcmp(keyname, "w") == 0) {
//			gtk_notebook_remove_page(
//					(GtkNotebook*)gtkconv->win->notebook,
//				  	gtk_notebook_get_current_page((GtkNotebook*)gtkconv->win->notebook));
//			gtk_widget_activate(gtkconv->close);
			//purple_conversation_destroy(gtkconv->active_conv);
//			GList *list = g_list_copy(gtkconv->convs);
//			g_list_foreach(list, (GFunc)purple_conversation_destroy, NULL);
//			g_list_free(list);
			pidgin_close_tab(gtkconv);
		} else {
			//printf("KEYNAME = %s\n", keyname);
			return TRUE;
		}

		return FALSE;
	}

	// command line mode keybindings
	if (current_mode == FALSE) {
		char ch;

		/* first check what we got. We're interested only in [a-zA-Z0-9], so ignore
		 * other stuff */
		if (strlen(keyname) > 1)
			/* strings longer than 1 char here means it's something like 'Shift' etc */
			return FALSE;

		ch = keyname[0];
		
		if (isalnum(ch) == 0)
			/* meaning it's not alphanumeric */
			return FALSE;

		if (keyevent->state & ShiftMask) {
			if (isalpha(ch) != 0) {			
				keyname = (char*)g_strdup_printf("%c", toupper(ch));
				printf("With shift: %s [%c]\n", keyname, toupper(ch));
			} else if (ch == '4') {
				keyname = strdup("$");
			} else if (ch == '6') {
				keyname = strdup("^");
			}
		}

		g_array_append_val(keychain, keyname);
		process_keychain(gtkconv);
			
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
keygrabber_init(GtkWidget *widget, PidginWindow *win)
{
	GdkWindow* root;

	root = gtk_widget_get_toplevel(GTK_WIDGET(widget))->window;
	
	if (g_object_get_data(GTK_OBJECT(root), "filter_set") == NULL) {
		gdk_window_add_filter(root, gdk_filter, (gpointer)win);
		g_object_set_data(GTK_OBJECT(root), "filter_set", "1");
	}
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

