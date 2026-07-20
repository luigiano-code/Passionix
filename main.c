#include "login.h"

int main(int argc, char *argv[])
{
    AdwApplication *app = adw_application_new("com.example.PinPadApp", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
