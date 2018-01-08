/*
 * Encrypted metadata functions
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
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>

#include "libfvde_checksum.h"
#include "libfvde_data_area_descriptor.h"
#include "libfvde_definitions.h"
#include "libfvde_encrypted_metadata.h"
#include "libfvde_encryption_context_plist.h"
#include "libfvde_io_handle.h"
#include "libfvde_keyring.h"
#include "libfvde_libbfio.h"
#include "libfvde_libcaes.h"
#include "libfvde_libcerror.h"
#include "libfvde_libcnotify.h"
#include "libfvde_libfguid.h"
#include "libfvde_libfplist.h"
#include "libfvde_libfvalue.h"
#include "libfvde_metadata_block.h"
#include "libfvde_password.h"
#include "libfvde_segment_descriptor.h"

#include "fvde_metadata.h"

/* Some constants used for FileVault medatada
 */
const uint32_t additional_offset_encrypted_volume_start = 80;

const uint8_t libfvde_encrypted_metadata_wrapped_kek_initialization_vector[ 8 ] = \
	{ 0xa6, 0xa6, 0xa6, 0xa6, 0xa6, 0xa6, 0xa6, 0xa6 };

/* Creates encrypted metadata
 * Make sure the value encrypted_metadata is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfvde_encrypted_metadata_initialize(
     libfvde_encrypted_metadata_t **encrypted_metadata,
     libcerror_error_t **error )
{
	static char *function = "libfvde_encrypted_metadata_initialize";

	if( encrypted_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encrypted metadata.",
		 function );

		return( -1 );
	}
	if( *encrypted_metadata != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid encrypted metadata value already set.",
		 function );

		return( -1 );
	}
	*encrypted_metadata = memory_allocate_structure(
	                       libfvde_encrypted_metadata_t );

	if( *encrypted_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create encrypted metadata.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *encrypted_metadata,
	     0,
	     sizeof( libfvde_encrypted_metadata_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear encryped metadata.",
		 function );

		memory_free(
		 *encrypted_metadata );

		*encrypted_metadata = NULL;

		return( -1 );
	}
	if( libfvde_encryption_context_plist_initialize(
	     &( ( *encrypted_metadata )->encryption_context_plist ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create encryption context plist.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( ( *encrypted_metadata )->data_area_descriptors ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data area descriptors array.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( ( *encrypted_metadata )->segment_descriptors ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create segment descriptors array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *encrypted_metadata != NULL )
	{
		if( ( *encrypted_metadata )->data_area_descriptors != NULL )
		{
			libcdata_array_free(
			 &( ( *encrypted_metadata )->data_area_descriptors ),
			 NULL,
			 NULL );
		}
		if( ( *encrypted_metadata )->encryption_context_plist != NULL )
		{
			libfvde_encryption_context_plist_free(
			 &( ( *encrypted_metadata )->encryption_context_plist ),
			 NULL );
		}
		memory_free(
		 *encrypted_metadata );

		*encrypted_metadata = NULL;
	}
	return( -1 );
}

/* Frees encrypted metadata
 * Returns 1 if successful or -1 on error
 */
int libfvde_encrypted_metadata_free(
     libfvde_encrypted_metadata_t **encrypted_metadata,
     libcerror_error_t **error )
{
	static char *function = "libfvde_encrypted_metadata_free";
	int result            = 1;

	if( encrypted_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encrypted metadata.",
		 function );

		return( -1 );
	}
	if( encrypted_metadata != NULL )
	{
		if( libfvde_encryption_context_plist_free(
		     &( ( *encrypted_metadata )->encryption_context_plist ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free encryption context plist.",
			 function );

			result = -1;
		}
		if( ( *encrypted_metadata )->segment_descriptors != NULL )
		{
			if( libcdata_array_free(
			     &( ( *encrypted_metadata )->segment_descriptors ),
			     (int (*)(intptr_t **, libcerror_error_t **)) &libfvde_segment_descriptor_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free segment descriptors array.",
				 function );

				result = -1;
			}
		}
		if( ( *encrypted_metadata )->data_area_descriptors != NULL )
		{
			if( libcdata_array_free(
			     &( ( *encrypted_metadata )->data_area_descriptors ),
			     (int (*)(intptr_t **, libcerror_error_t **)) &libfvde_data_area_descriptor_free,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free data area descriptors array.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *encrypted_metadata );

		*encrypted_metadata = NULL;
	}
	return( result );
}

/* Reads the encrypted metadata block type 0x0012
 * Returns 1 if successful or -1 on error
 */
int libfvde_encrypted_metadata_read_type_0x0012(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error )
{
	const uint8_t *xml_plist_data = NULL;
	static char *function         = "libfvde_encrypted_metadata_read_type_0x0012";

	if( encrypted_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encrypted metadata.",
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
	if( block_data_size < 54 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: block data size value too small.",
		 function );

		return( -1 );
	}
	xml_plist_data = &( block_data[ 48 ] );

	if( ( xml_plist_data[ 0 ] == (uint8_t) '<' )
	 && ( xml_plist_data[ 1 ] == (uint8_t) 'd' )
	 && ( xml_plist_data[ 2 ] == (uint8_t) 'i' )
	 && ( xml_plist_data[ 3 ] == (uint8_t) 'c' )
	 && ( xml_plist_data[ 4 ] == (uint8_t) 't' ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: XML:\n%s\n",
			 function,
			 (char *) &( block_data[ 48 ] ) );

			libcnotify_printf(
			 "\n" );
		}
#endif
	}
/* TODO get com.apple.corestorage.lvg.name */

	return( 1 );
}

/* Reads the encrypted metadata block type 0x0013
 * Returns 1 if successful or -1 on error
 */
int libfvde_encrypted_metadata_read_type_0x0013(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error )
{
	static char *function       = "libfvde_encrypted_metadata_read_type_0x0013";
	size_t block_data_offset    = 0;
	uint32_t entry_index        = 0;
	uint32_t number_of_entries1 = 0;
	uint32_t number_of_entries2 = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	system_character_t guid_string[ 48 ];

	libfguid_identifier_t *guid = NULL;
	uint64_t value_64bit        = 0;
	uint32_t value_32bit        = 0;
	uint16_t value_16bit        = 0;
	int result                  = 0;
#endif

	if( encrypted_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encrypted metadata.",
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
	if( block_data_size < 72 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: block data size value too small.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 &( block_data[ 56 ] ),
	 number_of_entries1 );

	byte_stream_copy_to_uint32_little_endian(
	 &( block_data[ 60 ] ),
	 number_of_entries2 );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 block_data,
		 value_32bit );
		libcnotify_printf(
		 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 4 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: initial value\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

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
		     &( block_data[ 8 ] ),
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
		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ 24 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ 32 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 40 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown3\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 44 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown4\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 48 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown5\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 52 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown6\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: number of entries1\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_entries1 );

		libcnotify_printf(
		 "%s: number of entries2\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_entries2 );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 64 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown9\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 68 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown10\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ 72 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown11\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 80 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown12\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 84 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown13\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ 88 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown14\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ 96 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown15\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ 104 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown16\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ 112 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown17\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 120 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown18\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint16_little_endian(
		 &( block_data[ 124 ] ),
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown19\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 &( block_data[ 126 ] ),
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown20\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "\n" );
	}
#endif
/* TODO: check bounds of number_of_entries */
	block_data_offset = 128;

	if( number_of_entries1 > 0 )
	{
		for( entry_index = 0;
		     entry_index < number_of_entries1;
		     entry_index++ )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				byte_stream_copy_to_uint64_little_endian(
				 &( block_data[ block_data_offset ] ),
				 value_64bit );
				libcnotify_printf(
				 "%s: entry1: %03d block unknown3\t\t: 0x%08" PRIx64 "\n",
				 function,
				 entry_index,
				 value_64bit );
			}
#endif
			block_data_offset += 8;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "\n" );
		}
#endif
	}
	if( number_of_entries2 > 0 )
	{
		for( entry_index = 0;
		     entry_index < number_of_entries2;
		     entry_index++ )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				byte_stream_copy_to_uint32_little_endian(
				 &( block_data[ block_data_offset ] ),
				 value_32bit );
				libcnotify_printf(
				 "%s: entry2: %03d unknown1\t\t: 0x%08" PRIx32 "\n",
				 function,
				 entry_index,
				 value_32bit );

				byte_stream_copy_to_uint32_little_endian(
				 &( block_data[ block_data_offset + 4 ] ),
				 value_32bit );
				libcnotify_printf(
				 "%s: entry2: %03d unknown2\t\t: 0x%08" PRIx32 "\n",
				 function,
				 entry_index,
				 value_32bit );
			}
#endif
			block_data_offset += 8;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "\n" );
		}
#endif
	}
	return( 1 );

#if defined( HAVE_DEBUG_OUTPUT )
on_error:
	if( guid != NULL )
	{
		libfguid_identifier_free(
		 &guid,
		 NULL );
	}
	return( -1 );
#endif
}

/* Reads the encrypted metadata block type 0x0014
 * Returns 1 if successful or -1 on error
 */
