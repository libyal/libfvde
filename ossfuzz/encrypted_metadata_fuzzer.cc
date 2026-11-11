/*
 * OSS-Fuzz target for libfvde encrypted metadata type
 *
 * Copyright (C) 2026, Joachim Metz <joachim.metz@gmail.com>
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

#include <stddef.h>
#include <stdint.h>

/* Note that some of the OSS-Fuzz engines use C++
 */
extern "C" {

#include "ossfuzz_libbfio.h"
#include "ossfuzz_libfvde.h"

#include "../libfvde/libfvde_encrypted_metadata.h"
#include "../libfvde/libfvde_io_handle.h"
#include "../libfvde/libfvde_logical_volume_descriptor.h"

int LLVMFuzzerTestOneInput(
     const uint8_t *data,
     size_t size )
{
	uint8_t uuid[ 16 ];
	uint8_t utf8_string[ 64 ];
	uint16_t utf16_string[ 64 ];

	/* The volume master key
	 *
	 * Note the tweak key is zero.
	 */
	uint8_t key[ 16 ] =
	{
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
		0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10
	};

	uint8_t tweak_key[ 16 ] =
	{
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

	libbfio_handle_t *file_io_handle                       = NULL;
	libfvde_encrypted_metadata_t *encrypted_metadata        = NULL;
	libfvde_io_handle_t *io_handle                          = NULL;
	libfvde_logical_volume_descriptor_t *logical_volume_descriptor          = NULL;
	libfvde_segment_descriptor_t *first_segment_descriptor  = NULL;
	libfvde_segment_descriptor_t *second_segment_descriptor = NULL;
	size64_t descriptor_size                             = 0;
	uint64_t block_number                                   = 0;
	uint16_t volume_index                                    = 0;
	size_t string_size                                       = 0;
	int descriptor_index                                    = 0;
	int number_of_logical_volume_descriptors                = 0;

	if( libbfio_memory_range_initialize(
	     &file_io_handle,
	     NULL ) != 1 )
	{
		return( 0 );
	}
	if( libbfio_memory_range_set(
	     file_io_handle,
	     (uint8_t *) data,
	     size,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libfvde_io_handle_initialize(
	     &io_handle,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	io_handle->bytes_per_sector = 4096;
	io_handle->block_size       = 4096;
	io_handle->metadata_size    = 8192;
	io_handle->serial_number    = 1;

	/* Reads the encrypted metadata using a Basic File IO (bfio) handle
	 */
	if( libfvde_encrypted_metadata_initialize(
	     &encrypted_metadata,
	     NULL ) != 1 )
	{
		goto on_error_libfvde_io_handle;
	}
	if( libfvde_encrypted_metadata_read_from_file_io_handle(
	     encrypted_metadata,
	     io_handle,
	     file_io_handle,
	     0,
	     (uint64_t) size,
	     key,
	     128,
	     tweak_key,
	     128,
	     NULL ) != 1 )
	{
		goto on_error_libfvde_encrypted_metadata;
	}
	if( libfvde_encrypted_metadata_get_number_of_logical_volume_descriptors(
	     encrypted_metadata,
	     &number_of_logical_volume_descriptors,
	     NULL ) == 1 )
	{
		for( descriptor_index = 0;
		     descriptor_index < number_of_logical_volume_descriptors;
		     descriptor_index++ )
		{
			if( libfvde_encrypted_metadata_get_logical_volume_descriptor_by_index(
			     encrypted_metadata,
			     descriptor_index,
			     &logical_volume_descriptor,
			     NULL ) != 1 )
			{
				break;
			}
			libfvde_logical_volume_descriptor_get_identifier(
			 logical_volume_descriptor,
			 uuid,
			 16,
			 NULL );

			libfvde_logical_volume_descriptor_get_family_identifier(
			 logical_volume_descriptor,
			 uuid,
			 16,
			 NULL );

			libfvde_logical_volume_descriptor_get_utf8_name_size(
			 logical_volume_descriptor,
			 &string_size,
			 NULL );

			libfvde_logical_volume_descriptor_get_utf8_name(
			 logical_volume_descriptor,
			 utf8_string,
			 64,
			 NULL );

			libfvde_logical_volume_descriptor_get_utf16_name_size(
			 logical_volume_descriptor,
			 &string_size,
			 NULL );

			libfvde_logical_volume_descriptor_get_utf16_name(
			 logical_volume_descriptor,
			 utf16_string,
			 64,
			 NULL );

			libfvde_logical_volume_descriptor_get_size(
			 logical_volume_descriptor,
			 &descriptor_size,
			 NULL );

			libfvde_logical_volume_descriptor_get_first_block_number(
			 logical_volume_descriptor,
			 &volume_index,
			 &block_number,
			 NULL );

			libfvde_logical_volume_descriptor_get_last_block_number(
			 logical_volume_descriptor,
			 &volume_index,
			 &block_number,
			 NULL );

			{
				int number_of_segment_descriptors = 0;

				if( libfvde_logical_volume_descriptor_get_number_of_segment_descriptors(
				     logical_volume_descriptor,
				     &number_of_segment_descriptors,
				     NULL ) == 1 )
				{
					if( number_of_segment_descriptors > 0 )
					{
						if( libfvde_logical_volume_descriptor_get_segment_descriptor_by_index(
						     logical_volume_descriptor,
						     0,
						     &first_segment_descriptor,
						     NULL ) == 1 )
						{
							if( number_of_segment_descriptors > 1 )
							{
								if( libfvde_logical_volume_descriptor_get_segment_descriptor_by_index(
									     logical_volume_descriptor,
									     1,
									     &second_segment_descriptor,
									     NULL ) == 1 )
								{
									libfvde_segment_descriptor_compare(
									 first_segment_descriptor,
									 second_segment_descriptor,
									 NULL );
									libfvde_segment_descriptor_free(
									 &second_segment_descriptor,
									 NULL );
								}
							}
							libfvde_segment_descriptor_free(
							 &first_segment_descriptor,
							 NULL );
						}
					}
				}
			}
			libfvde_logical_volume_descriptor_free(
			 &logical_volume_descriptor,
			 NULL );
		}
		if( libfvde_encrypted_metadata_get_logical_volume_descriptor_by_object_identifier(
		     encrypted_metadata,
		     1,
		     &logical_volume_descriptor,
		     NULL ) == 1 )
		{
			libfvde_logical_volume_descriptor_get_identifier(
			 logical_volume_descriptor,
			 uuid,
			 16,
			 NULL );
			libfvde_logical_volume_descriptor_free(
			 &logical_volume_descriptor,
			 NULL );
		}
		if( libfvde_encrypted_metadata_get_last_logical_volume_descriptor(
		     encrypted_metadata,
		     &logical_volume_descriptor,
		     NULL ) == 1 )
		{
			libfvde_logical_volume_descriptor_get_identifier(
			 logical_volume_descriptor,
			 uuid,
			 16,
			 NULL );
			libfvde_logical_volume_descriptor_free(
			 &logical_volume_descriptor,
			 NULL );
		}
	}
on_error_libfvde_encrypted_metadata:
	libfvde_encrypted_metadata_free(
	 &encrypted_metadata,
	 NULL );

	encrypted_metadata = NULL;

	/* Reads individual raw encrypted metadata blocks
	 */
	if( libfvde_encrypted_metadata_initialize(
	     &encrypted_metadata,
	     NULL ) != 1 )
	{
		goto on_error_libfvde_io_handle;
	}
	libfvde_encrypted_metadata_read_type_0x0010(
	 encrypted_metadata,
	 data,
	 size,
	 NULL );

	libfvde_encrypted_metadata_read_type_0x0011(
	 encrypted_metadata,
	 data,
	 size,
	 NULL );

	libfvde_encrypted_metadata_read_type_0x0012(
	 encrypted_metadata,
	 data,
	 size,
	 NULL );

	libfvde_encrypted_metadata_read_type_0x0013(
	 encrypted_metadata,
	 data,
	 size,
	 NULL );

	libfvde_encrypted_metadata_read_type_0x0014(
	 encrypted_metadata,
	 data,
	 size,
	 NULL );

	libfvde_encrypted_metadata_read_type_0x0016(
	 encrypted_metadata,
	 data,
	 size,
	 NULL );

	libfvde_encrypted_metadata_read_type_0x0017(
	 encrypted_metadata,
	 data,
	 size,
	 NULL );

	libfvde_encrypted_metadata_read_type_0x0018(
	 encrypted_metadata,
	 data,
	 size,
	 NULL );

	libfvde_encrypted_metadata_read_type_0x0019(
	 encrypted_metadata,
	 data,
	 size,
	 NULL );

	libfvde_encrypted_metadata_read_type_0x001a(
	 encrypted_metadata,
	 data,
	 size,
	 NULL );

	libfvde_encrypted_metadata_read_type_0x001c(
	 encrypted_metadata,
	 data,
	 size,
	 NULL );

	libfvde_encrypted_metadata_read_type_0x001d(
	 encrypted_metadata,
	 data,
	 size,
	 NULL );

	libfvde_encrypted_metadata_read_type_0x0021(
	 encrypted_metadata,
	 data,
	 size,
	 NULL );

	libfvde_encrypted_metadata_read_type_0x0022(
	 encrypted_metadata,
	 data,
	 size,
	 NULL );

	libfvde_encrypted_metadata_read_type_0x0024(
	 encrypted_metadata,
	 1,
	 data,
	 size,
	 NULL );

	libfvde_encrypted_metadata_read_type_0x0025(
	 encrypted_metadata,
	 data,
	 size,
	 NULL );

	libfvde_encrypted_metadata_read_type_0x0105(
	 encrypted_metadata,
	 data,
	 size,
	 NULL );

	libfvde_encrypted_metadata_read_type_0x0205(
	 encrypted_metadata,
	 data,
	 size,
	 NULL );

	libfvde_encrypted_metadata_read_type_0x0304(
	 encrypted_metadata,
	 data,
	 size,
	 NULL );

	libfvde_encrypted_metadata_read_type_0x0305(
	 encrypted_metadata,
	 1,
	 data,
	 size,
	 NULL );

	libfvde_encrypted_metadata_read_type_0x0404(
	 encrypted_metadata,
	 io_handle,
	 data,
	 size,
	 NULL );

	libfvde_encrypted_metadata_read_type_0x0405(
	 encrypted_metadata,
	 io_handle,
	 data,
	 size,
	 NULL );

	libfvde_encrypted_metadata_read_type_0x0505(
	 encrypted_metadata,
	 1,
	 data,
	 size,
	 NULL );

	libfvde_encrypted_metadata_read_type_0x0605(
	 encrypted_metadata,
	 data,
	 size,
	 NULL );

on_error_libfvde_encrypted_metadata2:
	libfvde_encrypted_metadata_free(
	 &encrypted_metadata,
	 NULL );

on_error_libfvde_io_handle:
	libfvde_io_handle_free(
	 &io_handle,
	 NULL );

on_error_libbfio:
	libbfio_handle_free(
	 &file_io_handle,
	 NULL );
	return( 0 );
}

} /* extern "C" */
