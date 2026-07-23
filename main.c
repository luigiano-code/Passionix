#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gtk/gtk.h>
#include <adwaita.h>

#include "logingtk.h"

#include "password.h"
#include "add.h"
#include "random.h"
#include "data.h"
#include "random.h"
#include "login.h"
#include "save.h"
#include "compare.h"

static void activate(GtkApplication *app, gpointer user_data)
{
    create_login_window(app, user_data);
}

int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = GTK_APPLICATION(
        adw_application_new(
            "com.passionix.App",
            G_APPLICATION_DEFAULT_FLAGS
        )
    );

    g_signal_connect(
        app,
        "activate",
        G_CALLBACK(activate),
        NULL
    );

    status = g_application_run(
        G_APPLICATION(app),
        argc,
        argv
    );

    g_object_unref(app);

    return status;
}

/*
int main()
{

	int action = login();
	int c;

	srand(time(NULL));


	if ( action == 1 )
	{
		while ((c = getchar()) != '\n' && c != EOF);
		add();		
		save_data("data.json");
	}
	else if	( action == 2 )
	{
		while ((c = getchar()) != '\n' && c != EOF);
		random_cli();
		save_data("data.json");
	}
	else if ( action == 3 )
	{
		while ((c = getchar()) != '\n' && c != EOF);
		password();
	}
	else if ( action == 4 )
	{
		compare();
	}
	else if ( action == 5 )
	{
		printf("save");
	}
}
*/