int libfvde_encrypted_metadata_read_type_0x0014(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error )
{
	static char *function       = "libfvde_encrypted_metadata_read_type_0x0014";
	size_t block_data_offset    = 0;
	uint32_t entry_index        = 0;
	uint32_t number_of_entries1 = 0;
	uint32_t number_of_entries2 = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	system_character_t guid_string[ 48 ];

	libfguid_identifier_t *guid = NULL;
	uint64_t value_64bit        = 0;
	uint32_t value_32bit        = 0;
	uint16_t value_16bit        = 0;
	int result                  = 0;
#endif

	if( encrypted_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encrypted metadata.",
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
	if( block_data_size < 72 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: block data size value too small.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 &( block_data[ 56 ] ),
	 number_of_entries1 );

	byte_stream_copy_to_uint32_little_endian(
	 &( block_data[ 60 ] ),
	 number_of_entries2 );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 block_data,
		 value_32bit );
		libcnotify_printf(
		 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 4 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: initial value\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

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
		     &( block_data[ 8 ] ),
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
		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ 24 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: block number1\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ 32 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 40 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown3\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 44 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown4\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 48 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown5\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 52 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown6\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: number of entries1\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_entries1 );

		libcnotify_printf(
		 "%s: number of entries2\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_entries2 );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 64 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown9\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 68 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown10\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ 72 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown11\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 80 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown12\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 84 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown13\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ 88 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown14\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ 96 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown15\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ 104 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown16\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ 112 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown17\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 120 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown18\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint16_little_endian(
		 &( block_data[ 124 ] ),
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown19\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 &( block_data[ 126 ] ),
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown20\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "\n" );
	}
#endif
/* TODO: check bounds of number_of_entries */
	block_data_offset = 128;

	if( number_of_entries1 > 0 )
	{
		for( entry_index = 0;
		     entry_index < number_of_entries1;
		     entry_index++ )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				byte_stream_copy_to_uint64_little_endian(
				 &( block_data[ block_data_offset ] ),
				 value_64bit );
				libcnotify_printf(
				 "%s: entry1: %03d unknown1\t\t: 0x%08" PRIx64 "\n",
				 function,
				 entry_index,
				 value_64bit );
			}
#endif
			block_data_offset += 8;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "\n" );
		}
#endif
	}
	if( number_of_entries2 > 0 )
	{
		for( entry_index = 0;
		     entry_index < number_of_entries2;
		     entry_index++ )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				byte_stream_copy_to_uint64_little_endian(
				 &( block_data[ block_data_offset ] ),
				 value_64bit );
				libcnotify_printf(
				 "%s: entry2: %03d unknown1\t\t: 0x%08" PRIx64 "\n",
				 function,
				 entry_index,
				 value_64bit );
			}
#endif
			block_data_offset += 8;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "\n" );
		}
#endif
	}
	return( 1 );

#if defined( HAVE_DEBUG_OUTPUT )
on_error:
	if( guid != NULL )
	{
		libfguid_identifier_free(
		 &guid,
		 NULL );
	}
	return( -1 );
#endif
}

/* Reads the encrypted metadata block type 0x0016
 * Returns 1 if successful or -1 on error
 */
int libfvde_encrypted_metadata_read_type_0x0016(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error )
{
	static char *function      = "libfvde_encrypted_metadata_read_type_0x0016";
	size_t block_data_offset   = 0;
	uint32_t entry_index       = 0;
	uint32_t number_of_entries = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit       = 0;
	uint32_t value_32bit       = 0;
#endif

	if( encrypted_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encrypted metadata.",
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
	if( block_data_size < 4 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: block data size value too small.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 &( block_data[ 0 ] ),
	 number_of_entries );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of entries\t\t: %" PRIu32 "\n",
		 function,
		 number_of_entries );

		libcnotify_printf(
		 "\n" );
	}
#endif
/* TODO: check bounds of number_of_entries */
	block_data_offset = 4;

	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint64_little_endian(
			 &( block_data[ block_data_offset ] ),
			 value_64bit );
			libcnotify_printf(
			 "%s: entry: %03d unknown1\t: 0x%08" PRIx64 "\n",
			 function,
			 entry_index,
			 value_64bit );

			byte_stream_copy_to_uint32_little_endian(
			 &( block_data[ block_data_offset + 8 ] ),
			 value_32bit );
			libcnotify_printf(
			 "%s: entry: %03d unknown2\t: %" PRIu32 "\n",
			 function,
			 entry_index,
			 value_32bit );
		}
#endif
		block_data_offset += 12;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}

/* Reads the encrypted metadata block type 0x0017
 * Returns 1 if successful or -1 on error
 */
int libfvde_encrypted_metadata_read_type_0x0017(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error )
{
	static char *function      = "libfvde_encrypted_metadata_read_type_0x0017";
	size_t block_data_offset   = 0;
	uint64_t entry_index       = 0;
	uint64_t number_of_entries = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit       = 0;
	uint32_t value_32bit       = 0;
#endif

	if( encrypted_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encrypted metadata.",
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
	if( block_data_size < 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: block data size value too small.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint64_little_endian(
	 &( block_data[ 0 ] ),
	 number_of_entries );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of entries\t\t: %" PRIu64 "\n",
		 function,
		 number_of_entries );

		libcnotify_printf(
		 "\n" );
	}
#endif
/* TODO: check bounds of number_of_entries */
	block_data_offset = 8;

	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint64_little_endian(
			 &( block_data[ block_data_offset ] ),
			 value_64bit );
			libcnotify_printf(
			 "%s: entry: %02d block group\t: %" PRIu64 "\n",
			 function,
			 entry_index,
			 value_64bit );

			byte_stream_copy_to_uint32_little_endian(
			 &( block_data[ block_data_offset + 8 ] ),
			 value_32bit );
			libcnotify_printf(
			 "%s: entry: %02d unknown2\t\t: 0x%08" PRIx32 "\n",
			 function,
			 entry_index,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 &( block_data[ block_data_offset + 12 ] ),
			 value_32bit );
			libcnotify_printf(
			 "%s: entry: %02d number of blocks\t: %" PRIu32 "\n",
			 function,
			 entry_index,
			 value_32bit );

			byte_stream_copy_to_uint64_little_endian(
			 &( block_data[ block_data_offset + 16 ] ),
			 value_64bit );
			libcnotify_printf(
			 "%s: entry: %02d unknown3\t\t: 0x%08" PRIx64 "\n",
			 function,
			 entry_index,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 &( block_data[ block_data_offset + 24 ] ),
			 value_64bit );
			libcnotify_printf(
			 "%s: entry: %02d unknown4\t\t: 0x%08" PRIx64 "\n",
			 function,
			 entry_index,
			 value_64bit );

			libcnotify_printf(
			 "\n" );
		}
#endif
		block_data_offset += 32;
	}
	return( 1 );
}

/* Reads the encrypted metadata block type 0x0018
 * Returns 1 if successful or -1 on error
 */
int libfvde_encrypted_metadata_read_type_0x0018(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error )
{
	static char *function = "libfvde_encrypted_metadata_read_type_0x0018";

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit  = 0;
#endif

	if( encrypted_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encrypted metadata.",
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
	if( block_data_size < 16 )
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
		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ 0 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ 8 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}

/* Reads the encrypted metadata block type 0x0019
 * Returns 1 if successful or -1 on error
 */
int libfvde_encrypted_metadata_read_type_0x0019(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error )
{
	const uint8_t *xml_plist_data  = NULL;
	static char *function          = "libfvde_encrypted_metadata_read_type_0x0019";
	uint32_t xml_plist_data_offset = 0;
	uint32_t xml_plist_data_size   = 0;
	int result                     = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit           = 0;
	uint32_t value_32bit           = 0;
#endif

	if( encrypted_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encrypted metadata.",
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
	if( block_data_size < 64 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: block data size value too small.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 &( block_data[ 48 ] ),
	 xml_plist_data_offset );

	byte_stream_copy_to_uint32_little_endian(
	 &( block_data[ 52 ] ),
	 xml_plist_data_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ 0 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ 8 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ 16 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown3\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ 24 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown4\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 32 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown5\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 36 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown6\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 40 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown7\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 44 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown8\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: XML plist data offset\t: 0x%08" PRIx32 "\n",
		 function,
		 xml_plist_data_offset );

		libcnotify_printf(
		 "%s: XML plist data size\t: %" PRIu32 "\n",
		 function,
		 xml_plist_data_size );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 56 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown9\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );
	}
#endif
	if( ( xml_plist_data_offset < 64 )
	 || ( xml_plist_data_offset >= block_data_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid XML plist data offset value out of bounds.",
		 function );

		return( -1 );
	}
	xml_plist_data = &( block_data[ xml_plist_data_offset - 64 ] );

	if( ( xml_plist_data[ 0 ] == (uint8_t) '<' )
	 && ( xml_plist_data[ 1 ] == (uint8_t) 'd' )
	 && ( xml_plist_data[ 2 ] == (uint8_t) 'i' )
	 && ( xml_plist_data[ 3 ] == (uint8_t) 'c' )
	 && ( xml_plist_data[ 4 ] == (uint8_t) 't' ) )
	{
		if( xml_plist_data_size > ( block_data_size - xml_plist_data_offset ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid XML plist data size value out of bounds.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: XML:\n%s\n",
			 function,
			 (char *) xml_plist_data );
		}
#endif
		if( encrypted_metadata->encryption_context_plist_file_is_set == 0 )
		{
			result = libfvde_encryption_context_plist_set_data(
				  encrypted_metadata->encryption_context_plist,
				  xml_plist_data,
				  xml_plist_data_size,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set encryption context plist data.",
				 function );

				return( -1 );
			}
			else if( result != 0 )
			{
				encrypted_metadata->encryption_context_plist_file_is_set = 1;
			}
		}
	}
/* TODO find com.apple.corestorage.lvf.encryption.context/WrappedVolumeKeys/?/BlockAlgorithm */
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}

/* Reads the encrypted metadata block type 0x001a
 * Returns 1 if successful or -1 on error
 */
