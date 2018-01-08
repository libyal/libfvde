/*
 * Volume functions
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
#include <memory.h>
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include "libfvde_codepage.h"
#include "libfvde_debug.h"
#include "libfvde_definitions.h"
#include "libfvde_encrypted_metadata.h"
#include "libfvde_encryption_context_plist.h"
#include "libfvde_encryption.h"
#include "libfvde_io_handle.h"
#include "libfvde_libbfio.h"
#include "libfvde_libcerror.h"
#include "libfvde_libcnotify.h"
#include "libfvde_libcthreads.h"
#include "libfvde_libfcache.h"
#include "libfvde_libfdata.h"
#include "libfvde_libhmac.h"
#include "libfvde_metadata.h"
#include "libfvde_password.h"
#include "libfvde_sector_data.h"
#include "libfvde_volume.h"

#include "fvde_metadata.h"

/* Creates a volume
 * Make sure the value volume is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_initialize(
     libfvde_volume_t **volume,
     libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_initialize";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	if( *volume != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume value already set.",
		 function );

		return( -1 );
	}
	internal_volume = memory_allocate_structure(
	                   libfvde_internal_volume_t );

	if( internal_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create volume.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_volume,
	     0,
	     sizeof( libfvde_internal_volume_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear volume.",
		 function );

		memory_free(
		 internal_volume );

		return( -1 );
	}
	if( libfvde_metadata_initialize(
	     &( internal_volume->primary_metadata ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create primary metadata.",
		 function );

		goto on_error;
	}
	if( libfvde_metadata_initialize(
	     &( internal_volume->secondary_metadata ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create secondary metadata.",
		 function );

		goto on_error;
	}
	if( libfvde_metadata_initialize(
	     &( internal_volume->tertiary_metadata ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create tertiary metadata.",
		 function );

		goto on_error;
	}
	if( libfvde_metadata_initialize(
	     &( internal_volume->quaternary_metadata ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create quaternary metadata.",
		 function );

		goto on_error;
	}
	if( libfvde_encrypted_metadata_initialize(
	     &( internal_volume->primary_encrypted_metadata ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create primary encrypted metadata.",
		 function );

		goto on_error;
	}
	if( libfvde_encrypted_metadata_initialize(
	     &( internal_volume->secondary_encrypted_metadata ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create secondary encrypted metadata.",
		 function );

		goto on_error;
	}
	if( libfvde_io_handle_initialize(
	      &( internal_volume->io_handle ),
	      error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create IO handle.",
		 function );

		goto on_error;
	}
	if( libfvde_keyring_initialize(
	      &( internal_volume->keyring ),
	      error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialise keyring.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_volume->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to intialize read/write lock.",
		 function );

		goto on_error;
	}
#endif
	internal_volume->is_locked = 1;

	*volume = (libfvde_volume_t *) internal_volume;

	return( 1 );

on_error:
	if( internal_volume != NULL )
	{
		if( internal_volume->secondary_encrypted_metadata != NULL )
		{
			libfvde_encrypted_metadata_free(
			 &( internal_volume->secondary_encrypted_metadata ),
			 NULL );
		}
		if( internal_volume->primary_encrypted_metadata != NULL )
		{
			libfvde_encrypted_metadata_free(
			 &( internal_volume->primary_encrypted_metadata ),
			 NULL );
		}
		if( internal_volume->quaternary_metadata != NULL )
		{
			libfvde_metadata_free(
			 &( internal_volume->quaternary_metadata ),
			 NULL );
		}
		if( internal_volume->tertiary_metadata != NULL )
		{
			libfvde_metadata_free(
			 &( internal_volume->tertiary_metadata ),
			 NULL );
		}
		if( internal_volume->secondary_metadata != NULL )
		{
			libfvde_metadata_free(
			 &( internal_volume->secondary_metadata ),
			 NULL );
		}
		if( internal_volume->primary_metadata != NULL )
		{
			libfvde_metadata_free(
			 &( internal_volume->primary_metadata ),
			 NULL );
		}
		memory_free(
		 internal_volume );
	}
	return( -1 );
}

/* Frees a volume
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_free(
     libfvde_volume_t **volume,
     libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_free";
	int result                                 = 1;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	if( *volume != NULL )
	{
		internal_volume = (libfvde_internal_volume_t *) *volume;

		if( internal_volume->file_io_handle != NULL )
		{
			if( libfvde_volume_close(
			     *volume,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close volume.",
				 function );

				result = -1;
			}
		}
		*volume = NULL;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_volume->read_write_lock ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free read/write lock.",
			 function );

			result = -1;
		}
#endif
		if( libfvde_encrypted_metadata_free(
		     &( internal_volume->secondary_encrypted_metadata ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free secondary encrypted metadata.",
			 function );

			result = -1;
		}
		if( libfvde_encrypted_metadata_free(
		     &( internal_volume->primary_encrypted_metadata ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free primary encrypted metadata.",
			 function );

			result = -1;
		}
		if( libfvde_metadata_free(
		     &( internal_volume->quaternary_metadata ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free quaternary metadata.",
			 function );

			result = -1;
		}
		if( libfvde_metadata_free(
		     &( internal_volume->tertiary_metadata ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free tertiary metadata.",
			 function );

			result = -1;
		}
		if( libfvde_metadata_free(
		     &( internal_volume->secondary_metadata ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free secondary metadata.",
			 function );

			result = -1;
		}
		if( libfvde_metadata_free(
		     &( internal_volume->primary_metadata ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free primary metadata.",
			 function );

			result = -1;
		}
		if( internal_volume->encrypted_root_plist != NULL )
		{
			if( libfvde_encryption_context_plist_free(
			     &( internal_volume->encrypted_root_plist ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free encrypted root plist.",
				 function );

				result = -1;
			}
		}
		if( libfvde_io_handle_free(
		     &( internal_volume->io_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free IO handle.",
			 function );

			result = -1;
		}
		if( libfvde_keyring_free(
		     &( internal_volume->keyring ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free keyring.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_volume );
	}
	return( result );
}

/* Signals the volume to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_signal_abort(
     libfvde_volume_t *volume,
     libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_signal_abort";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfvde_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	internal_volume->io_handle->abort = 1;

	return( 1 );
}

/* Opens a volume
 * Returns 1 if successful, 0 if the keys could not be read or -1 on error
 */
