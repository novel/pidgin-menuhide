#include <gtk/gtk.h>

#include <string.h>

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

void pidgin_restore_tab(PidginConversation *gtkconv)
{
	if (g_queue_is_empty(closed_convs) == TRUE)
		return;

	struct ClosedTabInfo *closed_tab_info = (struct CloedTabInfo*)g_queue_pop_tail(closed_convs);
	printf("to restore: %s\n", closed_tab_info->name);
	//g_free(closed_tab_info);
	purple_conversation_new(PURPLE_CONV_TYPE_IM, closed_tab_info->account, closed_tab_info->name);
}

void pidgin_conv_destroy_handler(PurpleConversation *conv) 
{
	_register_tab_close(conv);
}

/*
 * helper functions
 */

void _register_tab_close(PurpleConversation *conv) {
	printf("%s: closed: %s\n", __func__, conv->name);
	struct ClosedTabInfo *closed_tab_info = (struct ClosedTabInfo*)malloc(sizeof(struct ClosedTabInfo*));
	closed_tab_info->account = conv->account;
	closed_tab_info->name = strdup(conv->name);

	g_queue_push_head(closed_convs, closed_tab_info);
}

