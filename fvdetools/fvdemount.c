/*
 * Mounts a MacOS-X FileVault Drive Encryption (FVDE) encrypted volume
 *
 * Copyright (C) 2011-2018, Omar Choudary <choudary.omar@gmail.com>
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
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>

#if defined( HAVE_ERRNO_H )
#include <errno.h>
#endif

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if !defined( WINAPI ) || defined( USE_CRT_FUNCTIONS )
#if defined( TIME_WITH_SYS_TIME )
#include <sys/time.h>
#include <time.h>
#elif defined( HAVE_SYS_TIME_H )
#include <sys/time.h>
#else
#include <time.h>
#endif
#endif

#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )
#define FUSE_USE_VERSION	26

#if defined( HAVE_LIBFUSE )
#include <fuse.h>

#elif defined( HAVE_LIBOSXFUSE )
#include <osxfuse/fuse.h>
#endif

#endif /* defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE ) */

#include "fvdetools_getopt.h"
#include "fvdetools_libcerror.h"
#include "fvdetools_libclocale.h"
#include "fvdetools_libcnotify.h"
#include "fvdetools_libfvde.h"
#include "fvdetools_output.h"
#include "fvdetools_signal.h"
#include "fvdetools_unused.h"
#include "mount_handle.h"

mount_handle_t *fvdemount_mount_handle = NULL;
int fvdemount_abort                    = 0;

/* Prints the executable usage mountrmation
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use fvdemount to mount a MacOS-X FileVault Drive Encryption (FVDE)\n"
	                 "encrypted volume\n\n");

	fprintf( stream, "Usage: fvdemount [ -e filename ] [ -k keys ] [ -o offset ]\n"
	                 "                 [ -p password ] [ -r password ] [ -X extended_options ]\n"
	                 "                 [ -hvV ] source mount_point\n\n" );

	fprintf( stream, "\tsource:      the source file or device\n" );
	fprintf( stream, "\tmount_point: the directory to serve as mount point\n\n" );

	fprintf( stream, "\t-e:          specify the name of the EncryptedRoot.plist.wipekey\n"
	                 "\t             file\n" );
	fprintf( stream, "\t-k:          the volume master key formatted in base16\n" );
	fprintf( stream, "\t-h:          shows this help\n" );
	fprintf( stream, "\t-o:          specify the volume offset\n" );
	fprintf( stream, "\t-p:          specify the password\n" );
	fprintf( stream, "\t-r:          specify the recovery password\n" );
	fprintf( stream, "\t-v:          verbose output to stderr\n"
	                 "\t             fvdemount will remain running in the foreground\n" );
	fprintf( stream, "\t-V:          print version\n" );
	fprintf( stream, "\t-X:          extended options to pass to sub system\n" );
}

/* Signal handler for fvdemount
 */
void fvdemount_signal_handler(
      fvdetools_signal_t signal FVDETOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function   = "fvdemount_signal_handler";

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

#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )

#if ( SIZEOF_OFF_T != 8 ) && ( SIZEOF_OFF_T != 4 )
#error Size of off_t not supported
#endif

static char *fvdemount_fuse_path         = "/fvde1";
static size_t fvdemount_fuse_path_length = 6;

/* Opens a file
 * Returns 0 if successful or a negative errno value otherwise
 */
int fvdemount_fuse_open(
     const char *path,
     struct fuse_file_info *file_info )
{
	libcerror_error_t *error = NULL;
	static char *function   = "fvdemount_fuse_open";
	size_t path_length      = 0;
	int result              = 0;

	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	if( file_info == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file info.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	path_length = narrow_string_length(
	               path );

	if( ( path_length != fvdemount_fuse_path_length )
	 || ( narrow_string_compare(
	       path,
	       fvdemount_fuse_path,
	       fvdemount_fuse_path_length ) != 0 ) )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported path.",
		 function );

		result = -ENOENT;

		goto on_error;
	}
	if( ( file_info->flags & 0x03 ) != O_RDONLY )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		result = -EACCES;

		goto on_error;
	}
	return( 0 );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	return( result );
}

/* Reads a buffer of data at the specified offset
 * Returns number of bytes read if successful or a negative errno value otherwise
 */
