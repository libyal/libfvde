/*
 * The FVDE volume header functions
 *
 * Copyright (C) 2011-2022, Joachim Metz <joachim.metz@gmail.com>
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
#include "libfvde_debug.h"
#include "libfvde_definitions.h"
#include "libfvde_libbfio.h"
#include "libfvde_libcerror.h"
#include "libfvde_libcnotify.h"
#include "libfvde_libfguid.h"
#include "libfvde_volume_header.h"

#include "fvde_volume.h"

/* Creates a volume header
 * Make sure the value volume_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_header_initialize(
     libfvde_volume_header_t **volume_header,
     libcerror_error_t **error )
{
	static char *function = "libfvde_volume_header_initialize";

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
	if( *volume_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume header value already set.",
		 function );

		return( -1 );
	}
	*volume_header = memory_allocate_structure(
	                  libfvde_volume_header_t );

	if( *volume_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create volume header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *volume_header,
	     0,
	     sizeof( libfvde_volume_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear volume header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *volume_header != NULL )
	{
		memory_free(
		 *volume_header );

		*volume_header = NULL;
	}
	return( -1 );
}

/* Frees a volume header
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_header_free(
     libfvde_volume_header_t **volume_header,
     libcerror_error_t **error )
{
	static char *function = "libfvde_volume_header_free";

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
	if( *volume_header != NULL )
	{
		memory_free(
		 *volume_header );

		*volume_header = NULL;
	}
	return( 1 );
}

/* Reads the volume header
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_header_read_data(
     libfvde_volume_header_t *volume_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function        = "libfvde_volume_header_read_data";
	uint32_t calculated_checksum = 0;
	uint32_t checksum_algorithm  = 0;
	uint32_t initial_value       = 0;
	uint32_t stored_checksum     = 0;
	uint16_t block_type          = 0;
	uint16_t format_version      = 0;
	int metadata_block_index     = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit         = 0;
	uint32_t value_32bit         = 0;
	uint16_t value_16bit         = 0;
#endif

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
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( ( data_size < sizeof( fvde_volume_header_t ) )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: volume header data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fvde_volume_header_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (fvde_volume_header_t *) data )->checksum,
	 stored_checksum );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fvde_volume_header_t *) data )->initial_value,
	 initial_value );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fvde_volume_header_t *) data )->block_type,
	 block_type );

	byte_stream_copy_to_uint16_little_endian(
	 ( (fvde_volume_header_t *) data )->format_version,
	 format_version );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fvde_volume_header_t *) data )->serial_number,
	 volume_header->serial_number );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fvde_volume_header_t *) data )->bytes_per_sector,
	 volume_header->bytes_per_sector );

	byte_stream_copy_to_uint64_little_endian(
	 ( (fvde_volume_header_t *) data )->volume_size,
	 volume_header->physical_volume_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fvde_volume_header_t *) data )->checksum_algorithm,
	 checksum_algorithm );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fvde_volume_header_t *) data )->block_size,
	 volume_header->block_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (fvde_volume_header_t *) data )->metadata_size,
	 volume_header->metadata_size );

	for( metadata_block_index = 0;
	     metadata_block_index < 4;
	     metadata_block_index++ )
	{
		byte_stream_copy_to_uint64_little_endian(
		 &( ( ( (fvde_volume_header_t *) data )->metadata_block_numbers )[ metadata_block_index * 8 ] ),
		 volume_header->metadata_offsets[ metadata_block_index ] );
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (fvde_volume_header_t *) data )->encryption_method,
	 volume_header->physical_volume_encryption_method );

/* TODO copy all key data ? */
	if( memory_copy(
	     volume_header->key_data,
	     &( ( (fvde_volume_header_t *) data )->key_data ),
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy key data.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     volume_header->physical_volume_identifier,
	     &( ( (fvde_volume_header_t *) data )->physical_volume_identifier ),
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy physical volume identifier.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     volume_header->volume_group_identifier,
	     &( ( (fvde_volume_header_t *) data )->volume_group_identifier ),
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy volume group identifier.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		libcnotify_printf(
		 "%s: initial value\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 initial_value );

		libcnotify_printf(
		 "%s: format version\t\t\t\t: %" PRIu16 "\n",
		 function,
		 format_version );

		libcnotify_printf(
		 "%s: block type\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 block_type );

		libcnotify_printf(
		 "%s: serial number\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 volume_header->serial_number );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fvde_volume_header_t *) data )->unknown2,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fvde_volume_header_t *) data )->unknown3a,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown3a\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fvde_volume_header_t *) data )->unknown3b,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown3b\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fvde_volume_header_t *) data )->unknown3c,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown3c\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: bytes per sector\t\t\t: %" PRIu32 "\n",
		 function,
		 volume_header->bytes_per_sector );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fvde_volume_header_t *) data )->unknown4a,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown4a\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fvde_volume_header_t *) data )->unknown4b,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown4b\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: physical volume size\t\t\t: %" PRIu64 "\n",
		 function,
		 volume_header->physical_volume_size );

		libcnotify_printf(
		 "%s: unknown5:\n",
		 function );
		libcnotify_print_data(
		 ( (fvde_volume_header_t *) data )->unknown5,
		 16,
		 0 );

		libcnotify_printf(
		 "%s: core storage signature\t\t\t: %c%c\n",
		 function,
		 ( (fvde_volume_header_t *) data )->core_storage_signature[ 0 ],
		 ( (fvde_volume_header_t *) data )->core_storage_signature[ 1 ] );

		libcnotify_printf(
		 "%s: checksum algorithm\t\t\t: %" PRIu32 "\n",
		 function,
		 checksum_algorithm );

		byte_stream_copy_to_uint16_little_endian(
		 ( (fvde_volume_header_t *) data )->unknown6,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown6\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: block size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 volume_header->block_size );

		libcnotify_printf(
		 "%s: metadata size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 volume_header->metadata_size );

		for( metadata_block_index = 0;
		     metadata_block_index < 4;
		     metadata_block_index++ )
		{
			libcnotify_printf(
			 "%s: metadata: %d block number\t\t: %" PRIu64 "\n",
			 function,
			 metadata_block_index + 1,
			 volume_header->metadata_offsets[ metadata_block_index] );
		}
		libcnotify_printf(
		 "%s: unknown7:\n",
		 function );
		libcnotify_print_data(
		 ( (fvde_volume_header_t *) data )->unknown7,
		 32,
		 0 );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fvde_volume_header_t *) data )->key_data_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: key data size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: encryption method\t\t\t: %" PRIu32 "\n",
		 function,
		 volume_header->physical_volume_encryption_method );

		libcnotify_printf(
		 "%s: key data:\n",
		 function );
		libcnotify_print_data(
		 ( (fvde_volume_header_t *) data )->key_data,
		 128,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

		if( libfvde_debug_print_guid_value(
		     function,
		     "physical volume identifier\t\t",
		     volume_header->physical_volume_identifier,
		     16,
		     LIBFGUID_ENDIAN_BIG,
		     LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print GUID value.",
			 function );

			return( -1 );
		}
		if( libfvde_debug_print_guid_value(
		     function,
		     "volume group identifier\t\t",
		     volume_header->volume_group_identifier,
		     16,
		     LIBFGUID_ENDIAN_BIG,
		     LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print GUID value.",
			 function );

			return( -1 );
		}
		libcnotify_printf(
		 "%s: unknown8:\n",
		 function );
		libcnotify_print_data(
		 ( (fvde_volume_header_t *) data )->unknown8,
		 176,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( memory_compare(
	     ( (fvde_volume_header_t *) data )->core_storage_signature,
	     "CS",
	     2 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported core storage signature.",
		 function );

		return( -1 );
	}
	if( format_version != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format version: %" PRIu16 ".",
		 function,
		 format_version );

		return( -1 );
	}
	if( checksum_algorithm != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported checksum algorithm: %" PRIu32 ".",
		 function,
		 checksum_algorithm );

		return( -1 );
	}
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
	     &( data[ 8 ] ),
	     504,
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
	if( block_type != 0x0010 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported block type: 0x%08" PRIx16 ".",
		 function,
		 block_type );

		return( -1 );
	}
	if( volume_header->physical_volume_encryption_method != 2 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported encryption method: %" PRIu32 ".",
		 function,
		 volume_header->physical_volume_encryption_method );

		return( -1 );
	}
	if( volume_header->bytes_per_sector == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid bytes per sector value out of bounds.",
		 function );

		return( -1 );
	}
	if( volume_header->block_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid block size value out of bounds.",
		 function );

		return( -1 );
	}
	for( metadata_block_index = 0;
	     metadata_block_index < 4;
	     metadata_block_index++ )
	{
		if( volume_header->metadata_offsets[ metadata_block_index ] > ( (uint64_t) INT64_MAX / volume_header->block_size ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid metadata: %d block number value out of bounds.",
			 function,
			 metadata_block_index + 1 );

			return( -1 );
		}
		volume_header->metadata_offsets[ metadata_block_index ] *= volume_header->block_size;
	}
	return( 1 );
}

