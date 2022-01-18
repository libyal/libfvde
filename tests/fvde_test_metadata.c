/*
 * Library metadata type test program
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

#include "../libfvde/libfvde_metadata.h"

uint8_t fvde_test_metadata_volume_group_plist_data1[ 358 ] = {
	0x3c, 0x64, 0x69, 0x63, 0x74, 0x3e, 0x3c, 0x6b, 0x65, 0x79, 0x3e, 0x63, 0x6f, 0x6d, 0x2e, 0x61,
	0x70, 0x70, 0x6c, 0x65, 0x2e, 0x63, 0x6f, 0x72, 0x65, 0x73, 0x74, 0x6f, 0x72, 0x61, 0x67, 0x65,
	0x2e, 0x6c, 0x61, 0x62, 0x65, 0x6c, 0x2e, 0x73, 0x65, 0x71, 0x75, 0x65, 0x6e, 0x63, 0x65, 0x3c,
	0x2f, 0x6b, 0x65, 0x79, 0x3e, 0x3c, 0x69, 0x6e, 0x74, 0x65, 0x67, 0x65, 0x72, 0x20, 0x73, 0x69,
	0x7a, 0x65, 0x3d, 0x22, 0x33, 0x32, 0x22, 0x3e, 0x30, 0x78, 0x31, 0x3c, 0x2f, 0x69, 0x6e, 0x74,
	0x65, 0x67, 0x65, 0x72, 0x3e, 0x3c, 0x6b, 0x65, 0x79, 0x3e, 0x63, 0x6f, 0x6d, 0x2e, 0x61, 0x70,
	0x70, 0x6c, 0x65, 0x2e, 0x63, 0x6f, 0x72, 0x65, 0x73, 0x74, 0x6f, 0x72, 0x61, 0x67, 0x65, 0x2e,
	0x6c, 0x76, 0x67, 0x2e, 0x75, 0x75, 0x69, 0x64, 0x3c, 0x2f, 0x6b, 0x65, 0x79, 0x3e, 0x3c, 0x73,
	0x74, 0x72, 0x69, 0x6e, 0x67, 0x3e, 0x55, 0x55, 0x49, 0x44, 0x5f, 0x57, 0x41, 0x53, 0x5f, 0x53,
	0x4e, 0x49, 0x50, 0x50, 0x45, 0x44, 0x3c, 0x2f, 0x73, 0x74, 0x72, 0x69, 0x6e, 0x67, 0x3e, 0x3c,
	0x6b, 0x65, 0x79, 0x3e, 0x63, 0x6f, 0x6d, 0x2e, 0x61, 0x70, 0x70, 0x6c, 0x65, 0x2e, 0x63, 0x6f,
	0x72, 0x65, 0x73, 0x74, 0x6f, 0x72, 0x61, 0x67, 0x65, 0x2e, 0x6c, 0x76, 0x67, 0x2e, 0x6e, 0x61,
	0x6d, 0x65, 0x3c, 0x2f, 0x6b, 0x65, 0x79, 0x3e, 0x3c, 0x73, 0x74, 0x72, 0x69, 0x6e, 0x67, 0x3e,
	0x4d, 0x61, 0x63, 0x69, 0x6e, 0x74, 0x6f, 0x73, 0x68, 0x20, 0x48, 0x44, 0x3c, 0x2f, 0x73, 0x74,
	0x72, 0x69, 0x6e, 0x67, 0x3e, 0x3c, 0x6b, 0x65, 0x79, 0x3e, 0x63, 0x6f, 0x6d, 0x2e, 0x61, 0x70,
	0x70, 0x6c, 0x65, 0x2e, 0x63, 0x6f, 0x72, 0x65, 0x73, 0x74, 0x6f, 0x72, 0x61, 0x67, 0x65, 0x2e,
	0x6c, 0x76, 0x67, 0x2e, 0x70, 0x68, 0x79, 0x73, 0x69, 0x63, 0x61, 0x6c, 0x56, 0x6f, 0x6c, 0x75,
	0x6d, 0x65, 0x73, 0x3c, 0x2f, 0x6b, 0x65, 0x79, 0x3e, 0x3c, 0x61, 0x72, 0x72, 0x61, 0x79, 0x3e,
	0x3c, 0x73, 0x74, 0x72, 0x69, 0x6e, 0x67, 0x3e, 0x37, 0x35, 0x37, 0x37, 0x41, 0x34, 0x38, 0x36,
	0x2d, 0x38, 0x46, 0x39, 0x34, 0x2d, 0x34, 0x37, 0x46, 0x30, 0x2d, 0x41, 0x42, 0x46, 0x32, 0x2d,
	0x44, 0x44, 0x46, 0x31, 0x38, 0x41, 0x35, 0x43, 0x44, 0x41, 0x39, 0x41, 0x3c, 0x2f, 0x73, 0x74,
	0x72, 0x69, 0x6e, 0x67, 0x3e, 0x3c, 0x2f, 0x61, 0x72, 0x72, 0x61, 0x79, 0x3e, 0x3c, 0x2f, 0x64,
	0x69, 0x63, 0x74, 0x3e, 0x0a, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT )

/* Tests the libfvde_metadata_initialize function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_metadata_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libfvde_metadata_t *metadata    = NULL;
	int result                      = 0;

#if defined( HAVE_FVDE_TEST_MEMORY )
	int number_of_malloc_fail_tests = 2;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libfvde_metadata_initialize(
	          &metadata,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "metadata",
	 metadata );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvde_metadata_free(
	          &metadata,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "metadata",
	 metadata );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfvde_metadata_initialize(
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

	metadata = (libfvde_metadata_t *) 0x12345678UL;

	result = libfvde_metadata_initialize(
	          &metadata,
	          &error );

	metadata = NULL;

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
		/* Test libfvde_metadata_initialize with malloc failing
		 */
		fvde_test_malloc_attempts_before_fail = test_number;

		result = libfvde_metadata_initialize(
		          &metadata,
		          &error );

		if( fvde_test_malloc_attempts_before_fail != -1 )
		{
			fvde_test_malloc_attempts_before_fail = -1;

			if( metadata != NULL )
			{
				libfvde_metadata_free(
				 &metadata,
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
			 "metadata",
			 metadata );

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
		/* Test libfvde_metadata_initialize with memset failing
		 */
		fvde_test_memset_attempts_before_fail = test_number;

		result = libfvde_metadata_initialize(
		          &metadata,
		          &error );

		if( fvde_test_memset_attempts_before_fail != -1 )
		{
			fvde_test_memset_attempts_before_fail = -1;

			if( metadata != NULL )
			{
				libfvde_metadata_free(
				 &metadata,
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
			 "metadata",
			 metadata );

			FVDE_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_FVDE_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( metadata != NULL )
	{
		libfvde_metadata_free(
		 &metadata,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfvde_metadata_free function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_metadata_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfvde_metadata_free(
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

/* Tests the libfvde_metadata_read_volume_group_plist function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_metadata_read_volume_group_plist(
     void )
{
	libcerror_error_t *error     = NULL;
	libfvde_metadata_t *metadata = NULL;
	int result                   = 0;

	/* Initialize test
	 */
	result = libfvde_metadata_initialize(
	          &metadata,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "metadata",
	 metadata );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfvde_metadata_read_volume_group_plist(
	          metadata,
	          fvde_test_metadata_volume_group_plist_data1,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfvde_metadata_read_volume_group_plist(
	          NULL,
	          fvde_test_metadata_volume_group_plist_data1,
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

	result = libfvde_metadata_read_volume_group_plist(
	          metadata,
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

	/* Clean up
	 */
	result = libfvde_metadata_free(
	          &metadata,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "metadata",
	 metadata );

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
	if( metadata != NULL )
	{
		libfvde_metadata_free(
		 &metadata,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfvde_metadata_read_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_metadata_read_file_io_handle(
     void )
{
	libcerror_error_t *error     = NULL;
	libfvde_metadata_t *metadata = NULL;
	int result                   = 0;

	/* Initialize test
	 */
	result = libfvde_metadata_initialize(
	          &metadata,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "metadata",
	 metadata );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libfvde_metadata_read_file_io_handle(
	          NULL,
	          NULL,
	          NULL,
	          0,
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

	result = libfvde_metadata_read_file_io_handle(
	          metadata,
	          NULL,
	          NULL,
	          0,
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

	/* Clean up
	 */
	result = libfvde_metadata_free(
	          &metadata,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "metadata",
	 metadata );

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
	if( metadata != NULL )
	{
		libfvde_metadata_free(
		 &metadata,
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
	FVDE_TEST_UNREFERENCED_PARAMETER( argc )
	FVDE_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT )

	FVDE_TEST_RUN(
	 "libfvde_metadata_initialize",
	 fvde_test_metadata_initialize );

	FVDE_TEST_RUN(
	 "libfvde_metadata_free",
	 fvde_test_metadata_free );

	/* TODO: add tests for libfvde_metadata_read_type_0x0011 */

	FVDE_TEST_RUN(
	 "libfvde_metadata_read_volume_group_plist",
	 fvde_test_metadata_read_volume_group_plist );

	FVDE_TEST_RUN(
	 "libfvde_metadata_read_file_io_handle",
	 fvde_test_metadata_read_file_io_handle );

#endif /* defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT ) */
}