int fvdemount_fuse_read(
     const char *path,
     char *buffer,
     size_t size,
     off_t offset,
     struct fuse_file_info *file_info )
{
	libcerror_error_t *error = NULL;
	static char *function   = "fvdemount_fuse_read";
	size_t path_length      = 0;
	ssize_t read_count      = 0;
	int result              = 0;

	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	if( size > (size_t) INT_MAX )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	if( file_info == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file info.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	path_length = narrow_string_length(
	               path );

	if( ( path_length != fvdemount_fuse_path_length )
	 || ( narrow_string_compare(
	       path,
	       fvdemount_fuse_path,
	       fvdemount_fuse_path_length ) != 0 ) )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported path.",
		 function );

		result = -ENOENT;

		goto on_error;
	}
	if( mount_handle_seek_offset(
	     fvdemount_mount_handle,
	     (off64_t) offset,
	     SEEK_SET,
	     &error ) == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset in mount handle.",
		 function );

		result = -EIO;

		goto on_error;
	}
	read_count = mount_handle_read_buffer(
	              fvdemount_mount_handle,
	              (uint8_t *) buffer,
	              size,
	              &error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from mount handle.",
		 function );

		result = -EIO;

		goto on_error;
	}
	return( (int) read_count );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	return( result );
}

/* Reads a directory
 * Returns 0 if successful or a negative errno value otherwise
 */
int fvdemount_fuse_readdir(
     const char *path,
     void *buffer,
     fuse_fill_dir_t filler,
     off_t offset FVDETOOLS_ATTRIBUTE_UNUSED,
     struct fuse_file_info *file_info FVDETOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function   = "fvdemount_fuse_readdir";
	size_t path_length      = 0;
	int result              = 0;

	FVDETOOLS_UNREFERENCED_PARAMETER( offset )
	FVDETOOLS_UNREFERENCED_PARAMETER( file_info )

	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	path_length = narrow_string_length(
	               path );

	if( ( path_length != 1 )
	 || ( path[ 0 ] != '/' ) )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported path.",
		 function );

		result = -ENOENT;

		goto on_error;
	}
	if( filler(
	     buffer,
	     ".",
	     NULL,
	     0 ) == 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set directory entry.",
		 function );

		result = -EIO;

		goto on_error;
	}
	if( filler(
	     buffer,
	     "..",
	     NULL,
	     0 ) == 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set directory entry.",
		 function );

		result = -EIO;

		goto on_error;
	}
	if( filler(
	     buffer,
	     &( fvdemount_fuse_path[ 1 ] ),
	     NULL,
	     0 ) == 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set directory entry.",
		 function );

		result = -EIO;

		goto on_error;
	}
	return( 0 );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	return( result );
}

/* Retrieves the file stat info
 * Returns 0 if successful or a negative errno value otherwise
 */
int fvdemount_fuse_getattr(
     const char *path,
     struct stat *stat_info )
{
	libcerror_error_t *error = NULL;
	static char *function    = "fvdemount_fuse_getattr";
	size64_t volume_size     = 0;
	size_t path_length       = 0;
	int result               = -ENOENT;

#if defined( HAVE_TIME )
	time_t timestamp         = 0;
#endif

	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	if( stat_info == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stat info.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	if( memory_set(
	     stat_info,
	     0,
	     sizeof( struct stat ) ) == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stat info.",
		 function );

		result = errno;

		goto on_error;
	}
	path_length = narrow_string_length(
	               path );

	if( path_length == 1 )
	{
		if( path[ 0 ] == '/' )
		{
			stat_info->st_mode  = S_IFDIR | 0755;
			stat_info->st_nlink = 2;

			result = 0;
		}
	}
	else if( path_length == fvdemount_fuse_path_length )
	{
		if( narrow_string_compare(
		     path,
		     fvdemount_fuse_path,
		     fvdemount_fuse_path_length ) == 0 )
		{
			stat_info->st_mode  = S_IFREG | 0444;
			stat_info->st_nlink = 1;

			if( mount_handle_get_size(
			     fvdemount_mount_handle,
			     &volume_size,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unsupported to retrieve volume size.",
				 function );

				result = -EIO;

				goto on_error;
			}
#if SIZEOF_OFF_T == 4
			if( volume_size > (size64_t) UINT32_MAX )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: unsupported to volume size value out of bounds.",
				 function );

				result = -ERANGE;

				goto on_error;
			}
#endif
			stat_info->st_size = (off_t) volume_size;

