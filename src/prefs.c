/*
 * gnect prefs.c
 *
 */



#include "config.h"
#include "main.h"
#include "gnect.h"
#include "prefs.h"
#include "gui.h"
#include "dialog.h"
#include "gfx.h"



#define DEFAULT_FNAME_THEME            "default.gnect"  /* If no prefs exist, start with this theme */
#define DEFAULT_PLAYER_1               "0"              /* Human */
#define DEFAULT_PLAYER_2               "2"              /* Velena Engine, first level */
#define DEFAULT_KEY_LEFT               "106"            /* j */
#define DEFAULT_KEY_RIGHT              "108"            /* l */
#define DEFAULT_KEY_DROP               "107"            /* k */
#define DEFAULT_START_MODE             "2"              /* Players take turns at starting */
#define DEFAULT_SOUND_MODE             "1"              /* Beep */
#define DEFAULT_DO_GRIDS               "true"           /* Draw grids on background images by default */
#define DEFAULT_DO_ANIMATE             "true"           /* Use animation by default */
#define DEFAULT_DO_WIPES               "true"           /* Do animated wipes by default */
#define DEFAULT_DO_TOOLBAR             "false"          /* Toolbar disabled */
#define DEFAULT_DO_SOUND               "true"           /* Sound enabled */
#define DEFAULT_DO_VERIFY              "true"           /* Verification dialogs enabled */


extern gint      debugging;
extern Gnect     gnect;
extern Anim      anim;
extern GtkWidget *app;
extern Theme     *theme_base;
extern Theme     *theme_current;

Prefs  prefs;

static Prefs  prefs_tmp;
static GtkWidget *label_player_selection1;
static GtkWidget *label_player_selection2;
static GtkWidget *dlg_prefs = NULL;
static GtkWidget *checkbutton_animate;
static GtkWidget *checkbutton_wipe;
static GtkWidget *checkbutton_verify;
static GtkWidget *entry_key_left;
static GtkWidget *entry_key_right;
static GtkWidget *entry_key_drop;
static GtkWidget *optionmenu_theme;
static GtkWidget *optionmenu_theme_menu;
static GtkWidget *radio_player1[5];
static GtkWidget *radio_player2[5];
static GtkWidget *radio_start[4];
static GtkWidget *radio_sound[2];


static void cb_prefs_dialog_apply(GtkWidget *widget, gpointer *data);



static void
cb_prefs_response (GtkWidget *pref_dialog, int response_id, gpointer data)
{
        if (response_id == GTK_RESPONSE_CLOSE) {
                cb_prefs_dialog_apply (pref_dialog, (gpointer)-1);
                gtk_widget_destroy (dlg_prefs);
                dlg_prefs = NULL;
        }
        return;
}



static void
prefs_check (void)
{
        if (prefs.start_mode < 0 || prefs.start_mode > 3) {
                prefs.start_mode = atoi (DEFAULT_START_MODE);
                prefs.changed = TRUE;
        }
        if (prefs.player1 < 0 || prefs.player1 > 4) {
                prefs.player1 = atoi (DEFAULT_PLAYER_1);
                prefs.changed = TRUE;
        }
        if (prefs.player2 < 0 || prefs.player2 > 4) {
                prefs.player2 = atoi (DEFAULT_PLAYER_2);
                prefs.changed = TRUE;
        }
        if (prefs.sound_mode < 0 || prefs.sound_mode > 2) {
                prefs.sound_mode = atoi (DEFAULT_SOUND_MODE);
                prefs.changed = TRUE;
        }

        /* update prefs from gnect < 1.2.4 */
        if (prefs.sound_mode == SOUND_MODE_NONE) {
                prefs.sound_mode = atoi (DEFAULT_SOUND_MODE);
                prefs.do_sound = FALSE;
                prefs.changed = TRUE;
        }
}



