/*
 * Library sector_data_vector type test program
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

#include <common.h>
#include <byte_stream.h>
#include <file_stream.h>
#include <memory.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fvde_test_functions.h"
#include "fvde_test_libbfio.h"
#include "fvde_test_libcaes.h"
#include "fvde_test_libcdata.h"
#include "fvde_test_libcerror.h"
#include "fvde_test_libfvde.h"
#include "fvde_test_macros.h"
#include "fvde_test_memory.h"
#include "fvde_test_unused.h"

#include "../libfvde/libfvde_definitions.h"
#include "../libfvde/libfvde_encryption_context.h"
#include "../libfvde/libfvde_logical_volume_descriptor.h"
#include "../libfvde/libfvde_sector_data.h"
#include "../libfvde/libfvde_sector_data_vector.h"
#include "../libfvde/libfvde_segment_descriptor.h"

/* The test logical volume uses a block size of 4096, 512 bytes per sector,
 * a base physical block number of 2 and consists of 0x20000004 blocks
 *
 * logical blocks 0x00000000 - 0x00000001 are stored in physical blocks 2 - 3
 * logical blocks 0x10000000 - 0x10000001 are stored in physical blocks 4 - 5
 * (offset 1 TiB, sector 2^31)
 * logical blocks 0x20000000 - 0x20000001 are stored in physical blocks 6 - 7
 * (offset 2 TiB, sector 2^32)
 *
 * the other logical blocks are sparse
 */
uint64_t fvde_test_sector_data_vector_base_physical_block_number = 2;

uint64_t fvde_test_sector_data_vector_logical_block_numbers[ 3 ] = {
	0x00000000UL, 0x10000000UL, 0x20000000UL };

uint64_t fvde_test_sector_data_vector_number_of_blocks[ 3 ] = {
	2, 2, 2 };

uint64_t fvde_test_sector_data_vector_physical_block_numbers[ 3 ] = {
	0, 2, 4 };

/* Test keys based on the NIST SP 800-38A AES example keys
 */
uint8_t fvde_test_sector_data_vector_key[ 16 ] = {
	0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };

uint8_t fvde_test_sector_data_vector_tweak_key[ 16 ] = {
	0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81 };

/* The physical volume test data, 8 blocks of 4096 bytes
 */
uint8_t fvde_test_sector_data_vector_data1[ 32768 ];

#if defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT )

/* Retrieves the plaintext data of a specific sector of the test logical volume
 * Returns 1 if successful or -1 on error
 */
int fvde_test_sector_data_vector_get_plaintext(
     uint64_t sector_number,
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "fvde_test_sector_data_vector_get_plaintext";
	size_t data_offset    = 0;

	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* Every byte of the 64-bit sector number contributes to the plaintext
	 */
	for( data_offset = 0;
	     data_offset < data_size;
	     data_offset++ )
	{
		data[ data_offset ] = (uint8_t) ( data_offset ^ ( sector_number >> ( ( data_offset % 8 ) * 8 ) ) );
	}
	return( 1 );
}

/* Creates the physical volume test data
 * The sectors that are mapped by the segment descriptors are AES-XTS encrypted
 * with the sector number relative to the start of the logical volume as tweak value
 * Returns 1 if successful or -1 on error
 */
