/*
 * Metadata block functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfvde_checksum.h"
#include "libfvde_libcerror.h"
#include "libfvde_libcnotify.h"
#include "libfvde_metadata_block.h"
#include "libfvde_types.h"

#include "fvde_metadata.h"

/* Creates a metadata block
 * Make sure the value metadata_block is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfvde_metadata_block_initialize(
     libfvde_metadata_block_t **metadata_block,
     libcerror_error_t **error )
{
	static char *function = "libfvde_metadata_block_initialize";

	if( metadata_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata block.",
		 function );

		return( -1 );
	}
	if( *metadata_block != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid metadata block value already set.",
		 function );

		return( -1 );
	}
	*metadata_block = memory_allocate_structure(
	                   libfvde_metadata_block_t );

	if( *metadata_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create metadata block.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *metadata_block,
	     0,
	     sizeof( libfvde_metadata_block_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear metadata block.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *metadata_block != NULL )
	{
		memory_free(
		 *metadata_block );

		*metadata_block = NULL;
	}
	return( -1 );
}

/* Frees a metadata block
 * Returns 1 if successful or -1 on error
 */
int libfvde_metadata_block_free(
     libfvde_metadata_block_t **metadata_block,
     libcerror_error_t **error )
{
	static char *function = "libfvde_metadata_block_free";

	if( metadata_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata block.",
		 function );

		return( -1 );
	}
	if( *metadata_block != NULL )
	{
		/* The data is referenced and freed elsewhere */
		memory_free(
		 *metadata_block );

		*metadata_block = NULL;
	}
	return( 1 );
}

/* Checks if a buffer containing the metadata block data is filled with same value bytes (empty-block)
 * Returns 1 if a pattern was found, 0 if not or -1 on error
 */
int libfvde_metadata_block_check_for_empty_block(
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfvde_aligned_t *aligned_data_index = NULL;
	libfvde_aligned_t *aligned_data_start = NULL;
	uint8_t *data_index                   = NULL;
	uint8_t *data_start                   = NULL;
	static char *function                 = "libfvde_metadata_block_check_for_empty_block";

	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
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
	data_start = (uint8_t *) data;
	data_index = (uint8_t *) data + 1;
	data_size -= 1;

	/* Only optimize for data larger than the alignment
	 */
	if( data_size > ( 2 * sizeof( libfvde_aligned_t ) ) )
	{
		/* Align the data start
		 */
		while( ( (intptr_t) data_start % sizeof( libfvde_aligned_t ) ) != 0 )
		{
			if( *data_start != *data_index )
			{
				return( 0 );
			}
			data_start += 1;
			data_index += 1;
			data_size  -= 1;
		}
		/* Align the data index
		 */
		while( ( (intptr_t) data_index % sizeof( libfvde_aligned_t ) ) != 0 )
		{
			if( *data_start != *data_index )
			{
				return( 0 );
			}
			data_index += 1;
			data_size  -= 1;
		}
		aligned_data_start = (libfvde_aligned_t *) data_start;
		aligned_data_index = (libfvde_aligned_t *) data_index;

		while( data_size > sizeof( libfvde_aligned_t ) )
		{
			if( *aligned_data_start != *aligned_data_index )
			{
				return( 0 );
			}
			aligned_data_index += 1;
			data_size          -= sizeof( libfvde_aligned_t );
		}
		data_index = (uint8_t *) aligned_data_index;
	}
	while( data_size != 0 )
	{
		if( *data_start != *data_index )
		{
			return( 0 );
		}
		data_index += 1;
		data_size  -= 1;
	}
	return( 1 );
}

/* Reads the metadata block data
 * Returns 1 if successful, 0 if block is empty or -1 on error
 */
