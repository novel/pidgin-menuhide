#include <pidgin/gtkconv.h>
#include <stdio.h>

#include "notify.h"
#include "plugin.h"
#include "version.h"

/* macroses */
#ifndef get_notebook
#define get_notebook(gtkconv)	((GtkNotebook*)gtkconv->win->notebook)
#endif /* get_notebook */

/* main functions */

void pidgin_go_to_next_tab(PidginConversation *gtkconv);

void pidgin_go_to_prev_tab(PidginConversation *gtkconv);

void pidgin_go_to_first_tab(PidginConversation *gtkconv);

void pidgin_go_to_last_tab(PidginConversation *gtkconv);

void pidgin_close_tab(PidginConversation *gtkconv);
