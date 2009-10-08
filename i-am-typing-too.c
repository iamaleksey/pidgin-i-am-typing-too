#include "internal.h"
#include "version.h"
#include "plugin.h"
#include "signals.h"
#include "conversation.h"

static void
conv_send_typing(PurpleAccount *account, const char *name, PurpleTypingState state) {
  PurpleConversation* conv =
    purple_find_conversation_with_account(PURPLE_CONV_TYPE_IM, name, account);
  if(conv != NULL) {
    serv_send_typing(conv->account->gc, name, state);
  }
}

static void
buddy_typing_cb(PurpleAccount *account, const char *name) {
  conv_send_typing(account, name, PURPLE_TYPING);
}

static void
buddy_typing_stopped_cb(PurpleAccount *account, const char *name) {
  conv_send_typing(account, name, PURPLE_NOT_TYPING);
}

static gboolean
plugin_load(PurplePlugin *plugin)
{
  void *conv_handle = purple_conversations_get_handle();

  purple_signal_connect(
      conv_handle,
      "buddy-typing",
      plugin,
      PURPLE_CALLBACK(buddy_typing_cb),
      NULL);

  purple_signal_connect(
      conv_handle,
      "buddy-typing-stopped",
      plugin,
      PURPLE_CALLBACK(buddy_typing_stopped_cb),
      NULL);

  return TRUE;
}

static gboolean
plugin_unload(PurplePlugin *plugin)
{
  purple_signals_disconnect_by_handle(plugin);
  return TRUE;
}

static PurplePluginInfo info =
{
  PURPLE_PLUGIN_MAGIC,
  PURPLE_MAJOR_VERSION,
  PURPLE_MINOR_VERSION,
  PURPLE_PLUGIN_STANDARD,
  NULL, 0, NULL,
  PURPLE_PRIORITY_DEFAULT,

  "core-iamaleksey-iamtypingtoo",
  "I am typing too.",
  "1.0",

  "Show that you are also typing whenever someone is typing to you.",
  "Send PURPLE_TYPING and PURPLE_NOT_TYPING states upon receiving buddy-typing and buddy-typing-stopped signals.",
  "Aleksey Yeschenko <aleksey@yeschenko.com>",
  "http://github.com/iamaleksey/pidgin-i-am-typing-too",

  plugin_load,
  plugin_unload,

  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

static void
init_plugin(PurplePlugin *plugin) {}

PURPLE_INIT_PLUGIN(iamtypingtoo, init_plugin, info)
