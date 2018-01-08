/*
 * Input/Output (IO) handle functions
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
#include <system_string.h>
#include <types.h>

#include "libfvde_checksum.h"
#include "libfvde_definitions.h"
#include "libfvde_encryption.h"
#include "libfvde_io_handle.h"
#include "libfvde_libbfio.h"
#include "libfvde_libcerror.h"
#include "libfvde_libcnotify.h"
#include "libfvde_libfcache.h"
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

/* Reads the volume header
 * Returns 1 if successful or -1 on error
 */
int libfvde_io_handle_read_volume_header(
     libfvde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	fvde_volume_header_t volume_header;

	static char *function        = "libfvde_io_handle_read_volume_header";
	ssize_t read_count           = 0;
	uint32_t calculated_checksum = 0;
	uint32_t initial_value       = 0;
	uint32_t stored_checksum     = 0;
	uint16_t block_type          = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	system_character_t guid_string[ 48 ];

	libfguid_identifier_t *guid = NULL;
	uint64_t value_64bit        = 0;
	uint32_t value_32bit        = 0;
	uint16_t value_16bit        = 0;
	int result                  = 0;
#endif

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
		 "%s: reading volume header at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
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
		 "%s: unable to seek volume header offset: %" PRIi64 ".",
		 function,
		 file_offset );

		goto on_error;
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              (uint8_t *) &volume_header,
	              sizeof( fvde_volume_header_t ),
	              error );

	if( read_count != (ssize_t) sizeof( fvde_volume_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume header data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: volume header data:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) &volume_header,
		 sizeof( fvde_volume_header_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 volume_header.checksum,
	 stored_checksum );

	byte_stream_copy_to_uint32_little_endian(
	 volume_header.initial_value,
	 initial_value );

	byte_stream_copy_to_uint16_little_endian(
	 volume_header.block_type,
	 block_type );

	byte_stream_copy_to_uint32_little_endian(
	 volume_header.serial_number,
	 io_handle->serial_number );

	byte_stream_copy_to_uint16_little_endian(
	 volume_header.version,
	 io_handle->version );

	byte_stream_copy_to_uint32_little_endian(
	 volume_header.bytes_per_sector,
	 io_handle->bytes_per_sector );

	byte_stream_copy_to_uint64_little_endian(
	 volume_header.volume_size,
	 io_handle->physical_volume_size );

	byte_stream_copy_to_uint32_little_endian(
	 volume_header.checksum_algorithm,
	 io_handle->checksum_algorithm );

	byte_stream_copy_to_uint32_little_endian(
	 volume_header.block_size,
	 io_handle->block_size );

	byte_stream_copy_to_uint32_little_endian(
	 volume_header.metadata_size,
	 io_handle->metadata_size );

	byte_stream_copy_to_uint64_little_endian(
	 volume_header.first_metadata_block_number,
	 io_handle->first_metadata_offset );

	byte_stream_copy_to_uint64_little_endian(
	 volume_header.second_metadata_block_number,
	 io_handle->second_metadata_offset );

	byte_stream_copy_to_uint64_little_endian(
	 volume_header.third_metadata_block_number,
	 io_handle->third_metadata_offset );

	byte_stream_copy_to_uint64_little_endian(
	 volume_header.fourth_metadata_block_number,
	 io_handle->fourth_metadata_offset );

	byte_stream_copy_to_uint32_little_endian(
	 volume_header.encryption_method,
	 io_handle->physical_volume_encryption_method );

/* TODO copy all key data ? */
	if( memory_copy(
	     io_handle->key_data,
	     &( volume_header.key_data ),
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy key data.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     io_handle->physical_volume_identifier,
	     &( volume_header.physical_volume_identifier ),
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy physical volume identifier.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     io_handle->logical_volume_group_identifier,
	     &( volume_header.logical_volume_group_identifier ),
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy logical volume group identifier.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_checksum );

		libcnotify_printf(
		 "%s: initial value\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 initial_value );

		libcnotify_printf(
		 "%s: version\t\t\t\t: %" PRIu16 "\n",
		 function,
		 io_handle->version );

		libcnotify_printf(
		 "%s: block type\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 block_type );

		libcnotify_printf(
		 "%s: serial number\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 io_handle->serial_number );

		byte_stream_copy_to_uint64_little_endian(
		 volume_header.unknown2,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 volume_header.unknown3a,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown3a\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 volume_header.unknown3b,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown3b\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 volume_header.unknown3c,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown3c\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: bytes per sector\t\t\t: %" PRIu32 "\n",
		 function,
		 io_handle->bytes_per_sector );

		byte_stream_copy_to_uint32_little_endian(
		 volume_header.unknown4a,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown4a\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint64_little_endian(
		 volume_header.unknown4b,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown4b\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: physical volume size\t\t: %" PRIu64 "\n",
		 function,
		 io_handle->physical_volume_size );

		libcnotify_printf(
		 "%s: unknown5:\n",
		 function );
		libcnotify_print_data(
		 volume_header.unknown5,
		 16,
		 0 );

		libcnotify_printf(
		 "%s: core storage signature\t\t: %c%c\n",
		 function,
		 volume_header.core_storage_signature[ 0 ],
		 volume_header.core_storage_signature[ 1 ] );

		libcnotify_printf(
		 "%s: checksum algorithm\t\t: %" PRIu32 "\n",
		 function,
		 io_handle->checksum_algorithm );

		byte_stream_copy_to_uint16_little_endian(
		 volume_header.unknown6,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown6\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: block size\t\t\t: %" PRIu32 "\n",
		 function,
		 io_handle->block_size );

		libcnotify_printf(
		 "%s: metadata size\t\t\t: %" PRIu32 "\n",
		 function,
		 io_handle->metadata_size );

                libcnotify_printf(
		 "%s: first metadata block number\t: %" PRIu64 "\n",
		 function,
		 io_handle->first_metadata_offset );

                libcnotify_printf(
		 "%s: second metadata block number\t: %" PRIu64 "\n",
		 function,
		 io_handle->second_metadata_offset );

                libcnotify_printf(
		 "%s: third metadata block number\t: %" PRIu64 "\n",
		 function,
		 io_handle->third_metadata_offset );

                libcnotify_printf(
		 "%s: fourth metadata block number\t: %" PRIu64 "\n",
		 function,
		 io_handle->fourth_metadata_offset );

		libcnotify_printf(
		 "%s: unknown7:\n",
		 function );
		libcnotify_print_data(
		 volume_header.unknown7,
		 32,
		 0 );

		libcnotify_printf(
		 "%s: encryption method\t\t\t: %" PRIu32 "\n",
		 function,
		 io_handle->physical_volume_encryption_method );

		libcnotify_printf(
		 "%s: key data:\n",
		 function );
		libcnotify_print_data(
		 volume_header.key_data,
		 128,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

		if( libfguid_identifier_initialize(
		     &guid,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create GUID.",
			 function );

			goto on_error;
		}
		if( libfguid_identifier_copy_from_byte_stream(
		     guid,
		     io_handle->physical_volume_identifier,
		     16,
		     LIBFGUID_ENDIAN_BIG,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy byte stream to GUID.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfguid_identifier_copy_to_utf16_string(
			  guid,
			  (uint16_t *) guid_string,
			  48,
			  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
			  error );
#else
		result = libfguid_identifier_copy_to_utf8_string(
			  guid,
			  (uint8_t *) guid_string,
			  48,
			  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy GUID to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: physical volume identifier\t: %" PRIs_SYSTEM "\n",
		 function,
		 guid_string );

		if( libfguid_identifier_copy_from_byte_stream(
		     guid,
		     io_handle->logical_volume_group_identifier,
		     16,
		     LIBFGUID_ENDIAN_BIG,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy byte stream to GUID.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfguid_identifier_copy_to_utf16_string(
			  guid,
			  (uint16_t *) guid_string,
			  48,
			  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
			  error );
#else
		result = libfguid_identifier_copy_to_utf8_string(
			  guid,
			  (uint8_t *) guid_string,
			  48,
			  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy GUID to string.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: logical volume group identifier\t: %" PRIs_SYSTEM "\n",
		 function,
		 guid_string );

		if( libfguid_identifier_free(
		     &guid,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free GUID.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: unknown8:\n",
		 function );
		libcnotify_print_data(
		 volume_header.unknown8,
		 176,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( memory_compare(
	     volume_header.core_storage_signature,
	     libfvde_core_storage_signature,
	     2 ) != 0 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
                 "%s: unsupported core storage signature.",
                 function );

		goto on_error;
	}
	if( io_handle->version != 1 )
       	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
                 "%s: unsupported version: %" PRIu16 ".",
                 function,
		 io_handle->version );

		goto on_error;
	}
	if( io_handle->checksum_algorithm != 1 )
       	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
                 "%s: unsupported checksum algorithm: %" PRIu32 ".",
                 function,
		 io_handle->checksum_algorithm );

		goto on_error;
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

		goto on_error;
	}
	if( libfvde_checksum_calculate_weak_crc32(
	     &calculated_checksum,
	     &( ( (uint8_t *) &volume_header )[ 8 ] ),
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

		goto on_error;
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

		goto on_error;
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

		goto on_error;
	}
	if( io_handle->physical_volume_encryption_method != 2 )
       	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
                 "%s: unsupported encryption method: %" PRIu32 ".",
                 function,
		 io_handle->physical_volume_encryption_method );

		goto on_error;
	}
	if( io_handle->bytes_per_sector == 0 )
       	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
                 "%s: invalid bytes per sector value out of bounds.",
                 function );

		goto on_error;
	}
	if( io_handle->block_size == 0 )
       	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
                 "%s: invalid block size value out of bounds.",
                 function );

		goto on_error;
	}
	io_handle->sectors_per_block = io_handle->block_size / io_handle->bytes_per_sector;

/* TODO add bounds check */
	io_handle->first_metadata_offset  *= io_handle->block_size;
	io_handle->second_metadata_offset *= io_handle->block_size;
	io_handle->third_metadata_offset  *= io_handle->block_size;
	io_handle->fourth_metadata_offset *= io_handle->block_size;

	return( 1 );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( guid != NULL )
	{
		libfguid_identifier_free(
		 &guid,
		 NULL );
	}
#endif
	return( -1 );
}

/* Reads a sector
 * Callback function for the volume vector
 * Returns 1 if successful or -1 on error
 */
int libfvde_io_handle_read_sector(
     libfvde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfcache_cache_t *cache,
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

