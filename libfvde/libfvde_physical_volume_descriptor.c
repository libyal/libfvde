/*
 * Physical volume descriptor functions
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

#include "libfvde_libcerror.h"
#include "libfvde_physical_volume_descriptor.h"

/* Creates physical volume descriptor
 * Make sure the value physical_volume_descriptor is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfvde_physical_volume_descriptor_initialize(
     libfvde_physical_volume_descriptor_t **physical_volume_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfvde_physical_volume_descriptor_initialize";

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
	if( *physical_volume_descriptor != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid physical volume descriptor value already set.",
		 function );

		return( -1 );
	}
	*physical_volume_descriptor = memory_allocate_structure(
	                               libfvde_physical_volume_descriptor_t );

	if( *physical_volume_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to physical volume descriptor.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *physical_volume_descriptor,
	     0,
	     sizeof( libfvde_physical_volume_descriptor_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear physical volume descriptor.",
		 function );

		memory_free(
		 *physical_volume_descriptor );

		*physical_volume_descriptor = NULL;

		return( -1 );
	}
	return( 1 );

on_error:
	if( *physical_volume_descriptor != NULL )
	{
		memory_free(
		 *physical_volume_descriptor );

		*physical_volume_descriptor = NULL;
	}
	return( -1 );
}

/* Frees physical volume descriptor
 * Returns 1 if successful or -1 on error
 */
int libfvde_physical_volume_descriptor_free(
     libfvde_physical_volume_descriptor_t **physical_volume_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfvde_physical_volume_descriptor_free";

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
	if( *physical_volume_descriptor != NULL )
	{
		memory_free(
		 *physical_volume_descriptor );

		*physical_volume_descriptor = NULL;
	}
	return( 1 );
}

/* Retrieves the identifier
 * The identifier is a UUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
int libfvde_physical_volume_descriptor_get_identifier(
     libfvde_physical_volume_descriptor_t *physical_volume_descriptor,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libcerror_error_t **error )
{
	static char *function = "libfvde_physical_volume_descriptor_get_identifier";

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
	if( uuid_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UUID data.",
		 function );

		return( -1 );
	}
	if( ( uuid_data_size < 16 )
	 || ( uuid_data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UUID data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     uuid_data,
	     physical_volume_descriptor->identifier,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy identifier.",
		 function );

		return( -1 );
	}
	return( 1 );
}

