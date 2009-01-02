#include <gtk/gtk.h>

#include "dgin314.h"

void pidgin_go_to_next_tab(PidginConversation *gtkconv)
{
	gtk_notebook_next_page((GtkNotebook*)gtkconv->win->notebook);
}

void pidgin_go_to_prev_tab(PidginConversation *gtkconv)
{
	gtk_notebook_prev_page((GtkNotebook*)gtkconv->win->notebook);
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
