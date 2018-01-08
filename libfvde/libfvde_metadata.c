/*
 * Metadata functions
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
#include <types.h>

#include "libfvde_checksum.h"
#include "libfvde_io_handle.h"
#include "libfvde_libbfio.h"
#include "libfvde_libcerror.h"
#include "libfvde_libcnotify.h"
#include "libfvde_libfplist.h"
#include "libfvde_metadata.h"
#include "libfvde_metadata_block.h"

#include "fvde_metadata.h"

/* Creates metadata
 * Make sure the value metadata is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfvde_metadata_initialize(
     libfvde_metadata_t **metadata,
     libcerror_error_t **error )
{
	static char *function = "libfvde_metadata_initialize";

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
	if( *metadata != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid metadata value already set.",
		 function );

		return( -1 );
	}
	*metadata = memory_allocate_structure(
	             libfvde_metadata_t );

	if( *metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create metadata.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *metadata,
	     0,
	     sizeof( libfvde_metadata_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear metadata.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *metadata != NULL )
	{
		memory_free(
		 *metadata );

		*metadata = NULL;
	}
	return( -1 );
}

/* Frees metadata
 * Returns 1 if successful or -1 on error
 */
int libfvde_metadata_free(
     libfvde_metadata_t **metadata,
     libcerror_error_t **error )
{
	static char *function = "libfvde_metadata_free";

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
	if( *metadata != NULL )
	{
		memory_free(
		 *metadata );

		*metadata = NULL;
	}
	return( 1 );
}

/* Reads the metadata block type 0x0011
 * Returns 1 if successful or -1 on error
 */
int libfvde_metadata_read_type_0x0011(
     libfvde_metadata_t *metadata,
     libfvde_io_handle_t *io_handle,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error )
{
	static char *function                    = "libfvde_metadata_read_type_0x0011";
	size_t block_data_offset                 = 0;
	uint32_t entry_index                     = 0;
	uint32_t metadata_size                   = 0;
	uint32_t number_of_entries               = 0;
	uint32_t volume_groups_descriptor_offset = 0;
	uint32_t xml_offset                      = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit                     = 0;
	uint32_t value_32bit                     = 0;
	uint16_t value_16bit                     = 0;
#endif

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
/* TODO data size check */
	if( block_data_size < 48 )
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
	 block_data,
	 metadata_size );

	byte_stream_copy_to_uint32_little_endian(
	 &( block_data[ 156 ] ),
	 volume_groups_descriptor_offset );

	byte_stream_copy_to_uint32_little_endian(
	 &( block_data[ 160 ] ),
	 xml_offset );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: metadata size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 metadata_size );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 4 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 8 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: checksum\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 12 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: initial value\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 16 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 20 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown3\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 24 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown4\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 28 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown5\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: unknown6:\n",
		 function );
		libcnotify_print_data(
		 &( block_data[ 32 ] ),
		 40,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 72 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown7\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 76 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown8\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: unknown9:\n",
		 function );
		libcnotify_print_data(
		 &( block_data[ 80 ] ),
		 24,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 104 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown10\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 108 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown11\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 112 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown12\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint16_little_endian(
		 &( block_data[ 116 ] ),
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown13a\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 &( block_data[ 118 ] ),
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown13b\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 120 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown14\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint16_little_endian(
		 &( block_data[ 124 ] ),
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown15a\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 &( block_data[ 126 ] ),
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown15b\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 128 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown16\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: unknown17:\n",
		 function );
		libcnotify_print_data(
		 &( block_data[ 132 ] ),
		 24,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

		libcnotify_printf(
		 "%s: volume groups descriptor offset\t\t: 0x%08" PRIx32 "\n",
		 function,
		 volume_groups_descriptor_offset );

		libcnotify_printf(
		 "%s: XML offset\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 xml_offset );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 164 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: XML size\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 168 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: XML size copy\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 172 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown18\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ 176 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: volume group number of blocks\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "\n" );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 &( block_data[ 184 ] ),
	 number_of_entries );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: number of entries\t\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_entries );

		byte_stream_copy_to_uint32_little_endian(
		 &( block_data[ 188 ] ),
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown19\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "\n" );
	}
#endif
/* TODO: check bounds of number_of_entries */
	block_data_offset = 192;

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
			 "%s: entry: %03d block group\t\t\t: %" PRIu64 "\n",
			 function,
			 entry_index,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 &( block_data[ block_data_offset + 8 ] ),
			 value_64bit );
			libcnotify_printf(
			 "%s: entry: %03d unknown2\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 entry_index,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 &( block_data[ block_data_offset + 16 ] ),
			 value_64bit );
			libcnotify_printf(
			 "%s: entry: %03d unknown3\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 entry_index,
			 value_64bit );

			libcnotify_printf(
			 "\n" );
		}