int libfvde_encrypted_metadata_read_type_0x001a(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error )
{
	libfplist_property_list_t *property_list  = NULL;
	libfplist_property_t *root_property       = NULL;
	libfplist_property_t *sub_property        = NULL;
	const uint8_t *xml_plist_data             = NULL;
	static char *function                     = "libfvde_encrypted_metadata_read_type_0x001a";
	size_t xml_length                         = 0;
	uint64_t logical_volume_size              = 0;
	uint32_t compressed_xml_plist_data_size   = 0;
	uint32_t stored_xml_plist_data_offset     = 0;
	uint32_t stored_xml_plist_data_size       = 0;
	uint32_t uncompressed_xml_plist_data_size = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t *string                           = NULL;
	size_t string_size                        = 0;
	uint64_t value_64bit                      = 0;
#endif

	if( encrypted_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encrypted metadata.",
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
	if( block_data_size < 72 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: block data size value too small.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 &( block_data[ 56 ] ),
	 compressed_xml_plist_data_size );

	byte_stream_copy_to_uint32_little_endian(
	 &( block_data[ 60 ] ),
	 uncompressed_xml_plist_data_size );

	byte_stream_copy_to_uint32_little_endian(
	 &( block_data[ 64 ] ),
	 stored_xml_plist_data_offset );

	byte_stream_copy_to_uint32_little_endian(
	 &( block_data[ 68 ] ),
	 stored_xml_plist_data_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ 0 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ 8 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

/* TODO add more values */

		libcnotify_printf(
		 "%s: compressed XML plist data size\t: %" PRIu32 "\n",
		 function,
		 compressed_xml_plist_data_size );

		libcnotify_printf(
		 "%s: uncompressed XML plist data size\t: %" PRIu32 "\n",
		 function,
		 uncompressed_xml_plist_data_size );

		libcnotify_printf(
		 "%s: stored XML plist data offset\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_xml_plist_data_offset );

		libcnotify_printf(
		 "%s: stored XML plist data size\t\t: %" PRIu32 "\n",
		 function,
		 stored_xml_plist_data_size );
	}
#endif
	if( ( stored_xml_plist_data_offset < 72 )
	 || ( (size_t) stored_xml_plist_data_offset > block_data_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid stored XML plist data offset value out of bounds.",
		 function );

		goto on_error;
	}
	if( stored_xml_plist_data_size > ( block_data_size - stored_xml_plist_data_offset ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid stored XML plist data size value out of bounds.",
		 function );

		goto on_error;
	}
	xml_plist_data = &( block_data[ stored_xml_plist_data_offset - 64 ] );

/* TODO handle compressed XML plist data */
	if( ( xml_plist_data[ 0 ] == (uint8_t) '<' )
	 && ( xml_plist_data[ 1 ] == (uint8_t) 'd' )
	 && ( xml_plist_data[ 2 ] == (uint8_t) 'i' )
	 && ( xml_plist_data[ 3 ] == (uint8_t) 'c' )
	 && ( xml_plist_data[ 4 ] == (uint8_t) 't' ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: XML:\n%s\n",
			 function,
			 (char *) xml_plist_data );

			libcnotify_printf(
			 "\n" );
		}
#endif
/* TODO for now determine the XML string length */
/* TODO refactor this to a separate function */
		xml_length = narrow_string_length(
			      (char *) xml_plist_data );

		if( xml_length > (size_t) ( INT_MAX - 1 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid XML length value out of bounds.",
			 function );

			goto on_error;
		}
		if( libfplist_property_list_initialize(
		     &property_list,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create property list.",
			 function );

			goto on_error;
		}
		if( libfplist_property_list_copy_from_byte_stream(
		     property_list,
		     xml_plist_data,
		     xml_length + 1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy property list from byte stream.",
			 function );

			goto on_error;
		}
		if( libfplist_property_list_get_root_property(
		     property_list,
		     &root_property,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root property.",
			 function );

			goto on_error;
		}
		if( libfplist_property_get_sub_property_by_utf8_name(
		     root_property,
		     (uint8_t *) "com.apple.corestorage.lv.familyUUID",
		     35,
		     &sub_property,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve com.apple.corestorage.lv.familyUUID sub property.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( libfplist_property_get_value_string(
			     sub_property,
			     &string,
			     &string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve logical volume family identifier.",
				 function );

				goto on_error;
			}
			libcnotify_printf(
			 "%s: logical volume family identifier\t: %s\n",
			 function,
			 string );

			memory_free(
			 string );

			string = NULL;
		}
#endif
		if( libfplist_property_value_uuid_string_copy_to_byte_stream(
		     sub_property,
		     encrypted_metadata->logical_volume_family_identifier,
		     16,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
			 "%s: unable to copy LVF UUID string to byte stream.",
			 function );

			goto on_error;
		}
		if( libfplist_property_free(
		     &sub_property,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free com.apple.corestorage.lv.familyUUID property.",
			 function );

			goto on_error;
		}
		if( libfplist_property_get_sub_property_by_utf8_name(
		     root_property,
		     (uint8_t *) "com.apple.corestorage.lv.size",
		     29,
		     &sub_property,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve com.apple.corestorage.lv.size sub property.",
			 function );

			goto on_error;
		}
		if( libfplist_property_get_value_integer(
		     sub_property,
		     (uint64_t *) &logical_volume_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve logical volume size.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: logical volume size\t\t: %" PRIu64 "\n",
			 function,
			 logical_volume_size );
		}
#endif
		if( libfplist_property_free(
		     &sub_property,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free com.apple.corestorage.lv.size property.",
			 function );

			goto on_error;
		}
		if( libfplist_property_free(
		     &root_property,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free root property.",
			 function );

			goto on_error;
		}
		if( libfplist_property_list_free(
		     &property_list,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free property list.",
			 function );

			goto on_error;
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( string != NULL )
	{
		memory_free(
		 string );
	}
#endif
	if( sub_property != NULL )
	{
		libfplist_property_free(
		 &sub_property,
		 NULL );
	}
	if( root_property != NULL )
	{
		libfplist_property_free(
		 &root_property,
		 NULL );
	}
	if( property_list != NULL )
	{
		libfplist_property_list_free(
		 &property_list,
		 NULL );
	}
	return( -1 );
}

/* Reads the encrypted metadata block type 0x001c
 * Returns 1 if successful or -1 on error
 */
int libfvde_encrypted_metadata_read_type_0x001c(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error )
{
	static char *function      = "libfvde_encrypted_metadata_read_type_0x001c";
	size_t block_data_offset   = 0;
	uint64_t entry_index       = 0;
	uint64_t number_of_entries = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit       = 0;
#endif

	if( encrypted_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encrypted metadata.",
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
	if( block_data_size < 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: block data size value too small.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint64_little_endian(
	 &( block_data[ 8 ] ),
	 number_of_entries );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ 0 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: number of entries\t\t: %" PRIu64 "\n",
		 function,
		 number_of_entries );

		libcnotify_printf(
		 "\n" );
	}
#endif
/* TODO: check bounds of number_of_entries */
	block_data_offset = 16;

	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint64_little_endian(
			 &( block_data[ block_data_offset ] ),
			 value_64bit );
			libcnotify_printf(
			 "%s: entry: %02d unknown1\t\t: 0x%08" PRIx64 "\n",
			 function,
			 entry_index,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 &( block_data[ block_data_offset + 8 ] ),
			 value_64bit );
			libcnotify_printf(
			 "%s: entry: %02d unknown2\t\t: 0x%08" PRIx64 "\n",
			 function,
			 entry_index,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 &( block_data[ block_data_offset + 16 ] ),
			 value_64bit );
			libcnotify_printf(
			 "%s: entry: %02d block number1\t: %" PRIu64 "\n",
			 function,
			 entry_index,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 &( block_data[ block_data_offset + 24 ] ),
			 value_64bit );
			libcnotify_printf(
			 "%s: entry: %02d block number2\t: %" PRIu64 "\n",
			 function,
			 entry_index,
			 value_64bit );
		}
#endif
		block_data_offset += 32;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}

/* Reads the encrypted metadata block type 0x001d
 * Returns 1 if successful or -1 on error
 */
int libfvde_encrypted_metadata_read_type_0x001d(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error )
{
	static char *function      = "libfvde_encrypted_metadata_read_type_0x001d";
	size_t block_data_offset   = 0;
	uint64_t entry_index       = 0;
	uint64_t number_of_entries = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit       = 0;
#endif

	if( encrypted_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encrypted metadata.",
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
	if( block_data_size < 24 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: block data size value too small.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint64_little_endian(
	 &( block_data[ 16 ] ),
	 number_of_entries );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: unknown1:\n",
		 function );
		libcnotify_print_data(
		 block_data,
		 16,
		 0 );

		libcnotify_printf(
		 "%s: number of entries\t\t\t: %" PRIu64 "\n",
		 function,
		 number_of_entries );

		libcnotify_printf(
		 "\n" );
	}
#endif
/* TODO: check bounds of number_of_entries */
	block_data_offset = 24;

	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint32_little_endian(
			 &( block_data[ block_data_offset ] ),
			 value_32bit );
			libcnotify_printf(
			 "%s: entry: %03d number of blocks\t: %" PRIu32 "\n",
			 function,
			 entry_index,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 &( block_data[ block_data_offset + 4 ] ),
			 value_32bit );
			libcnotify_printf(
			 "%s: entry: %03d unknown1\t\t: 0x%08" PRIx32 "\n",
			 function,
			 entry_index,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 &( block_data[ block_data_offset + 8 ] ),
			 value_32bit );
			libcnotify_printf(
			 "%s: entry: %03d block number\t\t: %" PRIu32 "\n",
			 function,
			 entry_index,
			 value_32bit );

			byte_stream_copy_to_uint32_little_endian(
			 &( block_data[ block_data_offset + 12 ] ),
			 value_32bit );
			libcnotify_printf(
			 "%s: entry: %03d unknown2\t\t: 0x%08" PRIx32 "\n",
			 function,
			 entry_index,
			 value_32bit );

			libcnotify_printf(
			 "\n" );
		}
#endif
		block_data_offset += 16;
	}
	return( 1 );
}

/* Reads the encrypted metadata block type 0x0021
 * Returns 1 if successful or -1 on error
 */
int libfvde_encrypted_metadata_read_type_0x0021(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error )
{
	static char *function      = "libfvde_encrypted_metadata_read_type_0x0021";
	uint32_t number_of_blocks  = 0;
	uint16_t number_of_entries = 0;

	if( encrypted_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encrypted metadata.",
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
	if( block_data_size < 6 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: block data size value too small.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint16_little_endian(
	 &( block_data[ 0 ] ),
	 number_of_entries );

	byte_stream_copy_to_uint32_little_endian(
	 &( block_data[ 2 ] ),
	 number_of_blocks );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of entries\t\t\t: %" PRIu16 "\n",
		 function,
		 number_of_entries );

		libcnotify_printf(
		 "%s: logical volume number of blocks\t: %" PRIu32 "\n",
		 function,
		 number_of_blocks );

		libcnotify_printf(
		 "\n" );
	}
#endif
/* TODO: check bounds of number_of_entries */
	return( 1 );
}

/* Reads the encrypted metadata block type 0x0022
 * Returns 1 if successful or -1 on error
 */
int libfvde_encrypted_metadata_read_type_0x0022(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error )
{
	static char *function      = "libfvde_encrypted_metadata_read_type_0x0022";
	size_t block_data_offset   = 0;
	uint64_t entry_index       = 0;
	uint64_t number_of_entries = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit       = 0;
#endif

	if( encrypted_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encrypted metadata.",
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
	if( block_data_size < 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: block data size value too small.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint64_little_endian(
	 &( block_data[ 8 ] ),
	 number_of_entries );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ 0 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: number of entries\t\t\t: %" PRIu64 "\n",
		 function,
		 number_of_entries );

		libcnotify_printf(
		 "\n" );
	}
#endif
/* TODO: check bounds of number_of_entries */
	block_data_offset = 16;

	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint64_little_endian(
			 &( block_data[ block_data_offset ] ),
			 value_64bit );
			libcnotify_printf(
			 "%s: entry: %03d number of blocks\t: %" PRIu64 "\n",
			 function,
			 entry_index,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 &( block_data[ block_data_offset + 8 ] ),
			 value_64bit );
			libcnotify_printf(
			 "%s: entry: %03d unknown2\t\t: 0x%08" PRIx64 "\n",
			 function,
			 entry_index,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 &( block_data[ block_data_offset + 16 ] ),
			 value_64bit );
			libcnotify_printf(
			 "%s: entry: %03d unknown3\t\t: %" PRIu64 "\n",
			 function,
			 entry_index,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 &( block_data[ block_data_offset + 24 ] ),
			 value_64bit );
			libcnotify_printf(
			 "%s: entry: %03d block number\t\t: %" PRIu64 "\n",
			 function,
			 entry_index,
			 value_64bit );

			libcnotify_printf(
			 "\n" );
		}
#endif
		block_data_offset += 32;
	}
	return( 1 );
}

