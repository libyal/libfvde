/*
 * OSS-Fuzz target for libfvde encryption context plist type
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

#if !defined( LIBFVDE_HAVE_BFIO )

/* Reads an encryption context plist file (EncryptedRoot.plist) using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_encryption_context_plist_read_file_io_handle(
      libfvde_encryption_context_plist_t *plist,
      libbfio_handle_t *file_io_handle,
      libfvde_error_t **error );

#endif /* !defined( LIBFVDE_HAVE_BFIO ) */

int LLVMFuzzerTestOneInput(
     const uint8_t *data,
     size_t size )
{
	uint8_t copy_buffer[ 512 ];

	libbfio_handle_t *file_io_handle                 = NULL;
	libfvde_encryption_context_plist_t *plist        = NULL;
	size64_t data_size                               = 0;

	/* The decryption key
	 */
	const uint8_t key[ 16 ] =
	{
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
		0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10
	};

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
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
		return( 0 );
	}
	if( libfvde_encryption_context_plist_initialize(
	     &plist,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libfvde_encryption_context_plist_read_file_io_handle(
	     plist,
	     file_io_handle,
	     NULL ) != 1 )
	{
		goto on_error_libfvde_plist;
	}
	if( libfvde_encryption_context_plist_get_data_size(
	     plist,
	     &data_size,
	     NULL ) == 1 )
	{
		if( ( data_size > 0 )
		 && ( data_size <= (size64_t) sizeof( copy_buffer ) ) )
		{
			libfvde_encryption_context_plist_copy_data(
			 plist,
			 copy_buffer,
			 sizeof( copy_buffer ),
			 NULL );
		}
	}
	libfvde_encryption_context_plist_decrypt(
	 plist,
	 key,
	 128,
	 NULL );
	if( libfvde_encryption_context_plist_get_data_size(
	     plist,
	     &data_size,
	     NULL ) == 1 )
	{
		if( ( data_size > 0 )
		 && ( data_size <= (size64_t) sizeof( copy_buffer ) ) )
		{
			libfvde_encryption_context_plist_copy_data(
			 plist,
			 copy_buffer,
			 sizeof( copy_buffer ),
			 NULL );
		}
	}
on_error_libfvde_plist:
	libfvde_encryption_context_plist_free(
	 &plist,
	 NULL );

on_error_libbfio:
	libbfio_handle_free(
	 &file_io_handle,
	 NULL );
	return( 0 );
}

} /* extern "C" */