void
prefs_get (void)
{
        DEBUG_PRINT(1, "prefs_get\n");

        prefs.changed = FALSE;

        prefs.fname_theme   = gnome_config_get_string ("/" APPNAME "/Preferences/Theme="  DEFAULT_FNAME_THEME);
        prefs.player1       = gnome_config_get_int ("/" APPNAME "/Preferences/Player1="   DEFAULT_PLAYER_1);
        prefs.player2       = gnome_config_get_int ("/" APPNAME "/Preferences/Player2="   DEFAULT_PLAYER_2);
        prefs.key[KEY_LEFT] = gnome_config_get_int ("/" APPNAME "/Preferences/KeyLeft="   DEFAULT_KEY_LEFT);
        prefs.key[KEY_RIGHT]= gnome_config_get_int ("/" APPNAME "/Preferences/KeyRight="  DEFAULT_KEY_RIGHT);
        prefs.key[KEY_DROP] = gnome_config_get_int ("/" APPNAME "/Preferences/KeyDrop="   DEFAULT_KEY_DROP);
        prefs.sound_mode    = gnome_config_get_int ("/" APPNAME "/Preferences/SoundMode=" DEFAULT_SOUND_MODE);
        prefs.start_mode    = gnome_config_get_int ("/" APPNAME "/Preferences/StartMode=" DEFAULT_START_MODE);
        prefs.do_animate    = gnome_config_get_bool ("/" APPNAME "/Preferences/Animate="  DEFAULT_DO_ANIMATE);
        prefs.do_wipes      = gnome_config_get_bool ("/" APPNAME "/Preferences/Wipes="    DEFAULT_DO_WIPES);
        prefs.do_grids      = gnome_config_get_bool ("/" APPNAME "/Preferences/Grids="    DEFAULT_DO_GRIDS);
        prefs.do_toolbar    = gnome_config_get_bool ("/" APPNAME "/Preferences/Toolbar="  DEFAULT_DO_TOOLBAR);
        prefs.do_sound      = gnome_config_get_bool ("/" APPNAME "/Preferences/Sound="    DEFAULT_DO_SOUND);
        prefs.do_verify     = gnome_config_get_bool ("/" APPNAME "/Preferences/Verify="   DEFAULT_DO_VERIFY);

        prefs.descr_player1 = NULL;
        prefs.descr_player2 = NULL;

        prefs_check ();
}



void
prefs_save (void)
{
        if (!prefs.changed) return;

        DEBUG_PRINT(1, "prefs_save\n");

        gnome_config_set_string ("/" APPNAME "/Preferences/Theme",  prefs.fname_theme);
        gnome_config_set_int ("/" APPNAME "/Preferences/Player1",   prefs.player1);
        gnome_config_set_int ("/" APPNAME "/Preferences/Player2",   prefs.player2);
        gnome_config_set_int ("/" APPNAME "/Preferences/KeyLeft",   prefs.key[KEY_LEFT]);
        gnome_config_set_int ("/" APPNAME "/Preferences/KeyRight",  prefs.key[KEY_RIGHT]);
        gnome_config_set_int ("/" APPNAME "/Preferences/KeyDrop",   prefs.key[KEY_DROP]);
        gnome_config_set_int ("/" APPNAME "/Preferences/SoundMode", prefs.sound_mode);
        gnome_config_set_int ("/" APPNAME "/Preferences/StartMode", prefs.start_mode);
        gnome_config_set_bool ("/" APPNAME "/Preferences/Animate",  prefs.do_animate);
        gnome_config_set_bool ("/" APPNAME "/Preferences/Wipes",    prefs.do_wipes);
        gnome_config_set_bool ("/" APPNAME "/Preferences/Grids",    prefs.do_grids);
        gnome_config_set_bool ("/" APPNAME "/Preferences/Toolbar",  prefs.do_toolbar);
        gnome_config_set_bool ("/" APPNAME "/Preferences/Sound",    prefs.do_sound);
        gnome_config_set_bool ("/" APPNAME "/Preferences/Verify",   prefs.do_verify);

        gnome_config_sync ();
}



static void
prefs_dialog_update_player_selection_labels (void)
{
        if (dlg_prefs) {

                gchar *label_player1, *label_player2;

                label_player1 = g_strdup_printf (_("Player 1 : %s"), prefs.descr_player1);
                label_player2 = g_strdup_printf (_("Player 2 : %s"), prefs.descr_player2);
                gtk_label_set_text (GTK_LABEL(label_player_selection1), label_player1);
                gtk_label_set_text (GTK_LABEL(label_player_selection2), label_player2);

                g_free (label_player1);
                g_free (label_player2);

        }
}



