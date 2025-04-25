# Author: Assen Totin <assen.totin@gmail.com>
#
# This software is released under GNU General Public License.
# See the LICENSE file for details or visit http://www.gnu.org/copyleft/gpl.html 
# for details.

APPLET_FLAGS = @APPLET_FLAGS@
APPLET_LIBS = @APPLET_LIBS@

AM_CPPFLAGS = @APPLET_FLAGS@
LDADD = @APPLET_LIBS@

libexec_PROGRAMS = uptime_applet 

uptime_applet_SOURCES = main.c menu.c applet.h 

appletdir = $(datadir)/mate-panel/applets
applet_DATA = org.mate.applets.UptimeApplet.mate-panel-applet

servicedir = $(datadir)/dbus-1/services
service_DATA = org.mate.panel.applet.UptimeApplet.service

gsettingsdir = $(datadir)/glib-2.0/schemas
gsettings_DATA = org.mate.panel.applet.UptimeApplet.gschema.xml

install-data-hook:
	glib-compile-schemas $(DESTDIR)$(datadir)/glib-2.0/schemas 2>/dev/null

