/*
 * Common output functions for the fvdetools
 *
 * Copyright (C) 2011-2014, Omar Choudary <choudary.omar@gmail.com>
 *                          Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <file_stream.h>
#include <memory.h>
#include <types.h>

#if defined( HAVE_LOCAL_LIBUNA )
#include <libuna_definitions.h>
#elif defined( HAVE_LIBUNA_H )
#include <libuna.h>
#endif

#include "fvdeoutput.h"
#include "fvdetools_libbfio.h"
#include "fvdetools_libcerror.h"
#include "fvdetools_libclocale.h"
#include "fvdetools_libcnotify.h"
#include "fvdetools_libcstring.h"
#include "fvdetools_libcsystem.h"
#include "fvdetools_libfvde.h"

/* Prints the copyright information
 */
void fvdeoutput_copyright_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf(
	 stream,
	 "Copyright (c) 2011-2014, Omar Choudary <choudary.omar@gmail.com>\n"
	 "                         Joachim Metz <%s>.\n"
	 "This is free software; see the source for copying conditions. There is NO\n"
	 "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n",
	 PACKAGE_BUGREPORT );
}

/* Prints the version information
 */
void fvdeoutput_version_fprint(
      FILE *stream,
      const char *program )
{
	if( stream == NULL )
	{
		return;
	}
	if( program == NULL )
	{
		return;
	}
        fprintf(
	 stream,
	 "%s %s\n\n",
         program,
	 LIBFVDE_VERSION_STRING );
}

/* Prints the detailed version information
 */
void fvdeoutput_version_detailed_fprint(
      FILE *stream,
      const char *program )
{
	if( stream == NULL )
	{
		return;
	}
	if( program == NULL )
	{
		return;
	}
        fprintf(
	 stream,
	 "%s %s (libfvde %s",
         program,
	 LIBFVDE_VERSION_STRING,
	 LIBFVDE_VERSION_STRING );

	fprintf(
	 stream,
	 ", libuna %s",
	 LIBUNA_VERSION_STRING );

	fprintf(
	 stream,
	 ", libbfio %s",
	 LIBBFIO_VERSION_STRING );

        fprintf(
	 stream,
	 ")\n\n" );
}