static void
prefs_dialog_reset(void)
{
        /*
         * Make sure the dialog reflects current prefs settings.
         * Copy them in case user changes something then cancels.
         */

        DEBUG_PRINT(1, "prefs_dialog_reset\n");


        /* copy prefs */

        prefs_tmp.player1        = prefs.player1;
        prefs_tmp.player2        = prefs.player2;
        prefs_tmp.start_mode     = prefs.start_mode;
        prefs_tmp.key[KEY_LEFT]  = prefs.key[KEY_LEFT];
        prefs_tmp.key[KEY_RIGHT] = prefs.key[KEY_RIGHT];
        prefs_tmp.key[KEY_DROP]  = prefs.key[KEY_DROP];
        prefs_tmp.do_animate     = prefs.do_animate;
        prefs_tmp.do_wipes       = prefs.do_wipes;
        prefs_tmp.sound_mode     = prefs.sound_mode;

        g_free (prefs_tmp.fname_theme);
        prefs_tmp.fname_theme   = g_strdup (prefs.fname_theme);

        prefs_tmp.descr_player1 = prefs.descr_player1;
        prefs_tmp.descr_player2 = prefs.descr_player2;


        /* update dialog display */

        prefs_dialog_update_player_selection_labels ();

        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(radio_player1[prefs.player1]), TRUE);
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(radio_player2[prefs.player2]), TRUE);
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(radio_start[prefs.start_mode]), TRUE);

        gtk_option_menu_set_history (GTK_OPTION_MENU(optionmenu_theme), theme_current->id);

        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(checkbutton_animate), prefs.do_animate);
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(checkbutton_wipe), prefs.do_wipes);
        gtk_widget_set_sensitive (GTK_WIDGET(checkbutton_wipe), prefs.do_animate);

        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(radio_sound[prefs.sound_mode - 1]), TRUE);

        gtk_entry_set_text (GTK_ENTRY(entry_key_left), gdk_keyval_name (prefs.key[KEY_LEFT]));
        gtk_entry_set_text (GTK_ENTRY(entry_key_right), gdk_keyval_name (prefs.key[KEY_RIGHT]));
        gtk_entry_set_text (GTK_ENTRY(entry_key_drop), gdk_keyval_name (prefs.key[KEY_DROP]));

        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(checkbutton_verify), prefs.do_verify);


        /* flag as untouched */

        prefs_tmp.changed = FALSE;
}



static void
prefs_dialog_apply (gboolean kills_game)
{
        Theme *theme;


        DEBUG_PRINT(1, "prefs_dialog_apply\n");

        prefs.player1        = prefs_tmp.player1;
        prefs.player2        = prefs_tmp.player2;
        prefs.key[KEY_LEFT]  = prefs_tmp.key[KEY_LEFT];
        prefs.key[KEY_RIGHT] = prefs_tmp.key[KEY_RIGHT];
        prefs.key[KEY_DROP]  = prefs_tmp.key[KEY_DROP];
        prefs.start_mode     = prefs_tmp.start_mode;
        prefs.sound_mode     = prefs_tmp.sound_mode;
        prefs.do_animate     = prefs_tmp.do_animate;
        prefs.do_wipes       = prefs_tmp.do_wipes;
        prefs.do_verify      = prefs_tmp.do_verify;

        if (kills_game) {

                gnect_reset (FALSE);
                gnect_reset_display ();
                gnect_reset_scores ();

                gui_set_status_prompt_new_game (STATUS_MSG_SET);
        }


        if (strcmp (prefs_tmp.fname_theme, prefs.fname_theme)) {

                /* theme selection has changed */

                theme = theme_get_ptr_from_fname (prefs_tmp.fname_theme);

                if (!theme || !theme_load (theme)) {

                        gnome_app_warning (GNOME_APP(app), _("Error loading theme"));

                }
                else {

                        g_free (prefs.fname_theme);
                        prefs.fname_theme = g_strdup (theme->fname);


                        /* redraw player descriptions wherever they might be visible */

                        prefs_dialog_update_player_selection_labels ();

                        dialog_score_update ();

                        if (!kills_game) {

                                /* update status bar prompt */

                                if (!gnect.over) {
                                        gui_set_status_prompt (gnect.current_player);
                                }
                                else if (gnect.winner != -1) {
                                        gui_set_status_winner (gnect.winner, FALSE);
                                }

                        }

                }

        }


        /* flag prefs dialog as unchanged */

        prefs_tmp.changed = FALSE;


        /* flag to save prefs on exit */

        prefs.changed = TRUE;
}



static void
cb_prefs_verify_apply_kills_game (gint cancel, gpointer *data)
{
        if (!cancel) prefs_dialog_apply (TRUE);
}



