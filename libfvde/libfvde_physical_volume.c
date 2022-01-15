/*
 * Physical volume functions
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
#include <memory.h>
#include <types.h>

#include "libfvde_libcerror.h"
#include "libfvde_libcthreads.h"
#include "libfvde_physical_volume.h"
#include "libfvde_physical_volume_descriptor.h"
#include "libfvde_types.h"
#include "libfvde_volume_header.h"

/* Creates a physical volume
 * Make sure the value physical_volume is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfvde_physical_volume_initialize(
     libfvde_physical_volume_t **physical_volume,
     libfvde_volume_header_t *volume_header,
     libfvde_physical_volume_descriptor_t *physical_volume_descriptor,
     libcerror_error_t **error )
{
	libfvde_internal_physical_volume_t *internal_physical_volume = NULL;
	static char *function                                        = "libfvde_physical_volume_initialize";

	if( physical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid physical volume.",
		 function );

		return( -1 );
	}
	if( *physical_volume != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid physical volume value already set.",
		 function );

		return( -1 );
	}
	if( volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume header.",
		 function );

		return( -1 );
	}
	if( physical_volume_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid physical volume descriptor.",
		 function );

		return( -1 );
	}
	internal_physical_volume = memory_allocate_structure(
	                            libfvde_internal_physical_volume_t );

	if( internal_physical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create physical volume.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_physical_volume,
	     0,
	     sizeof( libfvde_internal_physical_volume_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear physical volume.",
		 function );

		memory_free(
		 internal_physical_volume );

		return( -1 );
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_physical_volume->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize read/write lock.",
		 function );

		goto on_error;
	}
#endif
	internal_physical_volume->volume_header              = volume_header;
	internal_physical_volume->physical_volume_descriptor = physical_volume_descriptor;

	*physical_volume = (libfvde_physical_volume_t *) internal_physical_volume;

	return( 1 );

on_error:
	if( internal_physical_volume != NULL )
	{
		memory_free(
		 internal_physical_volume );
	}
	return( -1 );
}

/* Frees a physical volume
 * Returns 1 if successful or -1 on error
 */
int libfvde_physical_volume_free(
     libfvde_physical_volume_t **physical_volume,
     libcerror_error_t **error )
{
	libfvde_internal_physical_volume_t *internal_physical_volume = NULL;
	static char *function                                        = "libfvde_physical_volume_free";
	int result                                                   = 1;

	if( physical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid physical volume.",
		 function );

		return( -1 );
	}
	if( *physical_volume != NULL )
	{
		internal_physical_volume = (libfvde_internal_physical_volume_t *) *physical_volume;
		*physical_volume         = NULL;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_physical_volume->read_write_lock ),
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
		/* The volume_header and physical_volume_descriptor references are freed elsewhere
		 */
		memory_free(
		 internal_physical_volume );
	}
	return( result );
}

/* Retrieves the physical volume identifier
 * The identifier is a UUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
int libfvde_physical_volume_get_identifier(
     libfvde_physical_volume_t *physical_volume,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libcerror_error_t **error )
{
	libfvde_internal_physical_volume_t *internal_physical_volume = NULL;
	static char *function                                        = "libfvde_physical_volume_get_identifier";
	int result                                                   = 1;

	if( physical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid physical volume.",
		 function );

		return( -1 );
	}
	internal_physical_volume = (libfvde_internal_physical_volume_t *) physical_volume;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_physical_volume->read_write_lock,
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
	if( libfvde_physical_volume_descriptor_get_identifier(
	     internal_physical_volume->physical_volume_descriptor,
	     uuid_data,
	     uuid_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve physical volume identifier from descriptor.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_physical_volume->read_write_lock,
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

/* Retrieves the volume encryption method
 * Returns 1 if successful or -1 on error
 */
int libfvde_physical_volume_get_encryption_method(
     libfvde_physical_volume_t *physical_volume,
     uint32_t *encryption_method,
     libcerror_error_t **error )
{
	libfvde_internal_physical_volume_t *internal_physical_volume = NULL;
	static char *function                                        = "libfvde_physical_volume_get_encryption_method";
	int result                                                   = 1;

	if( physical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid physical volume.",
		 function );

		return( -1 );
	}
	internal_physical_volume = (libfvde_internal_physical_volume_t *) physical_volume;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_physical_volume->read_write_lock,
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
	     internal_physical_volume->volume_header,
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
	     internal_physical_volume->read_write_lock,
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

/* Retrieves the volume size
 * Returns 1 if successful or -1 on error
 */
int libfvde_physical_volume_get_size(
     libfvde_physical_volume_t *physical_volume,
     size64_t *size,
     libcerror_error_t **error )
{
	libfvde_internal_physical_volume_t *internal_physical_volume = NULL;
	static char *function                                        = "libfvde_physical_volume_get_size";
	int result                                                   = 1;

	if( physical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid physical volume.",
		 function );

		return( -1 );
	}
	internal_physical_volume = (libfvde_internal_physical_volume_t *) physical_volume;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_physical_volume->read_write_lock,
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
	     internal_physical_volume->volume_header,
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
	     internal_physical_volume->read_write_lock,
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

