#include<gio/gio.h>
#include<gio/gunixfdlist.h>
#include<stdlib.h>

void
test_Add(GDBusProxy *proxy)
{
  GVariant *result;
  GError *error = NULL;
  gint32 res;

  g_print("Calling Add(30,40)...\n");
  result = g_dbus_proxy_call_sync(proxy,
                                  "Add",
                                  g_variant_new("(ii)", 30, 40),
                                  G_DBUS_CALL_FLAGS_NONE,
                                  -1,
                                  NULL,
                                  &error);

  g_assert_no_error(error);
  g_variant_get(result, "(i)", &res);
  g_print("The server answered: %d\n", res);
  g_variant_unref(result);
}

int
main(void)
{
  GDBusProxy *proxy;
  GDBusConnection *connection;
  GError *error = NULL;

  connection = g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, &error);
  g_assert_no_error(error);
  g_assert(connection != NULL);

  error = NULL;

  proxy = g_dbus_proxy_new_sync(connection,
                                G_DBUS_PROXY_FLAGS_NONE,
                                NULL,
                                "org.openprinting.ADD",
                                "/org/openprinting/ADD",
                                "org.openprinting.ADD",
                                NULL,
                                &error);

  g_assert_no_error(error);

  g_print("Testing server interface...\n");

  test_Add(proxy);
  g_object_unref(proxy);
  g_object_unref(connection);

  return 0;
}
