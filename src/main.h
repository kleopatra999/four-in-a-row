/*
 * gnect main.h
 *
 */



#ifndef _GNECT_MAIN_H_
#define _GNECT_MAIN_H_


#include <gnome.h>



/* #define GNECT_DEBUG */


#define APPNAME                 "gnect"

#ifdef G_HAVE_ISO_VARARGS
#ifdef GNECT_DEBUG
#define DEBUG_PRINT(level, ...)  if (debugging & level) g_printerr(APPNAME ": " __VA_ARGS__)
#else
#define DEBUG_PRINT(level, ...)
#endif

#define ERROR_PRINT(...)    g_printerr(APPNAME ": error: " __VA_ARGS__)
#define WARNING_PRINT(...)  g_printerr(APPNAME ": warning: " __VA_ARGS__)

#elif defined(G_HAVE_GNUC_VARARGS)

#ifdef GNECT_DEBUG
#define DEBUG_PRINT(level, args...)  if (debugging & level) g_printerr(APPNAME ": " args)
#else
#ifdef G_HAVE_GNUC_VARARGS
#define DEBUG_PRINT(level, args...)
#elif defined(G_HAVE_ISO_VARARGS)
#define DEBUG_PRINT(level, ...)
#else
static inline void DEBUG_PRINT(...) { }
#endif
#endif

#define ERROR_PRINT	g_error
#define WARNING_PRINT	g_warning
#endif

#define BASE_PIXMAP_DIR         GNECT_DATA_DIR "pixmaps" G_DIR_SEPARATOR_S
#define PACKAGE_PIXMAP_DIR      BASE_PIXMAP_DIR APPNAME  G_DIR_SEPARATOR_S
#define PACKAGE_THEME_DIR       GNECT_DATA_DIR  APPNAME  G_DIR_SEPARATOR_S

#define FNAME_GNECT_ICON        BASE_PIXMAP_DIR    "gnect-icon.png"
#define FNAME_GNECT_LOGO        FNAME_GNECT_ICON /* PACKAGE_PIXMAP_DIR "gnect-about.png" */

#define GNECT_ICON_WIDTH        48
#define GNECT_ICON_HEIGHT       48

#define GNECT_LOGO_WIDTH        400
#define GNECT_LOGO_HEIGHT       64

#define PLAYER_1                0
#define PLAYER_2                1
#define DRAWN_GAME              2

#define PLAYER_HUMAN            0
#define PLAYER_GNECT            1
#define PLAYER_VELENA_WEAK      2
#define PLAYER_VELENA_MEDIUM    3
#define PLAYER_VELENA_STRONG    4

#define TILE_PLAYER_1           0
#define TILE_PLAYER_2           1
#define TILE_CLEAR              2
#define TILE_CLEAR_CURSOR       3
#define TILE_PLAYER_1_CURSOR    4
#define TILE_PLAYER_2_CURSOR    5



#endif /* _GNECT_MAIN_H_ */
