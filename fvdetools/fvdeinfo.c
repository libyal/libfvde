/*
 * Shows information obtained from a FileVault Drive Encryption (FVDE) encrypted volume
 *
 * Copyright (C) 2011-2022, Omar Choudary <choudary.omar@gmail.com>
 *                          Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <file_stream.h>
#include <memory.h>
#include <system_string.h>
#include <types.h>

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fvdetools_getopt.h"
#include "fvdetools_i18n.h"
#include "fvdetools_input.h"
#include "fvdetools_libcerror.h"
#include "fvdetools_libclocale.h"
#include "fvdetools_libcnotify.h"
#include "fvdetools_libfvde.h"
#include "fvdetools_output.h"
#include "fvdetools_signal.h"
#include "fvdetools_unused.h"
#include "info_handle.h"

info_handle_t *fvdeinfo_info_handle = NULL;
int fvdeinfo_abort                  = 0;

/* Prints the executable usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use fvdeinfo to determine information about a MacOS-X FileVault\n"
	                 " Drive Encrypted (FVDE) volume\n\n" );

	fprintf( stream, "Usage: fvdeinfo [ -e plist_path ] [ -k key ] [ -o offset ]\n"
	                 "                [ -p password ] [ -r password ] [ -huvV ]\n"
	                 "                sources\n\n" );

	fprintf( stream, "\tsources: one or more source files or devices\n\n" );

	fprintf( stream, "\t-e:      specify the path of the EncryptedRoot.plist.wipekey file\n" );
	fprintf( stream, "\t-h:      shows this help\n" );
	fprintf( stream, "\t-k:      specify the volume master key formatted in base16\n" );
	fprintf( stream, "\t-o:      specify the volume offset\n" );
	fprintf( stream, "\t-p:      specify the password\n" );
	fprintf( stream, "\t-r:      specify the recovery password\n" );
	fprintf( stream, "\t-u:      unattended mode (disables user interaction)\n" );
	fprintf( stream, "\t-v:      verbose output to stderr\n" );
	fprintf( stream, "\t-V:      print version\n" );
}

/* Signal handler for fvdeinfo
 */
void fvdeinfo_signal_handler(
      fvdetools_signal_t signal FVDETOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "fvdeinfo_signal_handler";

	FVDETOOLS_UNREFERENCED_PARAMETER( signal )

	fvdeinfo_abort = 1;

	if( fvdeinfo_info_handle != NULL )
	{
		if( info_handle_signal_abort(
		     fvdeinfo_info_handle,
		     &error ) != 1 )
		{
			libcnotify_printf(
			 "%s: unable to signal info handle to abort.\n",
			 function );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
	}
	/* Force stdin to close otherwise any function reading it will remain blocked
	 */
#if defined( WINAPI ) && !defined( __CYGWIN__ )
	if( _close(
	     0 ) != 0 )
#else
	if( close(
	     0 ) != 0 )
#endif
	{
		libcnotify_printf(
		 "%s: unable to close stdin.\n",
		 function );
	}
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	system_character_t * const *sources                  = NULL;
	libfvde_error_t *error                               = NULL;
	system_character_t *option_encrypted_root_plist_path = NULL;
	system_character_t *option_key                       = NULL;
	system_character_t *option_password                  = NULL;
	system_character_t *option_recovery_password         = NULL;
	system_character_t *option_volume_offset             = NULL;
	char *program                                        = "fvdeinfo";
	system_integer_t option                              = 0;
	int number_of_sources                                = 0;
	int unattended_mode                                  = 0;
	int verbose                                          = 0;

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
        if( fvdetools_output_initialize(
             _IONBF,
             &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize output settings.\n" );

		goto on_error;
	}
	fvdetools_output_version_fprint(
	 stdout,
	 program );

	while( ( option = fvdetools_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "e:hk:o:p:r:uvV" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM "\n",
				 argv[ optind - 1 ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (system_integer_t) 'e':
				option_encrypted_root_plist_path = optarg;

				break;

			case (system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'k':
				option_key = optarg;

				break;

			case (system_integer_t) 'o':
				option_volume_offset = optarg;

				break;

			case (system_integer_t) 'p':
				option_password = optarg;

				break;

			case (system_integer_t) 'r':
				option_recovery_password = optarg;

				break;

			case (system_integer_t) 'u':
				unattended_mode = 1;

				break;

			case (system_integer_t) 'v':
				verbose = 1;

				break;

			case (system_integer_t) 'V':
				fvdetools_output_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing source file or device.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	sources           = &( argv[ optind ] );
	number_of_sources = argc - optind;

	libcnotify_verbose_set(
	 verbose );
	libfvde_notify_set_stream(
	 stderr,
	 NULL );
	libfvde_notify_set_verbose(
	 verbose );

	if( info_handle_initialize(
	     &fvdeinfo_info_handle,
	     unattended_mode,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize info handle.\n" );

		goto on_error;
	}
	if( option_encrypted_root_plist_path != NULL )
	{
		if( info_handle_set_encrypted_root_plist(
		     fvdeinfo_info_handle,
		     option_encrypted_root_plist_path,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set path of EncryptedRoot.plist.wipekey file.\n" );

			goto on_error;
		}
	}
	if( option_key != NULL )
	{
		if( info_handle_set_key(
		     fvdeinfo_info_handle,
		     option_key,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set key.\n" );

			goto on_error;
		}
	}
	if( option_password != NULL )
	{
		if( info_handle_set_password(
		     fvdeinfo_info_handle,
		     option_password,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set password.\n" );

			goto on_error;
		}
	}
	if( option_recovery_password != NULL )
	{
		if( info_handle_set_recovery_password(
		     fvdeinfo_info_handle,
		     option_recovery_password,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set recovery password.\n" );

			goto on_error;
		}
	}
	if( option_volume_offset != NULL )
	{
		if( info_handle_set_volume_offset(
		     fvdeinfo_info_handle,
		     option_volume_offset,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set volume offset.\n" );

			goto on_error;
		}
	}
	if( info_handle_open(
	     fvdeinfo_info_handle,
	     sources,
	     number_of_sources,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open: %" PRIs_SYSTEM ".\n",
		 sources[ 0 ] );

		goto on_error;
	}
	if( info_handle_volume_fprint(
	     fvdeinfo_info_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to print volume information.\n" );

		goto on_error;
	}
	if( info_handle_close(
	     fvdeinfo_info_handle,
	     &error ) != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to close info handle.\n" );

		goto on_error;
	}
	if( info_handle_free(
	     &fvdeinfo_info_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to free info handle.\n" );

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
	if( fvdeinfo_info_handle != NULL )
	{
		info_handle_free(
		 &fvdeinfo_info_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