/* Reads the encrypted metadata block type 0x0025
 * Returns 1 if successful or -1 on error
 */
int libfvde_encrypted_metadata_read_type_0x0025(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error )
{
	static char *function      = "libfvde_encrypted_metadata_read_type_0x0025";
	size_t block_data_offset   = 0;
	uint64_t entry_index       = 0;
	uint64_t number_of_entries = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit       = 0;
#endif

	if( encrypted_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encrypted metadata.",
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
	if( block_data_size < 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: block data size value too small.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint64_little_endian(
	 &( block_data[ 8 ] ),
	 number_of_entries );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ 0 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: number of entries\t\t\t: %" PRIu64 "\n",
		 function,
		 number_of_entries );

		libcnotify_printf(
		 "\n" );
	}
#endif
/* TODO: check bounds of number_of_entries */
	block_data_offset = 16;

	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint64_little_endian(
			 &( block_data[ block_data_offset ] ),
			 value_64bit );
			libcnotify_printf(
			 "%s: entry: %03d unknown1\t\t: %" PRIu64 "\n",
			 function,
			 entry_index,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 &( block_data[ block_data_offset + 8 ] ),
			 value_64bit );
			libcnotify_printf(
			 "%s: entry: %03d unknown2\t\t: %" PRIu64 "\n",
			 function,
			 entry_index,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 &( block_data[ block_data_offset + 16 ] ),
			 value_64bit );
			libcnotify_printf(
			 "%s: entry: %03d unknown3\t\t: %" PRIu64 "\n",
			 function,
			 entry_index,
			 value_64bit );
		}
#endif
		block_data_offset += 24;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}

/* Reads the encrypted metadata block type 0x0105
 * Returns 1 if successful or -1 on error
 */
int libfvde_encrypted_metadata_read_type_0x0105(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error )
{
	static char *function      = "libfvde_encrypted_metadata_read_type_0x0105";
	size_t block_data_offset   = 0;
	uint32_t entry_index       = 0;
	uint32_t number_of_entries = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit       = 0;
	uint32_t value_32bit       = 0;
#endif

	if( encrypted_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encrypted metadata.",
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
	if( block_data_size < 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: block data size value too small.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 &( block_data[ 0 ] ),
	 number_of_entries );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of entries\t\t: %" PRIu32 "\n",
		 function,
		 number_of_entries );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 4 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "\n" );
	}
#endif
/* TODO: check bounds of number_of_entries */
	block_data_offset = 8;

	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint64_little_endian(
			 &( block_data[ block_data_offset ] ),
			 value_64bit );
			libcnotify_printf(
			 "%s: entry: %03d unknown1\t: 0x%08" PRIx64 "\n",
			 function,
			 entry_index,
			 value_64bit );
		}
#endif
		block_data_offset += 8;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}

/* Reads the encrypted metadata block type 0x0304
 * Returns 1 if successful or -1 on error
 */
int libfvde_encrypted_metadata_read_type_0x0304(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error )
{
	static char *function      = "libfvde_encrypted_metadata_read_type_0x0304";
	size_t block_data_offset   = 0;
	uint32_t block_number      = 0;
	uint32_t entry_index       = 0;
	uint32_t number_of_blocks  = 0;
	uint32_t number_of_entries = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit       = 0;
	uint32_t value_32bit       = 0;
#endif

	if( encrypted_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encrypted metadata.",
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
	if( block_data_size < 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: block data size value too small.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 &( block_data[ 0 ] ),
	 number_of_entries );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of entries\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_entries );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 4 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "\n" );
	}
#endif
	block_data_offset = 8;

	if( number_of_entries > 0 )
	{
		if( ( (size_t) number_of_entries * 40 ) > ( block_data_size - block_data_offset ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid number of entries value out of bounds.",
			 function );

			return( -1 );
		}
		for( entry_index = 0;
		     entry_index < number_of_entries;
		     entry_index++ )
		{
			byte_stream_copy_to_uint32_little_endian(
			 &( block_data[ block_data_offset + 16 ] ),
			 number_of_blocks );

			byte_stream_copy_to_uint32_little_endian(
			 &( block_data[ block_data_offset + 32 ] ),
			 block_number );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				byte_stream_copy_to_uint64_little_endian(
				 &( block_data[ block_data_offset ] ),
				 value_64bit );
				libcnotify_printf(
				 "%s: entry: %03d unknown1\t\t: 0x%08" PRIx64 "\n",
				 function,
				 entry_index,
				 value_64bit );

				byte_stream_copy_to_uint64_little_endian(
				 &( block_data[ block_data_offset + 8 ] ),
				 value_64bit );
				libcnotify_printf(
				 "%s: entry: %03d unknown2\t\t: 0x%08" PRIx64 "\n",
				 function,
				 entry_index,
				 value_64bit );

				libcnotify_printf(
				 "%s: entry: %03d number of blocks\t: %" PRIu32 "\n",
				 function,
				 entry_index,
				 number_of_blocks );

				byte_stream_copy_to_uint32_little_endian(
				 &( block_data[ block_data_offset + 20 ] ),
				 value_32bit );
				libcnotify_printf(
				 "%s: entry: %03d unknown3\t\t: 0x%08" PRIx32 "\n",
				 function,
				 entry_index,
				 value_32bit );

				byte_stream_copy_to_uint32_little_endian(
				 &( block_data[ block_data_offset + 24 ] ),
				 value_32bit );
				libcnotify_printf(
				 "%s: entry: %03d unknown4\t\t: 0x%08" PRIx32 "\n",
				 function,
				 entry_index,
				 value_32bit );

				byte_stream_copy_to_uint32_little_endian(
				 &( block_data[ block_data_offset + 28 ] ),
				 value_32bit );
				libcnotify_printf(
				 "%s: entry: %03d unknown5\t\t: 0x%08" PRIx32 "\n",
				 function,
				 entry_index,
				 value_32bit );

				libcnotify_printf(
				 "%s: entry: %03d block number\t\t: %" PRIu32 "\n",
				 function,
				 entry_index,
				 block_number );

				byte_stream_copy_to_uint32_little_endian(
				 &( block_data[ block_data_offset + 36 ] ),
				 value_32bit );
				libcnotify_printf(
				 "%s: entry: %03d unknown6\t\t: 0x%08" PRIx32 "\n",
				 function,
				 entry_index,
				 value_32bit );

				libcnotify_printf(
				 "\n" );
			}
#endif
			block_data_offset += 40;
		}
	}
	return( 1 );
}