/* Reads the volume header
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_header_read_file_io_handle(
     libfvde_volume_header_t *volume_header,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	uint8_t volume_header_data[ sizeof( fvde_volume_header_t ) ];

	static char *function = "libfvde_volume_header_read_file_io_handle";
	ssize_t read_count    = 0;

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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading volume header at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              (uint8_t *) &volume_header_data,
	              sizeof( fvde_volume_header_t ),
	              file_offset,
	              error );

	if( read_count != (ssize_t) sizeof( fvde_volume_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume header data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	if( libfvde_volume_header_read_data(
	     volume_header,
	     (uint8_t *) &volume_header_data,
	     sizeof( fvde_volume_header_t ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume header data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the (logical) volume group identifier
 * The identifier is a UUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_header_get_volume_group_identifier(
     libfvde_volume_header_t *volume_header,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libcerror_error_t **error )
{
	static char *function = "libfvde_volume_header_get_volume_group_identifier";

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
	     volume_header->volume_group_identifier,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy volume group identifier.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the physical volume identifier
 * The identifier is a UUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_header_get_physical_volume_identifier(
     libfvde_volume_header_t *volume_header,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libcerror_error_t **error )
{
	static char *function = "libfvde_volume_header_get_physical_volume_identifier";

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
	     volume_header->physical_volume_identifier,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy physical volume identifier.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the physical volume encryption method
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_header_get_physical_volume_encryption_method(
     libfvde_volume_header_t *volume_header,
     uint32_t *encryption_method,
     libcerror_error_t **error )
{
	static char *function = "libfvde_volume_header_get_physical_volume_encryption_method";

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
	*encryption_method = volume_header->physical_volume_encryption_method;

	return( 1 );
}

/* Retrieves the physical volume size
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_header_get_physical_volume_size(
     libfvde_volume_header_t *volume_header,
     size64_t *size,
     libcerror_error_t **error )
{
	static char *function = "libfvde_volume_header_get_physical_volume_size";

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
	*size = volume_header->physical_volume_size;

	return( 1 );
}

