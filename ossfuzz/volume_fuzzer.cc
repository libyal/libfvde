/*
 * OSS-Fuzz target for libfvde volume type
 *
 * Copyright (C) 2011-2026, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( LIBFVDE_HAVE_BFIO )

/* Determines if a file contains a FVDE volume signature using a Basic File IO (bfio) handle
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_check_volume_signature_file_io_handle(
      libbfio_handle_t *file_io_handle,
      libfvde_error_t **error );

/* Opens a volume using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_volume_open_file_io_handle(
     libfvde_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libfvde_error_t **error );

/* Opens the physical volume files
 * This function assumes the physical volume files are in same order as defined by the metadata
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_volume_open_physical_volume_files_file_io_pool(
     libfvde_volume_t *volume,
     libbfio_pool_t *file_io_pool,
     libfvde_error_t **error );

#endif /* !defined( LIBFVDE_HAVE_BFIO ) */

int LLVMFuzzerTestOneInput(
     const uint8_t *data,
     size_t size )
{
	uint8_t uuid[ 16 ];
	uint8_t volume_master_key[ 16 ] = {
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10
	};

	libbfio_handle_t *file_io_handle     = NULL;
	libbfio_pool_t *file_io_pool         = NULL;
	libfvde_volume_t *volume             = NULL;
	libfvde_volume_group_t *volume_group = NULL;
	uint32_t encryption_method           = 0;
	size64_t volume_size                 = 0;
	off64_t volume_offset                = 0;
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
	libfvde_check_volume_signature_file_io_handle(
	 file_io_handle,
	 NULL );

	if( libbfio_pool_initialize(
	     &file_io_pool,
	     0,
	     0,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libfvde_volume_initialize(
	     &volume,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libfvde_volume_open_file_io_handle(
	     volume,
	     file_io_handle,
	     LIBFVDE_OPEN_READ,
	     NULL ) != 1 )
	{
		goto on_error_libfvde_volume;
	}
	if( libbfio_pool_append_handle(
	     file_io_pool,
	     &entry_index,
	     file_io_handle,
	     LIBBFIO_OPEN_READ,
	     NULL ) != 1 )
	{
		goto on_error_libfvde_volume;
	}
	/* The file IO pool takes over management of the file IO handle
	 */
	file_io_handle = NULL;

	if( libfvde_volume_open_physical_volume_files_file_io_pool(
	     volume,
	     file_io_pool,
	     NULL ) != 1 )
	{
		goto on_error_libfvde_volume;
	}
	libfvde_volume_is_locked(
	 volume,
	 NULL );

	libfvde_volume_get_size(
	 volume,
	 &volume_size,
	 NULL );

	libfvde_volume_get_offset(
	 volume,
	 &volume_offset,
	 NULL );

	libfvde_volume_seek_offset(
	 volume,
	 0,
	 0,
	 NULL );

	libfvde_volume_read_buffer_at_offset(
	 volume,
	 uuid,
	 16,
	 0,
	 NULL );

	libfvde_volume_set_utf8_password(
	 volume,
	 (uint8_t *) "password",
	 8,
	 NULL );

	libfvde_volume_set_keys(
	 volume,
	 volume_master_key,
	 16,
	 NULL );

	libfvde_volume_unlock(
	 volume,
	 NULL );

	libfvde_volume_get_encryption_method(
	 volume,
	 &encryption_method,
	 NULL );

	libfvde_volume_get_logical_volume_group_identifier(
	 volume,
	 uuid,
	 16,
	 NULL );

	libfvde_volume_get_physical_volume_size(
	 volume,
	 &volume_size,
	 NULL );

	libfvde_volume_get_physical_volume_identifier(
	 volume,
	 uuid,
	 16,
	 NULL );

	libfvde_volume_get_physical_volume_encryption_method(
	 volume,
	 &encryption_method,
	 NULL );

	libfvde_volume_get_logical_volume_size(
	 volume,
	 &volume_size,
	 NULL );

	libfvde_volume_get_logical_volume_identifier(
	 volume,
	 uuid,
	 16,
	 NULL );

	libfvde_volume_get_logical_volume_encryption_method(
	 volume,
	 &encryption_method,
	 NULL );

	libfvde_volume_signal_abort(
	 volume,
	 NULL );

	if( libfvde_volume_get_volume_group(
	     volume,
	     &volume_group,
	     NULL ) == 1 )
	{
		libfvde_volume_group_free(
		 &volume_group,
		 NULL );
	}
	libfvde_volume_close(
	 volume,
	 NULL );

on_error_libfvde_volume:
	libfvde_volume_free(
	 &volume,
	 NULL );

on_error_libbfio:
	/* Note that on error the volume still has a reference to file_io_pool
	 * that will be closed. Therefore the file IO pool and handle need to
	 * be freed after closing or freeing the volume.
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