int libfvde_volume_open(
     libfvde_volume_t *volume,
     const char *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle           = NULL;
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_open";
	size_t filename_length                     = 0;
	int result                                 = 0;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfvde_internal_volume_t *) volume;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBFVDE_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBFVDE_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBFVDE_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	filename_length = narrow_string_length(
	                   filename );

	if( filename_length == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		goto on_error;
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set track offsets read in file IO handle.",
		 function );

		goto on_error;
	}
#endif
	if( libbfio_file_set_name(
	     file_io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set filename in file IO handle.",
		 function );

		goto on_error;
	}
	result = libfvde_volume_open_file_io_handle(
	          volume,
	          file_io_handle,
	          access_flags,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open volume: %s.",
		 function,
		 filename );

		goto on_error;
	}
	else if( result == 0 )
	{
		if( libbfio_handle_free(
		     &file_io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file IO handle.",
			 function );

			goto on_error;
		}
	}
	else
	{
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_grab_for_write(
		     internal_volume->read_write_lock,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to grab read/write lock for writing.",
			 function );

			return( -1 );
		}
#endif
		internal_volume->file_io_handle_created_in_library = 1;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_release_for_write(
		     internal_volume->read_write_lock,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to release read/write lock for writing.",
			 function );

			return( -1 );
		}
#endif
	}
	return( result );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Opens a volume
 * Returns 1 if successful, 0 if the keys could not be read or -1 on error
 */
int libfvde_volume_open_wide(
     libfvde_volume_t *volume,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle           = NULL;
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_open_wide";
	size_t filename_length                     = 0;
	int result                                 = 0;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfvde_internal_volume_t *) volume;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBFVDE_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBFVDE_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBFVDE_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	filename_length = wide_string_length(
	                   filename );

	if( filename_length == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		goto on_error;
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set track offsets read in file IO handle.",
		 function );

		goto on_error;
	}
#endif
	if( libbfio_file_set_name_wide(
	     file_io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set filename in file IO handle.",
		 function );

		goto on_error;
	}
	result = libfvde_volume_open_file_io_handle(
	          volume,
	          file_io_handle,
	          access_flags,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open volume: %ls.",
		 function,
		 filename );

		goto on_error;
	}
	else if( result == 0 )
	{
		if( libbfio_handle_free(
		     &file_io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file IO handle.",
			 function );

			goto on_error;
		}
	}
	else
	{
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_grab_for_write(
		     internal_volume->read_write_lock,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to grab read/write lock for writing.",
			 function );

			return( -1 );
		}
#endif
		internal_volume->file_io_handle_created_in_library = 1;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_release_for_write(
		     internal_volume->read_write_lock,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to release read/write lock for writing.",
			 function );

			return( -1 );
		}
#endif
	}
	return( result );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Opens a volume using a Basic File IO (bfio) handle
 * Returns 1 if successful, 0 if the keys could not be read or -1 on error
 */
int libfvde_volume_open_file_io_handle(
     libfvde_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_open_file_io_handle";
	uint8_t file_io_handle_opened_in_library   = 0;
	int bfio_access_flags                      = 0;
	int file_io_handle_is_open                 = 0;
	int result                                 = 0;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfvde_internal_volume_t *) volume;

	if( internal_volume->file_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume - file IO handle already set.",
		 function );

		return( -1 );
	}
	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBFVDE_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBFVDE_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBFVDE_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBFVDE_ACCESS_FLAG_READ ) != 0 )
	{
		bfio_access_flags = LIBBFIO_ACCESS_FLAG_READ;
	}
	file_io_handle_is_open = libbfio_handle_is_open(
	                          file_io_handle,
	                          error );

	if( file_io_handle_is_open == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to determine if file IO handle is open.",
		 function );

		goto on_error;
	}
	else if( file_io_handle_is_open == 0 )
	{
		if( libbfio_handle_open(
		     file_io_handle,
		     bfio_access_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open file IO handle.",
			 function );

			goto on_error;
		}
		file_io_handle_opened_in_library = 1;
	}
	result = libfvde_volume_open_read(
	          internal_volume,
	          file_io_handle,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from file IO handle.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		if( file_io_handle_opened_in_library != 0 )
		{
			file_io_handle_opened_in_library = 0;

			if( libbfio_handle_close(
			     file_io_handle,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close file IO handle.",
				 function );

				goto on_error;
			}
		}
	}
	else
	{
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_grab_for_write(
		     internal_volume->read_write_lock,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to grab read/write lock for writing.",
			 function );

			return( -1 );
		}
#endif
		internal_volume->file_io_handle                   = file_io_handle;
		internal_volume->file_io_handle_opened_in_library = file_io_handle_opened_in_library;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_release_for_write(
		     internal_volume->read_write_lock,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to release read/write lock for writing.",
			 function );

			return( -1 );
		}
#endif
	}
	return( result );

on_error:
	if( file_io_handle_opened_in_library != 0 )
	{
		libbfio_handle_close(
		 file_io_handle,
		 NULL );
	}
	return( -1 );
}

/* Closes a volume
 * Returns 0 if successful or -1 on error
 */
int libfvde_volume_close(
     libfvde_volume_t *volume,
     libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_close";
	int result                                 = 0;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfvde_internal_volume_t *) volume;

	if( internal_volume->file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing file IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( internal_volume->file_io_handle_created_in_library != 0 )
		{
			if( libfvde_debug_print_read_offsets(
			     internal_volume->file_io_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print the read offsets.",
				 function );
			}
		}
	}
