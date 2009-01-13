#include <pidgin/gtkconv.h>
#include <stdio.h>

#include "notify.h"
#include "plugin.h"
#include "version.h"

/* macroses */
#ifndef get_notebook
#define get_notebook(gtkconv)	((GtkNotebook*)gtkconv->win->notebook)
#endif /* get_notebook */

/* global variables */
GQueue *closed_convs;

/* structs */
struct ClosedTabInfo {
	PurpleAccount *account;
	char *name;
};

/* main functions */
void pidgin_go_to_next_tab(PidginConversation *gtkconv);

void pidgin_go_to_prev_tab(PidginConversation *gtkconv);

void pidgin_go_to_first_tab(PidginConversation *gtkconv);

void pidgin_go_to_last_tab(PidginConversation *gtkconv);

void pidgin_close_tab(PidginConversation *gtkconv);

void pidgin_restore_tab(PidginConversation *gtkconv);

void pidgin_conv_destroy_handler(PurpleConversation *conv);


/* helper functions */
void _register_tab_close(PurpleConversation *conv);
