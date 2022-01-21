/*
 * Mounts a FileVault Drive Encrypted (FVDE) volume
 *
 * Copyright (C) 2011-2022, Omar Choudary <choudary.omar@gmail.com>,
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

#include <stdio.h>

#if defined( HAVE_IO_H ) || defined( WINAPI )
#include <io.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
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
#include "mount_dokan.h"
#include "mount_fuse.h"
#include "mount_handle.h"

mount_handle_t *fvdemount_mount_handle = NULL;
int fvdemount_abort                    = 0;

/* Prints usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use fvdemount to mount a FileVault Drive Encrypted (FVDE) volume\n\n" );

	fprintf( stream, "Usage: fvdemount [ -e plist_path ] [ -k key ] [ -o offset ] [ -p password ]\n"
	                 "                 [ -r recovery_password ] [ -X extended_options ] [ -huvV ]\n"
	                 "                 sources mount_point\n\n" );

	fprintf( stream, "\tsources:     one or more source files or devices\n\n" );
	fprintf( stream, "\tmount_point: the directory to serve as mount point\n\n" );

	fprintf( stream, "\t-e:          specify the path of the EncryptedRoot.plist.wipekey file\n" );
	fprintf( stream, "\t-h:          shows this help\n" );
	fprintf( stream, "\t-k:          specify the volume master key formatted in base16\n" );
	fprintf( stream, "\t-o:          specify the volume offset in bytes\n" );
	fprintf( stream, "\t-p:          specify the password/passphrase\n" );
	fprintf( stream, "\t-r:          specify the recovery password/passphrase\n" );
	fprintf( stream, "\t-u:          unattended mode (disables user interaction)\n" );
	fprintf( stream, "\t-v:          verbose output to stderr, while fvdemount will remain running in the\n"
	                 "\t             foreground\n" );
	fprintf( stream, "\t-V:          print version\n" );
	fprintf( stream, "\t-X:          extended options to pass to sub system\n" );
}

/* Signal handler for fvdemount
 */