#endif
	if( internal_volume->file_io_handle_opened_in_library != 0 )
	{
		if( libbfio_handle_close(
		     internal_volume->file_io_handle,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close file IO handle.",
			 function );

			result = -1;
		}
		internal_volume->file_io_handle_opened_in_library = 0;
	}
	if( internal_volume->file_io_handle_created_in_library != 0 )
	{
		if( libbfio_handle_free(
		     &( internal_volume->file_io_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file IO handle.",
			 function );

			result = -1;
		}
		internal_volume->file_io_handle_created_in_library = 0;
	}
	internal_volume->file_io_handle = NULL;
	internal_volume->current_offset = 0;
	internal_volume->is_locked      = 1;

	if( libfvde_io_handle_clear(
	     internal_volume->io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		result = -1;
	}
	if( libfdata_vector_free(
	     &( internal_volume->sectors_vector ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free sectors vector.",
		 function );

		result = -1;
	}
	if( libfcache_cache_free(
	     &( internal_volume->sectors_cache ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free sectors cache.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Opens a volume for reading
 * Returns 1 if successful, 0 if the keys could not be read or -1 on error
 */
int libfvde_volume_open_read(
     libfvde_internal_volume_t *internal_volume,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libfvde_volume_open_read";
	int result            = 0;
	int segment_index     = 0;

	if( internal_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_volume->primary_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume - missing primary metadata.",
		 function );

		return( -1 );
	}
	if( internal_volume->secondary_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume - missing secondary metadata.",
		 function );

		return( -1 );
	}
	if( internal_volume->tertiary_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume - missing tertiary metadata.",
		 function );

		return( -1 );
	}
	if( internal_volume->quaternary_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume - missing quaternary metadata.",
		 function );

		return( -1 );
	}
	if( internal_volume->primary_encrypted_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume - missing primary encrypted metadata.",
		 function );

		return( -1 );
	}
	if( internal_volume->secondary_encrypted_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume - missing secondary encrypted metadata.",
		 function );

		return( -1 );
	}
	if( internal_volume->sectors_vector != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - sectors vector already set.",
		 function );

		return( -1 );
	}
	if( internal_volume->sectors_cache != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - sectors cache already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading volume header:\n" );
	}
#endif
	if( libfvde_io_handle_read_volume_header(
	     internal_volume->io_handle,
	     file_io_handle,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume header.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading primary metadata:\n" );
	}
#endif
	if( libfvde_metadata_read(
	     internal_volume->primary_metadata,
	     internal_volume->io_handle,
	     file_io_handle,
	     (off64_t) internal_volume->io_handle->first_metadata_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read primary metadata.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading secondary metadata:\n" );
	}
#endif
	if( libfvde_metadata_read(
	     internal_volume->secondary_metadata,
	     internal_volume->io_handle,
	     file_io_handle,
	     (off64_t) internal_volume->io_handle->second_metadata_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read secondary metadata.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading tertiary metadata:\n" );
	}
#endif
	if( libfvde_metadata_read(
	     internal_volume->tertiary_metadata,
	     internal_volume->io_handle,
	     file_io_handle,
	     (off64_t) internal_volume->io_handle->third_metadata_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read tertiary metadata.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading quaternary metadata:\n" );
	}
#endif
	if( libfvde_metadata_read(
	     internal_volume->quaternary_metadata,
	     internal_volume->io_handle,
	     file_io_handle,
	     (off64_t) internal_volume->io_handle->fourth_metadata_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read quaternary metadata.",
		 function );

		goto on_error;
	}
/* TODO clean up and check */
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading encrypted metadata:\n" );
	}
#endif
/* TODO compare all 4 offsets */
	if( libfvde_encrypted_metadata_read(
	     internal_volume->primary_encrypted_metadata,
	     internal_volume->io_handle,
	     file_io_handle,
	     (off64_t) internal_volume->primary_metadata->primary_encrypted_metadata_offset,
	     internal_volume->primary_metadata->encrypted_metadata_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read primary encrypted metadata.",
		 function );

		goto on_error;
	}
	if( libfvde_encrypted_metadata_read(
	     internal_volume->secondary_encrypted_metadata,
	     internal_volume->io_handle,
	     file_io_handle,
	     (off64_t) internal_volume->primary_metadata->secondary_encrypted_metadata_offset,
	     internal_volume->primary_metadata->encrypted_metadata_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read secondary encrypted metadata.",
		 function );

		goto on_error;
	}
	if( internal_volume->primary_encrypted_metadata->logical_volume_size > 0 )
	{
		internal_volume->io_handle->logical_volume_offset = internal_volume->primary_encrypted_metadata->logical_volume_offset;
		internal_volume->io_handle->logical_volume_size   = internal_volume->primary_encrypted_metadata->logical_volume_size;
	}
	else if( internal_volume->secondary_encrypted_metadata->logical_volume_size > 0 )
	{
		internal_volume->io_handle->logical_volume_offset = internal_volume->secondary_encrypted_metadata->logical_volume_offset;
		internal_volume->io_handle->logical_volume_size   = internal_volume->secondary_encrypted_metadata->logical_volume_size;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: physical volume size\t\t\t\t: %" PRIu64 "\n",
		 function,
		 internal_volume->io_handle->physical_volume_size );

		libcnotify_printf(
		 "%s: logical volume offset\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 internal_volume->io_handle->logical_volume_offset );

		libcnotify_printf(
		 "%s: logical volume size\t\t\t\t: %" PRIu64 "\n",
		 function,
		 internal_volume->io_handle->logical_volume_size );

		libcnotify_printf(
		 "\n" );
	}
#endif
	if( internal_volume->io_handle->logical_volume_size == 0 )
	{
		result = 1;
	}
	else
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "Reading logical volume header:\n" );
		}
#endif
		result = libfvde_io_handle_read_logical_volume_header(
		          internal_volume->io_handle,
		          file_io_handle,
		          internal_volume->io_handle->logical_volume_offset + 1024,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read logical volume header.",
			 function );

			goto on_error;
		}
		else if( result == 0 )
		{
			result = libfvde_volume_open_read_keys_from_encrypted_metadata(
				  internal_volume,
				  internal_volume->primary_encrypted_metadata,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read keys from primary encrypted metadata.",
				 function );

				goto on_error;
			}
			else if( result == 0 )
			{
				result = libfvde_volume_open_read_keys_from_encrypted_metadata(
					  internal_volume,
					  internal_volume->secondary_encrypted_metadata,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read keys from secondary encrypted metadata.",
					 function );

					goto on_error;
				}
			}
			if( result != 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "Reading logical volume header:\n" );
				}
