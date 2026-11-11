/*
 * OSS-Fuzz target for libfvde encryption context type
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
#include <string.h>

/* Note that some of the OSS-Fuzz engines use C++
 */
extern "C" {

#include "ossfuzz_libbfio.h"
#include "ossfuzz_libfvde.h"

#include "../libfvde/libfvde_definitions.h"
#include "../libfvde/libfvde_encryption_context.h"

int LLVMFuzzerTestOneInput(
     const uint8_t *data,
     size_t size )
{
	uint8_t input_data[ 256 ];
	uint8_t output_data[ 256 ];
	uint8_t unwrapped_data[ 64 ];

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

	libfvde_encryption_context_t *encryption_context = NULL;
	size_t data_size                                 = 0;

	if( size > 256 )
	{
		data_size = 256;
	}
	else
	{
		data_size = size;
	}
	if( data_size > 0 )
	{
		memcpy( input_data, data, data_size );
	}
	/* Note the AES-XTS encryption requires the data size
	 * to be a multiple of the block size (16 bytes).
	 */
	data_size = ( data_size / 16 ) * 16;

	if( libfvde_encryption_context_initialize(
	     &encryption_context,
	     LIBFVDE_ENCRYPTION_METHOD_AES_128_XTS,
	     NULL ) != 1 )
	{
		return( 0 );
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
	if( data_size > 0 )
	{
		libfvde_encryption_context_crypt(
		 encryption_context,
		 LIBFVDE_ENCRYPTION_CRYPT_MODE_DECRYPT,
		 input_data,
		 data_size,
		 output_data,
		 data_size,
		 0,
		 NULL );

		libfvde_encryption_context_crypt(
		 encryption_context,
		 LIBFVDE_ENCRYPTION_CRYPT_MODE_DECRYPT,
		 input_data,
		 data_size,
		 output_data,
		 data_size,
		 1,
		 NULL );
	}
	libfvde_encryption_context_crypt(
	 encryption_context,
	 LIBFVDE_ENCRYPTION_CRYPT_MODE_ENCRYPT,
	 input_data,
	 16,
	 output_data,
	 16,
	 0,
	 NULL );
on_error_libfvde_encryption_context:
	libfvde_encryption_context_free(
	 &encryption_context,
	 NULL );

	/* Unwraps a wrapped key using AES
	 */
	data_size = ( size > 48 ) ? 48 : size;
	data_size = ( data_size / 8 ) * 8;

	if( data_size > 8 )
	{
		libfvde_encryption_aes_key_unwrap(
		 key,
		 128,
		 data,
		 data_size,
		 unwrapped_data,
		 sizeof( unwrapped_data ),
		 NULL );
	}
	return( 0 );
}

} /* extern "C" */
