/*
 * Sector data functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfvde_definitions.h"
#include "libfvde_encryption.h"
#include "libfvde_io_handle.h"
#include "libfvde_libcaes.h"
#include "libfvde_libbfio.h"
#include "libfvde_libcerror.h"
#include "libfvde_libcnotify.h"
#include "libfvde_sector_data.h"

/* Creates sector data
 * Make sure the value sector_data is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfvde_sector_data_initialize(
     libfvde_sector_data_t **sector_data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfvde_sector_data_initialize";

	if( sector_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( *sector_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid sector data value already set.",
		 function );

		return( -1 );
	}
	*sector_data = memory_allocate_structure(
	                libfvde_sector_data_t );

	if( *sector_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to sector data.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *sector_data,
	     0,
	     sizeof( libfvde_sector_data_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear sector data.",
		 function );

		memory_free(
		 *sector_data );

		*sector_data = NULL;

		return( -1 );
	}
	if( data_size > 0 )
	{
		( *sector_data )->encrypted_data = (uint8_t *) memory_allocate(
		                                                sizeof( uint8_t ) * data_size );

		if( ( *sector_data )->encrypted_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create encrypted data.",
			 function );

			goto on_error;
		}
		( *sector_data )->data = (uint8_t *) memory_allocate(
		                                      sizeof( uint8_t ) * data_size );

		if( ( *sector_data )->data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create data.",
			 function );

			goto on_error;
		}
		( *sector_data )->data_size = data_size;
	}
	return( 1 );

on_error:
	if( *sector_data != NULL )
	{
		if( ( *sector_data )->encrypted_data != NULL )
		{
			memory_free(
			 ( *sector_data )->encrypted_data );
		}
		memory_free(
		 *sector_data );

		*sector_data = NULL;
	}
	return( -1 );
}

/* Frees sector data
 * Returns 1 if successful or -1 on error
 */
int libfvde_sector_data_free(
     libfvde_sector_data_t **sector_data,
     libcerror_error_t **error )
{
	static char *function = "libfvde_sector_data_free";
	int result            = 1;

	if( sector_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector data.",
		 function );

		return( -1 );
	}
	if( *sector_data != NULL )
	{
		if( ( *sector_data )->data != NULL )
		{
			if( memory_set(
			     ( *sector_data )->data,
			     0,
			     ( *sector_data )->data_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear data.",
				 function );

				result = -1;
			}
			memory_free(
			 ( *sector_data )->data );
		}
		if( ( *sector_data )->encrypted_data != NULL )
		{
			memory_free(
			 ( *sector_data )->encrypted_data );
		}
		memory_free(
		 *sector_data );

		*sector_data = NULL;
	}
	return( result );
}

/* Reads sector data
 * Returns 1 if successful or -1 on error
 */
int libfvde_sector_data_read(
     libfvde_sector_data_t *sector_data,
     libfvde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcaes_tweaked_context_t *xts_context,
     libcerror_error_t **error )
{
	uint8_t tweak_value[ 16 ];

	static char *function = "libfvde_sector_data_read";
	off64_t block_offset  = 0;
	uint64_t block_number = 0;
	ssize_t read_count    = 0;

	if( sector_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector data.",
		 function );

		return( -1 );
	}
	if( sector_data->encrypted_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid sector data - missing encrypted data.",
		 function );

		return( -1 );
	}
	if( sector_data->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid sector data - missing data.",
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading sector data at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     file_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek sector data offset: %" PRIi64 ".",
		 function,
		 file_offset );

		return( -1 );
	}
	if( io_handle->is_encrypted != 0 )
	{
		read_count = libbfio_handle_read_buffer(
			      file_io_handle,
			      sector_data->encrypted_data,
			      sector_data->data_size,
			      error );

		if( read_count != (ssize_t) sector_data->data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read sector data.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: encrypted sector data:\n",
			 function );
			libcnotify_print_data(
			 sector_data->encrypted_data,
			 sector_data->data_size,
			 0 );
		}
#endif
		block_offset = file_offset - io_handle->logical_volume_offset;

		block_number = (uint64_t) ( block_offset / io_handle->bytes_per_sector );

		if( memory_set(
		     tweak_value,
		     0,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to copy block number to tweak value.",
			 function );

			return( -1 );
		}
		byte_stream_copy_from_uint64_little_endian(
		 tweak_value,
		 block_number );

		if( libcaes_crypt_xts(
		     xts_context,
		     LIBCAES_CRYPT_MODE_DECRYPT,
		     tweak_value,
		     16,
		     sector_data->encrypted_data,
		     sector_data->data_size,
		     sector_data->data,
		     sector_data->data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
			 LIBCERROR_ENCRYPTION_ERROR_GENERIC,
			 "%s: unable to decrypt sector data.",
			 function );

			return( -1 );
		}
	}
	else
	{
		read_count = libbfio_handle_read_buffer(
			      file_io_handle,
			      sector_data->data,
			      sector_data->data_size,
			      error );

		if( read_count != (ssize_t) sector_data->data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read sector data.",
			 function );

			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: sector data:\n",
		 function );
		libcnotify_print_data(
		 sector_data->data,
		 sector_data->data_size,
		 0 );
	}
#endif
	return( 1 );
}