/* Reads the encrypted metadata block type 0x0305
 * Returns 1 if successful or -1 on error
 */
int libfvde_encrypted_metadata_read_type_0x0305(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     uint64_t block_group,
     libcerror_error_t **error )
{
	libfvde_segment_descriptor_t *segment_descriptor = NULL;
	static char *function                            = "libfvde_encrypted_metadata_read_type_0x0305";
	size_t block_data_offset                         = 0;
	uint32_t block_number                            = 0xffffffffUL;
	uint32_t entry_index                             = 0;
	uint32_t number_of_blocks                        = 0;
	uint32_t number_of_entries                       = 0;
	int segment_descriptor_index                     = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit                             = 0;
	uint32_t value_32bit                             = 0;
#endif

	if( encrypted_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encrypted metadata.",
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
	if( block_data_size < 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: block data size value too small.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 &( block_data[ 0 ] ),
	 number_of_entries );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of entries\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_entries );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 4 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "\n" );
	}
#endif
	block_data_offset = 8;

	/* Use the most recent 0x0305 metadata block
	 */
	if( encrypted_metadata->block_group_0x0305 < block_group )
	{
		if( libcdata_array_empty(
		     encrypted_metadata->segment_descriptors,
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfvde_segment_descriptor_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to empty segment descriptors array.",
			 function );

			goto on_error;
		}
	}
	if( number_of_entries > 0 )
	{
		if( ( (size_t) number_of_entries * 40 ) > ( block_data_size - block_data_offset ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid number of entries value out of bounds.",
			 function );

			goto on_error;
		}
		for( entry_index = 0;
		     entry_index < number_of_entries;
		     entry_index++ )
		{
			if( libfvde_segment_descriptor_initialize(
			     &segment_descriptor,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
				 LIBCERROR_ENCRYPTION_ERROR_GENERIC,
				 "%s: unable to create segment descriptor.",
				 function );

				goto on_error;
			}
			byte_stream_copy_to_uint64_little_endian(
			 &( block_data[ block_data_offset + 8 ] ),
			 segment_descriptor->logical_block_number );

			byte_stream_copy_to_uint32_little_endian(
			 &( block_data[ block_data_offset + 16 ] ),
			 segment_descriptor->number_of_blocks );

			byte_stream_copy_to_uint32_little_endian(
			 &( block_data[ block_data_offset + 32 ] ),
			 segment_descriptor->physical_block_number );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				byte_stream_copy_to_uint64_little_endian(
				 &( block_data[ block_data_offset ] ),
				 value_64bit );
				libcnotify_printf(
				 "%s: entry: %03d unknown1\t\t: 0x%08" PRIx64 "\n",
				 function,
				 entry_index,
				 value_64bit );

				libcnotify_printf(
				 "%s: entry: %03d logical block number\t: %" PRIi64 "\n",
				 function,
				 entry_index,
				 segment_descriptor->logical_block_number );

				libcnotify_printf(
				 "%s: entry: %03d number of blocks\t: %" PRIu32 "\n",
				 function,
				 entry_index,
				 segment_descriptor->number_of_blocks );

				byte_stream_copy_to_uint32_little_endian(
				 &( block_data[ block_data_offset + 20 ] ),
				 value_32bit );
				libcnotify_printf(
				 "%s: entry: %03d unknown3\t\t: 0x%08" PRIx32 "\n",
				 function,
				 entry_index,
				 value_32bit );

				byte_stream_copy_to_uint32_little_endian(
				 &( block_data[ block_data_offset + 24 ] ),
				 value_32bit );
				libcnotify_printf(
				 "%s: entry: %03d unknown4\t\t: 0x%08" PRIx32 "\n",
				 function,
				 entry_index,
				 value_32bit );

				byte_stream_copy_to_uint32_little_endian(
				 &( block_data[ block_data_offset + 28 ] ),
				 value_32bit );
				libcnotify_printf(
				 "%s: entry: %03d unknown5\t\t: 0x%08" PRIx32 "\n",
				 function,
				 entry_index,
				 value_32bit );

				libcnotify_printf(
				 "%s: entry: %03d physical block number\t: %" PRIu32 "\n",
				 function,
				 entry_index,
				 segment_descriptor->physical_block_number );

				byte_stream_copy_to_uint32_little_endian(
				 &( block_data[ block_data_offset + 36 ] ),
				 value_32bit );
				libcnotify_printf(
				 "%s: entry: %03d unknown6\t\t: 0x%08" PRIx32 "\n",
				 function,
				 entry_index,
				 value_32bit );

				libcnotify_printf(
				 "\n" );
			}
#endif
			block_data_offset += 40;

			if( encrypted_metadata->block_group_0x0305 < block_group )
			{
				if( libcdata_array_append_entry(
				     encrypted_metadata->segment_descriptors,
				     &segment_descriptor_index,
				     (intptr_t *) segment_descriptor,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append segment descriptor to array.",
					 function );

					goto on_error;
				}
				if( block_number > segment_descriptor->physical_block_number )
				{
					block_number = segment_descriptor->physical_block_number;
				}
				number_of_blocks  += segment_descriptor->number_of_blocks;
				segment_descriptor = NULL;
			}
			else
			{
				if( libfvde_segment_descriptor_free(
				     &segment_descriptor,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free segment descriptor.",
					 function );

					goto on_error;
				}
			}
		}
		if( encrypted_metadata->block_group_0x0305 < block_group )
		{
			if( number_of_entries == 1 )
			{
				encrypted_metadata->logical_volume_block_number_0x0305     = block_number;
				encrypted_metadata->logical_volume_number_of_blocks_0x0305 = number_of_blocks;

				encrypted_metadata->block_group_0x0305 = block_group;
			}
		}
	}
	return( 1 );

on_error:
	if( segment_descriptor != NULL )
	{
		libfvde_segment_descriptor_free(
		 &segment_descriptor,
		 NULL );
	}
	return( -1 );
}

/* Reads the encrypted metadata block type 0x00404
 * Returns 1 if successful or -1 on error
 */
int libfvde_encrypted_metadata_read_type_0x0404(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     libfvde_io_handle_t *io_handle,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error )
{
	libfvde_data_area_descriptor_t *data_area_descriptor = NULL;
	static char *function                                = "libfvde_encrypted_metadata_read_type_0x0404";
	size_t block_data_offset                             = 0;
	uint32_t entry_index                                 = 0;
	uint32_t number_of_entries                           = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit                                 = 0;
	uint32_t value_32bit                                 = 0;
#endif

	if( encrypted_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encrypted metadata.",
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
	if( block_data_size < 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: block data size value too small.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 &( block_data[ 0 ] ),
	 number_of_entries );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of entries\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_entries );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 4 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "\n" );
	}
#endif
/* TODO: check bounds of number_of_entries */
	block_data_offset = 8;

	if( libcdata_array_empty(
	     encrypted_metadata->data_area_descriptors,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfvde_data_area_descriptor_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty data area descriptors array.",
		 function );

		goto on_error;
	}
	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
		if( libfvde_data_area_descriptor_initialize(
		     &data_area_descriptor,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
			 LIBCERROR_ENCRYPTION_ERROR_GENERIC,
			 "%s: unable to create data area descriptor.",
			 function );

			goto on_error;
		}
		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ block_data_offset ] ),
		 data_area_descriptor->offset );

		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ block_data_offset + 8 ] ),
		 data_area_descriptor->size );

		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ block_data_offset + 16 ] ),
		 data_area_descriptor->data_type );

		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ block_data_offset + 40 ] ),
		 data_area_descriptor->mapped_offset );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: entry: %03d physical block number\t: %" PRIi64 "\n",
			 function,
			 entry_index,
			 data_area_descriptor->offset );

			libcnotify_printf(
			 "%s: entry: %03d number of blocks\t: %" PRIu64 "\n",
			 function,
			 entry_index,
			 data_area_descriptor->size );

			libcnotify_printf(
			 "%s: entry: %03d data type\t\t: 0x%08" PRIx64 "\n",
			 function,
			 entry_index,
			 data_area_descriptor->data_type );

			byte_stream_copy_to_uint64_little_endian(
			 &( block_data[ block_data_offset + 24 ] ),
			 value_64bit );
			libcnotify_printf(
			 "%s: entry: %03d copy number\t\t: %" PRIu64 "\n",
			 function,
			 entry_index,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 &( block_data[ block_data_offset + 32 ] ),
			 value_64bit );
			libcnotify_printf(
			 "%s: entry: %03d unknown1\t\t: 0x%08" PRIx64 "\n",
			 function,
			 entry_index,
			 value_64bit );

			libcnotify_printf(
			 "%s: entry: %03d logical block number\t: %" PRIi64 "\n",
			 function,
			 entry_index,
			 data_area_descriptor->mapped_offset );

			libcnotify_printf(
			 "\n" );
		}
#endif
		block_data_offset += 48;

		data_area_descriptor->offset        *= io_handle->block_size;
		data_area_descriptor->size          *= io_handle->block_size;
		data_area_descriptor->mapped_offset *= io_handle->block_size;

#ifdef TODO
		if( libcdata_array_append_entry(
		     encrypted_metadata->data_area_descriptors,
		     &data_area_descriptor_index,
		     (intptr_t *) data_area_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append data area descriptor to array.",
			 function );

			goto on_error;
		}
#else
		if( libfvde_data_area_descriptor_free(
		     &data_area_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free data area descriptor.",
			 function );

			goto on_error;
		}
#endif
		data_area_descriptor = NULL;
	}
	return( 1 );

on_error:
	if( data_area_descriptor != NULL )
	{
		libfvde_data_area_descriptor_free(
		 &data_area_descriptor,
		 NULL );
	}
	return( -1 );
}

