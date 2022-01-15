/*
 * Logical volume functions
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
#include "libfvde_logical_volume.h"
#include "libfvde_logical_volume_descriptor.h"
#include "libfvde_types.h"

/* Creates a logical volume
 * Make sure the value logical_volume is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfvde_logical_volume_initialize(
     libfvde_logical_volume_t **logical_volume,
     libfvde_logical_volume_descriptor_t *logical_volume_descriptor,
     libcerror_error_t **error )
{
	libfvde_internal_logical_volume_t *internal_logical_volume = NULL;
	static char *function                                      = "libfvde_logical_volume_initialize";

	if( logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	if( *logical_volume != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid logical volume value already set.",
		 function );

		return( -1 );
	}
	if( logical_volume_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume descriptor.",
		 function );

		return( -1 );
	}
	internal_logical_volume = memory_allocate_structure(
	                           libfvde_internal_logical_volume_t );

	if( internal_logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create logical volume.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_logical_volume,
	     0,
	     sizeof( libfvde_internal_logical_volume_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear logical volume.",
		 function );

		memory_free(
		 internal_logical_volume );

		return( -1 );
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_logical_volume->read_write_lock ),
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
	internal_logical_volume->logical_volume_descriptor = logical_volume_descriptor;

	*logical_volume = (libfvde_logical_volume_t *) internal_logical_volume;

	return( 1 );

on_error:
	if( internal_logical_volume != NULL )
	{
		memory_free(
		 internal_logical_volume );
	}
	return( -1 );
}

/* Frees a logical volume
 * Returns 1 if successful or -1 on error
 */
int libfvde_logical_volume_free(
     libfvde_logical_volume_t **logical_volume,
     libcerror_error_t **error )
{
	libfvde_internal_logical_volume_t *internal_logical_volume = NULL;
	static char *function                                      = "libfvde_logical_volume_free";
	int result                                                 = 1;

	if( logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	if( *logical_volume != NULL )
	{
		internal_logical_volume = (libfvde_internal_logical_volume_t *) *logical_volume;
		*logical_volume         = NULL;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_logical_volume->read_write_lock ),
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
		/* The logical_volume_descriptor reference is freed elsewhere
		 */
		memory_free(
		 internal_logical_volume );
	}
	return( result );
}

/* Retrieves the logical volume identifier
 * The identifier is a UUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
int libfvde_logical_volume_get_identifier(
     libfvde_logical_volume_t *logical_volume,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libcerror_error_t **error )
{
	libfvde_internal_logical_volume_t *internal_logical_volume = NULL;
	static char *function                                      = "libfvde_logical_volume_get_identifier";
	int result                                                 = 1;

	if( logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	internal_logical_volume = (libfvde_internal_logical_volume_t *) logical_volume;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_logical_volume->read_write_lock,
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
	if( libfvde_logical_volume_descriptor_get_identifier(
	     internal_logical_volume->logical_volume_descriptor,
	     uuid_data,
	     uuid_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve logical volume identifier from descriptor.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_logical_volume->read_write_lock,
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

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfvde_logical_volume_get_utf8_name_size(
     libfvde_logical_volume_t *logical_volume,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libfvde_internal_logical_volume_t *internal_logical_volume = NULL;
	static char *function                                      = "libfvde_logical_volume_get_utf8_name_size";
	int result                                                 = 1;

	if( logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume group.",
		 function );

		return( -1 );
	}
	internal_logical_volume = (libfvde_internal_logical_volume_t *) logical_volume;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_logical_volume->read_write_lock,
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
	if( libfvde_logical_volume_descriptor_get_utf8_name_size(
	     internal_logical_volume->logical_volume_descriptor,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-8 volume group name from descriptor.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_logical_volume->read_write_lock,
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

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfvde_logical_volume_get_utf8_name(
     libfvde_logical_volume_t *logical_volume,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libfvde_internal_logical_volume_t *internal_logical_volume = NULL;
	static char *function                                      = "libfvde_logical_volume_get_utf8_name";
	int result                                                 = 1;

	if( logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume group.",
		 function );

		return( -1 );
	}
	internal_logical_volume = (libfvde_internal_logical_volume_t *) logical_volume;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_logical_volume->read_write_lock,
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
	if( libfvde_logical_volume_descriptor_get_utf8_name(
	     internal_logical_volume->logical_volume_descriptor,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 volume group name from descriptor.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_logical_volume->read_write_lock,
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

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfvde_logical_volume_get_utf16_name_size(
     libfvde_logical_volume_t *logical_volume,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libfvde_internal_logical_volume_t *internal_logical_volume = NULL;
	static char *function                                      = "libfvde_logical_volume_get_utf16_name_size";
	int result                                                 = 1;

	if( logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume group.",
		 function );

		return( -1 );
	}
	internal_logical_volume = (libfvde_internal_logical_volume_t *) logical_volume;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_logical_volume->read_write_lock,
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
	if( libfvde_logical_volume_descriptor_get_utf16_name_size(
	     internal_logical_volume->logical_volume_descriptor,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-16 volume group name from descriptor.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_logical_volume->read_write_lock,
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

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libfvde_logical_volume_get_utf16_name(
     libfvde_logical_volume_t *logical_volume,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libfvde_internal_logical_volume_t *internal_logical_volume = NULL;
	static char *function                                      = "libfvde_logical_volume_get_utf16_name";
	int result                                                 = 1;

	if( logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume group.",
		 function );

		return( -1 );
	}
	internal_logical_volume = (libfvde_internal_logical_volume_t *) logical_volume;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_logical_volume->read_write_lock,
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
	if( libfvde_logical_volume_descriptor_get_utf16_name(
	     internal_logical_volume->logical_volume_descriptor,
	     utf16_string,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 volume group name from descriptor.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_logical_volume->read_write_lock,
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

/* Retrieves the volume encryption method
 * Returns 1 if successful or -1 on error
 */
int libfvde_logical_volume_get_encryption_method(
     libfvde_logical_volume_t *logical_volume,
     uint32_t *encryption_method,
     libcerror_error_t **error )
{
	libfvde_internal_logical_volume_t *internal_logical_volume = NULL;
	static char *function                                      = "libfvde_logical_volume_get_encryption_method";
	int result                                                 = 1;

	if( logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	internal_logical_volume = (libfvde_internal_logical_volume_t *) logical_volume;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_logical_volume->read_write_lock,
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
	/* TODO implement */

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_logical_volume->read_write_lock,
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
int libfvde_logical_volume_get_size(
     libfvde_logical_volume_t *logical_volume,
     size64_t *size,
     libcerror_error_t **error )
{
	libfvde_internal_logical_volume_t *internal_logical_volume = NULL;
	static char *function                                      = "libfvde_logical_volume_get_size";
	int result                                                 = 1;

	if( logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	internal_logical_volume = (libfvde_internal_logical_volume_t *) logical_volume;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_logical_volume->read_write_lock,
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
	if( libfvde_logical_volume_descriptor_get_size(
	     internal_logical_volume->logical_volume_descriptor,
	     size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve logical volume size from descriptor.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_logical_volume->read_write_lock,
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