#endif
				result = libfvde_io_handle_read_logical_volume_header(
					  internal_volume->io_handle,
					  file_io_handle,
					  internal_volume->io_handle->logical_volume_offset + 1024,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read logical volume header.",
					 function );

					goto on_error;
				}
			}
		}
		if( result != 0 )
		{
/* TODO clone function ? */
			if( libfdata_vector_initialize(
			     &( internal_volume->sectors_vector ),
			     (size64_t) internal_volume->io_handle->bytes_per_sector,
			     (intptr_t *) internal_volume->io_handle,
			     NULL,
			     NULL,
			     (int (*)(intptr_t *, intptr_t *, libfdata_vector_t *, libfcache_cache_t *, int, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libfvde_io_handle_read_sector,
			     NULL,
			     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create sectors vector.",
				 function );

				goto on_error;
			}
			if( libfdata_vector_append_segment(
			     internal_volume->sectors_vector,
			     &segment_index,
			     0,
			     internal_volume->io_handle->logical_volume_offset,
			     internal_volume->io_handle->logical_volume_size,
			     LIBFVDE_RANGE_FLAG_ENCRYPTED,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append segment to sectors vector.",
				 function );

				goto on_error;
			}
			if( libfcache_cache_initialize(
			     &( internal_volume->sectors_cache ),
			     LIBFVDE_MAXIMUM_CACHE_ENTRIES_SECTORS,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create sectors cache.",
				 function );

				goto on_error;
			}
			internal_volume->is_locked = 0;
		}
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );

on_error:
	if( internal_volume->sectors_cache != NULL )
	{
		libfcache_cache_free(
		 &( internal_volume->sectors_cache ),
		 NULL );
	}
	if( internal_volume->sectors_vector != NULL )
	{
		libfdata_vector_free(
		 &( internal_volume->sectors_vector ),
		 NULL );
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_volume->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

/* Reads the keys from the encrypted metadata when opening the volume for reading
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libfvde_volume_open_read_keys_from_encrypted_metadata(
     libfvde_internal_volume_t *internal_volume,
     libfvde_encrypted_metadata_t *encrypted_metadata,
     libcerror_error_t **error )
{
	uint8_t tweak_key_data[ 32 ];

	static char *function = "libfvde_volume_open_read_keys_from_encrypted_metadata";
	int result            = 1;

	if( internal_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_volume->keyring == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing keyring handle.",
		 function );

		return( -1 );
	}
	if( encrypted_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encrypted metadata.",
		 function );

		return( -1 );
	}
	if( internal_volume->volume_master_key_is_set == 0 )
	{
		if( encrypted_metadata->encryption_context_plist_file_is_set != 0 )
		{
			result = libfvde_encrypted_metadata_get_volume_master_key(
				  encrypted_metadata,
				  internal_volume->io_handle,
				  encrypted_metadata->encryption_context_plist,
				  internal_volume->keyring,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve volume master key from encrypted metadata.",
				 function );

				goto on_error;
			}
			else if( result != 0 )
			{
				internal_volume->volume_master_key_is_set = 1;
			}
		}
		else if( internal_volume->encrypted_root_plist_file_is_set != 0 )
		{
			if( internal_volume->encrypted_root_plist_file_is_decrypted == 0 )
			{
				result = libfvde_encryption_context_plist_decrypt(
				          internal_volume->encrypted_root_plist,
				          internal_volume->io_handle->key_data,
				          128,
				          error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
					 LIBCERROR_ENCRYPTION_ERROR_DECRYPT_FAILED,
					 "%s: unable to decrypt encrypted root plist.",
					 function );

					goto on_error;
				}
				internal_volume->encrypted_root_plist_file_is_decrypted = result;
			}
			if( internal_volume->encrypted_root_plist_file_is_decrypted != 0 )
			{
				result = libfvde_encrypted_metadata_get_volume_master_key(
					  encrypted_metadata,
					  internal_volume->io_handle,
					  internal_volume->encrypted_root_plist,
					  internal_volume->keyring,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve volume master key from encrypted metadata.",
					 function );

					goto on_error;
				}
				else if( result != 0 )
				{
					internal_volume->volume_master_key_is_set = 1;
				}
			}
		}
/* TODO test */
		else
		{
#ifdef TEST
			uint8_t salt[ 16 ];

			uint32_t number_of_iterations = 41000;

			if( memory_set(
			     salt,
			     0,
			     16 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear salt.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: number of iterations\t: %" PRIu32 "\n",
				 function,
				 number_of_iterations );
			}
#endif
			if( internal_volume->io_handle->user_password_is_set != 0 )
			{
				if( libfvde_password_pbkdf2(
				     internal_volume->io_handle->user_password,
				     internal_volume->io_handle->user_password_size - 1,
				     salt,
				     16,
				     number_of_iterations,
				     internal_volume->keyring->volume_master_key,
				     16,
				     error ) == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to determine password key.",
					 function );

					goto on_error;
				}
			}
			else if( internal_volume->io_handle->recovery_password_is_set != 0 )
			{
				if( libfvde_password_pbkdf2(
				     internal_volume->io_handle->recovery_password,
				     internal_volume->io_handle->recovery_password_size - 1,
				     salt,
				     16,
				     number_of_iterations,
				     internal_volume->keyring->volume_master_key,
				     16,
				     error ) == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to determine recovery password key.",
					 function );

					goto on_error;
				}
			}
#endif
			internal_volume->volume_master_key_is_set = 1;
		}
	}
	if( internal_volume->volume_master_key_is_set != 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: volume master key:\n",
			 function );
			libcnotify_print_data(
			 internal_volume->keyring->volume_master_key,
			 16,
			 0 );
		}
#endif
		if( memory_copy(
		     &( tweak_key_data[ 0 ] ),
		     internal_volume->keyring->volume_master_key,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy volume master key to tweak key data.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     &( tweak_key_data[ 16 ] ),
		     encrypted_metadata->logical_volume_family_identifier,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy logical volume family identifier to tweak key data.",
			 function );

			goto on_error;
		}
		if( libhmac_sha256_calculate(
		     tweak_key_data,
		     32,
		     internal_volume->keyring->volume_tweak_key,
		     32,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to calculate SHA-256 of tweak key data.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: volume tweak key:\n",
			 function );
			libcnotify_print_data(
			 internal_volume->keyring->volume_tweak_key,
			 16,
			 0 );
		}