/* Reads the encrypted metadata block type 0x0405
 * Returns 1 if successful or -1 on error
 */
int libfvde_encrypted_metadata_read_type_0x0405(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     libfvde_io_handle_t *io_handle,
     const uint8_t *block_data,
     size_t block_data_size,
     uint64_t block_group,
     libcerror_error_t **error )
{
	libfvde_data_area_descriptor_t *data_area_descriptor = NULL;
	static char *function                                = "libfvde_encrypted_metadata_read_type_0x0405";
	size_t block_data_offset                             = 0;
	uint64_t block_number                                = 0;
	uint64_t number_of_blocks                            = 0;
	uint64_t unknown2                                    = 0;
	uint32_t entry_index                                 = 0;
	uint32_t number_of_entries                           = 0;
	int data_area_descriptor_index                       = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit                                 = 0;
	uint32_t value_32bit                                 = 0;
#endif

	if( encrypted_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encrypted metadata.",
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
	if( block_data_size < 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: block data size value too small.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 &( block_data[ 0 ] ),
	 number_of_entries );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of entries\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_entries );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 4 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "\n" );
	}
#endif
	if( libcdata_array_empty(
	     encrypted_metadata->data_area_descriptors,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfvde_data_area_descriptor_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty data area descriptors array.",
		 function );

		goto on_error;
	}
	block_data_offset = 8;

	if( number_of_entries > 0 )
	{
		if( ( (size_t) number_of_entries * 48 ) > ( block_data_size - block_data_offset ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid number of entries value out of bounds.",
			 function );

			return( -1 );
		}
		for( entry_index = 0;
		     entry_index < number_of_entries;
		     entry_index++ )
		{
			if( libfvde_data_area_descriptor_initialize(
			     &data_area_descriptor,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
				 LIBCERROR_ENCRYPTION_ERROR_GENERIC,
				 "%s: unable to create data area descriptor.",
				 function );

				goto on_error;
			}
			byte_stream_copy_to_uint64_little_endian(
			 &( block_data[ block_data_offset ] ),
			 data_area_descriptor->offset );

			byte_stream_copy_to_uint64_little_endian(
			 &( block_data[ block_data_offset + 8 ] ),
			 data_area_descriptor->size );

			byte_stream_copy_to_uint64_little_endian(
			 &( block_data[ block_data_offset + 16 ] ),
			 data_area_descriptor->data_type );

			byte_stream_copy_to_uint64_little_endian(
			 &( block_data[ block_data_offset + 40 ] ),
			 unknown2 );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: entry: %03d physical block number\t: %" PRIi64 "\n",
				 function,
				 entry_index,
				 data_area_descriptor->offset );

				libcnotify_printf(
				 "%s: entry: %03d number of blocks\t: %" PRIu64 "\n",
				 function,
				 entry_index,
				 data_area_descriptor->size );

				libcnotify_printf(
				 "%s: entry: %03d data type\t\t: 0x%08" PRIx64 "\n",
				 function,
				 entry_index,
				 data_area_descriptor->data_type );

				byte_stream_copy_to_uint64_little_endian(
				 &( block_data[ block_data_offset + 24 ] ),
				 value_64bit );
				libcnotify_printf(
				 "%s: entry: %03d copy number\t\t: %" PRIu64 "\n",
				 function,
				 entry_index,
				 value_64bit );

				byte_stream_copy_to_uint64_little_endian(
				 &( block_data[ block_data_offset + 32 ] ),
				 value_64bit );
				libcnotify_printf(
				 "%s: entry: %03d unknown1\t\t: 0x%08" PRIx64 "\n",
				 function,
				 entry_index,
				 value_64bit );

				libcnotify_printf(
				 "%s: entry: %03d logical block number\t: %" PRIi64 "\n",
				 function,
				 entry_index,
				 unknown2 );

				libcnotify_printf(
				 "\n" );
			}
#endif
			block_data_offset += 48;

			if( ( data_area_descriptor->data_type == 0x09 )
			 || ( data_area_descriptor->data_type == 0x0a ) )
			{
				if( unknown2 == 0 )
				{
					block_number     = (uint64_t) data_area_descriptor->offset;
					number_of_blocks = (uint64_t) data_area_descriptor->size;
				}
			}
			data_area_descriptor->offset *= io_handle->block_size;
			data_area_descriptor->size   *= io_handle->block_size;

			if( libcdata_array_append_entry(
			     encrypted_metadata->data_area_descriptors,
			     &data_area_descriptor_index,
			     (intptr_t *) data_area_descriptor,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append data area descriptor to array.",
				 function );

				goto on_error;
			}
			data_area_descriptor = NULL;
		}
		/* Use the most recent 0x0405 metadata block
		 */
		if( encrypted_metadata->block_group_0x0405 < block_group )
		{
			encrypted_metadata->logical_volume_block_number_0x0405     = block_number;
			encrypted_metadata->logical_volume_number_of_blocks_0x0405 = number_of_blocks;

			encrypted_metadata->block_group_0x0405 = block_group;
		}
	}
	return( 1 );

on_error:
	if( data_area_descriptor != NULL )
	{
		libfvde_data_area_descriptor_free(
		 &data_area_descriptor,
		 NULL );
	}
	return( -1 );
}

/* Reads the encrypted metadata block type 0x0505
 * Returns 1 if successful or -1 on error
 */
int libfvde_encrypted_metadata_read_type_0x0505(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     uint64_t block_group,
     libcerror_error_t **error )
{
	static char *function      = "libfvde_encrypted_metadata_read_type_0x0505";
	size_t block_data_offset   = 0;
	uint64_t block_number      = 0;
	uint64_t number_of_blocks  = 0;
	uint32_t entry_index       = 0;
	uint32_t number_of_entries = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit       = 0;
#endif

	if( encrypted_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encrypted metadata.",
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
	if( block_data_size < 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: block data size value too small.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 &( block_data[ 0 ] ),
	 number_of_entries );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of entries\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_entries );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 4 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "\n" );
	}
#endif
	block_data_offset = 8;

	if( number_of_entries > 0 )
	{
		if( ( (size_t) number_of_entries * 16 ) > ( block_data_size - block_data_offset ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid number of entries value out of bounds.",
			 function );

			return( -1 );
		}
		for( entry_index = 0;
		     entry_index < number_of_entries;
		     entry_index++ )
		{
			byte_stream_copy_to_uint64_little_endian(
			 &( block_data[ block_data_offset ] ),
			 number_of_blocks );

			byte_stream_copy_to_uint64_little_endian(
			 &( block_data[ block_data_offset + 8 ] ),
			 block_number );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: entry: %03d number of blocks\t: %" PRIu64 "\n",
				 function,
				 entry_index,
				 number_of_blocks );

				libcnotify_printf(
				 "%s: entry: %03d block number\t\t: %" PRIu64 "\n",
				 function,
				 entry_index,
				 block_number );

				libcnotify_printf(
				 "\n" );
			}
#endif
			block_data_offset += 16;
		}
		/* Use the most recent 0x0505 metadata block
		 */
		if( encrypted_metadata->block_group_0x0505 < block_group )
		{
			if( number_of_entries == 1 )
			{
				encrypted_metadata->logical_volume_block_number_0x0505     = block_number;
				encrypted_metadata->logical_volume_number_of_blocks_0x0505 = number_of_blocks;

				encrypted_metadata->block_group_0x0505 = block_group;
			}
		}
	}
	return( 1 );
}

/* Reads the encrypted metadata
 * Returns 1 if successful or -1 on error
 */
int libfvde_encrypted_metadata_read(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     libfvde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     uint64_t encrypted_metadata_size,
     libcerror_error_t **error )
{
	uint8_t tweak_value[ 16 ];

	libcaes_tweaked_context_t *xts_context   = NULL;
	libfvde_metadata_block_t *metadata_block = NULL;
	uint8_t *encrypted_metadata_data         = NULL;
	uint8_t *encrypted_data                  = NULL;
	uint8_t *metadata_block_data             = NULL;
	static char *function                    = "libfvde_encrypted_metadata_read";
	ssize_t read_count                       = 0;
	uint64_t calculated_block_number         = 0;
	uint8_t empty_block_found                = 0;
	int result                               = 0;

	if( encrypted_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encrypted metadata.",
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
	if( encrypted_metadata_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing encrypted metadata size.",
		 function );

		goto on_error;
	}
	if( encrypted_metadata_size > (uint64_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid encrypted metadata size value exceeds maximum.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading encrypted metadata at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
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
		 "%s: unable to seek encrypted metadata offset: %" PRIi64 ".",
		 function,
		 file_offset );

		goto on_error;
	}
	encrypted_data = (uint8_t *) memory_allocate(
	                              sizeof( uint8_t ) * (size_t) encrypted_metadata_size );

	if( encrypted_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create encrypted metadata.",
		 function );

		goto on_error;
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              encrypted_data,
	              (size_t) encrypted_metadata_size,
	              error );

	if( read_count != (ssize_t) encrypted_metadata_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read encrypted metadata.",
		 function );

		goto on_error;
	}
	if( libcaes_tweaked_context_initialize(
	     &xts_context,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
		 LIBCERROR_ENCRYPTION_ERROR_GENERIC,
		 "%s: unable to initialize XTS context.",
		 function );

		goto on_error;
	}
	if( libcaes_tweaked_context_set_keys(
	     xts_context,
	     LIBCAES_CRYPT_MODE_DECRYPT,
	     io_handle->key_data,
	     128,
	     io_handle->physical_volume_identifier,
	     128,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to set AES-XTS keys.",
		 function );

		goto on_error;
	}
	metadata_block_data = (uint8_t *) memory_allocate(
	                                   sizeof( uint8_t ) * 8192 );

	if( metadata_block_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create metadata block data.",
		 function );

		goto on_error;
	}
	encrypted_metadata_data = encrypted_data;

