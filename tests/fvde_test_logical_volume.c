/*
 * Library logical_volume type test program
 *
 * Copyright (C) 2011-2022, Joachim Metz <joachim.metz@gmail.com>
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
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fvde_test_libcerror.h"
#include "fvde_test_libfvde.h"
#include "fvde_test_macros.h"
#include "fvde_test_memory.h"
#include "fvde_test_unused.h"

#include "../libfvde/libfvde_io_handle.h"
#include "../libfvde/libfvde_logical_volume.h"
#include "../libfvde/libfvde_logical_volume_descriptor.h"

#define FVDE_TEST_LOGICAL_VOLUME_READ_BUFFER_SIZE	4096

#if defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT )

/* Tests the libfvde_logical_volume_initialize function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_logical_volume_initialize(
     void )
{
	libcerror_error_t *error                                       = NULL;
	libfvde_io_handle_t *io_handle                                 = NULL;
	libfvde_logical_volume_t *logical_volume                       = NULL;
	libfvde_logical_volume_descriptor_t *logical_volume_descriptor = NULL;
	int result                                                     = 0;

#if defined( HAVE_FVDE_TEST_MEMORY )
	int number_of_malloc_fail_tests                                = 1;
	int number_of_memset_fail_tests                                = 1;
	int test_number                                                = 0;
#endif

	/* Initialize test
	 */
	result = libfvde_io_handle_initialize(
	          &io_handle,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

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
	result = libfvde_logical_volume_initialize(
	          &logical_volume,
	          io_handle,
	          NULL,
	          logical_volume_descriptor,
	          NULL,
	          NULL,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "logical_volume",
	 logical_volume );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvde_logical_volume_free(
	          &logical_volume,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "logical_volume",
	 logical_volume );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfvde_logical_volume_initialize(
	          NULL,
	          io_handle,
	          NULL,
	          logical_volume_descriptor,
	          NULL,
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

	logical_volume = (libfvde_logical_volume_t *) 0x12345678UL;

	result = libfvde_logical_volume_initialize(
	          &logical_volume,
	          io_handle,
	          NULL,
	          logical_volume_descriptor,
	          NULL,
	          NULL,
	          &error );

	logical_volume = NULL;

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfvde_logical_volume_initialize(
	          &logical_volume,
	          NULL,
	          NULL,
	          logical_volume_descriptor,
	          NULL,
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

	result = libfvde_logical_volume_initialize(
	          &logical_volume,
	          io_handle,
	          NULL,
	          NULL,
	          NULL,
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

#if defined( HAVE_FVDE_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfvde_logical_volume_initialize with malloc failing
		 */
		fvde_test_malloc_attempts_before_fail = test_number;

		result = libfvde_logical_volume_initialize(
		          &logical_volume,
		          io_handle,
		          NULL,
		          logical_volume_descriptor,
		          NULL,
		          NULL,
		          &error );

		if( fvde_test_malloc_attempts_before_fail != -1 )
		{
			fvde_test_malloc_attempts_before_fail = -1;

			if( logical_volume != NULL )
			{
				libfvde_logical_volume_free(
				 &logical_volume,
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
			 "logical_volume",
			 logical_volume );

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
		/* Test libfvde_logical_volume_initialize with memset failing
		 */
		fvde_test_memset_attempts_before_fail = test_number;

		result = libfvde_logical_volume_initialize(
		          &logical_volume,
		          io_handle,
		          NULL,
		          logical_volume_descriptor,
		          NULL,
		          NULL,
		          &error );

		if( fvde_test_memset_attempts_before_fail != -1 )
		{
			fvde_test_memset_attempts_before_fail = -1;

			if( logical_volume != NULL )
			{
				libfvde_logical_volume_free(
				 &logical_volume,
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
			 "logical_volume",
			 logical_volume );

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

	result = libfvde_io_handle_free(
	          &io_handle,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

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
	if( logical_volume != NULL )
	{
		libfvde_logical_volume_free(
		 &logical_volume,
		 NULL );
	}
	if( logical_volume_descriptor != NULL )
	{
		libfvde_logical_volume_descriptor_free(
		 &logical_volume_descriptor,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libfvde_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT ) */

/* Tests the libfvde_logical_volume_free function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_logical_volume_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfvde_logical_volume_free(
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

#if defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT )

/* Tests the libfvde_internal_logical_volume_read_buffer_from_file_io_pool function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_internal_logical_volume_read_buffer_from_file_io_pool(
     libfvde_logical_volume_t *logical_volume )
{
	uint8_t buffer[ FVDE_TEST_LOGICAL_VOLUME_READ_BUFFER_SIZE ];

	libcerror_error_t *error = NULL;
	time_t timestamp         = 0;
	size64_t remaining_size  = 0;
	size64_t size            = 0;
	size_t read_size         = 0;
	ssize_t read_count       = 0;
	off64_t offset           = 0;
	off64_t read_offset      = 0;
	int number_of_tests      = 1024;
	int random_number        = 0;
	int result               = 0;
	int test_number          = 0;

	/* Determine size
	 */
	result = libfvde_logical_volume_get_size(
	          logical_volume,
	          &size,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Reset offset to 0
	 */
	offset = libfvde_logical_volume_seek_offset(
	          logical_volume,
	          0,
	          SEEK_SET,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	read_size = FVDE_TEST_LOGICAL_VOLUME_READ_BUFFER_SIZE;

	if( size < FVDE_TEST_LOGICAL_VOLUME_READ_BUFFER_SIZE )
	{
		read_size = (size_t) size;
	}
	read_count = libfvde_internal_logical_volume_read_buffer_from_file_io_pool(
	              (libfvde_internal_logical_volume_t *) logical_volume,
	              ( (libfvde_internal_logical_volume_t *) logical_volume )->file_io_pool,
	              buffer,
	              FVDE_TEST_LOGICAL_VOLUME_READ_BUFFER_SIZE,
	              &error );

	FVDE_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) read_size );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( size > 8 )
	{
		/* Set offset to size - 8
		 */
		offset = libfvde_logical_volume_seek_offset(
		          logical_volume,
		          -8,
		          SEEK_END,
		          &error );

		FVDE_TEST_ASSERT_EQUAL_INT64(
		 "offset",
		 offset,
		 (int64_t) size - 8 );

		FVDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Read buffer on size boundary
		 */
		read_count = libfvde_internal_logical_volume_read_buffer_from_file_io_pool(
		              (libfvde_internal_logical_volume_t *) logical_volume,
		              ( (libfvde_internal_logical_volume_t *) logical_volume )->file_io_pool,
		              buffer,
		              FVDE_TEST_LOGICAL_VOLUME_READ_BUFFER_SIZE,
		              &error );

		FVDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 8 );

		FVDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		/* Read buffer beyond size boundary
		 */
		read_count = libfvde_internal_logical_volume_read_buffer_from_file_io_pool(
		              (libfvde_internal_logical_volume_t *) logical_volume,
		              ( (libfvde_internal_logical_volume_t *) logical_volume )->file_io_pool,
		              buffer,
		              FVDE_TEST_LOGICAL_VOLUME_READ_BUFFER_SIZE,
		              &error );

		FVDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) 0 );

		FVDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Stress test read buffer
	 */
	timestamp = time(
	             NULL );

	srand(
	 (unsigned int) timestamp );

	offset = libfvde_logical_volume_seek_offset(
	          logical_volume,
	          0,
	          SEEK_SET,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	remaining_size = size;

	for( test_number = 0;
	     test_number < number_of_tests;
	     test_number++ )
	{
		random_number = rand();

		FVDE_TEST_ASSERT_GREATER_THAN_INT(
		 "random_number",
		 random_number,
		 -1 );

		read_size = (size_t) random_number % FVDE_TEST_LOGICAL_VOLUME_READ_BUFFER_SIZE;

#if defined( FVDE_TEST_VOLUME_VERBOSE )
		fprintf(
		 stdout,
		 "libfvde_logical_volume_read_buffer: at offset: %" PRIi64 " (0x%08" PRIx64 ") of size: %" PRIzd "\n",
		 read_offset,
		 read_offset,
		 read_size );
#endif
		read_count = libfvde_internal_logical_volume_read_buffer_from_file_io_pool(
		              (libfvde_internal_logical_volume_t *) logical_volume,
		              ( (libfvde_internal_logical_volume_t *) logical_volume )->file_io_pool,
		              buffer,
		              read_size,
		              &error );

		if( read_size > remaining_size )
		{
			read_size = (size_t) remaining_size;
		}
		FVDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) read_size );

		FVDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		read_offset += read_count;

		result = libfvde_logical_volume_get_offset(
		          logical_volume,
		          &offset,
		          &error );

		FVDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		FVDE_TEST_ASSERT_EQUAL_INT64(
		 "offset",
		 offset,
		 read_offset );

		FVDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		remaining_size -= read_count;

		if( remaining_size == 0 )
		{
			offset = libfvde_logical_volume_seek_offset(
			          logical_volume,
			          0,
			          SEEK_SET,
			          &error );

			FVDE_TEST_ASSERT_EQUAL_INT64(
			 "offset",
			 offset,
			 (int64_t) 0 );

			FVDE_TEST_ASSERT_IS_NULL(
			 "error",
			 error );

			read_offset = 0;

			remaining_size = size;
		}
	}
	/* Reset offset to 0
	 */
	offset = libfvde_logical_volume_seek_offset(
	          logical_volume,
	          0,
	          SEEK_SET,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 0 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	read_count = libfvde_internal_logical_volume_read_buffer_from_file_io_pool(
	              NULL,
	              ( (libfvde_internal_logical_volume_t *) logical_volume )->file_io_pool,
	              buffer,
	              FVDE_TEST_LOGICAL_VOLUME_READ_BUFFER_SIZE,
	              &error );

	FVDE_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libfvde_internal_logical_volume_read_buffer_from_file_io_pool(
	              (libfvde_internal_logical_volume_t *) logical_volume,
	              ( (libfvde_internal_logical_volume_t *) logical_volume )->file_io_pool,
	              NULL,
	              FVDE_TEST_LOGICAL_VOLUME_READ_BUFFER_SIZE,
	              &error );

	FVDE_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	read_count = libfvde_internal_logical_volume_read_buffer_from_file_io_pool(
	              (libfvde_internal_logical_volume_t *) logical_volume,
	              ( (libfvde_internal_logical_volume_t *) logical_volume )->file_io_pool,
	              buffer,
	              (size_t) SSIZE_MAX + 1,
	              &error );

	FVDE_TEST_ASSERT_EQUAL_SSIZE(
	 "read_count",
	 read_count,
	 (ssize_t) -1 );

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
	FVDE_TEST_UNREFERENCED_PARAMETER( argc )
	FVDE_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT )

	FVDE_TEST_RUN(
	 "libfvde_logical_volume_initialize",
	 fvde_test_logical_volume_initialize );

#endif /* defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT ) */

	FVDE_TEST_RUN(
	 "libfvde_logical_volume_free",
	 fvde_test_logical_volume_free );

#if defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT )

	/* TODO: add tests for libfvde_internal_logical_volume_open_read */

	/* TODO: add tests for libfvde_internal_logical_volume_open_read_keys_from_encrypted_metadata */

#endif /* defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT ) */

#if defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT )

/* TODO
		FVDE_TEST_RUN_WITH_ARGS(
		 "libfvde_internal_logical_volume_read_buffer_from_file_io_pool",
		 fvde_test_internal_logical_volume_read_buffer_from_file_io_pool,
		 logical_volume );
*/

#endif /* defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT ) */


#if defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT )

	/* TODO: add tests for libfvde_internal_logical_volume_seek_offset */

#endif /* defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