#endif
		if( libcaes_tweaked_context_initialize(
		     &( internal_volume->io_handle->xts_context ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create XTS context.",
			 function );

			goto on_error;
		}
		if( libcaes_tweaked_context_set_keys(
		     internal_volume->io_handle->xts_context,
		     LIBCAES_CRYPT_MODE_DECRYPT,
		     internal_volume->keyring->volume_master_key,
		     128,
		     internal_volume->keyring->volume_tweak_key,
		     128,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set keys in XTS context.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     tweak_key_data,
		     0,
		     32 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear tweak key data.",
			 function );

			goto on_error;
		}
		internal_volume->io_handle->is_encrypted = 1;
	}
	return( result );

on_error:
	memory_set(
	 tweak_key_data,
	 0,
	 32 );

	return( -1 );
}

/* Determines if the volume is locked
 * Returns 1 if locked, 0 if not or -1 on error
 */
int libfvde_volume_is_locked(
     libfvde_volume_t *volume,
     libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_is_locked";
	uint8_t is_locked                          = 0;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfvde_internal_volume_t *) volume;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	is_locked = internal_volume->is_locked;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( is_locked );
}

/* Reads (volume) data from the last current into a buffer using a Basic File IO (bfio) handle
 * This function is not multi-thread safe acquire write lock before call
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfvde_internal_volume_read_buffer_from_file_io_handle(
         libfvde_internal_volume_t *internal_volume,
         libbfio_handle_t *file_io_handle,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libfvde_sector_data_t *sector_data = NULL;
	static char *function              = "libfvde_internal_volume_read_buffer_from_file_io_handle";
	off64_t element_data_offset        = 0;
	size_t buffer_offset               = 0;
	size_t read_size                   = 0;
	size_t sector_data_offset          = 0;
	ssize_t total_read_count           = 0;

	if( internal_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_volume->sectors_vector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing sectors vector.",
		 function );

		return( -1 );
	}
	if( internal_volume->sectors_cache == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing sectors cache.",
		 function );

		return( -1 );
	}
	if( internal_volume->current_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid volume - current offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( (size64_t) internal_volume->current_offset >= internal_volume->io_handle->logical_volume_size )
	{
		return( 0 );
	}
	if( (size64_t) ( internal_volume->current_offset + buffer_size ) >= internal_volume->io_handle->logical_volume_size )
	{
		buffer_size = (size_t) ( internal_volume->io_handle->logical_volume_size - internal_volume->current_offset );
	}
	sector_data_offset = (size_t) ( internal_volume->current_offset % internal_volume->io_handle->bytes_per_sector );

	while( buffer_size > 0 )
	{
		if( libfdata_vector_get_element_value_at_offset(
		     internal_volume->sectors_vector,
		     (intptr_t *) file_io_handle,
		     internal_volume->sectors_cache,
		     internal_volume->current_offset,
		     &element_data_offset,
		     (intptr_t **) &sector_data,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sector data at offset: %" PRIi64 ".",
			 function,
			 internal_volume->current_offset );

			return( -1 );
		}
		if( sector_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing sector data at offset: %" PRIi64 ".",
			 function,
			 internal_volume->current_offset );

			return( -1 );
		}
		read_size = sector_data->data_size - sector_data_offset;

		if( read_size > buffer_size )
		{
			read_size = buffer_size;
		}
		if( read_size == 0 )
		{
			break;
		}
		if( memory_copy(
		     &( ( (uint8_t *) buffer )[ buffer_offset ] ),
		     &( ( sector_data->data )[ sector_data_offset ] ),
		     read_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy sector data to buffer.",
			 function );

			return( -1 );
		}
		buffer_offset     += read_size;
		buffer_size       -= read_size;
		total_read_count  += (ssize_t) read_size;
		sector_data_offset = 0;

		internal_volume->current_offset += (off64_t) read_size;

		if( (size64_t) internal_volume->current_offset >= internal_volume->io_handle->logical_volume_size )
		{
			break;
		}
		if( internal_volume->io_handle->abort != 0 )
		{
			break;
		}
	}
	return( total_read_count );
}

/* Reads data at the current offset into a buffer
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfvde_volume_read_buffer(
         libfvde_volume_t *volume,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_read_buffer";
	ssize_t read_count                         = 0;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfvde_internal_volume_t *) volume;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	read_count = libfvde_internal_volume_read_buffer_from_file_io_handle(
		      internal_volume,
		      internal_volume->file_io_handle,
		      buffer,
		      buffer_size,
		      error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer.",
		 function );

		read_count = -1;
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( read_count );
}

/* Reads (media) data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfvde_volume_read_buffer_at_offset(
         libfvde_volume_t *volume,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_read_buffer_at_offset";
	ssize_t read_count                         = 0;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfvde_internal_volume_t *) volume;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libfvde_internal_volume_seek_offset(
	     internal_volume,
	     offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		goto on_error;
	}
	read_count = libfvde_internal_volume_read_buffer_from_file_io_handle(
		      internal_volume,
		      internal_volume->file_io_handle,
		      buffer,
		      buffer_size,
		      error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( read_count );

on_error:
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	libcthreads_read_write_lock_release_for_write(
	 internal_volume->read_write_lock,
	 NULL );
#endif
	return( -1 );
}

#ifdef TODO_WRITE_SUPPORT

/* Writes (media) data at the current offset
 * Returns the number of input bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libfvde_volume_write_buffer(
         libfvde_volume_t *volume,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	return( -1 );
}

/* Writes (media) data at a specific offset,
 * Returns the number of input bytes written, 0 when no longer bytes can be written or -1 on error
 */
ssize_t libfvde_volume_write_buffer_at_offset(
         libfvde_volume_t *volume,
         const void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error )
{
	static char *function = "libfvde_volume_write_buffer_at_offset";
	ssize_t write_count   = 0;

	if( libfvde_volume_seek_offset(
	     volume,
	     offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		return( -1 );
	}
	write_count = libfvde_volume_write_buffer(
	               volume,
	               buffer,
	               buffer_size,
	               error );

	if( write_count < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_WRITE_FAILED,
		 "%s: unable to write buffer.",
		 function );

		return( -1 );
	}
	return( write_count );
}

#endif /* TODO_WRITE_SUPPORT */