/* TODO move data allocation into metadata_block? */
	if( libfvde_metadata_block_initialize(
	     &metadata_block,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create metadata block.",
		 function );

		goto on_error;
	}
	while( encrypted_metadata_size > 0 )
	{
		result = libfvde_metadata_block_check_for_empty_block(
			  encrypted_metadata_data,
			  8192,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if encrypted medadata block data is empty.",
			 function );

			return( -1 );
		}
		else if( result != 0 )
		{
			empty_block_found = 1;
		}
		else if( empty_block_found != 0 )
		{
			break;
		}
		else
		{
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
			 calculated_block_number );

			if( libcaes_crypt_xts(
			     xts_context,
			     LIBCAES_CRYPT_MODE_DECRYPT,
			     tweak_value,
			     16,
			     encrypted_metadata_data,
			     8192,
			     metadata_block_data,
			     8192,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
				 LIBCERROR_ENCRYPTION_ERROR_DECRYPT_FAILED,
				 "%s: unable to decrypt metadata data block: %" PRIu64 " data.",
				 function,
				 calculated_block_number );

				goto on_error;
			}
			if( libfvde_metadata_block_read_data(
			     metadata_block,
			     metadata_block_data,
			     8192,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read metadata block.",
				 function );

				goto on_error;
			}
			if( metadata_block->is_lvf_wiped == 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( ( libcnotify_verbose != 0 )
				 && ( metadata_block->serial_number != io_handle->serial_number ) )
				{
					libcnotify_printf(
					 "%s: mismatch in serial number ( 0x%08" PRIx32 " != 0x%08" PRIx32 " ).\n",
					 function,
					 metadata_block->serial_number,
					 io_handle->serial_number );
				}
#endif
				switch( metadata_block->type )
				{
					case 0x0012:
						result = libfvde_encrypted_metadata_read_type_0x0012(
							  encrypted_metadata,
							  metadata_block->data,
							  metadata_block->data_size,
							  error );
						break;

					case 0x0013:
						result = libfvde_encrypted_metadata_read_type_0x0013(
							  encrypted_metadata,
							  metadata_block->data,
							  metadata_block->data_size,
							  error );
						break;

					case 0x0014:
						result = libfvde_encrypted_metadata_read_type_0x0014(
							  encrypted_metadata,
							  metadata_block->data,
							  metadata_block->data_size,
							  error );
						break;

					case 0x0016:
						result = libfvde_encrypted_metadata_read_type_0x0016(
							  encrypted_metadata,
							  metadata_block->data,
							  metadata_block->data_size,
							  error );
						break;

					case 0x0017:
						result = libfvde_encrypted_metadata_read_type_0x0017(
							  encrypted_metadata,
							  metadata_block->data,
							  metadata_block->data_size,
							  error );
						break;

					case 0x0018:
						result = libfvde_encrypted_metadata_read_type_0x0018(
							  encrypted_metadata,
							  metadata_block->data,
							  metadata_block->data_size,
							  error );
						break;

					case 0x0019:
						result = libfvde_encrypted_metadata_read_type_0x0019(
							  encrypted_metadata,
							  metadata_block->data,
							  metadata_block->data_size,
							  error );
						break;

					case 0x001a:
						result = libfvde_encrypted_metadata_read_type_0x001a(
							  encrypted_metadata,
							  metadata_block->data,
							  metadata_block->data_size,
							  error );
						break;

					case 0x001c:
						result = libfvde_encrypted_metadata_read_type_0x001c(
							  encrypted_metadata,
							  metadata_block->data,
							  metadata_block->data_size,
							  error );
						break;

					case 0x001d:
						result = libfvde_encrypted_metadata_read_type_0x001d(
							  encrypted_metadata,
							  metadata_block->data,
							  metadata_block->data_size,
							  error );
						break;

					case 0x0021:
						result = libfvde_encrypted_metadata_read_type_0x0021(
							  encrypted_metadata,
							  metadata_block->data,
							  metadata_block->data_size,
							  error );
						break;

					case 0x0022:
						result = libfvde_encrypted_metadata_read_type_0x0022(
							  encrypted_metadata,
							  metadata_block->data,
							  metadata_block->data_size,
							  error );
						break;

					case 0x0025:
						result = libfvde_encrypted_metadata_read_type_0x0025(
							  encrypted_metadata,
							  metadata_block->data,
							  metadata_block->data_size,
							  error );
						break;

					case 0x0105:
						result = libfvde_encrypted_metadata_read_type_0x0105(
							  encrypted_metadata,
							  metadata_block->data,
							  metadata_block->data_size,
							  error );
						break;

					case 0x0304:
						result = libfvde_encrypted_metadata_read_type_0x0304(
							  encrypted_metadata,
							  metadata_block->data,
							  metadata_block->data_size,
							  error );
						break;

					case 0x0305:
						result = libfvde_encrypted_metadata_read_type_0x0305(
							  encrypted_metadata,
							  metadata_block->data,
							  metadata_block->data_size,
							  metadata_block->group,
							  error );
						break;

					case 0x0404:
						result = libfvde_encrypted_metadata_read_type_0x0404(
							  encrypted_metadata,
							  io_handle,
							  metadata_block->data,
							  metadata_block->data_size,
							  error );
						break;

					case 0x0405:
						result = libfvde_encrypted_metadata_read_type_0x0405(
							  encrypted_metadata,
							  io_handle,
							  metadata_block->data,
							  metadata_block->data_size,
							  metadata_block->group,
							  error );
						break;

					case 0x0505:
						result = libfvde_encrypted_metadata_read_type_0x0505(
							  encrypted_metadata,
							  metadata_block->data,
							  metadata_block->data_size,
							  metadata_block->group,
							  error );
						break;

					default:
						result = 0;
						break;
				}
				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read metadata block type 0x%04" PRIx16 ".",
					 function,
					 metadata_block->type );

					goto on_error;
				}
			}
		}
		encrypted_metadata_data += 8192;
		encrypted_metadata_size -= 8192;

		calculated_block_number += 1;
	}
/* TODO only use when needed
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( encrypted_metadata_size > 0 )
		{
			libcnotify_printf(
			 "%s: trailing data:\n",
			 function );
			libcnotify_print_data(
			 encrypted_metadata_data,
			 encrypted_metadata_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
	}
#endif
*/
	if( libfvde_metadata_block_free(
	     &metadata_block,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free metadata block.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     metadata_block_data,
	     0,
	     8192 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear metadata block data.",
		 function );

		goto on_error;
	}
	memory_free(
	 metadata_block_data );

	metadata_block_data = NULL;

	if( libcaes_tweaked_context_free(
	     &xts_context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free AES-XTS context.",
		 function );

		goto on_error;
	}
	memory_free(
	 encrypted_data );

	encrypted_data = NULL;

	if( encrypted_metadata->logical_volume_number_of_blocks_0x0505 != 0 )
	{
		encrypted_metadata->logical_volume_offset = encrypted_metadata->logical_volume_block_number_0x0505;
		encrypted_metadata->logical_volume_size   = encrypted_metadata->logical_volume_number_of_blocks_0x0505;
	}
	else if( encrypted_metadata->logical_volume_number_of_blocks_0x0305 != 0 )
	{
		encrypted_metadata->logical_volume_offset = encrypted_metadata->logical_volume_block_number_0x0305;
		encrypted_metadata->logical_volume_size   = encrypted_metadata->logical_volume_number_of_blocks_0x0305;
	}
	else if( encrypted_metadata->logical_volume_number_of_blocks_0x0405 != 0 )
	{
		encrypted_metadata->logical_volume_offset = encrypted_metadata->logical_volume_block_number_0x0405;
		encrypted_metadata->logical_volume_size   = encrypted_metadata->logical_volume_number_of_blocks_0x0405;
	}
	encrypted_metadata->logical_volume_offset *= io_handle->block_size;
	encrypted_metadata->logical_volume_size   *= io_handle->block_size;

	return( 1 );

on_error:
	if( encrypted_metadata->encryption_context_plist != NULL )
	{
		libfvde_encryption_context_plist_initialize(
		 &( encrypted_metadata->encryption_context_plist ),
		 NULL );

		encrypted_metadata->encryption_context_plist_file_is_set = 0;
	}
	if( metadata_block != NULL )
	{
		libfvde_metadata_block_free(
		 &metadata_block,
		 NULL );
	}
	if( metadata_block_data != NULL )
	{
		memory_set(
		 metadata_block_data,
		 0,
		 8192 );
		memory_free(
		 metadata_block_data );
	}
	if( xts_context != NULL )
	{
		libcaes_tweaked_context_free(
		 &xts_context,
		 NULL );
	}
	if( encrypted_data != NULL )
	{
		memory_free(
		 encrypted_data );
	}
	return( -1 );
}

/* Retrieves the volume master key
 * Returns 1 if successful, 0 in not or -1 on error
 */
