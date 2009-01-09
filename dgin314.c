#include <gtk/gtk.h>

#include "dgin314.h"

/*
 * Main functions
 */

void pidgin_go_to_next_tab(PidginConversation *gtkconv)
{
	if (gtk_notebook_get_current_page((GtkNotebook*)gtkconv->win->notebook) == 
			gtk_notebook_get_n_pages((GtkNotebook*)gtkconv->win->notebook) - 1) {
		/* if we're at the last page, go to the first one */
		pidgin_go_to_first_tab(gtkconv);
	} else {
		gtk_notebook_next_page(get_notebook(gtkconv));
	}
}

void pidgin_go_to_prev_tab(PidginConversation *gtkconv)
{
	if (gtk_notebook_get_current_page(get_notebook(gtkconv)) == 0) {
		/* if we're at the first page, go to the last one */
		pidgin_go_to_last_tab(gtkconv);
	} else {
		gtk_notebook_prev_page((GtkNotebook*)gtkconv->win->notebook);
	}
}

void pidgin_go_to_first_tab(PidginConversation *gtkconv)
{
	if (gtk_notebook_get_n_pages((GtkNotebook*)gtkconv->win->notebook) < 2)
		return;

	gtk_notebook_set_current_page((GtkNotebook*)gtkconv->win->notebook, 0);
}

void pidgin_go_to_last_tab(PidginConversation *gtkconv)
{
	if (gtk_notebook_get_n_pages((GtkNotebook*)gtkconv->win->notebook) < 2)
		return;

	gtk_notebook_set_current_page((GtkNotebook*)gtkconv->win->notebook, -1);
}

void pidgin_close_tab(PidginConversation *gtkconv)
{
	purple_conversation_destroy(gtkconv->active_conv);
}