/* Seeks a certain offset of the (volume) data
 * This function is not multi-thread safe acquire write lock before call
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libfvde_internal_volume_seek_offset(
         libfvde_internal_volume_t *internal_volume,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	static char *function = "libfvde_internal_volume_seek_offset";

	if( internal_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( whence != SEEK_CUR )
	 && ( whence != SEEK_END )
	 && ( whence != SEEK_SET ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported whence.",
		 function );

		return( -1 );
	}
	if( whence == SEEK_CUR )
	{	
		offset += internal_volume->current_offset;
	}
	else if( whence == SEEK_END )
	{	
		offset += (off64_t) internal_volume->io_handle->logical_volume_size;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: seeking media data offset: %" PRIi64 ".\n",
		 function,
		 offset );
	}
#endif
	if( offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid offset value out of bounds.",
		 function );

		return( -1 );
	}
	internal_volume->current_offset = offset;

	return( offset );
}

/* Seeks a certain offset of the data
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libfvde_volume_seek_offset(
         libfvde_volume_t *volume,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_seek_offset";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfvde_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	offset = libfvde_internal_volume_seek_offset(
	          internal_volume,
	          offset,
	          whence,
	          error );

	if( offset == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		offset = -1;
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( offset );
}

/* Retrieves the current offset of the (volume) data
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_get_offset(
     libfvde_volume_t *volume,
     off64_t *offset,
     libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_get_offset";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfvde_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*offset = internal_volume->current_offset;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves the logical volume size
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_get_logical_volume_size(
     libfvde_volume_t *volume,
     size64_t *size,
     libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_get_logical_volume_size";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfvde_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*size = internal_volume->io_handle->logical_volume_size;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves the encryption method of the logical volume
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_get_logical_volume_encryption_method(
     libfvde_volume_t *volume,
     uint32_t *encryption_method,
     libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_get_logical_volume_encryption_method";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfvde_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( encryption_method == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encryption method.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*encryption_method = internal_volume->io_handle->logical_volume_encryption_method;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves the logical volume identifier
 * The identifier is a UUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_get_logical_volume_identifier(
     libfvde_volume_t *volume,
     uint8_t *identifier,
     size_t size,
     libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_get_logical_volume_identifier";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfvde_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	if( size < 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: identifier too small.",
		 function );

		return( -1 );
	}
/* TODO HAVE_LIBFVDE_MULTI_THREAD_SUPPORT */
	if( memory_copy(
	     identifier,
	     internal_volume->io_handle->logical_volume_identifier,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set identifier.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the logical volume group identifier
 * The identifier is a UUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_get_logical_volume_group_identifier(
     libfvde_volume_t *volume,
     uint8_t *group_identifier,
     size_t size,
     libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_get_logical_volume_group_identifier";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfvde_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( group_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid group identifier.",
		 function );

		return( -1 );
	}
	if( size < 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: identifier too small.",
		 function );

		return( -1 );
	}
/* TODO HAVE_LIBFVDE_MULTI_THREAD_SUPPORT */
	if( memory_copy(
	     group_identifier,
	     internal_volume->io_handle->logical_volume_group_identifier,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set group identifier.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the physical volume size
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_get_physical_volume_size(
     libfvde_volume_t *volume,
     size64_t *size,
     libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_get_physical_volume_size";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfvde_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid size.",
		 function );

		return( -1 );
	}
/* TODO HAVE_LIBFVDE_MULTI_THREAD_SUPPORT */
	*size = internal_volume->io_handle->physical_volume_size;

	return( 1 );
}

/* Retrieves the encryption method of the physical volume
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_get_physical_volume_encryption_method(
     libfvde_volume_t *volume,
     uint32_t *encryption_method,
     libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_get_physical_volume_encryption_method";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfvde_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( encryption_method == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encryption method.",
		 function );

		return( -1 );
	}
/* TODO HAVE_LIBFVDE_MULTI_THREAD_SUPPORT */
	*encryption_method = internal_volume->io_handle->physical_volume_encryption_method;

	return( 1 );
}

/* Retrieves the physical volume identifier
 * The identifier is a UUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_get_physical_volume_identifier(
     libfvde_volume_t *volume,
     uint8_t *identifier,
     size_t size,
     libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_get_physical_volume_identifier";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfvde_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	if( size < 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: identifier too small.",
		 function );

		return( -1 );
	}
/* TODO HAVE_LIBFVDE_MULTI_THREAD_SUPPORT */
	if( memory_copy(
	     identifier,
	     internal_volume->io_handle->physical_volume_identifier,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set identifier.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the keys
 * This function needs to be used before one of the open functions
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_set_keys(
     libfvde_volume_t *volume,
     const uint8_t *volume_master_key,
     size_t volume_master_key_size,
     libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_set_keys";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfvde_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_volume->keyring == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing keyring handle.",
		 function );

		return( -1 );
	}
	if( internal_volume->file_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume - file IO handle already set.",
		 function );

		return( -1 );
	}
	if( volume_master_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume master key.",
		 function );

		return( -1 );
	}
	if( volume_master_key_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid volume master key size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( volume_master_key_size != 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported volume master key size.",
		 function );

		return( -1 );
	}
/* TODO HAVE_LIBFVDE_MULTI_THREAD_SUPPORT */
	if( memory_copy(
	     internal_volume->keyring->volume_master_key,
	     volume_master_key,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy volume master key to keyring.",
		 function );

		return( -1 );
	}
	internal_volume->volume_master_key_is_set = 1;

	return( 1 );
}

/* Sets an UTF-8 formatted password
 * This function needs to be used before one of the open functions
 * Returns 1 if successful, 0 if password is invalid or -1 on error
 */
int libfvde_volume_set_utf8_password(
     libfvde_volume_t *volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_set_utf8_password";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfvde_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_volume->file_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume - file IO handle already set.",
		 function );

		return( -1 );
	}