static void
cb_prefs_dialog_apply (GtkWidget *widget, gpointer *data)
{
        gboolean kills_game;


        if ((gint)data != -1 || !prefs_tmp.changed) return;


        DEBUG_PRINT(1, "cb_prefs_dialog_apply\n");


        /* changing player selection resets the game - make sure that's okay */

        kills_game = (prefs_tmp.player1 != prefs.player1 || prefs_tmp.player2 != prefs.player2);

        if (kills_game && !gnect.over && prefs_tmp.do_verify) {

                gnome_app_ok_cancel_modal (GNOME_APP(app),
                                           _("Applying this change to Player Selection\nwill end the current game"),
                                           (GnomeReplyCallback)cb_prefs_verify_apply_kills_game,
                                           NULL);
        }
        else {

                prefs_dialog_apply (kills_game);

        }
}



static void
cb_prefs_dialog_animate_select (GtkWidget *widget, gpointer *data)
{
        prefs_tmp.do_animate = GTK_TOGGLE_BUTTON(widget)->active;
        gtk_widget_set_sensitive (GTK_WIDGET(checkbutton_wipe), prefs_tmp.do_animate);
        prefs_tmp.changed = TRUE;
}



static void
cb_prefs_dialog_wipe_select (GtkWidget *widget, gpointer *data)
{
        prefs_tmp.do_wipes = GTK_TOGGLE_BUTTON(widget)->active;
        prefs_tmp.changed = TRUE;
}



static void
cb_prefs_dialog_player1_select (GtkWidget *widget, gpointer *data)
{
        if ( (prefs_tmp.player1 = (gint)data) != prefs.player1 ) {
                prefs_tmp.changed = TRUE;
        }
}



static void
cb_prefs_dialog_player2_select (GtkWidget *widget, gpointer *data)
{
        if ( (prefs_tmp.player2 = (gint)data) != prefs.player2 ) {
                prefs_tmp.changed = TRUE;
        }
}



static void
cb_prefs_dialog_who_starts_select (GtkWidget *widget, gpointer *data)
{
        if ( (prefs_tmp.start_mode = (gint)data) != prefs.start_mode ) {
                prefs_tmp.changed = TRUE;
        }
}



static void
cb_prefs_dialog_sound_select (GtkWidget *widget, gpointer *data)
{
        if ( (prefs_tmp.sound_mode = (gint)data) != prefs.sound_mode ) {
                prefs_tmp.changed = TRUE;
        }
}



static void
cb_prefs_dialog_theme_select (GtkWidget *widget, gpointer *data)
{
        Theme *theme = (Theme*)data;


        if (strcmp (theme->fname, prefs.fname_theme) != 0) {

                prefs_tmp.fname_theme = theme->fname;

                prefs_tmp.descr_player1 = theme->descr_player1;
                prefs_tmp.descr_player2 = theme->descr_player2;

                prefs_tmp.changed = TRUE;

        }
}



static void
cb_prefs_dialog_key_select (GtkWidget *widget, GdkEventKey *data)
{
        gtk_entry_set_text (GTK_ENTRY(widget), gdk_keyval_name (data->keyval));

        if (widget == entry_key_left) {
                prefs_tmp.key[KEY_LEFT] = data->keyval;
        }
        else if (widget == entry_key_right) {
                prefs_tmp.key[KEY_RIGHT] = data->keyval;
        }
        else {
                prefs_tmp.key[KEY_DROP] = data->keyval;
        }
        prefs_tmp.changed = TRUE;
}



static void
cb_prefs_dialog_verify_select (GtkWidget *widget, gpointer *data)
{
        prefs_tmp.do_verify = GTK_TOGGLE_BUTTON(widget)->active;
        prefs_tmp.changed = TRUE;
}



static void
prefs_dialog_fill_theme_menu (void)
{
        /* add theme titles to theme selection menu */


        Theme *theme = theme_base;
        gint itemno = 0;


        while (theme) {

                GtkWidget *item;

                gchar *title = theme->title;

                item = gtk_menu_item_new_with_label (title);

                if (theme->tooltip) gui_set_tooltip (GTK_WIDGET(item), theme->tooltip);

                gtk_widget_show (item);
                gtk_menu_shell_append (GTK_MENU_SHELL (optionmenu_theme_menu), item);
                g_signal_connect (GTK_OBJECT(item), "activate", GTK_SIGNAL_FUNC(cb_prefs_dialog_theme_select), theme);

                /* select current theme */
                if (strcmp (prefs.fname_theme, theme->fname) == 0) {
                        gtk_menu_set_active (GTK_MENU (optionmenu_theme_menu), itemno);
                }

                itemno++;

                theme = theme->next;

        }
}



