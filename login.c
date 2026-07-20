#include "login.h"
#include "app.h"

typedef struct {
    GtkWidget *entry;
    GtkWidget *window;
    AdwApplication *app;
} LoginData;

static void on_number_clicked(GtkButton *button, gpointer user_data)
{
    GtkEditable *editable = GTK_EDITABLE(user_data);
    const char *digit = gtk_button_get_label(button);
    int pos = -1;
    gtk_editable_insert_text(editable, digit, -1, &pos);
}

static void on_clear_clicked(GtkButton *button, gpointer user_data)
{
    GtkEditable *editable = GTK_EDITABLE(user_data);
    gtk_editable_set_text(editable, "");
}

static void on_submit_clicked(GtkButton *button, gpointer user_data)
{
    LoginData *data = (LoginData *)user_data;
    const char *text = gtk_editable_get_text(GTK_EDITABLE(data->entry));

    FILE *file = fopen("password.txt", "r");
    if (file == NULL)
    {
        return;
    }

    char hash[256];
    gboolean loaded = FALSE;

    if (fscanf(file, "%255s", hash) == 1)
    {
        loaded = TRUE;
    }
    fclose(file);

    if (!loaded) return;

    char *input_hash = hash_password(text);

    if (input_hash && g_strcmp0(input_hash, hash) == 0)
    {
        show_main_view(data->app, text);
        
        gtk_window_destroy(GTK_WINDOW(data->window));
        g_free(data);
    }

    g_free(input_hash);
}

void activate(AdwApplication *app, gpointer user_data)
{
    GtkWidget *window = adw_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_title(GTK_WINDOW(window), "Passionix");
    gtk_window_set_default_size(GTK_WINDOW(window), 320, 420);

    GtkWidget *toolbar = adw_toolbar_view_new();
    GtkWidget *header = adw_header_bar_new();
    adw_toolbar_view_add_top_bar(ADW_TOOLBAR_VIEW(toolbar), header);

    GtkWidget *content = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_widget_set_margin_top(content, 20);
    gtk_widget_set_margin_bottom(content, 20);
    gtk_widget_set_margin_start(content, 20);
    gtk_widget_set_margin_end(content, 20);

    GtkWidget *label = gtk_label_new("Insert PIN:");
    GtkWidget *entry = gtk_password_entry_new();

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 8);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 8);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);

    const char *buttons[9] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
    for (int i = 0; i < 9; i++) {
        GtkWidget *btn = gtk_button_new_with_label(buttons[i]);
        g_signal_connect(btn, "clicked", G_CALLBACK(on_number_clicked), entry);
        int row = i / 3;
        int col = i % 3;
        gtk_grid_attach(GTK_GRID(grid), btn, col, row, 1, 1);
    }

    GtkWidget *btn_clear = gtk_button_new_with_label("C");
    g_signal_connect(btn_clear, "clicked", G_CALLBACK(on_clear_clicked), entry);
    gtk_grid_attach(GTK_GRID(grid), btn_clear, 0, 3, 1, 1);

    GtkWidget *btn_zero = gtk_button_new_with_label("0");
    g_signal_connect(btn_zero, "clicked", G_CALLBACK(on_number_clicked), entry);
    gtk_grid_attach(GTK_GRID(grid), btn_zero, 1, 3, 1, 1);

    LoginData *data = g_new(LoginData, 1);
    data->entry = entry;
    data->window = window;
    data->app = app;

    GtkWidget *btn_submit = gtk_button_new_with_label("OK");
    gtk_widget_add_css_class(btn_submit, "suggested-action");
    g_signal_connect(btn_submit, "clicked", G_CALLBACK(on_submit_clicked), data);
    gtk_grid_attach(GTK_GRID(grid), btn_submit, 2, 3, 1, 1);

    gtk_box_append(GTK_BOX(content), label);
    gtk_box_append(GTK_BOX(content), entry);
    gtk_box_append(GTK_BOX(content), grid);

    adw_toolbar_view_set_content(ADW_TOOLBAR_VIEW(toolbar), content);
    adw_application_window_set_content(ADW_APPLICATION_WINDOW(window), toolbar);

    gtk_window_present(GTK_WINDOW(window));
}