int libfvde_metadata_block_read_data(
     libfvde_metadata_block_t *metadata_block,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error )
{
	static char *function        = "libfvde_metadata_block_read_data";
	uint32_t block_size          = 0;
	uint32_t calculated_checksum = 0;
	uint32_t initial_value       = 0;
	uint32_t stored_checksum     = 0;
	uint16_t version             = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit         = 0;
	uint32_t value_32bit         = 0;
#endif

	if( metadata_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata block.",
		 function );

		return( -1 );
	}
	if( block_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block data.",
		 function );

		return( -1 );
	}
	if( block_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid block data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( block_data_size < sizeof( fvde_metadata_block_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: block data size value too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: header data:\n",
		 function );
		libcnotify_print_data(
		 block_data,
		 sizeof( fvde_metadata_block_header_t ),
		 0 );
	}
#endif
	/* Check if metadata block starts with: LVFwiped
	 */
	if( ( block_data[ 0 ] == (uint8_t) 'L' )
	 && ( block_data[ 1 ] == (uint8_t) 'V' )
	 && ( block_data[ 2 ] == (uint8_t) 'F' )
	 && ( block_data[ 3 ] == (uint8_t) 'w' )
	 && ( block_data[ 4 ] == (uint8_t) 'i' )
	 && ( block_data[ 5 ] == (uint8_t) 'p' )
	 && ( block_data[ 6 ] == (uint8_t) 'e' )
	 && ( block_data[ 7 ] == (uint8_t) 'd' ) )
	{
		metadata_block->is_lvf_wiped = 1;
	}
	else
	{
		metadata_block->is_lvf_wiped = 0;
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (fvde_metadata_block_header_t *) block_data )->checksum,
	 stored_checksum );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fvde_metadata_block_header_t *) block_data )->initial_value,
	 initial_value );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fvde_metadata_block_header_t *) block_data )->version,
	 version );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fvde_metadata_block_header_t *) block_data )->type,
	 metadata_block->type );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fvde_metadata_block_header_t *) block_data )->serial_number,
	 metadata_block->serial_number );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fvde_metadata_block_header_t *) block_data )->transaction_identifier,
	 metadata_block->transaction_identifier );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fvde_metadata_block_header_t *) block_data )->object_identifier,
	 metadata_block->object_identifier );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fvde_metadata_block_header_t *) block_data )->number,
	 metadata_block->number );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fvde_metadata_block_header_t *) block_data )->block_size,
	 block_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( metadata_block->is_lvf_wiped == 0 )
		{
			libcnotify_printf(
			 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 stored_checksum );

			libcnotify_printf(
			 "%s: initial value\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 initial_value );
		}
		else
		{
			libcnotify_printf(
			 "%s: signature\t\t\t\t: LVFwiped\n",
			 function );
		}
		libcnotify_printf(
		 "%s: version\t\t\t\t: %" PRIu16 "\n",
		 function,
		 version );

		libcnotify_printf(
		 "%s: type\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 metadata_block->type );

		libcnotify_printf(
		 "%s: serial number\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 metadata_block->serial_number );

		libcnotify_printf(
		 "%s: transaction identifier\t\t: %" PRIu64 "\n",
		 function,
		 metadata_block->transaction_identifier );

		libcnotify_printf(
		 "%s: object identifier\t\t\t: %" PRIu64 "\n",
		 function,
		 metadata_block->object_identifier );

		libcnotify_printf(
		 "%s: number\t\t\t\t: %" PRIu64 "\n",
		 function,
		 metadata_block->number );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fvde_metadata_block_header_t *) block_data )->unknown5,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown5\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: size\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 block_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fvde_metadata_block_header_t *) block_data )->unknown6,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown6\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fvde_metadata_block_header_t *) block_data )->unknown7,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown7\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( block_size != 8192 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported block size: %" PRIu64 ".",
		 function,
		 block_size );

		return( -1 );
	}
	if( metadata_block->is_lvf_wiped == 0 )
	{
		if( initial_value != 0xffffffffUL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported initial value: 0x%08" PRIx32 ".",
			 function,
			 initial_value );

			return( -1 );
		}
		if( libfvde_checksum_calculate_weak_crc32(
		     &calculated_checksum,
		     &( block_data[ 8 ] ),
		     8184,
		     initial_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to calculate weak CRC-32.",
			 function );

			return( -1 );
		}
		if( stored_checksum != calculated_checksum )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_INPUT,
			 LIBCERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
			 "%s: mismatch in checksum ( 0x%08" PRIx32 " != 0x%08" PRIx32 " ).",
			 function,
			 stored_checksum,
			 calculated_checksum );

			return( -1 );
		}
	}
	metadata_block->data      = block_data + sizeof( fvde_metadata_block_header_t );
	metadata_block->data_size = block_data_size - sizeof( fvde_metadata_block_header_t );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: data:\n",
		 function );
		libcnotify_print_data(
		 metadata_block->data,
		 metadata_block->data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	return( 1 );
}

