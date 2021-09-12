#include "include/libwinmedia/libwinmedia_plugin.h"

#include <flutter_linux/flutter_linux.h>
#include <gtk/gtk.h>
#include <sys/utsname.h>

#include <cstring>

#define LIBWINMEDIA_PLUGIN(obj)                                     \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), libwinmedia_plugin_get_type(), \
                              LibwinmediaPlugin))

struct _LibwinmediaPlugin {
  GObject parent_instance;
};

G_DEFINE_TYPE(LibwinmediaPlugin, libwinmedia_plugin, g_object_get_type())

// Called when a method call is received from Flutter.
static void libwinmedia_plugin_handle_method_call(LibwinmediaPlugin* self,
                                                  FlMethodCall* method_call) {
  fl_method_call_respond(
      method_call, FL_METHOD_RESPONSE(fl_method_not_implemented_response_new()),
      nullptr);
}

static void libwinmedia_plugin_dispose(GObject* object) {
  G_OBJECT_CLASS(libwinmedia_plugin_parent_class)->dispose(object);
}

static void libwinmedia_plugin_class_init(LibwinmediaPluginClass* klass) {
  G_OBJECT_CLASS(klass)->dispose = libwinmedia_plugin_dispose;
}

static void libwinmedia_plugin_init(LibwinmediaPlugin* self) {}

static void method_call_cb(FlMethodChannel* channel, FlMethodCall* method_call,
                           gpointer user_data) {
  LibwinmediaPlugin* plugin = LIBWINMEDIA_PLUGIN(user_data);
  libwinmedia_plugin_handle_method_call(plugin, method_call);
}

void libwinmedia_plugin_register_with_registrar(FlPluginRegistrar* registrar) {
  LibwinmediaPlugin* plugin =
      LIBWINMEDIA_PLUGIN(g_object_new(libwinmedia_plugin_get_type(), nullptr));

  g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
  g_autoptr(FlMethodChannel) channel =
      fl_method_channel_new(fl_plugin_registrar_get_messenger(registrar),
                            "libwinmedia", FL_METHOD_CODEC(codec));
  fl_method_channel_set_method_call_handler(
      channel, method_call_cb, g_object_ref(plugin), g_object_unref);

  g_object_unref(plugin);
}