int fvde_test_sector_data_vector_create_test_data(
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	uint8_t plaintext_data[ 512 ];
	uint8_t tweak_value[ 16 ];

	libcaes_tweaked_context_t *tweaked_context = NULL;
	static char *function                      = "fvde_test_sector_data_vector_create_test_data";
	size_t data_offset                         = 0;
	uint64_t block_number                      = 0;
	uint64_t physical_block_number             = 0;
	uint64_t sector_number                     = 0;
	int sector_index                           = 0;
	int segment_index                          = 0;

	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* Data that is not mapped by a segment descriptor is filled with 0xaa
	 */
	if( memory_set(
	     data,
	     0xaa,
	     data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data.",
		 function );

		goto on_error;
	}
	if( libcaes_tweaked_context_initialize(
	     &tweaked_context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize tweaked context.",
		 function );

		goto on_error;
	}
	if( libcaes_tweaked_context_set_keys(
	     tweaked_context,
	     LIBCAES_CRYPT_MODE_ENCRYPT,
	     fvde_test_sector_data_vector_key,
	     128,
	     fvde_test_sector_data_vector_tweak_key,
	     128,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set keys in tweaked context.",
		 function );

		goto on_error;
	}
	for( segment_index = 0;
	     segment_index < 3;
	     segment_index++ )
	{
		for( block_number = 0;
		     block_number < fvde_test_sector_data_vector_number_of_blocks[ segment_index ];
		     block_number++ )
		{
			physical_block_number = fvde_test_sector_data_vector_base_physical_block_number
			                      + fvde_test_sector_data_vector_physical_block_numbers[ segment_index ]
			                      + block_number;

			for( sector_index = 0;
			     sector_index < 8;
			     sector_index++ )
			{
				sector_number = ( ( fvde_test_sector_data_vector_logical_block_numbers[ segment_index ] + block_number ) * 8 )
				              + sector_index;

				data_offset = (size_t) ( physical_block_number * 4096 ) + ( sector_index * 512 );

				if( ( data_offset > data_size )
				 || ( ( data_size - data_offset ) < 512 ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
					 "%s: invalid data size value too small.",
					 function );

					goto on_error;
				}
				if( fvde_test_sector_data_vector_get_plaintext(
				     sector_number,
				     plaintext_data,
				     512,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve plaintext of sector: %" PRIu64 ".",
					 function,
					 sector_number );

					goto on_error;
				}
				if( memory_set(
				     tweak_value,
				     0,
				     16 ) == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_SET_FAILED,
					 "%s: unable to clear tweak value.",
					 function );

					goto on_error;
				}
				byte_stream_copy_from_uint64_little_endian(
				 tweak_value,
				 sector_number );

				if( libcaes_crypt_xts(
				     tweaked_context,
				     LIBCAES_CRYPT_MODE_ENCRYPT,
				     tweak_value,
				     16,
				     plaintext_data,
				     512,
				     &( data[ data_offset ] ),
				     512,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
					 LIBCERROR_ENCRYPTION_ERROR_ENCRYPT_FAILED,
					 "%s: unable to encrypt sector: %" PRIu64 ".",
					 function,
					 sector_number );

					goto on_error;
				}
			}
		}
	}
	if( libcaes_tweaked_context_free(
	     &tweaked_context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free tweaked context.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( tweaked_context != NULL )
	{
		libcaes_tweaked_context_free(
		 &tweaked_context,
		 NULL );
	}
	return( -1 );
}

/* Creates the logical volume descriptor of the test logical volume
 * Make sure the value logical_volume_descriptor is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int fvde_test_sector_data_vector_create_logical_volume_descriptor(
     libfvde_logical_volume_descriptor_t **logical_volume_descriptor,
     libcerror_error_t **error )
{
	libfvde_segment_descriptor_t *segment_descriptor = NULL;
	static char *function                            = "fvde_test_sector_data_vector_create_logical_volume_descriptor";
	int entry_index                                  = 0;
	int segment_index                                = 0;

	if( logical_volume_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume descriptor.",
		 function );

		return( -1 );
	}
	if( libfvde_logical_volume_descriptor_initialize(
	     logical_volume_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create logical volume descriptor.",
		 function );

		goto on_error;
	}
	( *logical_volume_descriptor )->base_physical_block_number = fvde_test_sector_data_vector_base_physical_block_number;
	( *logical_volume_descriptor )->size                       = (size64_t) 0x20000004000UL;

	/* The segment descriptors are stored sorted by logical block number
	 */
	for( segment_index = 0;
	     segment_index < 3;
	     segment_index++ )
	{
		if( libfvde_segment_descriptor_initialize(
		     &segment_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create segment descriptor: %d.",
			 function,
			 segment_index );

			goto on_error;
		}
		segment_descriptor->logical_block_number  = fvde_test_sector_data_vector_logical_block_numbers[ segment_index ];
		segment_descriptor->number_of_blocks      = fvde_test_sector_data_vector_number_of_blocks[ segment_index ];
		segment_descriptor->physical_block_number = fvde_test_sector_data_vector_physical_block_numbers[ segment_index ];
		segment_descriptor->physical_volume_index = 0;

		if( libcdata_array_append_entry(
		     ( *logical_volume_descriptor )->segment_descriptors,
		     &entry_index,
		     (intptr_t *) segment_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append segment descriptor: %d to array.",
			 function,
			 segment_index );

			goto on_error;
		}
		segment_descriptor = NULL;
	}
	return( 1 );

on_error:
	if( segment_descriptor != NULL )
	{
		libfvde_segment_descriptor_free(
		 &segment_descriptor,
		 NULL );
	}
	if( *logical_volume_descriptor != NULL )
	{
		libfvde_logical_volume_descriptor_free(
		 logical_volume_descriptor,
		 NULL );
	}
	return( -1 );
}

