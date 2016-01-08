/*
 * Decrypts a EncryptedRoot.plist.wipekey file
 *
 * Copyright (C) 2011-2016, Omar Choudary <choudary.omar@gmail.com>
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

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fvdeoutput.h"
#include "fvdetools_libcerror.h"
#include "fvdetools_libclocale.h"
#include "fvdetools_libcnotify.h"
#include "fvdetools_libcstring.h"
#include "fvdetools_libcsystem.h"
#include "fvdetools_libfvde.h"
#include "wipekey_handle.h"

wipekey_handle_t *fvdewipekey_wipekey_handle = NULL;
int fvdewipekey_abort                        = 0;

/* Prints the executable usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use fvdewipkey to decrypt an EncryptedRoot.plist.wipekey file\n\n" );

	fprintf( stream, "Usage: fvdewipeky filename volume_key_data [ -hvV ]\n" );

	fprintf( stream, "\tfilename:        the name of the EncryptedRoot.plist.wipekey file\n" );
	fprintf( stream, "\tvolume_key_data: the volume key data formatted in base16\n\n" );

	fprintf( stream, "\t-h:     shows this help\n" );
	fprintf( stream, "\t-v:     verbose output to stderr\n" );
	fprintf( stream, "\t-V:     print version\n" );
}

/* Signal handler for fvdewipekey
 */
void fvdewipekey_signal_handler(
      libcsystem_signal_t signal LIBCSYSTEM_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function   = "fvdewipekey_signal_handler";

	LIBCSYSTEM_UNREFERENCED_PARAMETER( signal )

	fvdewipekey_abort = 1;

	if( fvdewipekey_wipekey_handle != NULL )
	{
		if( wipekey_handle_signal_abort(
		     fvdewipekey_wipekey_handle,
		     &error ) != 1 )
		{
			libcnotify_printf(
			 "%s: unable to signal wipekey handle to abort.\n",
			 function );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
	}
	/* Force stdin to close otherwise any function reading it will remain blocked
	 */
	if( libcsystem_file_io_close(
	     0 ) != 0 )
	{
		libcnotify_printf(
		 "%s: unable to close stdin.\n",
		 function );
	}
}

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libfvde_error_t *error                                       = NULL;
	libcstring_system_character_t *encrypted_root_plist_filename = NULL;
	libcstring_system_character_t *volume_key_data               = NULL;
	char *program                                                = "fvdewipekey";
	libcstring_system_integer_t option                           = 0;
	int result                                                   = 0;
	int verbose                                                  = 0;

	libcnotify_stream_set(
	 stderr,
	 NULL );
	libcnotify_verbose_set(
	 1 );

	if( libclocale_initialize(
             "fvdetools",
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize locale values.\n" );

		goto on_error;
	}
        if( libcsystem_initialize(
             _IONBF,
             &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize system values.\n" );

		goto on_error;
	}
	fvdeoutput_version_fprint(
	 stdout,
	 program );

	while( ( option = libcsystem_getopt(
	                   argc,
	                   argv,
	                   _LIBCSTRING_SYSTEM_STRING( "hvV" ) ) ) != (libcstring_system_integer_t) -1 )
	{
		switch( option )
		{
			case (libcstring_system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_LIBCSTRING_SYSTEM "\n",
				 argv[ optind - 1 ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (libcstring_system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (libcstring_system_integer_t) 'v':
				verbose = 1;

				break;

			case (libcstring_system_integer_t) 'V':
				fvdeoutput_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing name of the EncryptedRoot.plist.wipekey file.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	encrypted_root_plist_filename = argv[ optind++ ];

	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing volume key data.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	volume_key_data = argv[ optind ];

	libcnotify_verbose_set(
	 verbose );
	libfvde_notify_set_stream(
	 stderr,
	 NULL );
	libfvde_notify_set_verbose(
	 verbose );

	if( wipekey_handle_initialize(
	     &fvdewipekey_wipekey_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize wipekey handle.\n" );

		goto on_error;
	}
	if( wipekey_handle_set_volume_key_data(
	     fvdewipekey_wipekey_handle,
	     volume_key_data,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to set volume key data.\n" );

		goto on_error;
	}
	result = wipekey_handle_open_input(
	          fvdewipekey_wipekey_handle,
	          encrypted_root_plist_filename,
	          &error );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to open: %" PRIs_LIBCSTRING_SYSTEM ".\n",
		 encrypted_root_plist_filename );

		goto on_error;
	}
	else if( result == 0 )
	{
		fprintf(
		 stderr,
		 "Unable to unlock EncryptedRoot.plist.wipekey.\n" );

		goto on_error;
	}
	if( wipekey_handle_wipekey_fprint(
	     fvdewipekey_wipekey_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to print decrypted EncryptedRoot.plist.wipekey.\n" );

		goto on_error;
	}
	if( wipekey_handle_close_input(
	     fvdewipekey_wipekey_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close wipekey handle.\n" );

		goto on_error;
	}
	if( wipekey_handle_free(
	     &fvdewipekey_wipekey_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free wipekey handle.\n" );

		goto on_error;
	}
	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( fvdewipekey_wipekey_handle != NULL )
	{
		wipekey_handle_free(
		 &fvdewipekey_wipekey_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

