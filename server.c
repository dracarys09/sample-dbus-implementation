#include<gio/gio.h>
#include<gio/gunixfdlist.h>
#include<stdlib.h>

static GDBusNodeInfo *introspection_data = NULL;

static void
handle_method_call (GDBusConnection       *connection,
                    const gchar           *sender,
                    const gchar           *object_path,
                    const gchar           *interface_name,
                    const gchar           *method_name,
                    GVariant              *parameters,
                    GDBusMethodInvocation  *invocation,
                    gpointer              user_data)
{
  g_message("Method Called : %s", method_name);
  if( g_strcmp0 (method_name, "Add") == 0 )
    {
      gint32 first, second;
      gint res;

      g_variant_get(parameters, "(ii)", &first, &second);

      res = first + second;
      g_print("%d + %d = %d\n", first, second, res);

      g_dbus_method_invocation_return_value (invocation,
                                             g_variant_new ("(i)", res));
    }
}

static const GDBusInterfaceVTable interface_vtable =
  {
    handle_method_call,
    NULL,
    NULL
  };

static void
on_name_acquired (GDBusConnection *connection,
                  const gchar     *name,
                  gpointer         user_data)
{
  g_message("Name Acquired: %s", name);
  guint registration_id;

  registration_id = g_dbus_connection_register_object (connection,
                                                       "/org/openprinting/ADD",
                                                       introspection_data->interfaces[0],
                                                       &interface_vtable,
                                                       NULL,
                                                       NULL,
                                                       NULL);
  g_assert(registration_id > 0);
}

static void
on_name_lost (GDBusConnection *connection,
              const gchar     *name,
              gpointer         user_data)
{
  g_message("Name Lost: %s", name);
  exit(1);
}

int
main (int argc, char **argv)
{
  GDBusConnection *connection;
  GError *error;
  GMainLoop *loop;
  gchar *contents;
  guint owner_id;
  guint subscription_id;

  error = NULL;
  connection = g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, &error);
  g_assert_no_error(error);
  g_assert(connection != NULL);

  error = NULL;

  g_file_get_contents("./org.openprinting.Test.xml", &contents, NULL, &error);
  g_assert_no_error(error);
  g_assert(contents != NULL);

  introspection_data = g_dbus_node_info_new_for_xml(contents, NULL);
  g_assert(introspection_data != NULL);
  g_free(contents);

  owner_id = g_bus_own_name_on_connection (connection,
                                           "org.openprinting.ADD",
                                           G_BUS_NAME_OWNER_FLAGS_NONE,
                                           on_name_acquired,
                                           on_name_lost,
                                           NULL,
                                           NULL);

  loop = g_main_loop_new(NULL, FALSE);
  g_main_loop_run(loop);
  g_bus_unown_name(owner_id);
  g_dbus_node_info_unref(introspection_data);

  return 0;
}
