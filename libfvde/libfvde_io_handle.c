/*
 * Input/Output (IO) handle functions
 *
 * Copyright (C) 2011-2021, Omar Choudary <choudary.omar@gmail.com>
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
#include <byte_stream.h>
#include <memory.h>
#include <system_string.h>
#include <types.h>

#include "libfvde_checksum.h"
#include "libfvde_debug.h"
#include "libfvde_definitions.h"
#include "libfvde_encryption.h"
#include "libfvde_io_handle.h"
#include "libfvde_libbfio.h"
#include "libfvde_libcerror.h"
#include "libfvde_libcnotify.h"
#include "libfvde_libfdata.h"
#include "libfvde_libfguid.h"
#include "libfvde_sector_data.h"
#include "libfvde_unused.h"

#include "fvde_volume.h"

const char *libfvde_core_storage_signature = "CS";

/* Creates an IO handle
 * Make sure the value io_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfvde_io_handle_initialize(
     libfvde_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libfvde_io_handle_initialize";

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
	if( *io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid IO handle value already set.",
		 function );

		return( -1 );
	}
	*io_handle = memory_allocate_structure(
	              libfvde_io_handle_t );

	if( *io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create IO handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *io_handle,
	     0,
	     sizeof( libfvde_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		goto on_error;
	}
	( *io_handle )->bytes_per_sector = 512;

	return( 1 );

on_error:
	if( *io_handle != NULL )
	{
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( -1 );
}

/* Frees an IO handle
 * Returns 1 if successful or -1 on error
 */
int libfvde_io_handle_free(
     libfvde_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libfvde_io_handle_free";
	int result            = 1;

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
	if( *io_handle != NULL )
	{
		if( libfvde_io_handle_clear(
		     *io_handle,
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
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( result );
}

/* Clears the IO handle
 * Returns 1 if successful or -1 on error
 */
int libfvde_io_handle_clear(
     libfvde_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function = "libfvde_io_handle_clear";
	int result            = 1;

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
	if( io_handle->xts_context != NULL )
	{
		if( libcaes_tweaked_context_free(
		     &( io_handle->xts_context ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free XTS context.",
			 function );

			result = -1;
		}
	}
	if( io_handle->user_password != NULL )
	{
		if( memory_set(
		     io_handle->user_password,
		     0,
		     io_handle->user_password_size ) == NULL )
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
		 io_handle->user_password );

		io_handle->user_password      = NULL;
		io_handle->user_password_size = 0;
	}
	if( io_handle->recovery_password != NULL )
	{
		if( memory_set(
		     io_handle->recovery_password,
		     0,
		     io_handle->recovery_password_size ) == NULL )
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
		 io_handle->recovery_password );

		io_handle->recovery_password      = NULL;
		io_handle->recovery_password_size = 0;
	}
	if( memory_set(
	     io_handle,
	     0,
	     sizeof( libfvde_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		result = -1;
	}
	io_handle->bytes_per_sector = 512;

	return( result );
}

/* Reads a sector
 * Callback function for the volume vector
 * Returns 1 if successful or -1 on error
 */
int libfvde_io_handle_read_sector(
     libfvde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfdata_cache_t *cache,
     int element_index,
     int element_data_file_index LIBFVDE_ATTRIBUTE_UNUSED,
     off64_t element_data_offset,
     size64_t element_data_size LIBFVDE_ATTRIBUTE_UNUSED,
     uint32_t element_data_flags,
     uint8_t read_flags LIBFVDE_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libfvde_sector_data_t *sector_data = NULL;
	static char *function              = "libfvde_io_handle_read_sector";

	LIBFVDE_UNREFERENCED_PARAMETER( element_data_file_index );
	LIBFVDE_UNREFERENCED_PARAMETER( element_data_size );
	LIBFVDE_UNREFERENCED_PARAMETER( read_flags );

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
/* TODO handle virtual sectors, what about different sector sizes? */
	if( libfvde_sector_data_initialize(
	     &sector_data,
	     (size_t) ( (libfvde_io_handle_t *) io_handle )->bytes_per_sector,
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
	if( libfvde_sector_data_read(
	     sector_data,
	     (libfvde_io_handle_t *) io_handle,
	     file_io_handle,
	     element_data_offset,
	     ( (libfvde_io_handle_t *) io_handle )->xts_context,
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
	if( element_data_flags == LIBFVDE_RANGE_FLAG_ENCRYPTED )
	{
/* TODO decrypt */
	}
	if( libfdata_vector_set_element_value_by_index(
	     vector,
	     (intptr_t *) file_io_handle,
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

/* Reads the logical volume header
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libfvde_io_handle_read_logical_volume_header(
     libfvde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	libfvde_sector_data_t *sector_data = NULL;
	static char *function              = "libfvde_io_handle_read_logical_volume_header";
	uint32_t block_size                = 0;
	uint32_t number_of_blocks          = 0;
	uint16_t volume_signature          = 0;
	int result                         = 0;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading logical volume header at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	if( libfvde_sector_data_initialize(
	     &sector_data,
	     io_handle->bytes_per_sector,
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
	if( libfvde_sector_data_read(
	     sector_data,
	     io_handle,
	     file_io_handle,
	     file_offset,
	     io_handle->xts_context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume header sector data.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint16_big_endian(
	 sector_data->data,
	 volume_signature );

	if( ( volume_signature == 0x482b )
	 || ( volume_signature == 0x4858 ) )
	{
		byte_stream_copy_to_uint32_big_endian(
		 &( ( sector_data->data )[ 40 ] ),
		 block_size );

		byte_stream_copy_to_uint32_big_endian(
		 &( ( sector_data->data )[ 44 ] ),
		 number_of_blocks );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: block size\t\t: %" PRIu32 "\n",
			 function,
			 block_size );

			libcnotify_printf(
			 "%s: number of blocks\t\t: %" PRIu32 "\n",
			 function,
			 number_of_blocks );

			libcnotify_printf(
			 "\n" );
		}
#endif
		result = 1;
	}
	if( libfvde_sector_data_free(
	     &sector_data,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free sector data.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( sector_data != NULL )
	{
		libfvde_sector_data_free(
		 &sector_data,
		 NULL );
	}
	return( -1 );
}

