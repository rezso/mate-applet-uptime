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
 *  Uptime Applet was writen by Assen Totin <assen.totin@gmail.com>
 *  
 */

#include <mate-panel-applet.h>
#include <libintl.h>
#include <string.h>
#include <stdlib.h>
#include <glib/gi18n.h>

#define APPLET_FACTORY "UptimeAppletFactory"
#define APPLET_ID "UptimeApplet"
#define APPLET_NAME "uptime"
#define APPLET_ICON "applet_uptime.png"
#define APPLET_VERSION "1"

#define APPLET_PROC_UPTIME "/proc/uptime"

// GSettings
#define APPLET_GSETTINGS_SCHEMA "org.mate.panel.applet.UptimeApplet"
#define APPLET_GSETTINGS_PATH "/org/mate/panel/objects/uptime/"
#define APPLET_GSETTINGS_KEY_FORMAT "format"

enum {
	FORMAT_UPPERCASE = 0,
	FORMAT_LOWERCASE,
	FORMAT_CLOCK,
	NUM_FORMATS
};

typedef struct {
	MatePanelApplet *applet;
	//GtkWidget *event_box;
	GtkWidget *vbox, *label_top, *label_bottom;
	int uptime, format;
	GtkDialog *about, *settings;
	GSettings *gsettings;
} UptimeApplet;

void about_cb (GtkAction *, UptimeApplet *);
void settings_cb (GtkAction *, UptimeApplet *);

static const GtkActionEntry applet_menu_actions [] = {
	{ "Properties", N_("Properties"), N_("Settings"), NULL, NULL, G_CALLBACK (settings_cb) },
	{ "About", N_("About"), N_("About"), NULL, NULL, G_CALLBACK (about_cb) }
};

static char *ui  =
"<menuitem name='Item 1' action='Properties' />"
"<menuitem name='Item 2' action='About' />"
;