static void
cb_prefs_dialog_help (GtkWidget *widget, gpointer *data)
{
        gchar *fname_help = gnome_program_locate_file (NULL,
                                                       GNOME_FILE_DOMAIN_HELP,
                                                       "usage.html#prefsdialog",
                                                       FALSE, NULL);

        if (fname_help) {

                gchar *url_help;

                url_help = g_strconcat ("file:", fname_help, NULL);
                gnome_help_display_uri (url_help, NULL);
                g_free (url_help);
                g_free (fname_help);

        }
}




static gchar *
prefs_dialog_get_player_selection_label (gint n)
{
        switch (n) {
        case 0 :
                return _("Human");
                break;
        case 1 :
                return _("Non-Velena / Simple");
                break;
        case 2 :
                return _("Velena / Weak");
                break;
        case 3 :
                return _("Velena / Medium");
                break;
        case 4 :
                return _("Velena / Strong");
                break;
        }
        return "";
}



static gchar *
prefs_dialog_get_who_starts_label (gint n)
{
        switch (n) {
        case 0 :
                return _("Player 1");
                break;
        case 1 :
                return _("Player 2");
                break;
        case 2 :
                return _("Take turns");
                break;
        case 3 :
                return _("Surprise me");
                break;
        }
        return "";
}



static void
prefs_dialog_create (void)
{
        GtkWidget *action_area;
        GtkWidget *vbox1, *vbox2, *hbox1;
        GtkWidget *label;
        GtkWidget *sep;
        GtkWidget *table;
        GSList    *group_player1 = NULL;
        GSList    *group_player2 = NULL;
        GSList    *group_start   = NULL;
        GSList    *group_sound   = NULL;
        gint i;


        DEBUG_PRINT(1, "prefs_dialog_create\n");

        dlg_prefs = gtk_dialog_new_with_buttons (_("Gnect Preferences"),
                                                 GTK_WINDOW (app),
                                                 GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                 GTK_STOCK_CLOSE,
                                                 GTK_RESPONSE_CLOSE,
                                                 NULL);

        action_area = gtk_notebook_new ();
        gtk_box_pack_start (GTK_BOX(GTK_DIALOG(dlg_prefs)->vbox), action_area, TRUE, TRUE, 0);
        gtk_widget_show (action_area);

        vbox1 = gtk_vbox_new (FALSE, 0);
        gtk_widget_show (vbox1);
        gtk_container_add (GTK_CONTAINER(action_area), vbox1);
        gtk_container_set_border_width (GTK_CONTAINER(vbox1), 10);

        hbox1 = gtk_hbox_new (FALSE, 0);
        gtk_widget_show (hbox1);
        gtk_box_pack_start (GTK_BOX(vbox1), hbox1, FALSE, FALSE, 0);


        /* player 1 */

        vbox2 = gtk_vbox_new (FALSE, 0);
        gtk_widget_show (vbox2);
        gtk_box_pack_start (GTK_BOX(hbox1), vbox2, FALSE, FALSE, 0);

        label_player_selection1 = gtk_label_new (NULL);
        gtk_widget_show (label_player_selection1);
        gtk_box_pack_start (GTK_BOX(vbox2), label_player_selection1, FALSE, FALSE, 0);
        gtk_misc_set_alignment (GTK_MISC(label_player_selection1), 7.45058e-09, 0.5);
        gtk_misc_set_padding (GTK_MISC(label_player_selection1), 0, 10);

        for (i = 0; i < 5; i++) {
                radio_player1[i] = gtk_radio_button_new_with_label (group_player1, prefs_dialog_get_player_selection_label (i));
                group_player1 = gtk_radio_button_get_group (GTK_RADIO_BUTTON(radio_player1[i]));
                gtk_widget_show (radio_player1[i]);
                gtk_box_pack_start (GTK_BOX(vbox2), radio_player1[i], FALSE, FALSE, 0);
                gtk_container_set_border_width (GTK_CONTAINER(radio_player1[i]), 3);
        }

        sep = gtk_vseparator_new ();
        gtk_widget_show (sep);
        gtk_box_pack_start (GTK_BOX(hbox1), sep, FALSE, FALSE, 10);


        /* player 2 */

        vbox2 = gtk_vbox_new (FALSE, 0);
        gtk_widget_show (vbox2);
        gtk_box_pack_start (GTK_BOX(hbox1), vbox2, FALSE, FALSE, 0);

        label_player_selection2 = gtk_label_new (NULL);
        gtk_widget_show (label_player_selection2);
        gtk_box_pack_start (GTK_BOX(vbox2), label_player_selection2, FALSE, FALSE, 0);
        gtk_misc_set_alignment (GTK_MISC(label_player_selection2), 7.45058e-09, 0.5);
        gtk_misc_set_padding (GTK_MISC(label_player_selection2), 0, 10);

        for (i = 0; i < 5; i++) {
                radio_player2[i] = gtk_radio_button_new_with_label (group_player2, prefs_dialog_get_player_selection_label (i));
                group_player2 = gtk_radio_button_get_group (GTK_RADIO_BUTTON(radio_player2[i]));
                gtk_widget_show (radio_player2[i]);
                gtk_box_pack_start (GTK_BOX(vbox2), radio_player2[i], FALSE, FALSE, 0);
                gtk_container_set_border_width (GTK_CONTAINER(radio_player2[i]), 3);
        }

        sep = gtk_vseparator_new ();
        gtk_widget_show (sep);
        gtk_box_pack_start (GTK_BOX(hbox1), sep, FALSE, FALSE, 10);


        /* who starts? */

        vbox2 = gtk_vbox_new (FALSE, 0);
        gtk_widget_show (vbox2);
        gtk_box_pack_start (GTK_BOX(hbox1), vbox2, TRUE, TRUE, 0);

        label = gtk_label_new (_("Who starts?"));
        gtk_widget_show (label);
        gtk_box_pack_start (GTK_BOX(vbox2), label, FALSE, FALSE, 0);
        gtk_misc_set_alignment (GTK_MISC(label), 7.45058e-09, 0.5);
        gtk_misc_set_padding (GTK_MISC(label), 0, 10);

        for (i = 0; i < 4; i++) {
                radio_start[i] = gtk_radio_button_new_with_label (group_start, prefs_dialog_get_who_starts_label (i));
                group_start = gtk_radio_button_get_group (GTK_RADIO_BUTTON(radio_start[i]));
                gtk_widget_show (radio_start[i]);
                gtk_box_pack_start (GTK_BOX(vbox2), radio_start[i], FALSE, FALSE, 0);
                gtk_container_set_border_width (GTK_CONTAINER(radio_start[i]), 3);
        }

        sep = gtk_hseparator_new ();
        gtk_widget_show (sep);
        gtk_box_pack_start (GTK_BOX(vbox1), sep, FALSE, FALSE, 10);

        label = gtk_label_new (_("Player Selection"));
        gtk_widget_show (label);
        gtk_notebook_set_tab_label (GTK_NOTEBOOK(action_area), gtk_notebook_get_nth_page (GTK_NOTEBOOK(action_area), 0), label);
        gtk_misc_set_padding (GTK_MISC(label), 10, 0);

        hbox1 = gtk_hbox_new (FALSE, 0);
        gtk_widget_show (hbox1);
        gtk_container_add (GTK_CONTAINER(action_area), hbox1);
        gtk_container_set_border_width (GTK_CONTAINER(hbox1), 5);


        /* theme */

        vbox1 = gtk_vbox_new (FALSE, 0);
        gtk_widget_show (vbox1);
        gtk_box_pack_start (GTK_BOX(hbox1), vbox1, FALSE, FALSE, 5);
        gtk_container_set_border_width (GTK_CONTAINER(vbox1), 5);

        label = gtk_label_new (_("Theme selection:"));
        gtk_widget_show (label);
        gtk_box_pack_start (GTK_BOX(vbox1), label, FALSE, FALSE, 0);
        gtk_misc_set_alignment (GTK_MISC(label), 0, 0.5);
        gtk_misc_set_padding (GTK_MISC(label), 0, 10);

        optionmenu_theme = gtk_option_menu_new ();
        gtk_widget_show (optionmenu_theme);
        gtk_box_pack_start (GTK_BOX(vbox1), optionmenu_theme, FALSE, FALSE, 0);

        optionmenu_theme_menu = gtk_menu_new ();
        prefs_dialog_fill_theme_menu ();
        gtk_option_menu_set_menu (GTK_OPTION_MENU(optionmenu_theme), optionmenu_theme_menu);


        /* keyboard */

        label = gtk_label_new (_("Keyboard control:"));
        gtk_widget_show (label);
        gtk_box_pack_start (GTK_BOX(vbox1), label, FALSE, FALSE, 0);
        gtk_misc_set_alignment (GTK_MISC(label), 7.45058e-09, 0.5);
        gtk_misc_set_padding (GTK_MISC(label), 0, 10);

        table = gtk_table_new (3, 2, FALSE);
        gtk_widget_show (table);
        gtk_box_pack_start (GTK_BOX(vbox1), table, FALSE, FALSE, 0);
        gtk_container_set_border_width (GTK_CONTAINER(table), 5);

        label = gtk_label_new (_("Move left"));
        gtk_widget_show (label);
        gtk_table_attach (GTK_TABLE(table), label, 0, 1, 0, 1,
                          (GtkAttachOptions)(GTK_FILL),
                          (GtkAttachOptions)(0), 0, 0);
        gtk_misc_set_alignment (GTK_MISC(label), 0, 0.5);
        gtk_misc_set_padding (GTK_MISC(label), 10, 0);

        label = gtk_label_new (_("Move right"));
        gtk_widget_show (label);
        gtk_table_attach (GTK_TABLE(table), label, 0, 1, 1, 2,
                          (GtkAttachOptions)(GTK_FILL),
                          (GtkAttachOptions)(0), 0, 0);
        gtk_misc_set_alignment (GTK_MISC(label), 0, 0.5);
        gtk_misc_set_padding (GTK_MISC(label), 10, 0);

        label = gtk_label_new (_("Drop counter"));
        gtk_widget_show (label);
        gtk_table_attach (GTK_TABLE(table), label, 0, 1, 2, 3,
                          (GtkAttachOptions)(GTK_FILL),
                          (GtkAttachOptions)(0), 0, 0);
        gtk_misc_set_alignment (GTK_MISC(label), 0, 0.5);
        gtk_misc_set_padding (GTK_MISC(label), 10, 0);

        entry_key_left = gtk_entry_new ();
        gtk_widget_show (entry_key_left);
        gtk_table_attach (GTK_TABLE(table), entry_key_left, 1, 2, 0, 1,
                          (GtkAttachOptions)(0),
                          (GtkAttachOptions)(0), 0, 0);

        entry_key_right = gtk_entry_new ();
        gtk_widget_show (entry_key_right);
        gtk_table_attach (GTK_TABLE(table), entry_key_right, 1, 2, 1, 2,
                          (GtkAttachOptions)(0),
                          (GtkAttachOptions)(0), 0, 0);

        entry_key_drop = gtk_entry_new ();
        gtk_widget_show (entry_key_drop);
        gtk_table_attach (GTK_TABLE(table), entry_key_drop, 1, 2, 2, 3,
                          (GtkAttachOptions)(0),
                          (GtkAttachOptions)(0), 0, 0);

        sep = gtk_vseparator_new ();
        gtk_widget_show (sep);
        gtk_box_pack_start (GTK_BOX(hbox1), sep, FALSE, FALSE, 5);


        /* animation */

        vbox1 = gtk_vbox_new (FALSE, 0);
        gtk_widget_show (vbox1);
        gtk_box_pack_start (GTK_BOX(hbox1), vbox1, TRUE, TRUE, 5);
        gtk_container_set_border_width (GTK_CONTAINER(vbox1), 5);

        label = gtk_label_new (_("Animation:"));
        gtk_widget_show (label);
        gtk_box_pack_start (GTK_BOX(vbox1), label, FALSE, FALSE, 0);
        gtk_misc_set_alignment (GTK_MISC(label), 7.45058e-09, 0.5);

        checkbutton_animate = gtk_check_button_new_with_label (_("Yes please!"));
        gtk_widget_show (checkbutton_animate);
        gtk_box_pack_start (GTK_BOX(vbox1), checkbutton_animate, FALSE, FALSE, 0);

        checkbutton_wipe = gtk_check_button_new_with_label (_("Between games, too"));
        gtk_widget_show (checkbutton_wipe);
        gtk_box_pack_start (GTK_BOX(vbox1), checkbutton_wipe, FALSE, FALSE, 0);

        sep = gtk_hseparator_new ();
        gtk_widget_show (sep);
        gtk_box_pack_start (GTK_BOX(vbox1), sep, TRUE, FALSE, 5);


        /* sound */

        label = gtk_label_new (_("Sound type:"));
        gtk_widget_show (label);
        gtk_box_pack_start (GTK_BOX(vbox1), label, FALSE, FALSE, 0);
        gtk_misc_set_alignment (GTK_MISC(label), 7.45058e-09, 0.5);

        radio_sound[0] = gtk_radio_button_new_with_label (group_sound, _("Speaker beep"));
        group_sound = gtk_radio_button_get_group (GTK_RADIO_BUTTON(radio_sound[0]));
        gtk_widget_show (radio_sound[0]);
        gtk_box_pack_start (GTK_BOX(vbox1), radio_sound[0], FALSE, FALSE, 0);

        radio_sound[1] = gtk_radio_button_new_with_label (group_sound, _("GNOME sound"));
        group_sound = gtk_radio_button_get_group (GTK_RADIO_BUTTON(radio_sound[1]));
        gtk_widget_show (radio_sound[1]);
        gui_set_tooltip (GTK_WIDGET(radio_sound[1]), _("This can be set up using the GNOME Control Center"));
        gtk_box_pack_start (GTK_BOX(vbox1), radio_sound[1], FALSE, FALSE, 0);

        sep = gtk_hseparator_new ();
        gtk_widget_show (sep);
        gtk_box_pack_start (GTK_BOX(vbox1), sep, TRUE, FALSE, 5);


        /* verify */

        label = gtk_label_new (_("If stopping an unfinished game:"));
        gtk_widget_show (label);
        gtk_box_pack_start (GTK_BOX(vbox1), label, FALSE, FALSE, 0);
        gtk_misc_set_alignment (GTK_MISC(label), 7.45058e-09, 0.5);

        checkbutton_verify = gtk_check_button_new_with_label (_("Ask me first"));
        gtk_widget_show (checkbutton_verify);
        gtk_box_pack_start (GTK_BOX(vbox1), checkbutton_verify, FALSE, FALSE, 0);

        label = gtk_label_new (_("Appearance and Behaviour"));
        gtk_widget_show (label);
        gtk_notebook_set_tab_label (GTK_NOTEBOOK(action_area), gtk_notebook_get_nth_page (GTK_NOTEBOOK(action_area), 1), label);
        gtk_misc_set_padding (GTK_MISC(label), 10, 0);



        g_signal_connect (dlg_prefs, "response", G_CALLBACK(cb_prefs_response), &dlg_prefs);
        for (i = 0; i < 5; i++) {
                g_signal_connect (GTK_OBJECT(radio_player1[i]), "toggled", GTK_SIGNAL_FUNC(cb_prefs_dialog_player1_select),(gpointer)i);
                g_signal_connect (GTK_OBJECT(radio_player2[i]), "toggled", GTK_SIGNAL_FUNC(cb_prefs_dialog_player2_select),(gpointer)i);
        }
        for (i = 0; i < 4; i++) {
                g_signal_connect (GTK_OBJECT(radio_start[i]), "toggled", GTK_SIGNAL_FUNC(cb_prefs_dialog_who_starts_select),(gpointer)i);
        }
        g_signal_connect (GTK_OBJECT(radio_sound[0]), "toggled", GTK_SIGNAL_FUNC(cb_prefs_dialog_sound_select),(gpointer)SOUND_MODE_BEEP);
        g_signal_connect (GTK_OBJECT(radio_sound[1]), "toggled", GTK_SIGNAL_FUNC(cb_prefs_dialog_sound_select),(gpointer)SOUND_MODE_PLAY);
        g_signal_connect (GTK_OBJECT(checkbutton_animate), "toggled", GTK_SIGNAL_FUNC(cb_prefs_dialog_animate_select), NULL);
        g_signal_connect (GTK_OBJECT(checkbutton_wipe), "toggled", GTK_SIGNAL_FUNC(cb_prefs_dialog_wipe_select), NULL);
        g_signal_connect (GTK_OBJECT(entry_key_left), "key_press_event", GTK_SIGNAL_FUNC(cb_prefs_dialog_key_select), NULL);
        g_signal_connect (GTK_OBJECT(entry_key_right), "key_press_event", GTK_SIGNAL_FUNC(cb_prefs_dialog_key_select), NULL);
        g_signal_connect (GTK_OBJECT(entry_key_drop), "key_press_event", GTK_SIGNAL_FUNC(cb_prefs_dialog_key_select), NULL);
        g_signal_connect (GTK_OBJECT(checkbutton_verify), "toggled", GTK_SIGNAL_FUNC(cb_prefs_dialog_verify_select), NULL);


        /* fill with current prefs settings */
        prefs_dialog_reset ();
}



void
prefs_dialog (void)
{
        if (!dlg_prefs) {

                /* build and show it */
                prefs_dialog_create ();
                gtk_widget_show (dlg_prefs);

        }
        else {

                /* make sure it's visible */
                prefs_dialog_reset ();
                gtk_widget_show (dlg_prefs);
                gdk_window_show (dlg_prefs->window);
                gdk_window_raise (dlg_prefs->window);

        }
}