/* TODO HAVE_LIBFVDE_MULTI_THREAD_SUPPORT */
	if( internal_volume->io_handle->user_password != NULL )
	{
		if( memory_set(
		     internal_volume->io_handle->user_password,
		     0,
		     internal_volume->io_handle->user_password_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear user password.",
			 function );

			return( -1 );
		}
		memory_free(
		 internal_volume->io_handle->user_password );

		internal_volume->io_handle->user_password      = NULL;
		internal_volume->io_handle->user_password_size = 0;
	}
	if( libuna_byte_stream_size_from_utf8(
	     utf8_string,
	     utf8_string_length,
	     LIBFVDE_CODEPAGE_US_ASCII,
	     &( internal_volume->io_handle->user_password_size ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set password size.",
		 function );

		goto on_error;
	}
	internal_volume->io_handle->user_password_size += 1;

	internal_volume->io_handle->user_password = (uint8_t *) memory_allocate(
	                                                         sizeof( uint8_t ) * internal_volume->io_handle->user_password_size );

	if( internal_volume->io_handle->user_password == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to create user password.",
		 function );

		goto on_error;
	}
	if( libuna_byte_stream_copy_from_utf8(
	     internal_volume->io_handle->user_password,
	     internal_volume->io_handle->user_password_size,
	     LIBFVDE_CODEPAGE_US_ASCII,
	     utf8_string,
	     utf8_string_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy user password.",
		 function );

		goto on_error;
	}
	internal_volume->io_handle->user_password[ internal_volume->io_handle->user_password_size - 1 ] = 0;

	internal_volume->io_handle->user_password_is_set = 1;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: user password: %s\n",
		 function,
		 internal_volume->io_handle->user_password );
	}
#endif
	return( 1 );

on_error:
	if( internal_volume->io_handle->user_password != NULL )
	{
		memory_set(
		 internal_volume->io_handle->user_password,
		 0,
		 internal_volume->io_handle->user_password_size );
		memory_free(
		 internal_volume->io_handle->user_password );

		internal_volume->io_handle->user_password = NULL;
	}
	internal_volume->io_handle->user_password_size = 0;

	return( -1 );
}

/* Sets an UTF-16 formatted password
 * This function needs to be used before one of the open functions
 * Returns 1 if successful, 0 if password is invalid or -1 on error
 */
int libfvde_volume_set_utf16_password(
     libfvde_volume_t *volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_set_utf16_password";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfvde_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_volume->file_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume - file IO handle already set.",
		 function );

		return( -1 );
	}
/* TODO HAVE_LIBFVDE_MULTI_THREAD_SUPPORT */
	if( internal_volume->io_handle->user_password != NULL )
	{
		if( memory_set(
		     internal_volume->io_handle->user_password,
		     0,
		     internal_volume->io_handle->user_password_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear user password.",
			 function );

			return( -1 );
		}
		memory_free(
		 internal_volume->io_handle->user_password );

		internal_volume->io_handle->user_password      = NULL;
		internal_volume->io_handle->user_password_size = 0;
	}
	if( libuna_byte_stream_size_from_utf16(
	     utf16_string,
	     utf16_string_length,
	     LIBFVDE_CODEPAGE_US_ASCII,
	     &( internal_volume->io_handle->user_password_size ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set password length.",
		 function );

		goto on_error;
	}
	internal_volume->io_handle->user_password_size += 1;

	internal_volume->io_handle->user_password = (uint8_t *) memory_allocate(
	                                                         sizeof( uint8_t ) * internal_volume->io_handle->user_password_size );

	if( internal_volume->io_handle->user_password == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to create user password.",
		 function );

		goto on_error;
	}
	if( libuna_byte_stream_copy_from_utf16(
	     internal_volume->io_handle->user_password,
	     internal_volume->io_handle->user_password_size,
	     LIBFVDE_CODEPAGE_US_ASCII,
	     utf16_string,
	     utf16_string_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy user password.",
		 function );

		goto on_error;
	}
	internal_volume->io_handle->user_password[ internal_volume->io_handle->user_password_size - 1 ] = 0;

	internal_volume->io_handle->user_password_is_set = 1;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: user password: %s\n",
		 function,
		 internal_volume->io_handle->user_password );
	}
#endif
	return( 1 );

on_error:
	if( internal_volume->io_handle->user_password != NULL )
	{
		memory_set(
		 internal_volume->io_handle->user_password,
		 0,
		 internal_volume->io_handle->user_password_size );
		memory_free(
		 internal_volume->io_handle->user_password );

		internal_volume->io_handle->user_password = NULL;
	}
	internal_volume->io_handle->user_password_size = 0;

	return( -1 );
}

/* Sets an UTF-8 formatted recovery password
 * This function needs to be used before one of the open functions
 * Returns 1 if successful, 0 if recovery password is invalid or -1 on error
 */
int libfvde_volume_set_utf8_recovery_password(
     libfvde_volume_t *volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_set_utf8_recovery_password";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfvde_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_volume->file_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume - file IO handle already set.",
		 function );

		return( -1 );
	}
/* TODO HAVE_LIBFVDE_MULTI_THREAD_SUPPORT */
	if( internal_volume->io_handle->recovery_password != NULL )
	{
		if( memory_set(
		     internal_volume->io_handle->recovery_password,
		     0,
		     internal_volume->io_handle->recovery_password_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear recovery password.",
			 function );

			return( -1 );
		}
		memory_free(
		 internal_volume->io_handle->recovery_password );

		internal_volume->io_handle->recovery_password      = NULL;
		internal_volume->io_handle->recovery_password_size = 0;
	}
	if( libuna_byte_stream_size_from_utf8(
	     utf8_string,
	     utf8_string_length,
	     LIBFVDE_CODEPAGE_US_ASCII,
	     &( internal_volume->io_handle->recovery_password_size ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set recovery pasword size.",
		 function );

		goto on_error;
	}
	internal_volume->io_handle->recovery_password_size += 1;

	internal_volume->io_handle->recovery_password = (uint8_t *) memory_allocate(
	                                                             sizeof( uint8_t ) * internal_volume->io_handle->recovery_password_size );

	if( internal_volume->io_handle->recovery_password == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to create recovery password.",
		 function );

		goto on_error;
	}
	if( libuna_byte_stream_copy_from_utf8(
	     internal_volume->io_handle->recovery_password,
	     internal_volume->io_handle->recovery_password_size,
	     LIBFVDE_CODEPAGE_US_ASCII,
	     utf8_string,
	     utf8_string_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy recovery password.",
		 function );

		goto on_error;
	}
	internal_volume->io_handle->recovery_password[ internal_volume->io_handle->recovery_password_size - 1 ] = 0;

	internal_volume->io_handle->recovery_password_is_set = 1;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: recovery password: %s\n",
		 function,
		 internal_volume->io_handle->recovery_password );
	}
#endif
	return( 1 );

