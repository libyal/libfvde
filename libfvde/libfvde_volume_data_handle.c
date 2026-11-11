/*
 * Volume data handle functions
 *
 * Copyright (C) 2011-2026, Omar Choudary <choudary.omar@gmail.com>
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

#include "libfvde_definitions.h"
#include "libfvde_encryption_context.h"
#include "libfvde_io_handle.h"
#include "libfvde_libbfio.h"
#include "libfvde_libcerror.h"
#include "libfvde_volume_data_handle.h"

/* Creates volume data handle
 * Make sure the value volume_data_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_data_handle_initialize(
     libfvde_volume_data_handle_t **volume_data_handle,
     libfvde_io_handle_t *io_handle,
     off64_t logical_volume_offset,
     libcerror_error_t **error )
{
	static char *function = "libfvde_volume_data_handle_initialize";

	if( volume_data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume data handle.",
		 function );

		return( -1 );
	}
	if( *volume_data_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume data handle value already set.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	*volume_data_handle = memory_allocate_structure(
	                       libfvde_volume_data_handle_t );

	if( *volume_data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to volume data handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *volume_data_handle,
	     0,
	     sizeof( libfvde_volume_data_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear volume data handle.",
		 function );

		memory_free(
		 *volume_data_handle );

		*volume_data_handle = NULL;

		return( -1 );
	}
	( *volume_data_handle )->io_handle             = io_handle;
	( *volume_data_handle )->logical_volume_offset = logical_volume_offset;

	return( 1 );

on_error:
	if( *volume_data_handle != NULL )
	{
		memory_free(
		 *volume_data_handle );

		*volume_data_handle = NULL;
	}
	return( -1 );
}

/* Frees volume data handle
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_data_handle_free(
     libfvde_volume_data_handle_t **volume_data_handle,
     libcerror_error_t **error )
{
	static char *function = "libfvde_volume_data_handle_free";
	int result            = 1;

	if( volume_data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume data handle.",
		 function );

		return( -1 );
	}
	if( *volume_data_handle != NULL )
	{
		if( ( *volume_data_handle )->encryption_context != NULL )
		{
			if( libfvde_encryption_context_free(
			     &( ( *volume_data_handle )->encryption_context ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free encryption context.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *volume_data_handle );

		*volume_data_handle = NULL;
	}
	return( result );
}

