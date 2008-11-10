#define PURPLE_PLUGINS

#include <glib.h>
#include <gtkutils.h>

#include <stdio.h>

#include "notify.h"
#include "plugin.h"
#include "version.h"

static void menuhide_attach(PurpleConversation *conv);

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
	PidginConversation *gtkconv;

	gtkconv = PIDGIN_CONVERSATION(conv);
	
	gtk_widget_hide(gtkconv->win->menu.menubar);
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