on_error:
	if( internal_volume->io_handle->recovery_password != NULL )
	{
		memory_set(
		 internal_volume->io_handle->recovery_password,
		 0,
		 internal_volume->io_handle->recovery_password_size );
		memory_free(
		 internal_volume->io_handle->recovery_password );

		internal_volume->io_handle->recovery_password = NULL;
	}
	internal_volume->io_handle->recovery_password_size = 0;

	return( -1 );
}

/* Sets an UTF-16 formatted recovery password
 * This function needs to be used before one of the open functions
 * Returns 1 if successful, 0 if recovery password is invalid or -1 on error
 */
int libfvde_volume_set_utf16_recovery_password(
     libfvde_volume_t *volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                     = "libfvde_volume_set_utf16_recovery_password";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfvde_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_volume->file_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume - file IO handle already set.",
		 function );

		return( -1 );
	}
/* TODO HAVE_LIBFVDE_MULTI_THREAD_SUPPORT */
	if( internal_volume->io_handle->recovery_password != NULL )
	{
		if( memory_set(
		     internal_volume->io_handle->recovery_password,
		     0,
		     internal_volume->io_handle->recovery_password_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear recovery password.",
			 function );

			return( -1 );
		}
		memory_free(
		 internal_volume->io_handle->recovery_password );

		internal_volume->io_handle->recovery_password      = NULL;
		internal_volume->io_handle->recovery_password_size = 0;
	}
	if( libuna_byte_stream_size_from_utf16(
	     utf16_string,
	     utf16_string_length,
	     LIBFVDE_CODEPAGE_US_ASCII,
	     &(internal_volume->io_handle->recovery_password_size ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set recovery size.",
		 function );

		goto on_error;
	}
	internal_volume->io_handle->recovery_password_size += 1;

	internal_volume->io_handle->recovery_password = (uint8_t *) memory_allocate(
	                                                             sizeof( uint8_t ) * internal_volume->io_handle->recovery_password_size );

	if( internal_volume->io_handle->recovery_password == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to create recovery password.",
		 function );

		goto on_error;
	}
	if( libuna_byte_stream_copy_from_utf16(
	     internal_volume->io_handle->recovery_password,
	     internal_volume->io_handle->recovery_password_size,
	     LIBFVDE_CODEPAGE_US_ASCII,
	     utf16_string,
	     utf16_string_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy recovery password.",
		 function );

		goto on_error;
	}
	internal_volume->io_handle->recovery_password[ internal_volume->io_handle->recovery_password_size - 1 ] = 0;

	internal_volume->io_handle->recovery_password_is_set = 1;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: recovery password: %s\n",
		 function,
		 internal_volume->io_handle->recovery_password );
	}
#endif
	return( 1 );

on_error:
	if( internal_volume->io_handle->recovery_password != NULL )
	{
		memory_set(
		 internal_volume->io_handle->recovery_password,
		 0,
		 internal_volume->io_handle->recovery_password_size );
		memory_free(
		 internal_volume->io_handle->recovery_password );

		internal_volume->io_handle->recovery_password = NULL;
	}
	internal_volume->io_handle->recovery_password_size = 0;

	return( -1 );
}

/* Reads the EncryptedRoot.plist file
 * This function needs to be used before one of the open functions.
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_read_encrypted_root_plist(
     libfvde_volume_t *volume,
     const char *filename,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libfvde_volume_read_encrypted_root_plist";
	size_t filename_length           = 0;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
/* TODO HAVE_LIBFVDE_MULTI_THREAD_SUPPORT */
	filename_length = narrow_string_length(
	                   filename );

	if( filename_length == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		goto on_error;
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
	if( libbfio_file_set_name(
	     file_io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set filename in file IO handle.",
		 function );

		goto on_error;
	}
	if( libfvde_volume_read_encrypted_root_plist_file_io_handle(
	     volume,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read encrypted root plist file: %s.",
		 function,
		 filename );

		goto on_error;
	}
	if( libbfio_handle_free(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file IO handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Reads the EncryptedRoot.plist file
 * This function needs to be used before one of the open functions.
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_read_encrypted_root_plist_wide(
     libfvde_volume_t *volume,
     const wchar_t *filename,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	static char *function            = "libfvde_volume_read_encrypted_root_plist_wide";
	size_t filename_length           = 0;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
/* TODO HAVE_LIBFVDE_MULTI_THREAD_SUPPORT */
	filename_length = wide_string_length(
	                   filename );

	if( filename_length == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		goto on_error;
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
	if( libbfio_file_set_name_wide(
	     file_io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set filename in file IO handle.",
		 function );

		goto on_error;
	}
	if( libfvde_volume_read_encrypted_root_plist_file_io_handle(
	     volume,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read encrypted root plist file: %s.",
		 function,
		 filename );

		goto on_error;
	}
	if( libbfio_handle_free(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file IO handle.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Reads the EncryptedRoot.plist file using a Basic File IO (bfio) handle
 * This function needs to be used before one of the open functions.
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_read_encrypted_root_plist_file_io_handle(
     libfvde_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_read_encrypted_root_plist_file_io_handle";

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	internal_volume = (libfvde_internal_volume_t *) volume;

	if( internal_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_volume->file_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume - file IO handle already set.",
		 function );

		return( -1 );
	}
/* TODO HAVE_LIBFVDE_MULTI_THREAD_SUPPORT */
	if( internal_volume->encrypted_root_plist != NULL )
	{
		if( libfvde_encryption_context_plist_free(
		     &( internal_volume->encrypted_root_plist ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free encrypted root plist.",
			 function );

			goto on_error;
		}
	}
	if( libfvde_encryption_context_plist_initialize(
	     &( internal_volume->encrypted_root_plist ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create encrypted root plist.",
		 function );

		goto on_error;
	}
	if( libfvde_encryption_context_plist_read_file_io_handle(
	     internal_volume->encrypted_root_plist,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_GENERIC,
		 "%s: unable to read encrypted root plist.",
		 function );

		goto on_error;
	}
	internal_volume->encrypted_root_plist_file_is_set = 1;

	return( 1 );

on_error:
	if( internal_volume->encrypted_root_plist != NULL )
	{
		libfvde_encryption_context_plist_free(
		 &( internal_volume->encrypted_root_plist ),
		 NULL );
	}
	return( -1 );
}

