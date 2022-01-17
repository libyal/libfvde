/*
 * Legacy functions
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
#include <memory.h>
#include <types.h>

#include "libfvde_debug.h"
#include "libfvde_io_handle.h"
#include "libfvde_libcerror.h"
#include "libfvde_libcnotify.h"
#include "libfvde_password.h"
#include "libfvde_types.h"
#include "libfvde_volume.h"

/* Unlocks the volume
 * Returns 1 if the volume is unlocked, 0 if not or -1 on error
 */
int libfvde_volume_unlock(
     libfvde_volume_t *volume,
     libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_unlock";
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
	result = libfvde_logical_volume_unlock(
	          internal_volume->legacy_logical_volume,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to unlock volume.",
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

/* Determines if the volume is locked
 * Returns 1 if locked, 0 if not or -1 on error
 */
int libfvde_volume_is_locked(
     libfvde_volume_t *volume,
     libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_is_locked";
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
	result = libfvde_logical_volume_is_locked(
	          internal_volume->legacy_logical_volume,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if logical volume is locked.",
		 function );

		result = -1;
	}
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
	return( result );
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
	read_count = libfvde_logical_volume_read_buffer(
		      internal_volume->legacy_logical_volume,
		      buffer,
		      buffer_size,
		      error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer from logical volume.",
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

/* Reads data at a specific offset
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
	read_count = libfvde_logical_volume_read_buffer_at_offset(
		      internal_volume->legacy_logical_volume,
		      buffer,
		      buffer_size,
		      offset,
		      error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer from logical volume.",
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
	offset = libfvde_logical_volume_seek_offset(
	          internal_volume->legacy_logical_volume,
	          offset,
	          whence,
	          error );

	if( offset == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset in logical volume.",
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

/* Retrieves the current offset of the data
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_get_offset(
     libfvde_volume_t *volume,
     off64_t *offset,
     libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_get_offset";
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
	if( libfvde_logical_volume_get_offset(
	     internal_volume->legacy_logical_volume,
	     offset,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve offset from logical volume.",
		 function );

		result = -1;
	}
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
	return( result );
}

/* Retrieves the size
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_get_size(
     libfvde_volume_t *volume,
     size64_t *size,
     libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_get_size";
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
	if( libfvde_logical_volume_get_size(
	     internal_volume->legacy_logical_volume,
	     size,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size from logical volume.",
		 function );

		result = -1;
	}
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
	return( result );
}

/* Retrieves the encryption method of the physical volume
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_get_encryption_method(
     libfvde_volume_t *volume,
     uint32_t *encryption_method,
     libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_get_encryption_method";

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

	if( internal_volume->volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume - missing volume header.",
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
	*encryption_method = internal_volume->volume_header->physical_volume_encryption_method;

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

/* Retrieves the logical volume group identifier
 * The identifier is a UUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_get_logical_volume_group_identifier(
     libfvde_volume_t *volume,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_get_logical_volume_group_identifier";
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
	if( libfvde_volume_header_get_logical_volume_group_identifier(
	     internal_volume->volume_header,
	     uuid_data,
	     uuid_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve logical volume group identifier from volume header.",
		 function );

		result = -1;
	}
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
	return( result );
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
	if( libfvde_volume_header_get_physical_volume_size(
	     internal_volume->volume_header,
	     size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve physical volume size from volume header.",
		 function );

		result = -1;
	}
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
	return( result );
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
	if( libfvde_volume_header_get_physical_volume_encryption_method(
	     internal_volume->volume_header,
	     encryption_method,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve physical volume encryption method from volume header.",
		 function );

		result = -1;
	}
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
	return( result );
}

/* Retrieves the physical volume identifier
 * The identifier is a UUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_get_physical_volume_identifier(
     libfvde_volume_t *volume,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_get_physical_volume_identifier";
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
	if( libfvde_volume_header_get_physical_volume_identifier(
	     internal_volume->volume_header,
	     uuid_data,
	     uuid_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve physical volume identifier from volume header.",
		 function );

		result = -1;
	}
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
	return( result );
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
	if( libfvde_logical_volume_get_size(
	     internal_volume->legacy_logical_volume,
	     size,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size from logical volume.",
		 function );

		result = -1;
	}
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
	return( result );
}

/* Retrieves the logical volume identifier
 * The identifier is a UUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_get_logical_volume_identifier(
     libfvde_volume_t *volume,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_get_logical_volume_identifier";
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
	if( libfvde_logical_volume_get_identifier(
	     internal_volume->legacy_logical_volume,
	     uuid_data,
	     uuid_data_size,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve identifier from logical volume.",
		 function );

		result = -1;
	}
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
	return( result );
}

/* Sets the keys
 * This function needs to be used before one of the open or unlock functions
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
	if( libfvde_logical_volume_set_keys(
	     internal_volume->legacy_logical_volume,
	     volume_master_key,
	     volume_master_key_size,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set keys in logical volume.",
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

/* Sets an UTF-8 formatted password
 * This function needs to be used before the unlock function
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
	if( internal_volume->legacy_user_password != NULL )
	{
		if( memory_set(
		     internal_volume->legacy_user_password,
		     0,
		     internal_volume->legacy_user_password_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear user password.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_volume->legacy_user_password );

		internal_volume->legacy_user_password      = NULL;
		internal_volume->legacy_user_password_size = 0;
	}
	if( result == 1 )
	{
		if( libfvde_password_copy_from_utf8_string(
		     &( internal_volume->legacy_user_password ),
		     &( internal_volume->legacy_user_password_size ),
		     utf8_string,
		     utf8_string_length,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set user password.",
			 function );

			result = -1;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		else if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: user password: %s\n",
			 function,
			 internal_volume->legacy_user_password );
		}
#endif
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

/* Sets an UTF-16 formatted password
 * This function needs to be used before the unlock function
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
	if( internal_volume->legacy_user_password != NULL )
	{
		if( memory_set(
		     internal_volume->legacy_user_password,
		     0,
		     internal_volume->legacy_user_password_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear user password.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_volume->legacy_user_password );

		internal_volume->legacy_user_password      = NULL;
		internal_volume->legacy_user_password_size = 0;
	}
	if( result == 1 )
	{
		if( libfvde_password_copy_from_utf16_string(
		     &( internal_volume->legacy_user_password ),
		     &( internal_volume->legacy_user_password_size ),
		     utf16_string,
		     utf16_string_length,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set user password.",
			 function );

			result = -1;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		else if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: user password: %s\n",
			 function,
			 internal_volume->legacy_user_password );
		}
#endif
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

/* Sets an UTF-8 formatted recovery password
 * This function needs to be used before the unlock function
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
	if( internal_volume->legacy_recovery_password != NULL )
	{
		if( memory_set(
		     internal_volume->legacy_recovery_password,
		     0,
		     internal_volume->legacy_recovery_password_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear recovery password.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_volume->legacy_recovery_password );

		internal_volume->legacy_recovery_password      = NULL;
		internal_volume->legacy_recovery_password_size = 0;
	}
	if( result == 1 )
	{
		if( libfvde_password_copy_from_utf8_string(
		     &( internal_volume->legacy_recovery_password ),
		     &( internal_volume->legacy_recovery_password_size ),
		     utf8_string,
		     utf8_string_length,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set recovery password.",
			 function );

			result = -1;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		else if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: recovery password: %s\n",
			 function,
			 internal_volume->legacy_recovery_password );
		}
#endif
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

/* Sets an UTF-16 formatted recovery password
 * This function needs to be used before the unlock function
 * Returns 1 if successful, 0 if recovery password is invalid or -1 on error
 */
int libfvde_volume_set_utf16_recovery_password(
     libfvde_volume_t *volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	libfvde_internal_volume_t *internal_volume = NULL;
	static char *function                      = "libfvde_volume_set_utf16_recovery_password";
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
	if( internal_volume->legacy_recovery_password != NULL )
	{
		if( memory_set(
		     internal_volume->legacy_recovery_password,
		     0,
		     internal_volume->legacy_recovery_password_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear recovery password.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_volume->legacy_recovery_password );

		internal_volume->legacy_recovery_password      = NULL;
		internal_volume->legacy_recovery_password_size = 0;
	}
	if( result == 1 )
	{
		if( libfvde_password_copy_from_utf16_string(
		     &( internal_volume->legacy_recovery_password ),
		     &( internal_volume->legacy_recovery_password_size ),
		     utf16_string,
		     utf16_string_length,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set recovery password.",
			 function );

			result = -1;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		else if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: recovery password: %s\n",
			 function,
			 internal_volume->legacy_recovery_password );
		}
#endif
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

