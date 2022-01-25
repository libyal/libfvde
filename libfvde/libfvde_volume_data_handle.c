/*
 * Volume data handle functions
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

#include "libfvde_definitions.h"
#include "libfvde_encryption_context.h"
#include "libfvde_io_handle.h"
#include "libfvde_libbfio.h"
#include "libfvde_libcerror.h"
#include "libfvde_libfdata.h"
#include "libfvde_sector_data.h"
#include "libfvde_volume_data_handle.h"
#include "libfvde_unused.h"

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

/* Reads a sector
 * Callback function for the volume vector
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_data_handle_read_sector(
     libfvde_volume_data_handle_t *volume_data_handle,
     libbfio_pool_t *file_io_pool,
     libfdata_vector_t *vector,
     libfdata_cache_t *cache,
     int element_index,
     int element_data_file_index,
     off64_t element_data_offset,
     size64_t element_data_size LIBFVDE_ATTRIBUTE_UNUSED,
     uint32_t element_data_flags,
     uint8_t read_flags LIBFVDE_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libfvde_sector_data_t *sector_data = NULL;
	static char *function              = "libfvde_volume_data_handle_read_sector";

	LIBFVDE_UNREFERENCED_PARAMETER( element_data_size );
	LIBFVDE_UNREFERENCED_PARAMETER( read_flags );

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
	if( volume_data_handle->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid volume data handle - missing IO handle.",
		 function );

		return( -1 );
	}
	if( libfvde_sector_data_initialize(
	     &sector_data,
	     (size_t) volume_data_handle->io_handle->bytes_per_sector,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sector data.",
		 function );

		goto on_error;
	}
	if( ( element_data_flags & LIBFVDE_RANGE_FLAG_IS_SPARSE ) != 0 )
	{
		if( memory_set(
		     sector_data->data,
		     0,
		     sector_data->data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear sector data.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( libfvde_sector_data_read(
		     sector_data,
		     volume_data_handle->encryption_context,
		     file_io_pool,
		     element_data_file_index,
		     element_data_offset,
		     (uint64_t) element_index,
		     volume_data_handle->is_encrypted,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read sector data.",
			 function );

			goto on_error;
		}
	}
	if( libfdata_vector_set_element_value_by_index(
	     vector,
	     (intptr_t *) file_io_pool,
	     cache,
	     element_index,
	     (intptr_t *) sector_data,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfvde_sector_data_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set sector data as element value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( sector_data != NULL )
	{
		libfvde_sector_data_free(
		 &sector_data,
		 NULL );
	}
	return( -1 );
}

