//    Copyright (C) 2012 Dirk Vanden Boer <dirk.vdb@gmail.com>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#ifndef UTILS_TRACE_H
#define UTILS_TRACE_H

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#ifdef ENABLE_TRACE
	#include <glib.h>
	#include <glib/gprintf.h>
	#include <cstdarg>
	#include <unistd.h>
#endif

namespace utils
{
    inline void trace(const char *format, ...)
    {
	#ifdef ENABLE_TRACE
	    va_list args;
	    char* formatted;
	    char* str;

	    va_start(args, format);
	    formatted = g_strdup_vprintf (format, args);
	    va_end(args);

	    str = g_strdup_printf ("MARK: %s: %s", g_get_prgname(), formatted);
	    g_free (formatted);

	    access(str, F_OK);
	    g_free(str);
	#endif
	}
}

#endif