int libfvde_encrypted_metadata_get_volume_master_key(
     libfvde_encrypted_metadata_t *metadata,
     libfvde_io_handle_t *io_handle,
     libfvde_encryption_context_plist_t *encryption_context_plist,
     libfvde_keyring_t *keyring,
     libcerror_error_t **error )
{
	uint8_t passphrase_key[ 16 ];
	uint8_t volume_key_wrapped_kek[ 24 ];
	uint8_t volume_master_key_wrapped_kek[ 24 ];

	uint8_t *kek_wrapped_volume_key    = NULL;
	uint8_t *passphrase_wrapped_kek    = NULL;
	static char *function              = "libfvde_encrypted_metadata_get_volume_master_key";
	size_t kek_wrapped_volume_key_size = 0;
	size_t passphrase_wrapped_kek_size = 0;
	uint32_t number_of_iterations      = 0;
	uint32_t value_size                = 0;
	uint32_t value_type                = 0;
	int found_key                      = 0;
	int passphrase_wrapped_kek_index   = 0;
	int result                         = 0;

	if( metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid metadata.",
		 function );

		return( -1 );
	}
	if( encryption_context_plist == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid encryption context plist.",
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
	if( keyring == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid keyring.",
		 function );

		return( -1 );
	}
	if( ( io_handle->user_password_is_set != 0 )
	 || ( io_handle->recovery_password_is_set != 0 ) )
	{
		do
		{
			result = libfvde_encryption_context_plist_get_passphrase_wrapped_kek(
			          encryption_context_plist,
			          passphrase_wrapped_kek_index,
			          &passphrase_wrapped_kek,
			          &passphrase_wrapped_kek_size,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_GENERIC,
				 "%s: unable to retrieve passphrase wrapped KEK: %d from encryption context plist.",
				 function,
				 passphrase_wrapped_kek_index );

				goto on_error;
			}
			else if( result == 0 )
			{
				break;
			}
			if( passphrase_wrapped_kek == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing passphrase wrapped KEK: %d.",
				 function,
				 passphrase_wrapped_kek_index );

				goto on_error;
			}
			if( passphrase_wrapped_kek_size != 284 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid passphrase wrapped KEK: %d size value out of bounds.",
				 function,
				 passphrase_wrapped_kek_index );

				goto on_error;
			}
			byte_stream_copy_to_uint32_little_endian(
			 &( passphrase_wrapped_kek[ 0 ] ),
			 value_type );

			byte_stream_copy_to_uint32_little_endian(
			 &( passphrase_wrapped_kek[ 4 ] ),
			 value_size );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: value type\t\t: 0x%08" PRIx32 "\n",
				 function,
				 value_type );

				libcnotify_printf(
				 "%s: value size\t\t: %" PRIu32 "\n",
				 function,
				 value_size );
			}
#endif
			if( value_type != 0x00000003UL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported value type.",
				 function );

				goto on_error;
			}
			if( value_size != 16 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported value size.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: salt:\n",
				 function );
				libcnotify_print_data(
				 &( passphrase_wrapped_kek[ 8 ] ),
				 value_size,
				 0 );
			}
#endif
			byte_stream_copy_to_uint32_little_endian(
			 &( passphrase_wrapped_kek[ 168 ] ),
			 number_of_iterations );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: number of iterations\t: %" PRIu32 "\n",
				 function,
				 number_of_iterations );
			}
#endif
			if( io_handle->user_password_is_set != 0 )
			{
				if( libfvde_password_pbkdf2(
				     io_handle->user_password,
				     io_handle->user_password_size - 1,
				     &( passphrase_wrapped_kek[ 8 ] ),
				     value_size,
				     number_of_iterations,
				     passphrase_key,
				     16,
				     error ) == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to determine password key.",
					 function );

					goto on_error;
				}
			}
			else if( io_handle->recovery_password_is_set != 0 )
			{
				if( libfvde_password_pbkdf2(
				     io_handle->recovery_password,
				     io_handle->recovery_password_size - 1,
				     &( passphrase_wrapped_kek[ 8 ] ),
				     value_size,
				     number_of_iterations,
				     passphrase_key,
				     16,
				     error ) == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to determine recovery password key.",
					 function );

					goto on_error;
				}
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: passphrase key:\n",
				 function );
				libcnotify_print_data(
				 passphrase_key,
				 16,
				 0 );
			}
#endif
			byte_stream_copy_to_uint32_little_endian(
			 &( passphrase_wrapped_kek[ 24 ] ),
			 value_type );

			byte_stream_copy_to_uint32_little_endian(
			 &( passphrase_wrapped_kek[ 28 ] ),
			 value_size );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: value type\t\t: 0x%08" PRIx32 "\n",
				 function,
				 value_type );

				libcnotify_printf(
				 "%s: value size\t\t: %" PRIu32 "\n",
				 function,
				 value_size );
			}
#endif
			if( value_type != 0x00000010UL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported value type.",
				 function );

				goto on_error;
			}
			if( value_size != 24 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported value size.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: encrypted volume key wrapped KEK:\n",
				 function );
				libcnotify_print_data(
				 &( passphrase_wrapped_kek[ 32 ] ),
				 value_size,
				 0 );
			}
#endif
			if( libfvde_encryption_aes_key_unwrap(
			     passphrase_key,
			     16 * 8,
			     &( passphrase_wrapped_kek[ 32 ] ),
			     value_size,
			     volume_key_wrapped_kek,
			     24,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to retrieve volume key wrapped KEK.",
				 function );

				goto on_error;
			}
			if( memory_set(
			     passphrase_key,
			     0,
			     16 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear passphrase key.",
				 function );

				goto on_error;
			}
			memory_free(
			 passphrase_wrapped_kek );

			passphrase_wrapped_kek = NULL;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: volume key wrapped KEK:\n",
				 function );
				libcnotify_print_data(
				 volume_key_wrapped_kek,
				 24,
				 0 );
			}
#endif
	                if( memory_compare(
			     volume_key_wrapped_kek,
			     libfvde_encrypted_metadata_wrapped_kek_initialization_vector,
			     8 ) == 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: KEK wrapped volume key encryption key:\n",
					 function );
					libcnotify_print_data(
					 &( volume_key_wrapped_kek[ 8 ] ),
					 16,
					 0 );
				}
#endif
				found_key = 1;

				break;
			}
			passphrase_wrapped_kek_index++;
		}
		while( result != 0 );

		if( found_key != 0 )
		{
			if( libfvde_encryption_context_plist_get_kek_wrapped_volume_key(
			     encryption_context_plist,
			     &kek_wrapped_volume_key,
			     &kek_wrapped_volume_key_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_GENERIC,
				 "%s: unable to retrieve KEK wrapped volume key from encryption context plist data.",
				 function );

				goto on_error;
			}
			if( kek_wrapped_volume_key == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing KEK wrapped volume key.",
				 function );

				goto on_error;
			}
			if( kek_wrapped_volume_key_size != 256 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid KEK wrapped volume key size value out of bounds.",
				 function );

				goto on_error;
			}
/* TODO: again this could be improved to get size dynamically
 * in case it uses larger keys
 * the first 8 bytes of the KEK is the IV
 */
			if( libfvde_encryption_aes_key_unwrap(
			     &( volume_key_wrapped_kek[ 8 ] ),
			     16 * 8,
			     &( kek_wrapped_volume_key[ 8 ] ),
			     24,
			     volume_master_key_wrapped_kek,
			     24,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to retrieve volume master key.",
				 function );

				goto on_error;
			}
			if( memory_set(
			     volume_key_wrapped_kek,
			     0,
			     24 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear volume key wrapped KEK.",
				 function );

				goto on_error;
			}
			memory_free(
			 kek_wrapped_volume_key );

			kek_wrapped_volume_key = NULL;

			if( memory_compare(
			     volume_master_key_wrapped_kek,
			     libfvde_encrypted_metadata_wrapped_kek_initialization_vector,
			     8 ) == 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: volume master key:\n",
					 function );
					libcnotify_print_data(
					 volume_master_key_wrapped_kek,
					 24,
					 0 );
				}
#endif
				if( memory_copy(
				     &( keyring->volume_master_key ),
				     &( volume_master_key_wrapped_kek[ 8 ] ),
				     16 ) == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
					 "%s: unable to copy volume master key from KEK.",
					 function );

					goto on_error;
				}
			}
			if( memory_set(
			     volume_master_key_wrapped_kek,
			     0,
			     24 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear volume master key wrapped KEK.",
				 function );

				goto on_error;
			}
		}
	}
	return( found_key );

on_error:
	if( kek_wrapped_volume_key != NULL )
	{
		memory_free(
		 kek_wrapped_volume_key );
	}
	if( passphrase_wrapped_kek != NULL )
	{
		memory_free(
		 passphrase_wrapped_kek );
	}
	memory_set(
	 volume_master_key_wrapped_kek,
	 0,
	 24 );

	memory_set(
	 volume_key_wrapped_kek,
	 0,
	 24 );

	memory_set(
	 passphrase_key,
	 0,
	 16 );

	return( -1 );
}

/* Retrieves the number data area descriptors
 * Returns 1 if successful or -1 on error
 */
int libfvde_encrypted_metadata_get_number_of_data_area_descriptors(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     int *data_area_descriptors,
     libcerror_error_t **error )
{
	static char *function = "libfvde_encrypted_metadata_get_number_of_data_area_descriptors";

	if( encrypted_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encrypted metadata.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     encrypted_metadata->data_area_descriptors,
	     data_area_descriptors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of data area descriptors.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific data area descriptor
 * Returns 1 if successful or -1 on error
 */
int libfvde_encrypted_metadata_get_data_area_descriptor_by_index(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     int data_area_descriptor_index,
     libfvde_data_area_descriptor_t **data_area_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfvde_encrypted_metadata_get_data_area_descriptor_by_index";

	if( encrypted_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid encrypted metadata.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     encrypted_metadata->data_area_descriptors,
	     data_area_descriptor_index,
	     (intptr_t **) data_area_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data area descriptor: %d.",
		 function,
		 data_area_descriptor_index );

		return( -1 );
	}
	return( 1 );
}