/* Tests the libfvde_sector_data_vector_initialize function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_sector_data_vector_initialize(
     void )
{
	libcerror_error_t *error                                       = NULL;
	libfvde_logical_volume_descriptor_t *logical_volume_descriptor = NULL;
	libfvde_sector_data_vector_t *sector_data_vector               = NULL;
	int result                                                     = 0;

#if defined( HAVE_FVDE_TEST_MEMORY )
	int number_of_malloc_fail_tests                                = 5;
	int number_of_memset_fail_tests                                = 5;
	int test_number                                                = 0;
#endif

	/* Initialize test
	 */
	result = libfvde_logical_volume_descriptor_initialize(
	          &logical_volume_descriptor,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "logical_volume_descriptor",
	 logical_volume_descriptor );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfvde_sector_data_vector_initialize(
	          &sector_data_vector,
	          512,
	          4096,
	          logical_volume_descriptor,
	          (size64_t) 0x20000004000UL,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "sector_data_vector",
	 sector_data_vector );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvde_sector_data_vector_free(
	          &sector_data_vector,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "sector_data_vector",
	 sector_data_vector );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test maximum supported data size
	 */
	result = libfvde_sector_data_vector_initialize(
	          &sector_data_vector,
	          512,
	          4096,
	          logical_volume_descriptor,
	          (size64_t) INT64_MAX,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "sector_data_vector",
	 sector_data_vector );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvde_sector_data_vector_free(
	          &sector_data_vector,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "sector_data_vector",
	 sector_data_vector );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfvde_sector_data_vector_initialize(
	          NULL,
	          512,
	          4096,
	          logical_volume_descriptor,
	          (size64_t) 0x20000004000UL,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	sector_data_vector = (libfvde_sector_data_vector_t *) 0x12345678UL;

	result = libfvde_sector_data_vector_initialize(
	          &sector_data_vector,
	          512,
	          4096,
	          logical_volume_descriptor,
	          (size64_t) 0x20000004000UL,
	          &error );

	sector_data_vector = NULL;

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfvde_sector_data_vector_initialize(
	          &sector_data_vector,
	          0,
	          4096,
	          logical_volume_descriptor,
	          (size64_t) 0x20000004000UL,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "sector_data_vector",
	 sector_data_vector );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfvde_sector_data_vector_initialize(
	          &sector_data_vector,
	          (uint32_t) MEMORY_MAXIMUM_ALLOCATION_SIZE + 1,
	          4096,
	          logical_volume_descriptor,
	          (size64_t) 0x20000004000UL,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "sector_data_vector",
	 sector_data_vector );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfvde_sector_data_vector_initialize(
	          &sector_data_vector,
	          512,
	          0,
	          logical_volume_descriptor,
	          (size64_t) 0x20000004000UL,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "sector_data_vector",
	 sector_data_vector );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where the block size is not a multiple of the bytes per sector
	 */
	result = libfvde_sector_data_vector_initialize(
	          &sector_data_vector,
	          512,
	          4000,
	          logical_volume_descriptor,
	          (size64_t) 0x20000004000UL,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "sector_data_vector",
	 sector_data_vector );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where the block size is smaller than the bytes per sector
	 */
	result = libfvde_sector_data_vector_initialize(
	          &sector_data_vector,
	          512,
	          256,
	          logical_volume_descriptor,
	          (size64_t) 0x20000004000UL,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "sector_data_vector",
	 sector_data_vector );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfvde_sector_data_vector_initialize(
	          &sector_data_vector,
	          512,
	          4096,
	          NULL,
	          (size64_t) 0x20000004000UL,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "sector_data_vector",
	 sector_data_vector );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfvde_sector_data_vector_initialize(
	          &sector_data_vector,
	          512,
	          4096,
	          logical_volume_descriptor,
	          (size64_t) INT64_MAX + 1,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "sector_data_vector",
	 sector_data_vector );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_FVDE_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfvde_sector_data_vector_initialize with malloc failing
		 */
		fvde_test_malloc_attempts_before_fail = test_number;

		result = libfvde_sector_data_vector_initialize(
		          &sector_data_vector,
		          512,
		          4096,
		          logical_volume_descriptor,
		          (size64_t) 0x20000004000UL,
		          &error );

		if( fvde_test_malloc_attempts_before_fail != -1 )
		{
			fvde_test_malloc_attempts_before_fail = -1;

			if( sector_data_vector != NULL )
			{
				libfvde_sector_data_vector_free(
				 &sector_data_vector,
				 NULL );
			}
		}
		else
		{
			FVDE_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			FVDE_TEST_ASSERT_IS_NULL(
			 "sector_data_vector",
			 sector_data_vector );

			FVDE_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libfvde_sector_data_vector_initialize with memset failing
		 */
		fvde_test_memset_attempts_before_fail = test_number;

		result = libfvde_sector_data_vector_initialize(
		          &sector_data_vector,
		          512,
		          4096,
		          logical_volume_descriptor,
		          (size64_t) 0x20000004000UL,
		          &error );

		if( fvde_test_memset_attempts_before_fail != -1 )
		{
			fvde_test_memset_attempts_before_fail = -1;

			if( sector_data_vector != NULL )
			{
				libfvde_sector_data_vector_free(
				 &sector_data_vector,
				 NULL );
			}
		}
		else
		{
			FVDE_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			FVDE_TEST_ASSERT_IS_NULL(
			 "sector_data_vector",
			 sector_data_vector );

			FVDE_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_FVDE_TEST_MEMORY ) */

	/* Clean up
	 */
	result = libfvde_logical_volume_descriptor_free(
	          &logical_volume_descriptor,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "logical_volume_descriptor",
	 logical_volume_descriptor );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( sector_data_vector != NULL )
	{
		libfvde_sector_data_vector_free(
		 &sector_data_vector,
		 NULL );
	}
	if( logical_volume_descriptor != NULL )
	{
		libfvde_logical_volume_descriptor_free(
		 &logical_volume_descriptor,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfvde_sector_data_vector_free function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_sector_data_vector_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfvde_sector_data_vector_free(
	          NULL,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfvde_sector_data_vector_get_sector_data_at_offset function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_sector_data_vector_get_sector_data_at_offset(
     libfvde_logical_volume_descriptor_t *logical_volume_descriptor,
     libbfio_pool_t *file_io_pool,
     libfvde_encryption_context_t *encryption_context )
{
	uint8_t expected_data[ 512 ];
	uint8_t zero_data[ 512 ];

	off64_t mapped_offsets[ 6 ]                      = { 0, 7680, 0x10000000000UL, 0x10000001200UL, 0x20000000000UL, 0x20000001e00UL };
	off64_t sparse_offsets[ 6 ]                      = { 8192, 16384, 0x8000000000UL, 0xfffffffe00UL, 0x20000002000UL, 0x20000003e00UL };
	libcerror_error_t *error                         = NULL;
	libfvde_sector_data_t *cached_sector_data        = NULL;
	libfvde_sector_data_t *sector_data               = NULL;
	libfvde_sector_data_vector_t *sector_data_vector = NULL;
	void *memset_result                              = NULL;
	uint32_t block_size                              = 0;
	uint32_t bytes_per_sector                        = 0;
	int offset_index                                 = 0;
	int result                                       = 0;

	/* Initialize test
	 */
	memset_result = memory_set(
	                 zero_data,
	                 0,
	                 sizeof( uint8_t ) * 512 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "memset_result",
	 memset_result );

	result = libfvde_sector_data_vector_initialize(
	          &sector_data_vector,
	          512,
	          4096,
	          logical_volume_descriptor,
	          (size64_t) 0x20000004000UL,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "sector_data_vector",
	 sector_data_vector );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */

	/* Test sectors in each segment, this includes sector 2^31 at offset 1 TiB
	 * and sector 2^32 at offset 2 TiB, which can only be decrypted if the tweak
	 * value is the 64-bit sector number relative to the start of the logical volume
	 */
	for( offset_index = 0;
	     offset_index < 6;
	     offset_index++ )
	{
		result = fvde_test_sector_data_vector_get_plaintext(
		          (uint64_t) mapped_offsets[ offset_index ] / 512,
		          expected_data,
		          512,
		          &error );

		FVDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		FVDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		sector_data = NULL;

		result = libfvde_sector_data_vector_get_sector_data_at_offset(
		          sector_data_vector,
		          file_io_pool,
		          encryption_context,
		          1,
		          mapped_offsets[ offset_index ],
		          &sector_data,
		          &error );

		FVDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		FVDE_TEST_ASSERT_IS_NOT_NULL(
		 "sector_data",
		 sector_data );

		FVDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		FVDE_TEST_ASSERT_EQUAL_SIZE(
		 "sector_data->data_size",
		 sector_data->data_size,
		 (size_t) 512 );

		result = memory_compare(
		          sector_data->data,
		          expected_data,
		          512 );

		FVDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 0 );
	}
	/* Test sectors that are not mapped by a segment descriptor (sparse)
	 * Offset 16384 is 8192 bytes into the sparse range that starts at offset 8192
	 * The libfdata vector cached such a sector under file index 0 and offset 8192,
	 * the same cache identifier as the sector at offset 0 read before, which is
	 * stored at physical offset 8192 of physical volume 0
	 */
	for( offset_index = 0;
	     offset_index < 6;
	     offset_index++ )
	{
		sector_data = NULL;

		result = libfvde_sector_data_vector_get_sector_data_at_offset(
		          sector_data_vector,
		          file_io_pool,
		          encryption_context,
		          1,
		          sparse_offsets[ offset_index ],
		          &sector_data,
		          &error );

		FVDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		FVDE_TEST_ASSERT_IS_NOT_NULL(
		 "sector_data",
		 sector_data );

		FVDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		FVDE_TEST_ASSERT_EQUAL_SIZE(
		 "sector_data->data_size",
		 sector_data->data_size,
		 (size_t) 512 );

		result = memory_compare(
		          sector_data->data,
		          zero_data,
		          512 );

		FVDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 0 );
	}
	/* Test a sparse sector at a logical offset that equals the physical offset
	 * of the sector read just before, the sector at offset 512 is stored at
	 * physical offset 8704
	 * This would share a cache identifier if mapped sectors were cached by
	 * physical offset and sparse sectors by logical offset
	 */
	result = fvde_test_sector_data_vector_get_plaintext(
	          1,
	          expected_data,
	          512,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	sector_data = NULL;

	result = libfvde_sector_data_vector_get_sector_data_at_offset(
	          sector_data_vector,
	          file_io_pool,
	          encryption_context,
	          1,
	          512,
	          &sector_data,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "sector_data",
	 sector_data );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          sector_data->data,
	          expected_data,
	          512 );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	sector_data = NULL;

	result = libfvde_sector_data_vector_get_sector_data_at_offset(
	          sector_data_vector,
	          file_io_pool,
	          encryption_context,
	          1,
	          8704,
	          &sector_data,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "sector_data",
	 sector_data );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          sector_data->data,
	          zero_data,
	          512 );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test that a cached sector is returned without reading it again,
	 * the sector at offset 1024 is stored at physical offset 9216
	 */
	result = fvde_test_sector_data_vector_get_plaintext(
	          2,
	          expected_data,
	          512,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvde_sector_data_vector_get_sector_data_at_offset(
	          sector_data_vector,
	          file_io_pool,
	          encryption_context,
	          1,
	          1024,
	          &cached_sector_data,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "cached_sector_data",
	 cached_sector_data );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          cached_sector_data->data,
	          expected_data,
	          512 );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	fvde_test_sector_data_vector_data1[ 9216 ] ^= 0xff;

	sector_data = NULL;

	result = libfvde_sector_data_vector_get_sector_data_at_offset(
	          sector_data_vector,
	          file_io_pool,
	          encryption_context,
	          1,
	          1024,
	          &sector_data,
	          &error );

	fvde_test_sector_data_vector_data1[ 9216 ] ^= 0xff;

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_EQUAL_UINT64(
	 "sector_data",
	 (uint64_t) (intptr_t) sector_data,
	 (uint64_t) (intptr_t) cached_sector_data );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          sector_data->data,
	          expected_data,
	          512 );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	sector_data = NULL;

	result = libfvde_sector_data_vector_get_sector_data_at_offset(
	          NULL,
	          file_io_pool,
	          encryption_context,
	          1,
	          0,
	          &sector_data,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "sector_data",
	 sector_data );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where bytes per sector is invalid
	 */
	bytes_per_sector = sector_data_vector->bytes_per_sector;

	sector_data_vector->bytes_per_sector = 0;

	result = libfvde_sector_data_vector_get_sector_data_at_offset(
	          sector_data_vector,
	          file_io_pool,
	          encryption_context,
	          1,
	          0,
	          &sector_data,
	          &error );

	sector_data_vector->bytes_per_sector = bytes_per_sector;

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "sector_data",
	 sector_data );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where block size is invalid
	 */
	block_size = sector_data_vector->block_size;

	sector_data_vector->block_size = 0;

	result = libfvde_sector_data_vector_get_sector_data_at_offset(
	          sector_data_vector,
	          file_io_pool,
	          encryption_context,
	          1,
	          0,
	          &sector_data,
	          &error );

	sector_data_vector->block_size = block_size;

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "sector_data",
	 sector_data );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where offset is negative
	 */
	result = libfvde_sector_data_vector_get_sector_data_at_offset(
	          sector_data_vector,
	          file_io_pool,
	          encryption_context,
	          1,
	          -512,
	          &sector_data,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "sector_data",
	 sector_data );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where offset is not sector aligned
	 */
	result = libfvde_sector_data_vector_get_sector_data_at_offset(
	          sector_data_vector,
	          file_io_pool,
	          encryption_context,
	          1,
	          1,
	          &sector_data,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "sector_data",
	 sector_data );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfvde_sector_data_vector_get_sector_data_at_offset(
	          sector_data_vector,
	          file_io_pool,
	          encryption_context,
	          1,
	          (off64_t) 0x20000000100UL,
	          &sector_data,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "sector_data",
	 sector_data );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where offset is equal to the data size
	 */
	result = libfvde_sector_data_vector_get_sector_data_at_offset(
	          sector_data_vector,
	          file_io_pool,
	          encryption_context,
	          1,
	          (off64_t) 0x20000004000UL,
	          &sector_data,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "sector_data",
	 sector_data );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where offset exceeds the data size
	 */
	result = libfvde_sector_data_vector_get_sector_data_at_offset(
	          sector_data_vector,
	          file_io_pool,
	          encryption_context,
	          1,
	          (off64_t) 0x30000000000UL,
	          &sector_data,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "sector_data",
	 sector_data );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfvde_sector_data_vector_get_sector_data_at_offset(
	          sector_data_vector,
	          file_io_pool,
	          encryption_context,
	          1,
	          0,
	          NULL,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where the encryption context is missing
	 */
	result = libfvde_sector_data_vector_get_sector_data_at_offset(
	          sector_data_vector,
	          file_io_pool,
	          NULL,
	          1,
	          1536,
	          &sector_data,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "sector_data",
	 sector_data );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where the file IO pool is missing
	 */
	result = libfvde_sector_data_vector_get_sector_data_at_offset(
	          sector_data_vector,
	          NULL,
	          encryption_context,
	          1,
	          1536,
	          &sector_data,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "sector_data",
	 sector_data );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test that sector data that failed to read was not cached
	 */
	result = fvde_test_sector_data_vector_get_plaintext(
	          3,
	          expected_data,
	          512,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvde_sector_data_vector_get_sector_data_at_offset(
	          sector_data_vector,
	          file_io_pool,
	          encryption_context,
	          1,
	          1536,
	          &sector_data,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "sector_data",
	 sector_data );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          sector_data->data,
	          expected_data,
	          512 );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Clean up
	 */
	result = libfvde_sector_data_vector_free(
	          &sector_data_vector,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "sector_data_vector",
	 sector_data_vector );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( sector_data_vector != NULL )
	{
		libfvde_sector_data_vector_free(
		 &sector_data_vector,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfvde_sector_data_vector_get_sector_data_at_offset function with unencrypted data
 * Returns 1 if successful or 0 if not
 */
int fvde_test_sector_data_vector_get_sector_data_at_offset_unencrypted(
     libfvde_logical_volume_descriptor_t *logical_volume_descriptor,
     libbfio_pool_t *file_io_pool )
{
	uint8_t zero_data[ 512 ];

	off64_t mapped_offsets[ 4 ]                      = { 0, 0x10000001200UL, 0x20000000200UL, 0x20000001e00UL };
	size_t physical_offsets[ 4 ]                     = { 8192, 20992, 25088, 32256 };
	libcerror_error_t *error                         = NULL;
	libfvde_sector_data_t *sector_data               = NULL;
	libfvde_sector_data_vector_t *sector_data_vector = NULL;
	void *memset_result                              = NULL;
	int offset_index                                 = 0;
	int result                                       = 0;

	/* Initialize test
	 */
	memset_result = memory_set(
	                 zero_data,
	                 0,
	                 sizeof( uint8_t ) * 512 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "memset_result",
	 memset_result );

	result = libfvde_sector_data_vector_initialize(
	          &sector_data_vector,
	          512,
	          4096,
	          logical_volume_descriptor,
	          (size64_t) 0x20000004000UL,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "sector_data_vector",
	 sector_data_vector );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */

	/* Test that the data of sectors in each segment is returned as stored
	 */
	for( offset_index = 0;
	     offset_index < 4;
	     offset_index++ )
	{
		sector_data = NULL;

		result = libfvde_sector_data_vector_get_sector_data_at_offset(
		          sector_data_vector,
		          file_io_pool,
		          NULL,
		          0,
		          mapped_offsets[ offset_index ],
		          &sector_data,
		          &error );

		FVDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		FVDE_TEST_ASSERT_IS_NOT_NULL(
		 "sector_data",
		 sector_data );

		FVDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		FVDE_TEST_ASSERT_EQUAL_SIZE(
		 "sector_data->data_size",
		 sector_data->data_size,
		 (size_t) 512 );

		result = memory_compare(
		          sector_data->data,
		          &( fvde_test_sector_data_vector_data1[ physical_offsets[ offset_index ] ] ),
		          512 );

		FVDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 0 );
	}
	/* Test a sector that is not mapped by a segment descriptor (sparse)
	 */
	sector_data = NULL;

	result = libfvde_sector_data_vector_get_sector_data_at_offset(
	          sector_data_vector,
	          file_io_pool,
	          NULL,
	          0,
	          8192,
	          &sector_data,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "sector_data",
	 sector_data );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          sector_data->data,
	          zero_data,
	          512 );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Clean up
	 */
	result = libfvde_sector_data_vector_free(
	          &sector_data_vector,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "sector_data_vector",
	 sector_data_vector );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( sector_data_vector != NULL )
	{
		libfvde_sector_data_vector_free(
		 &sector_data_vector,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfvde_sector_data_vector_get_sector_data_at_offset function with out of bounds physical block numbers
 * Returns 1 if successful or 0 if not
 */
int fvde_test_sector_data_vector_get_sector_data_at_offset_out_of_bounds(
     libfvde_logical_volume_descriptor_t *logical_volume_descriptor,
     libbfio_pool_t *file_io_pool,
     libfvde_encryption_context_t *encryption_context )
{
	uint8_t expected_data[ 512 ];

	libcerror_error_t *error                         = NULL;
	libfvde_sector_data_t *sector_data               = NULL;
	libfvde_sector_data_vector_t *sector_data_vector = NULL;
	uint64_t base_physical_block_number              = 0;
	uint64_t maximum_block_number                    = 0;
	int result                                       = 0;

	/* Initialize test
	 */
	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "logical_volume_descriptor",
	 logical_volume_descriptor );

	base_physical_block_number = logical_volume_descriptor->base_physical_block_number;
	maximum_block_number       = ( (uint64_t) INT64_MAX - 4096 ) / 4096;

	result = libfvde_sector_data_vector_initialize(
	          &sector_data_vector,
	          512,
	          4096,
	          logical_volume_descriptor,
	          (size64_t) 0x20000004000UL,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "sector_data_vector",
	 sector_data_vector );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 * No file IO pool is passed, hence a read that is not prevented by
	 * a bounds check fails with a different error
	 */

	/* Test error case where the base physical block number exceeds the maximum
	 */
	logical_volume_descriptor->base_physical_block_number = maximum_block_number + 1;

	sector_data = NULL;

	result = libfvde_sector_data_vector_get_sector_data_at_offset(
	          sector_data_vector,
	          NULL,
	          encryption_context,
	          1,
	          0,
	          &sector_data,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "sector_data",
	 sector_data );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	result = libcerror_error_matches(
	          error,
	          LIBCERROR_ERROR_DOMAIN_RUNTIME,
	          LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	libcerror_error_free(
	 &error );

	logical_volume_descriptor->base_physical_block_number = 0xffffffffffffffffUL;

	result = libfvde_sector_data_vector_get_sector_data_at_offset(
	          sector_data_vector,
	          NULL,
	          encryption_context,
	          1,
	          0,
	          &sector_data,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "sector_data",
	 sector_data );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	result = libcerror_error_matches(
	          error,
	          LIBCERROR_ERROR_DOMAIN_RUNTIME,
	          LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	libcerror_error_free(
	 &error );

	/* Test error case where the base physical block number and the segment
	 * physical block number exceed the maximum
	 */
	logical_volume_descriptor->base_physical_block_number = maximum_block_number;

	result = libfvde_sector_data_vector_get_sector_data_at_offset(
	          sector_data_vector,
	          NULL,
	          encryption_context,
	          1,
	          (off64_t) 0x10000000000UL,
	          &sector_data,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "sector_data",
	 sector_data );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	result = libcerror_error_matches(
	          error,
	          LIBCERROR_ERROR_DOMAIN_RUNTIME,
	          LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	libcerror_error_free(
	 &error );

	/* Test error case where the physical block number of the logical block
	 * within the segment exceeds the maximum
	 */
	result = libfvde_sector_data_vector_get_sector_data_at_offset(
	          sector_data_vector,
	          NULL,
	          encryption_context,
	          1,
	          4096,
	          &sector_data,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "sector_data",
	 sector_data );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	result = libcerror_error_matches(
	          error,
	          LIBCERROR_ERROR_DOMAIN_RUNTIME,
	          LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	libcerror_error_free(
	 &error );

	logical_volume_descriptor->base_physical_block_number = base_physical_block_number;

	/* Test that sector data that failed to read was not cached
	 */
	result = fvde_test_sector_data_vector_get_plaintext(
	          0,
	          expected_data,
	          512,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvde_sector_data_vector_get_sector_data_at_offset(
	          sector_data_vector,
	          file_io_pool,
	          encryption_context,
	          1,
	          0,
	          &sector_data,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "sector_data",
	 sector_data );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          sector_data->data,
	          expected_data,
	          512 );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Clean up
	 */
	result = libfvde_sector_data_vector_free(
	          &sector_data_vector,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "sector_data_vector",
	 sector_data_vector );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( logical_volume_descriptor != NULL )
	{
		logical_volume_descriptor->base_physical_block_number = base_physical_block_number;
	}
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( sector_data_vector != NULL )
	{
		libfvde_sector_data_vector_free(
		 &sector_data_vector,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT ) */

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc FVDE_TEST_ATTRIBUTE_UNUSED,
     wchar_t * const argv[] FVDE_TEST_ATTRIBUTE_UNUSED )
#else
int main(
     int argc FVDE_TEST_ATTRIBUTE_UNUSED,
     char * const argv[] FVDE_TEST_ATTRIBUTE_UNUSED )
#endif
{
#if defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT )

	libbfio_handle_t *file_io_handle                               = NULL;
	libbfio_pool_t *file_io_pool                                   = NULL;
	libcerror_error_t *error                                       = NULL;
	libfvde_encryption_context_t *encryption_context               = NULL;
	libfvde_logical_volume_descriptor_t *logical_volume_descriptor = NULL;
	int entry_index                                                = 0;
	int result                                                     = 0;

#endif /* defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT ) */

	FVDE_TEST_UNREFERENCED_PARAMETER( argc )
	FVDE_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT )

	FVDE_TEST_RUN(
	 "libfvde_sector_data_vector_initialize",
	 fvde_test_sector_data_vector_initialize );

	FVDE_TEST_RUN(
	 "libfvde_sector_data_vector_free",
	 fvde_test_sector_data_vector_free );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize test
	 */
	result = fvde_test_sector_data_vector_create_test_data(
	          fvde_test_sector_data_vector_data1,
	          32768,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = fvde_test_sector_data_vector_create_logical_volume_descriptor(
	          &logical_volume_descriptor,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "logical_volume_descriptor",
	 logical_volume_descriptor );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvde_encryption_context_initialize(
	          &encryption_context,
	          LIBFVDE_ENCRYPTION_METHOD_AES_128_XTS,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "encryption_context",
	 encryption_context );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvde_encryption_context_set_keys(
	          encryption_context,
	          fvde_test_sector_data_vector_key,
	          128,
	          fvde_test_sector_data_vector_tweak_key,
	          128,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = fvde_test_open_file_io_handle(
	          &file_io_handle,
	          fvde_test_sector_data_vector_data1,
	          32768,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_handle",
	 file_io_handle );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbfio_pool_initialize(
	          &file_io_pool,
	          0,
	          LIBBFIO_POOL_UNLIMITED_NUMBER_OF_OPEN_HANDLES,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_pool",
	 file_io_pool );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbfio_pool_append_handle(
	          file_io_pool,
	          &entry_index,
	          file_io_handle,
	          LIBBFIO_OPEN_READ,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	/* The file IO handle is now managed by the file IO pool
	 */
	file_io_handle = NULL;

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "entry_index",
	 entry_index,
	 0 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	FVDE_TEST_RUN_WITH_ARGS(
	 "libfvde_sector_data_vector_get_sector_data_at_offset",
	 fvde_test_sector_data_vector_get_sector_data_at_offset,
	 logical_volume_descriptor,
	 file_io_pool,
	 encryption_context );

	FVDE_TEST_RUN_WITH_ARGS(
	 "libfvde_sector_data_vector_get_sector_data_at_offset (unencrypted)",
	 fvde_test_sector_data_vector_get_sector_data_at_offset_unencrypted,
	 logical_volume_descriptor,
	 file_io_pool );

	FVDE_TEST_RUN_WITH_ARGS(
	 "libfvde_sector_data_vector_get_sector_data_at_offset (out of bounds)",
	 fvde_test_sector_data_vector_get_sector_data_at_offset_out_of_bounds,
	 logical_volume_descriptor,
	 file_io_pool,
	 encryption_context );

	/* Clean up
	 */
	result = libbfio_pool_close_all(
	          file_io_pool,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbfio_pool_free(
	          &file_io_pool,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "file_io_pool",
	 file_io_pool );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvde_encryption_context_free(
	          &encryption_context,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "encryption_context",
	 encryption_context );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvde_logical_volume_descriptor_free(
	          &logical_volume_descriptor,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "logical_volume_descriptor",
	 logical_volume_descriptor );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

#endif /* defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT )

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	if( file_io_pool != NULL )
	{
		libbfio_pool_free(
		 &file_io_pool,
		 NULL );
	}
	if( encryption_context != NULL )
	{
		libfvde_encryption_context_free(
		 &encryption_context,
		 NULL );
	}
	if( logical_volume_descriptor != NULL )
	{
		libfvde_logical_volume_descriptor_free(
		 &logical_volume_descriptor,
		 NULL );
	}
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT ) */
}

