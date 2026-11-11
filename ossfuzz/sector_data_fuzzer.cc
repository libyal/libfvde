/*
 * OSS-Fuzz target for libfvde sector data type
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

#include "../libfvde/libfvde_definitions.h"
#include "../libfvde/libfvde_encryption_context.h"
#include "../libfvde/libfvde_sector_data.h"

int LLVMFuzzerTestOneInput(
     const uint8_t *data,
     size_t size )
{
	/* The AES-128 key
	 */
	uint8_t key[ 16 ] =
	{
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
		0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10
	};

	/* The AES-128 tweak key
	 */
	uint8_t tweak_key[ 16 ] =
	{
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};

	libbfio_handle_t *file_io_handle     = NULL;
	libbfio_pool_t *file_io_pool         = NULL;
	libfvde_encryption_context_t *encryption_context = NULL;
	libfvde_sector_data_t *sector_data   = NULL;
	uint64_t block_number                = 0;
	int entry_index                      = 0;

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
	if( libbfio_pool_initialize(
	     &file_io_pool,
	     0,
	     0,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libfvde_encryption_context_initialize(
	     &encryption_context,
	     LIBFVDE_ENCRYPTION_METHOD_AES_128_XTS,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libfvde_encryption_context_set_keys(
	     encryption_context,
	     key,
	     16,
	     tweak_key,
	     16,
	     NULL ) != 1 )
	{
		goto on_error_libfvde_encryption_context;
	}
	if( libfvde_sector_data_initialize(
	     &sector_data,
	     4096,
	     NULL ) != 1 )
	{
		goto on_error_libfvde_encryption_context;
	}
	if( libbfio_pool_append_handle(
	     file_io_pool,
	     &entry_index,
	     file_io_handle,
	     LIBBFIO_OPEN_READ,
	     NULL ) != 1 )
	{
		goto on_error_libfvde_sector_data;
	}
	/* The file IO pool takes over management of the file IO handle
	 */
	file_io_handle = NULL;

	/* Reads the sector data as-is
	 */
	libfvde_sector_data_read(
	 sector_data,
	 NULL,
	 file_io_pool,
	 entry_index,
	 0,
	 block_number,
	 0,
	 NULL );

	block_number += 1;

	/* Reads and decrypts the sector data
	 */
	libfvde_sector_data_read(
	 sector_data,
	 encryption_context,
	 file_io_pool,
	 entry_index,
	 0,
	 block_number,
	 1,
	 NULL );

on_error_libfvde_sector_data:
	libfvde_sector_data_free(
	 &sector_data,
	 NULL );

on_error_libfvde_encryption_context:
	libfvde_encryption_context_free(
	 &encryption_context,
	 NULL );

on_error_libbfio:
	/* Note that on error the sector data still has a reference to file_io_pool
	 * that needs to be closed. Therefore the file IO pool and handle need to
	 * be freed after freeing the sector data.
	 */
	if( file_io_pool != NULL )
	{
		libbfio_pool_free(
		 &file_io_pool,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( 0 );
}

} /* extern "C" */
