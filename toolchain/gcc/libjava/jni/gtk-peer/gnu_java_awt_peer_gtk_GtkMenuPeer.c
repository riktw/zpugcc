/* gtkmenupeer.c -- Native implementation of GtkMenuPeer
   Copyright (C) 1999 Free Software Foundation, Inc.

This file is part of GNU Classpath.

GNU Classpath is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU Classpath is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Classpath; see the file COPYING.  If not, write to the
Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
02111-1307 USA.

Linking this library statically or dynamically with other modules is
making a combined work based on this library.  Thus, the terms and
conditions of the GNU General Public License cover the whole
combination.

As a special exception, the copyright holders of this library give you
permission to link this library with independent modules to produce an
executable, regardless of the license terms of these independent
modules, and to copy and distribute the resulting executable under
terms of your choice, provided that you also meet, for each linked
independent module, the terms and conditions of the license of that
module.  An independent module is a module which is not derived from
or based on this library.  If you modify this library, you may extend
this exception to your version of the library, but you are not
obligated to do so.  If you do not wish to do so, delete this
exception statement from your version. */


#include "gtkpeer.h"
#include "gnu_java_awt_peer_gtk_GtkMenuPeer.h"

static void
accel_attach (GtkMenuItem *menu_item,
	      gpointer *user_data __attribute__((unused)))
{
  GtkAccelGroup *accel;

  accel = gtk_menu_get_accel_group (GTK_MENU (menu_item->submenu));
  _gtk_accel_group_attach (accel, 
    G_OBJECT (gtk_widget_get_toplevel (GTK_WIDGET(menu_item))));
}

JNIEXPORT void JNICALL Java_gnu_java_awt_peer_gtk_GtkMenuPeer_setupAccelGroup
  (JNIEnv *env, jobject obj, jobject parent)
{
  void *ptr1, *ptr2;

  ptr1 = NSA_GET_PTR (env, obj);

  gdk_threads_enter ();
  if (!parent)
    {
      gtk_menu_set_accel_group (GTK_MENU (GTK_MENU_ITEM (ptr1)->submenu), 
				gtk_accel_group_new ());

      if (GTK_WIDGET_REALIZED (GTK_WIDGET (ptr1)))
	accel_attach (GTK_MENU_ITEM (ptr1), NULL);
      else
	g_signal_connect (G_OBJECT (ptr1),
			    "realize",
			    GTK_SIGNAL_FUNC (accel_attach), 
			    NULL);
    }
  else
    {
      GtkAccelGroup *parent_accel;

      ptr2 = NSA_GET_PTR (env, parent);
      parent_accel = gtk_menu_get_accel_group (GTK_MENU (GTK_MENU_ITEM (ptr2)->submenu));
      
      gtk_menu_set_accel_group (GTK_MENU (GTK_MENU_ITEM (ptr1)->submenu),
				parent_accel);
    }
      
  gdk_threads_leave ();
}


JNIEXPORT void JNICALL Java_gnu_java_awt_peer_gtk_GtkMenuPeer_create
  (JNIEnv *env, jobject obj, jstring label)
{
  GtkWidget *menu_title, *menu;
  const char *str;

  /* Create global reference and save it for future use */
  NSA_SET_GLOBAL_REF (env, obj);

  str = (*env)->GetStringUTFChars (env, label, NULL);

  gdk_threads_enter ();
  
  menu = gtk_menu_new ();
  
  menu_title = gtk_menu_item_new_with_label (str);
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (menu_title), menu);

  gtk_widget_show (menu);
  gtk_widget_show (menu_title);

  NSA_SET_PTR (env, obj, menu_title);

  gdk_threads_leave ();

  (*env)->ReleaseStringUTFChars (env, label, str);
}

JNIEXPORT void JNICALL Java_gnu_java_awt_peer_gtk_GtkMenuPeer_addItem
  (JNIEnv *env, jobject obj, jobject menuitempeer, jint key, jboolean shift)
{
  void *ptr1, *ptr2;
  GtkMenu *menu;

  ptr1 = NSA_GET_PTR (env, obj);
  ptr2 = NSA_GET_PTR (env, menuitempeer);

  gdk_threads_enter ();

  menu = GTK_MENU (GTK_MENU_ITEM (ptr1)->submenu);
  gtk_menu_append (menu, GTK_WIDGET (ptr2));

  if (key)
    {
      gtk_widget_add_accelerator (GTK_WIDGET (ptr2), "activate",
				  gtk_menu_get_accel_group (menu), key, 
				  (GDK_CONTROL_MASK
				   | ((shift) ? GDK_SHIFT_MASK : 0)), 
				  GTK_ACCEL_VISIBLE);
    }

  gdk_threads_leave ();
}

JNIEXPORT void JNICALL Java_gnu_java_awt_peer_gtk_GtkMenuPeer_delItem
  (JNIEnv *env, jobject obj, jint index)
{
  void *ptr;
  GList *list;

  ptr = NSA_GET_PTR (env, obj);

  gdk_threads_enter ();
  list = gtk_container_children (GTK_CONTAINER (ptr));
  list = g_list_nth (list, index);
  gtk_container_remove (GTK_CONTAINER (ptr), GTK_WIDGET (list->data));
  gdk_threads_leave ();
}


