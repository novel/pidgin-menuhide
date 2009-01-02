#include <gtk/gtk.h>

#include "dgin314.h"

void pigdin_go_to_next_tab(PidginConversation *gtkconv)
{
	gtk_notebook_next_page((GtkNotebook*)gtkconv->win->notebook);
}