#endif
		block_data_offset += 24;
	}
	if( metadata_size != io_handle->metadata_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_VALUE_MISMATCH,
		 "%s: value mismatch for metadata size.",
		 function );

		return( -1 );
	}
	if( ( volume_groups_descriptor_offset < 248 )
	 || ( volume_groups_descriptor_offset > io_handle->metadata_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid volume groups descriptor offset value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: remaining data:\n",
		 function );
		libcnotify_print_data(
		 &( block_data[ 8192 - 64 ] ),
		 io_handle->metadata_size - 8192,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	/* The offset is relative to the start of the metadata block */
	volume_groups_descriptor_offset -= 64;

	byte_stream_copy_to_uint64_little_endian(
	 &( block_data[ volume_groups_descriptor_offset + 8 ] ),
	 metadata->encrypted_metadata_size );

	byte_stream_copy_to_uint64_little_endian(
	 &( block_data[ volume_groups_descriptor_offset + 32 ] ),
	 metadata->primary_encrypted_metadata_offset );

	byte_stream_copy_to_uint64_little_endian(
	 &( block_data[ volume_groups_descriptor_offset + 40 ] ),
	 metadata->secondary_encrypted_metadata_offset );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ volume_groups_descriptor_offset ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: encrypted metadata number of blocks\t\t: %" PRIu64 "\n",
		 function,
		 metadata->encrypted_metadata_size );

		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ volume_groups_descriptor_offset + 16 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown3\t\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 &( block_data[ volume_groups_descriptor_offset + 24 ] ),
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown4\t\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: primary encrypted metadata block number\t: %" PRIu64 "\n",
		 function,
		 metadata->primary_encrypted_metadata_offset );

		libcnotify_printf(
		 "%s: secondary encrypted metadata block number\t: %" PRIu64 "\n",
		 function,
		 metadata->secondary_encrypted_metadata_offset );

		libcnotify_printf(
		 "\n" );
	}
#endif
	metadata->encrypted_metadata_size *= io_handle->block_size;

	if( ( xml_offset < 248 )
	 || ( xml_offset > io_handle->metadata_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid XML offset value out of bounds.",
		 function );

		return( -1 );
	}
	/* The offset is relative to the start of the metadata block */
	if( libfvde_metadata_read_core_storage_plist(
	     metadata,
	     &( block_data[ xml_offset - 64 ] ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read metadata block type 0x0011.",
		 function );

		return( -1 );
	}
/* TODO add bounds check */
	metadata->primary_encrypted_metadata_offset   *= io_handle->block_size;
	metadata->secondary_encrypted_metadata_offset *= io_handle->block_size;

	return( 1 );
}

/* Reads the core storage (XML) plist
 * Returns 1 if successful or -1 on error
 */
int libfvde_metadata_read_core_storage_plist(
     libfvde_metadata_t *metadata,
     const uint8_t *xml_plist_data,
     libcerror_error_t **error )
{
	libfplist_property_list_t *property_list = NULL;
	libfplist_property_t *root_property      = NULL;
	libfplist_property_t *sub_property       = NULL;
	static char *function                    = "libfvde_metadata_read_core_storage_plist";
	size_t xml_length                        = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t *string                          = NULL;
	size_t string_size                       = 0;
#endif

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
	if( xml_plist_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid XML plist data.",
		 function );

		return( -1 );
	}
/* TODO return 0 if not a valid plist? */
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
		     (uint8_t *) "com.apple.corestorage.lvg.uuid",
		     30,
		     &sub_property,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve com.apple.corestorage.lvg.uuid sub property.",
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
				 "%s: unable to retrieve logical volume group identifier.",
				 function );

				goto on_error;
			}
			libcnotify_printf(
			 "%s: logical volume group identifier\t\t: %s\n",
			 function,
			 string );

			memory_free(
			 string );

			string = NULL;
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
			 "%s: unable to free com.apple.corestorage.lvg.uuid property.",
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

/* Reads the metadata
 * Returns 1 if successful or -1 on error
 */
int libfvde_metadata_read(
     libfvde_metadata_t *metadata,
     libfvde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	libfvde_metadata_block_t *metadata_block = NULL;
	uint8_t *metadata_block_data             = NULL;
	static char *function                    = "libfvde_metadata_read";
	size_t metadata_block_data_size          = 0;
	ssize_t read_count                       = 0;

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
	if( io_handle->block_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - missing block size.",
		 function );

		return( -1 );
	}
	if( io_handle->metadata_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - missing metadata block size.",
		 function );

		return( -1 );
	}
/* TODO a minimal size check */
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading metadata at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
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
		 "%s: unable to seek metadata offset: %" PRIi64 ".",
		 function,
		 file_offset );

		goto on_error;
	}
	metadata_block_data_size = (size_t) io_handle->metadata_size;

	metadata_block_data = (uint8_t *) memory_allocate(
	                                   sizeof( uint8_t ) * metadata_block_data_size );

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
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              metadata_block_data,
	              metadata_block_data_size,
	              error );

	if( read_count != (ssize_t) metadata_block_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read metadata block data.",
		 function );

		goto on_error;
	}
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
	if( metadata_block->type != 0x0011 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported metadata block type: 0x%04" PRIx16 ".",
		 function,
		 metadata_block->type );

		goto on_error;
	}
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
	if( libfvde_metadata_read_type_0x0011(
	     metadata,
	     io_handle,
	     metadata_block->data,
	     metadata_block->data_size,
	     error ) != 1 )
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
	memory_free(
	 metadata_block_data );

	metadata_block_data = NULL;

	return( 1 );

on_error:
	if( metadata_block != NULL )
	{
		libfvde_metadata_block_free(
		 &metadata_block,
		 NULL );
	}
	if( metadata_block_data != NULL )
	{
		memory_free(
		 metadata_block_data );
	}
	return( -1 );
}

