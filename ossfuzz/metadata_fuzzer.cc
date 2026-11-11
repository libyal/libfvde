/*
 * OSS-Fuzz target for libfvde metadata type
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

#include "../libfvde/libfvde_io_handle.h"
#include "../libfvde/libfvde_metadata.h"

int LLVMFuzzerTestOneInput(
     const uint8_t *data,
     size_t size )
{
	uint8_t uuid[ 16 ];
	uint8_t utf8_string[ 64 ];
	uint16_t utf16_string[ 64 ];

	libbfio_handle_t *file_io_handle                          = NULL;
	libfvde_io_handle_t *io_handle                             = NULL;
	libfvde_metadata_t *metadata                               = NULL;
	libfvde_physical_volume_descriptor_t *physical_volume_descriptor = NULL;
	size_t string_size                                         = 0;
	int number_of_physical_volume_descriptors                = 0;

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

	/* Reads the volume group plist
	 */
	if( libfvde_metadata_initialize(
	     &metadata,
	     NULL ) != 1 )
	{
		goto on_error_libfvde_io_handle;
	}
	if( libfvde_metadata_read_volume_group_plist(
	     metadata,
	     data,
	     size,
	     NULL ) != 1 )
	{
		goto on_error_libfvde_metadata;
	}
	libfvde_metadata_get_utf8_volume_group_name_size(
	 metadata,
	 &string_size,
	 NULL );

	libfvde_metadata_get_utf8_volume_group_name(
	 metadata,
	 utf8_string,
	 64,
	 NULL );

	libfvde_metadata_get_utf16_volume_group_name_size(
	 metadata,
	 &string_size,
	 NULL );

	libfvde_metadata_get_utf16_volume_group_name(
	 metadata,
	 utf16_string,
	 64,
	 NULL );

	if( libfvde_metadata_get_number_of_physical_volume_descriptors(
	     metadata,
	     &number_of_physical_volume_descriptors,
	     NULL ) == 1 )
	{
		if( number_of_physical_volume_descriptors > 0 )
		{
			if( libfvde_metadata_get_physical_volume_descriptor_by_index(
			     metadata,
			     0,
			     &physical_volume_descriptor,
			     NULL ) == 1 )
			{
				libfvde_physical_volume_descriptor_get_identifier(
				 physical_volume_descriptor,
				 uuid,
				 16,
				 NULL );
				libfvde_physical_volume_descriptor_free(
				 &physical_volume_descriptor,
				 NULL );
			}
		}
	}
on_error_libfvde_metadata:
	libfvde_metadata_free(
	 &metadata,
	 NULL );

	metadata = NULL;

	/* Reads the metadata block of type 0x0011
	 */
	if( libfvde_metadata_initialize(
	     &metadata,
	     NULL ) != 1 )
	{
		goto on_error_libfvde_io_handle;
	}
	if( libfvde_metadata_read_type_0x0011(
	     metadata,
	     io_handle,
	     0,
	     data,
	     size,
	     NULL ) != 1 )
	{
		goto on_error_libfvde_metadata2;
	}
	libfvde_metadata_get_utf8_volume_group_name_size(
	 metadata,
	 &string_size,
	 NULL );

	libfvde_metadata_get_utf8_volume_group_name(
	 metadata,
	 utf8_string,
	 64,
	 NULL );

	libfvde_metadata_get_utf16_volume_group_name_size(
	 metadata,
	 &string_size,
	 NULL );

	libfvde_metadata_get_utf16_volume_group_name(
	 metadata,
	 utf16_string,
	 64,
	 NULL );

	if( libfvde_metadata_get_number_of_physical_volume_descriptors(
	     metadata,
	     &number_of_physical_volume_descriptors,
	     NULL ) == 1 )
	{
		if( number_of_physical_volume_descriptors > 0 )
		{
			if( libfvde_metadata_get_physical_volume_descriptor_by_index(
			     metadata,
			     0,
			     &physical_volume_descriptor,
			     NULL ) == 1 )
			{
				libfvde_physical_volume_descriptor_get_identifier(
				 physical_volume_descriptor,
				 uuid,
				 16,
				 NULL );
				libfvde_physical_volume_descriptor_free(
				 &physical_volume_descriptor,
				 NULL );
			}
		}
	}
on_error_libfvde_metadata2:
	libfvde_metadata_free(
	 &metadata,
	 NULL );

	metadata = NULL;

	/* Reads the metadata using a Basic File IO (bfio) handle
	 */
	if( libfvde_metadata_initialize(
	     &metadata,
	     NULL ) != 1 )
	{
		goto on_error_libfvde_io_handle;
	}
	if( libfvde_metadata_read_file_io_handle(
	     metadata,
	     io_handle,
	     file_io_handle,
	     0,
	     NULL ) != 1 )
	{
		goto on_error_libfvde_metadata3;
	}
	libfvde_metadata_get_utf8_volume_group_name_size(
	 metadata,
	 &string_size,
	 NULL );

	libfvde_metadata_get_utf8_volume_group_name(
	 metadata,
	 utf8_string,
	 64,
	 NULL );

	libfvde_metadata_get_utf16_volume_group_name_size(
	 metadata,
	 &string_size,
	 NULL );

	libfvde_metadata_get_utf16_volume_group_name(
	 metadata,
	 utf16_string,
	 64,
	 NULL );

	if( libfvde_metadata_get_number_of_physical_volume_descriptors(
	     metadata,
	     &number_of_physical_volume_descriptors,
	     NULL ) == 1 )
	{
		if( number_of_physical_volume_descriptors > 0 )
		{
			if( libfvde_metadata_get_physical_volume_descriptor_by_index(
			     metadata,
			     0,
			     &physical_volume_descriptor,
			     NULL ) == 1 )
			{
				libfvde_physical_volume_descriptor_get_identifier(
				 physical_volume_descriptor,
				 uuid,
				 16,
				 NULL );
				libfvde_physical_volume_descriptor_free(
				 &physical_volume_descriptor,
				 NULL );
			}
		}
	}
on_error_libfvde_metadata3:
	libfvde_metadata_free(
	 &metadata,
	 NULL );

	metadata = NULL;

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
