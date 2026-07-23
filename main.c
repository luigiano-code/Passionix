#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gtk/gtk.h>
#include <adwaita.h>

#include "logingtk.h"
#include "maingtk.h"
#include "randomgtk.h"
#include "addgtk.h"
#include "passwordgtk.h"

#include "password.h"
#include "add.h"
#include "random.h"
#include "data.h"
#include "random.h"
#include "login.h"
#include "save.h"
#include "compare.h"

static void main_password_page(GtkButton *button, gpointer user_data);
static void main_add(GtkButton *button, gpointer user_data);
static void main_random(GtkButton *button, gpointer user_data);
static void login_next(GtkButton *button, gpointer user_data);

static void activate(GtkApplication *app, gpointer user_data)
{
    AdwApplicationWindow *window;
    AdwHeaderBar *header;
    GtkWidget *toolbar_view;
    GtkWidget *stack;

    GtkWidget *login_page;
    GtkWidget *main_page;
    GtkWidget *random_page;
    GtkWidget *add_page;
    GtkWidget *password_page;

    window = ADW_APPLICATION_WINDOW(
        adw_application_window_new(app)
    );

    gtk_window_set_title(
        GTK_WINDOW(window),
        "Passionix"
    );

    gtk_window_set_default_size(
        GTK_WINDOW(window),
        600,
        400
    );


    header = ADW_HEADER_BAR(
        adw_header_bar_new()
    );
    adw_header_bar_set_title_widget(
        header,
        gtk_label_new("Passionix")
    );


    toolbar_view = adw_toolbar_view_new();
    adw_toolbar_view_add_top_bar(
        ADW_TOOLBAR_VIEW(toolbar_view),
        GTK_WIDGET(header)
    );


    stack = gtk_stack_new();

	login_page = create_login_page(
		G_CALLBACK(login_next),
		stack
	);
    gtk_stack_add_named(
        GTK_STACK(stack),
        login_page,
        "login"
    );
    gtk_stack_set_visible_child_name(
        GTK_STACK(stack),
        "login"
    );

	main_page = create_main_page(
		G_CALLBACK(main_random),
		G_CALLBACK(main_add),
		G_CALLBACK(main_password_page),
		stack
	);
    gtk_stack_add_named(
        GTK_STACK(stack),
        main_page,
        "main"
    );

	random_page = create_random_page(
		G_CALLBACK(main_random),
		stack
	);
    gtk_stack_add_named(
        GTK_STACK(stack),
        random_page,
        "random"
    );

	add_page = create_add_page(
		G_CALLBACK(main_add),
		stack
	);
    gtk_stack_add_named(
        GTK_STACK(stack),
        add_page,
        "add"
    );

	password_page = create_add_page(
		G_CALLBACK(main_password_page),
		stack
	);
    gtk_stack_add_named(
        GTK_STACK(stack),
        password_page,
        "password"
    );

    adw_toolbar_view_set_content(
        ADW_TOOLBAR_VIEW(toolbar_view),
        stack
    );

    adw_application_window_set_content(
        window,
        toolbar_view
    );

    gtk_window_present(
        GTK_WINDOW(window)
    );
}

static void main_password_page(
    GtkButton *button,
    gpointer user_data
)
{
    GtkStack *stack = GTK_STACK(user_data);

    gtk_stack_set_visible_child_name(
        stack,
        "password"
    );
}

static void main_random(
    GtkButton *button,
    gpointer user_data
)
{
    GtkStack *stack = GTK_STACK(user_data);

    gtk_stack_set_visible_child_name(
        stack,
        "random"
    );
}

static void main_add(
    GtkButton *button,
    gpointer user_data
)
{
    GtkStack *stack = GTK_STACK(user_data);

    gtk_stack_set_visible_child_name(
        stack,
        "add"
    );
}

static void login_next(
    GtkButton *button,
    gpointer user_data
)
{
    GtkStack *stack = GTK_STACK(user_data);

    gtk_stack_set_visible_child_name(
        stack,
        "main"
    );
}

int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = GTK_APPLICATION(
        adw_application_new(
            "com.passionix.app",
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
