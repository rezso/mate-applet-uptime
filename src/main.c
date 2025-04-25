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
 *  Uptime applet for MATE by Assen Totin <assen.totin@gmail.com>
 *
 */

#include "applet.h"
#include "../config.h"

/* Free the memory of the applet struct
 * Save the current state of the applet
 */
void applet_destroy(MatePanelApplet *applet_widget, UptimeApplet *applet) {
	g_assert(applet);
	g_free(applet);
	return;
}


void applet_back_change (MatePanelApplet *a, MatePanelAppletBackgroundType type, GdkRGBA *color, cairo_pattern_t *pattern, UptimeApplet *applet) {
	// Use MATE-provided wrapper to change the background (same for both GTK2 and GTK3)
	mate_panel_applet_set_background_widget (a, GTK_WIDGET(applet->applet));
	//mate_panel_applet_set_background_widget (a, GTK_WIDGET(applet->event_box));
}


/*
 * Get the uptime
 */

void get_uptime(UptimeApplet *applet) {
	FILE *fp = fopen(APPLET_PROC_UPTIME, "r");
	if (!fp)
		return;
	char tmp1[128];
	fgets(&tmp1[0], sizeof(tmp1), fp);
	fclose(fp);
	
	char *tmp2 = strtok(&tmp1[0], " ");
	char *tmp3 = strtok(tmp2, ".");
	applet->uptime = atoi(tmp3);
}


/* 
 * Format uptime
 */
void format_uptime(UptimeApplet *applet, char *s, char *t) {
	int d = applet->uptime / 86400;
	int h = (applet->uptime - d * 86400) / 3600;
	int m = (applet->uptime - (d * 86400) - (h * 3600)) / 60;

	switch(applet->format) {
		case FORMAT_UPPERCASE :
			if (d > 0)
				sprintf(s, "%s%uD%02uH%s", "<span font_desc=\"10.0\">", d, h, "</span>");
			else
				sprintf(s, "%s%02uH%02uM%s", "<span font_desc=\"10.0\">", h, m, "</span>");
			break;
		case FORMAT_LOWERCASE :
			if (d > 0)
				sprintf(s, "%s%ud%02uh%s", "<span font_desc=\"10.0\">", d, h, "</span>");
			else
				sprintf(s, "%s%02uh%02um%s", "<span font_desc=\"10.0\">", h, m, "</span>");
			break;
		case FORMAT_CLOCK :
			sprintf(s, "%s%02u.%02u:%02u%s", "<span font_desc=\"10.0\">", d, h, m, "</span>");
  			break;
		default :
			// Same as FORMAT_UPPERCASE
			if (d > 0)
				sprintf(s, "%s%uD%02uH%s", "<span font_desc=\"10.0\">", d, h, "</span>");
			else
				sprintf(s, "%s%02uH%02uM%s", "<span font_desc=\"10.0\">", h, m, "</span>");
	}

	sprintf(t, "%s %u %s, %u %s, %u %s", _("System uptime:"), d, _("days"), h, _("hours"), m, _("minutes"));
}

/*
 * Update the applet
 */
int applet_check_uptime(UptimeApplet *applet) {
	get_uptime(applet);
	char uptime[128], tooltip[1024]; 

	format_uptime(applet, &uptime[0], &tooltip[0]);
	gtk_label_set_markup(GTK_LABEL(applet->label_bottom), &uptime[0]);
	gtk_widget_set_tooltip_text (GTK_WIDGET (applet->applet), &tooltip[0]);
	
	return TRUE;
}

/* The "main" function
 */
static gboolean uptime_applet_factory(MatePanelApplet *applet_widget, const gchar *iid, gpointer data) {
	char *buf;
	UptimeApplet *applet;
	GdkPixbuf *icon;
	GList *iconlist = NULL;

	if (strcmp (iid, APPLET_ID) != 0) 
		return FALSE;

	// i18n
	setlocale (LC_ALL, "");
	bindtextdomain (PACKAGE_NAME, LOCALEDIR);
	bind_textdomain_codeset(PACKAGE_NAME, "utf-8");
	textdomain (PACKAGE_NAME);

	/* Set an icon for all windows */
/*
	char image_file[1024];
	sprintf(&image_file[0], "%s/%s", APPLET_ICON_PATH, APPLET_ICON);
	icon = gdk_pixbuf_new_from_file(&image_file[0], NULL);
	iconlist = g_list_append(NULL, (gpointer)icon);
	gtk_window_set_default_icon_list(iconlist);
	g_object_unref(icon);
*/	
	applet = g_malloc0(sizeof(UptimeApplet));
	applet->applet = applet_widget;

	applet->gsettings = g_settings_new_with_path(APPLET_GSETTINGS_SCHEMA, APPLET_GSETTINGS_PATH);
	applet->format = g_settings_get_int(applet->gsettings, APPLET_GSETTINGS_KEY_FORMAT);
	
	applet->vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);

	char msg_top[128];
	sprintf(&msg_top[0], "%s%s%s", "<span font_desc=\"8.0\">", _("uptime"), "</span>");
	applet->label_top = gtk_label_new("");
	gtk_label_set_markup(GTK_LABEL(applet->label_top), &msg_top[0]);

	char msg_bottom[128], tooltip[1024];
	get_uptime(applet);
	format_uptime(applet, &msg_bottom[0], &tooltip[0]);
	applet->label_bottom = gtk_label_new("");
	gtk_label_set_markup(GTK_LABEL(applet->label_bottom), &msg_bottom[0]);
	gtk_widget_set_tooltip_text (GTK_WIDGET (applet->applet), &tooltip[0]);

	gtk_box_pack_start(GTK_BOX(applet->vbox), applet->label_top, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(applet->vbox), applet->label_bottom, FALSE, FALSE, 0);
	
	gtk_container_add(GTK_CONTAINER(applet_widget), applet->vbox);

	GtkActionGroup *action_group = gtk_action_group_new ("Uptime Applet Actions");
	gtk_action_group_set_translation_domain(action_group, PACKAGE_NAME);
	gtk_action_group_add_actions (action_group, applet_menu_actions, G_N_ELEMENTS (applet_menu_actions), applet);
	mate_panel_applet_setup_menu(applet->applet, ui, action_group);

	gtk_widget_show_all(GTK_WIDGET(applet_widget));

	g_signal_connect(G_OBJECT(applet_widget), "destroy", G_CALLBACK(applet_destroy), (gpointer)applet);
	g_signal_connect(G_OBJECT(applet_widget), "change_background", G_CALLBACK (applet_back_change), (gpointer)applet);

	g_timeout_add(60000, (GSourceFunc) applet_check_uptime, (gpointer)applet);

	return TRUE;
}

MATE_PANEL_APPLET_OUT_PROCESS_FACTORY (APPLET_FACTORY, PANEL_TYPE_APPLET, APPLET_NAME, uptime_applet_factory, NULL)