			result = 0;
		}
	}
	if( result == 0 )
	{
#if defined( HAVE_TIME )
		if( time( &timestamp ) == (time_t) -1 )
		{
			timestamp = 0;
		}
		stat_info->st_atime = timestamp;
		stat_info->st_mtime = timestamp;
		stat_info->st_ctime = timestamp;
#else
		stat_info->st_atime = 0;
		stat_info->st_mtime = 0;
		stat_info->st_ctime = 0;
#endif
#if defined( HAVE_GETEUID )
		stat_info->st_uid = geteuid();
#else
		stat_info->st_uid = 0;
#endif
#if defined( HAVE_GETEGID )
		stat_info->st_gid = getegid();
#else
		stat_info->st_gid = 0;
#endif
	}
	return( result );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	return( result );
}

/* Cleans up when fuse is done
 */
void fvdemount_fuse_destroy(
      void *private_data FVDETOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "fvdemount_fuse_destroy";

	FVDETOOLS_UNREFERENCED_PARAMETER( private_data )

	if( fvdemount_mount_handle != NULL )
	{
		if( mount_handle_free(
		     &fvdemount_mount_handle,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free mount handle.",
			 function );

			goto on_error;
		}
	}
	return;

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	return;
}

#endif /* defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE ) */

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libfvde_error_t *error                                   = NULL;
	system_character_t *mount_point                          = NULL;
	system_character_t *option_encrypted_root_plist_filename = NULL;
	system_character_t *option_extended_options              = NULL;
	system_character_t *option_keys                          = NULL;
	system_character_t *option_password                      = NULL;
	system_character_t *option_recovery_password             = NULL;
	system_character_t *option_volume_offset                 = NULL;
	system_character_t *source                               = NULL;
	char *program                                            = "fvdemount";
	system_integer_t option                                  = 0;
	int result                                               = 0;
	int verbose                                              = 0;

#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )
	struct fuse_operations fvdemount_fuse_operations;

	struct fuse_args fvdemount_fuse_arguments                = FUSE_ARGS_INIT(0, NULL);
	struct fuse_chan *fvdemount_fuse_channel                 = NULL;
	struct fuse *fvdemount_fuse_handle                       = NULL;
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
	fvdeoutput_version_fprint(
	 stdout,
	 program );

	while( ( option = fvdetools_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "e:hk:o:p:r:vVX:" ) ) ) != (system_integer_t) -1 )
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
				option_encrypted_root_plist_filename = optarg;

				break;

			case (system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'k':
				option_keys = optarg;

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

			case (system_integer_t) 'v':
				verbose = 1;

				break;

			case (system_integer_t) 'V':
				fvdeoutput_copyright_fprint(
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
		 "Missing source file or device.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	source = argv[ optind++ ];

	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing mount point.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	mount_point = argv[ optind ];

	libcnotify_verbose_set(
	 verbose );
	libfvde_notify_set_stream(
	 stderr,
	 NULL );
	libfvde_notify_set_verbose(
	 verbose );

	if( mount_handle_initialize(
	     &fvdemount_mount_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize mount handle.\n" );

		goto on_error;
	}
	if( option_keys != NULL )
	{
		if( mount_handle_set_keys(
		     fvdemount_mount_handle,
		     option_keys,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set keys.\n" );

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
	if( option_encrypted_root_plist_filename != NULL )
	{
		if( mount_handle_read_encrypted_root_plist(
		     fvdemount_mount_handle,
		     option_encrypted_root_plist_filename,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to read encrypted root plist file.\n" );

			goto on_error;
		}
	}
	if( option_volume_offset != NULL )
	{
		if( mount_handle_set_volume_offset(
		     fvdemount_mount_handle,
		     option_volume_offset,
		     &error ) != 1 )
		{
			fprintf(
			 stderr,
			 "Unable to set volume offset.\n" );

			goto on_error;
		}
	}
	result = mount_handle_open_input(
	          fvdemount_mount_handle,
	          source,
	          &error );

	if( result == -1 )
	{
		fprintf(
		 stderr,
		 "Unable to open: %" PRIs_SYSTEM ".\n",
		 source );

		goto on_error;
	}
	else if( result == 0 )
	{
		fprintf(
		 stderr,
		 "Unable to unlock keys.\n" );

		goto on_error;
	}
#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )
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
	fvdemount_fuse_operations.open    = &fvdemount_fuse_open;
	fvdemount_fuse_operations.read    = &fvdemount_fuse_read;
	fvdemount_fuse_operations.readdir = &fvdemount_fuse_readdir;
	fvdemount_fuse_operations.getattr = &fvdemount_fuse_getattr;
	fvdemount_fuse_operations.destroy = &fvdemount_fuse_destroy;

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
#else
	fprintf(
	 stderr,
	 "No sub system to mount FVDE volume.\n" );

	return( EXIT_FAILURE );
#endif

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

