/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 *  USA.
 *
 *  MATE Uptime applet written by Assen Totin <assen.totin@gmail.com>
 *  
 */

#include "applet.h"
#include "../config.h"


void quitDialogOK( GtkWidget *widget, gpointer data ){
	GtkWidget *quitDialog = data;
	gtk_widget_destroy(quitDialog);
}


void about_cb (GtkAction *action, UptimeApplet *applet) {
	GtkWidget *about = gtk_about_dialog_new();

	gtk_about_dialog_set_program_name (GTK_ABOUT_DIALOG(about), _("Uptime Applet"));

	gtk_about_dialog_set_version (GTK_ABOUT_DIALOG(about), VERSION);

	gtk_about_dialog_set_copyright (GTK_ABOUT_DIALOG(about), "Copyleft 2014-2017. See License for details.");

	const gchar *authors[2];
	authors[0] = "Assen Totin <assen.totin@gmail.com>";
	authors[1] = NULL;
	gtk_about_dialog_set_authors (GTK_ABOUT_DIALOG(about),  &authors[0]);

	gtk_about_dialog_set_translator_credits(GTK_ABOUT_DIALOG(about), _("translator-credits"));

	gtk_about_dialog_set_comments (GTK_ABOUT_DIALOG(about), _("An applet that shows the system uptime"));

	gtk_about_dialog_set_website (GTK_ABOUT_DIALOG(about), "http://www.zavedil.com/software-uptime-applet");
	gtk_about_dialog_set_website_label (GTK_ABOUT_DIALOG(about), _("Home page"));

	char image_file[1024];
	snprintf(&image_file[0], 1023, "%s/%s", APPLET_ICON_PATH, "applet_uptime.32.png");
	gtk_about_dialog_set_logo (GTK_ABOUT_DIALOG(about), gtk_image_get_pixbuf(GTK_IMAGE(gtk_image_new_from_file (image_file))));

	gtk_about_dialog_set_license_type (GTK_ABOUT_DIALOG(about), GTK_LICENSE_GPL_2_0);

	gtk_dialog_run (GTK_DIALOG(about));
	gtk_widget_destroy(about);
}


void settings_cb_action(GtkWidget *widget, gpointer data) {
        UptimeApplet *applet = data;
	int format = FORMAT_UPPERCASE;

        gchar *format_text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget));

	if (strstr(format_text, "D"))
		format = FORMAT_UPPERCASE;
	else if (strstr(format_text, "d"))
		format = FORMAT_LOWERCASE;
	else if (strstr(format_text, ":"))
		format = FORMAT_CLOCK;

	applet->format = format;
	g_settings_set_int(applet->gsettings, APPLET_GSETTINGS_KEY_FORMAT, format);

        free(format_text);
}


void settings_cb (GtkAction *action, UptimeApplet *applet) {
	int i;

	// Prepare Feed tab
	GtkWidget *format_label = gtk_label_new(_("Choose output format"));

	GtkWidget *format_combo = gtk_combo_box_text_new();

	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(format_combo), "04D13H");
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(format_combo), "04d13h");
	gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(format_combo), "04.13:53");
	for (i=0; i<NUM_FORMATS; i++) {
		if (applet->format == i) {
			gtk_combo_box_set_active(GTK_COMBO_BOX(format_combo), i);
			break;
		}
	}

	g_signal_connect (G_OBJECT(format_combo), "changed", G_CALLBACK (settings_cb_action), (gpointer) applet);

	GtkWidget *format_vbox_1;
	format_vbox_1 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);

	gtk_box_pack_start(GTK_BOX(format_vbox_1), format_label, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(format_vbox_1), format_combo, FALSE, FALSE, 0);

	// Assemble window
	GtkWidget *quitDialog = gtk_dialog_new_with_buttons (_("Uptime Applet Settings"), GTK_WINDOW(applet), GTK_DIALOG_MODAL, NULL);
	GtkWidget *buttonClose = gtk_dialog_add_button (GTK_DIALOG(quitDialog), GTK_STOCK_CLOSE, GTK_RESPONSE_CANCEL);

	gtk_dialog_set_default_response (GTK_DIALOG (quitDialog), GTK_RESPONSE_CANCEL);

	gtk_container_add (GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(quitDialog))), format_vbox_1);

	g_signal_connect (G_OBJECT(buttonClose), "clicked", G_CALLBACK (quitDialogOK), (gpointer) quitDialog);

	gtk_widget_show_all(GTK_WIDGET(quitDialog));
}

