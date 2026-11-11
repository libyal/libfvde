/*
 * OSS-Fuzz target for libfvde volume header type
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

#include "../libfvde/libfvde_volume_header.h"

int LLVMFuzzerTestOneInput(
     const uint8_t *data,
     size_t size )
{
	uint8_t uuid[ 16 ];

	libbfio_handle_t *file_io_handle            = NULL;
	libfvde_volume_header_t *volume_header      = NULL;
	uint32_t encryption_method                  = 0;
	size64_t physical_volume_size               = 0;

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
	/* Reads the volume header using a Basic File IO (bfio) handle
	 */
	if( libfvde_volume_header_initialize(
	     &volume_header,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libfvde_volume_header_read_file_io_handle(
	     volume_header,
	     file_io_handle,
	     0,
	     NULL ) != 1 )
	{
		goto on_error_libfvde_volume_header;
	}
	libfvde_volume_header_get_volume_group_identifier(
	 volume_header,
	 uuid,
	 16,
	 NULL );

	libfvde_volume_header_get_physical_volume_identifier(
	 volume_header,
	 uuid,
	 16,
	 NULL );

	libfvde_volume_header_get_physical_volume_encryption_method(
	 volume_header,
	 &encryption_method,
	 NULL );

	libfvde_volume_header_get_physical_volume_size(
	 volume_header,
	 &physical_volume_size,
	 NULL );

on_error_libfvde_volume_header:
	libfvde_volume_header_free(
	 &volume_header,
	 NULL );

	volume_header = NULL;

	/* Reads the volume header data
	 */
	if( libfvde_volume_header_initialize(
	     &volume_header,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libfvde_volume_header_read_data(
	     volume_header,
	     data,
	     size,
	     NULL ) != 1 )
	{
		goto on_error_libfvde_volume_header2;
	}
	libfvde_volume_header_get_volume_group_identifier(
	 volume_header,
	 uuid,
	 16,
	 NULL );

	libfvde_volume_header_get_physical_volume_identifier(
	 volume_header,
	 uuid,
	 16,
	 NULL );

	libfvde_volume_header_get_physical_volume_encryption_method(
	 volume_header,
	 &encryption_method,
	 NULL );

	libfvde_volume_header_get_physical_volume_size(
	 volume_header,
	 &physical_volume_size,
	 NULL );

on_error_libfvde_volume_header2:
	libfvde_volume_header_free(
	 &volume_header,
	 NULL );

	volume_header = NULL;

on_error_libbfio:
	libbfio_handle_free(
	 &file_io_handle,
	 NULL );
	return( 0 );
}

} /* extern "C" */