void fvdemount_signal_handler(
      fvdetools_signal_t signal FVDETOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "fvdemount_signal_handler";

	FVDETOOLS_UNREFERENCED_PARAMETER( signal )

	fvdemount_abort = 1;

	if( fvdemount_mount_handle != NULL )
	{
		if( mount_handle_signal_abort(
		     fvdemount_mount_handle,
		     &error ) != 1 )
		{
			libcnotify_printf(
			 "%s: unable to signal mount handle to abort.\n",
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
	system_character_t *mount_point                      = NULL;
	system_character_t *option_encrypted_root_plist_path = NULL;
	system_character_t *option_extended_options          = NULL;
	system_character_t *option_key                       = NULL;
	system_character_t *option_offset                    = NULL;
	system_character_t *option_password                  = NULL;
	system_character_t *option_recovery_password         = NULL;
	const system_character_t *path_prefix                = NULL;
	char *program                                        = "fvdemount";
	system_integer_t option                              = 0;
	size_t path_prefix_size                              = 0;
	int number_of_sources                                = 0;
	int result                                           = 0;
	int unattended_mode                                  = 0;
	int verbose                                          = 0;

#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )
	struct fuse_operations fvdemount_fuse_operations;

	struct fuse_args fvdemount_fuse_arguments            = FUSE_ARGS_INIT(0, NULL);
	struct fuse_chan *fvdemount_fuse_channel             = NULL;
	struct fuse *fvdemount_fuse_handle                   = NULL;

#elif defined( HAVE_LIBDOKAN )
	DOKAN_OPERATIONS fvdemount_dokan_operations;
	DOKAN_OPTIONS fvdemount_dokan_options;
#endif

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
	                   _SYSTEM_STRING( "e:hk:o:p:r:uvVX:" ) ) ) != (system_integer_t) -1 )
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
				option_offset = optarg;

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

			case (system_integer_t) 'X':
				option_extended_options = optarg;

				break;
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing source volume.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	sources           = &( argv[ optind ] );
	number_of_sources = argc - optind - 1;

	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing mount point.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	mount_point = argv[ argc - 1 ];

	libcnotify_verbose_set(
	 verbose );
	libfvde_notify_set_stream(
	 stderr,
	 NULL );
	libfvde_notify_set_verbose(
	 verbose );

	if( mount_handle_initialize(
	     &fvdemount_mount_handle,
	     unattended_mode,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize mount handle.\n" );

		goto on_error;
	}
	if( option_encrypted_root_plist_path != NULL )
	{
		if( mount_handle_set_encrypted_root_plist(
		     fvdemount_mount_handle,
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
		if( mount_handle_set_key(
		     fvdemount_mount_handle,
		     option_key,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set key.\n" );

			goto on_error;
		}
	}
	if( option_offset != NULL )
	{
		if( mount_handle_set_offset(
		     fvdemount_mount_handle,
		     option_offset,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set volume offset.\n" );

			goto on_error;
		}
	}
	if( option_password != NULL )
	{
		if( mount_handle_set_password(
		     fvdemount_mount_handle,
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
		if( mount_handle_set_recovery_password(
		     fvdemount_mount_handle,
		     option_recovery_password,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set recovery password.\n" );

			goto on_error;
		}
	}
#if defined( WINAPI )
	path_prefix = _SYSTEM_STRING( "\\FVDE" );
#else
	path_prefix = _SYSTEM_STRING( "/fvde" );
#endif
	path_prefix_size = 1 + system_string_length(
	                        path_prefix );

	if( mount_handle_set_path_prefix(
	     fvdemount_mount_handle,
	     path_prefix,
	     path_prefix_size,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to set path prefix.\n" );

		goto on_error;
	}
	if( mount_handle_open(
	     fvdemount_mount_handle,
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
#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )
	if( option_extended_options != NULL )
	{
		/* This argument is required but ignored
		 */
		if( fuse_opt_add_arg(
		     &fvdemount_fuse_arguments,
		     "" ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable add fuse arguments.\n" );

			goto on_error;
		}
		if( fuse_opt_add_arg(
		     &fvdemount_fuse_arguments,
		     "-o" ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable add fuse arguments.\n" );

			goto on_error;
		}
		if( fuse_opt_add_arg(
		     &fvdemount_fuse_arguments,
		     option_extended_options ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable add fuse arguments.\n" );

			goto on_error;
		}
	}
	if( memory_set(
	     &fvdemount_fuse_operations,
	     0,
	     sizeof( struct fuse_operations ) ) == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to clear fuse operations.\n" );

		goto on_error;
	}
	fvdemount_fuse_operations.open       = &mount_fuse_open;
	fvdemount_fuse_operations.read       = &mount_fuse_read;
	fvdemount_fuse_operations.release    = &mount_fuse_release;
	fvdemount_fuse_operations.opendir    = &mount_fuse_opendir;
	fvdemount_fuse_operations.readdir    = &mount_fuse_readdir;
	fvdemount_fuse_operations.releasedir = &mount_fuse_releasedir;
	fvdemount_fuse_operations.getattr    = &mount_fuse_getattr;
	fvdemount_fuse_operations.destroy    = &mount_fuse_destroy;

	fvdemount_fuse_channel = fuse_mount(
	                          mount_point,
	                          &fvdemount_fuse_arguments );

	if( fvdemount_fuse_channel == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to create fuse channel.\n" );

		goto on_error;
	}
	fvdemount_fuse_handle = fuse_new(
	                         fvdemount_fuse_channel,
	                         &fvdemount_fuse_arguments,
	                         &fvdemount_fuse_operations,
	                         sizeof( struct fuse_operations ),
	                         fvdemount_mount_handle );

	if( fvdemount_fuse_handle == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to create fuse handle.\n" );

		goto on_error;
	}
	if( verbose == 0 )
	{
		if( fuse_daemonize(
		     0 ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable to daemonize fuse.\n" );

			goto on_error;
		}
	}
	result = fuse_loop(
	          fvdemount_fuse_handle );

	if( result != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to run fuse loop.\n" );

		goto on_error;
	}
	fuse_destroy(
	 fvdemount_fuse_handle );

	fuse_opt_free_args(
	 &fvdemount_fuse_arguments );

	return( EXIT_SUCCESS );

#elif defined( HAVE_LIBDOKAN )
	if( memory_set(
	     &fvdemount_dokan_operations,
	     0,
	     sizeof( DOKAN_OPERATIONS ) ) == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to clear dokan operations.\n" );

		goto on_error;
	}
	if( memory_set(
	     &fvdemount_dokan_options,
	     0,
	     sizeof( DOKAN_OPTIONS ) ) == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to clear dokan options.\n" );

		goto on_error;
	}
	fvdemount_dokan_options.Version     = DOKAN_VERSION;
	fvdemount_dokan_options.ThreadCount = 0;
	fvdemount_dokan_options.MountPoint  = mount_point;

	if( verbose != 0 )
	{
		fvdemount_dokan_options.Options |= DOKAN_OPTION_STDERR;
#if defined( HAVE_DEBUG_OUTPUT )
		fvdemount_dokan_options.Options |= DOKAN_OPTION_DEBUG;
#endif
	}
/* This will only affect the drive properties
	fvdemount_dokan_options.Options |= DOKAN_OPTION_REMOVABLE;
*/

#if ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 )
	fvdemount_dokan_options.Options |= DOKAN_OPTION_KEEP_ALIVE;

	fvdemount_dokan_operations.CreateFile           = &mount_dokan_CreateFile;
	fvdemount_dokan_operations.OpenDirectory        = &mount_dokan_OpenDirectory;
	fvdemount_dokan_operations.CreateDirectory      = NULL;
	fvdemount_dokan_operations.Cleanup              = NULL;
	fvdemount_dokan_operations.CloseFile            = &mount_dokan_CloseFile;
	fvdemount_dokan_operations.ReadFile             = &mount_dokan_ReadFile;
	fvdemount_dokan_operations.WriteFile            = NULL;
	fvdemount_dokan_operations.FlushFileBuffers     = NULL;
	fvdemount_dokan_operations.GetFileInformation   = &mount_dokan_GetFileInformation;
	fvdemount_dokan_operations.FindFiles            = &mount_dokan_FindFiles;
	fvdemount_dokan_operations.FindFilesWithPattern = NULL;
	fvdemount_dokan_operations.SetFileAttributes    = NULL;
	fvdemount_dokan_operations.SetFileTime          = NULL;
	fvdemount_dokan_operations.DeleteFile           = NULL;
	fvdemount_dokan_operations.DeleteDirectory      = NULL;
	fvdemount_dokan_operations.MoveFile             = NULL;
	fvdemount_dokan_operations.SetEndOfFile         = NULL;
	fvdemount_dokan_operations.SetAllocationSize    = NULL;
	fvdemount_dokan_operations.LockFile             = NULL;
	fvdemount_dokan_operations.UnlockFile           = NULL;
	fvdemount_dokan_operations.GetFileSecurity      = NULL;
	fvdemount_dokan_operations.SetFileSecurity      = NULL;
	fvdemount_dokan_operations.GetDiskFreeSpace     = NULL;
	fvdemount_dokan_operations.GetVolumeInformation = &mount_dokan_GetVolumeInformation;
	fvdemount_dokan_operations.Unmount              = &mount_dokan_Unmount;

#else
	fvdemount_dokan_operations.ZwCreateFile         = &mount_dokan_ZwCreateFile;
	fvdemount_dokan_operations.Cleanup              = NULL;
	fvdemount_dokan_operations.CloseFile            = &mount_dokan_CloseFile;
	fvdemount_dokan_operations.ReadFile             = &mount_dokan_ReadFile;
	fvdemount_dokan_operations.WriteFile            = NULL;
	fvdemount_dokan_operations.FlushFileBuffers     = NULL;
	fvdemount_dokan_operations.GetFileInformation   = &mount_dokan_GetFileInformation;
	fvdemount_dokan_operations.FindFiles            = &mount_dokan_FindFiles;
	fvdemount_dokan_operations.FindFilesWithPattern = NULL;
	fvdemount_dokan_operations.SetFileAttributes    = NULL;
	fvdemount_dokan_operations.SetFileTime          = NULL;
	fvdemount_dokan_operations.DeleteFile           = NULL;
	fvdemount_dokan_operations.DeleteDirectory      = NULL;
	fvdemount_dokan_operations.MoveFile             = NULL;
	fvdemount_dokan_operations.SetEndOfFile         = NULL;
	fvdemount_dokan_operations.SetAllocationSize    = NULL;
	fvdemount_dokan_operations.LockFile             = NULL;
	fvdemount_dokan_operations.UnlockFile           = NULL;
	fvdemount_dokan_operations.GetFileSecurity      = NULL;
	fvdemount_dokan_operations.SetFileSecurity      = NULL;
	fvdemount_dokan_operations.GetDiskFreeSpace     = NULL;
	fvdemount_dokan_operations.GetVolumeInformation = &mount_dokan_GetVolumeInformation;
	fvdemount_dokan_operations.Unmounted            = NULL;
	fvdemount_dokan_operations.FindStreams          = NULL;
	fvdemount_dokan_operations.Mounted              = NULL;

#endif /* ( DOKAN_VERSION >= 600 ) && ( DOKAN_VERSION < 800 ) */

	result = DokanMain(
	          &fvdemount_dokan_options,
	          &fvdemount_dokan_operations );

	switch( result )
	{
		case DOKAN_SUCCESS:
			break;

		case DOKAN_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: generic error\n" );
			break;

		case DOKAN_DRIVE_LETTER_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: bad drive letter\n" );
			break;

		case DOKAN_DRIVER_INSTALL_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: unable to load driver\n" );
			break;

		case DOKAN_START_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: driver error\n" );
			break;

		case DOKAN_MOUNT_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: unable to assign drive letter\n" );
			break;

		case DOKAN_MOUNT_POINT_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: mount point error\n" );
			break;

		default:
			fprintf(
			 stderr,
			 "Unable to run dokan main: unknown error: %d\n",
			 result );
			break;
	}
	return( EXIT_SUCCESS );

#else
	fprintf(
	 stderr,
	 "No sub system to mount FVDE format.\n" );

	return( EXIT_FAILURE );

#endif /* defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE ) */

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )
	if( fvdemount_fuse_handle != NULL )
	{
		fuse_destroy(
		 fvdemount_fuse_handle );
	}
	fuse_opt_free_args(
	 &fvdemount_fuse_arguments );
#endif
	if( fvdemount_mount_handle != NULL )
	{
		mount_handle_free(
		 &fvdemount_mount_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

