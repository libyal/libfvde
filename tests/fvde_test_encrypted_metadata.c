/*
 * Library encrypted_metadata type test program
 *
 * Copyright (C) 2011-2018, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
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

#include "../libfvde/libfvde_encrypted_metadata.h"

#if defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT )

/* Tests the libfvde_encrypted_metadata_initialize function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_encrypted_metadata_initialize(
     void )
{
	libcerror_error_t *error                         = NULL;
	libfvde_encrypted_metadata_t *encrypted_metadata = NULL;
	int result                                       = 0;

#if defined( HAVE_FVDE_TEST_MEMORY )
	int number_of_malloc_fail_tests                  = 1;
	int number_of_memset_fail_tests                  = 1;
	int test_number                                  = 0;
#endif

	/* Test regular cases
	 */
	result = libfvde_encrypted_metadata_initialize(
	          &encrypted_metadata,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "encrypted_metadata",
	 encrypted_metadata );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvde_encrypted_metadata_free(
	          &encrypted_metadata,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "encrypted_metadata",
	 encrypted_metadata );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfvde_encrypted_metadata_initialize(
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

	encrypted_metadata = (libfvde_encrypted_metadata_t *) 0x12345678UL;

	result = libfvde_encrypted_metadata_initialize(
	          &encrypted_metadata,
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

	encrypted_metadata = NULL;

#if defined( HAVE_FVDE_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfvde_encrypted_metadata_initialize with malloc failing
		 */
		fvde_test_malloc_attempts_before_fail = test_number;

		result = libfvde_encrypted_metadata_initialize(
		          &encrypted_metadata,
		          &error );

		if( fvde_test_malloc_attempts_before_fail != -1 )
		{
			fvde_test_malloc_attempts_before_fail = -1;

			if( encrypted_metadata != NULL )
			{
				libfvde_encrypted_metadata_free(
				 &encrypted_metadata,
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
			 "encrypted_metadata",
			 encrypted_metadata );

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
		/* Test libfvde_encrypted_metadata_initialize with memset failing
		 */
		fvde_test_memset_attempts_before_fail = test_number;

		result = libfvde_encrypted_metadata_initialize(
		          &encrypted_metadata,
		          &error );

		if( fvde_test_memset_attempts_before_fail != -1 )
		{
			fvde_test_memset_attempts_before_fail = -1;

			if( encrypted_metadata != NULL )
			{
				libfvde_encrypted_metadata_free(
				 &encrypted_metadata,
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
			 "encrypted_metadata",
			 encrypted_metadata );

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
	if( encrypted_metadata != NULL )
	{
		libfvde_encrypted_metadata_free(
		 &encrypted_metadata,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfvde_encrypted_metadata_free function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_encrypted_metadata_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfvde_encrypted_metadata_free(
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

/* Tests the libfvde_encrypted_metadata_read function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_encrypted_metadata_read(
     void )
{
	libcerror_error_t *error                         = NULL;
	libfvde_encrypted_metadata_t *encrypted_metadata = NULL;
	int result                                       = 0;

	/* Initialize test
	 */
	result = libfvde_encrypted_metadata_initialize(
	          &encrypted_metadata,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "encrypted_metadata",
	 encrypted_metadata );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfvde_encrypted_metadata_read(
	          NULL,
	          NULL,
	          NULL,
	          0,
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

	result = libfvde_encrypted_metadata_read(
	          encrypted_metadata,
	          NULL,
	          NULL,
	          0,
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
	result = libfvde_encrypted_metadata_free(
	          &encrypted_metadata,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "encrypted_metadata",
	 encrypted_metadata );

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
	if( encrypted_metadata != NULL )
	{
		libfvde_encrypted_metadata_free(
		 &encrypted_metadata,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfvde_encrypted_metadata_get_number_of_data_area_descriptors function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_encrypted_metadata_get_number_of_data_area_descriptors(
     void )
{
	libcerror_error_t *error                         = NULL;
	libfvde_encrypted_metadata_t *encrypted_metadata = NULL;
	int number_of_data_area_descriptors              = 0;
	int number_of_data_area_descriptors_is_set       = 0;
	int result                                       = 0;

	/* Initialize test
	 */
	result = libfvde_encrypted_metadata_initialize(
	          &encrypted_metadata,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "encrypted_metadata",
	 encrypted_metadata );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfvde_encrypted_metadata_get_number_of_data_area_descriptors(
	          encrypted_metadata,
	          &number_of_data_area_descriptors,
	          &error );

	FVDE_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	number_of_data_area_descriptors_is_set = result;

	/* Test error cases
	 */
	result = libfvde_encrypted_metadata_get_number_of_data_area_descriptors(
	          NULL,
	          &number_of_data_area_descriptors,
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

	if( number_of_data_area_descriptors_is_set != 0 )
	{
		result = libfvde_encrypted_metadata_get_number_of_data_area_descriptors(
		          encrypted_metadata,
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
	}
	/* Clean up
	 */
	result = libfvde_encrypted_metadata_free(
	          &encrypted_metadata,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "encrypted_metadata",
	 encrypted_metadata );

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
	if( encrypted_metadata != NULL )
	{
		libfvde_encrypted_metadata_free(
		 &encrypted_metadata,
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
	 "libfvde_encrypted_metadata_initialize",
	 fvde_test_encrypted_metadata_initialize );

	FVDE_TEST_RUN(
	 "libfvde_encrypted_metadata_free",
	 fvde_test_encrypted_metadata_free );

	/* TODO: add tests for libfvde_encrypted_metadata_read_block_header */

	/* TODO: add tests for libfvde_encrypted_metadata_read_type_0x0012 */

	/* TODO: add tests for libfvde_encrypted_metadata_read_type_0x0013 */

	/* TODO: add tests for libfvde_encrypted_metadata_read_type_0x0014 */

	/* TODO: add tests for libfvde_encrypted_metadata_read_type_0x0016 */

	/* TODO: add tests for libfvde_encrypted_metadata_read_type_0x0017 */

	/* TODO: add tests for libfvde_encrypted_metadata_read_type_0x0018 */

	/* TODO: add tests for libfvde_encrypted_metadata_read_type_0x0019 */

	/* TODO: add tests for libfvde_encrypted_metadata_read_type_0x001a */

	/* TODO: add tests for libfvde_encrypted_metadata_read_type_0x001c */

	/* TODO: add tests for libfvde_encrypted_metadata_read_type_0x001d */

	/* TODO: add tests for libfvde_encrypted_metadata_read_type_0x0021 */

	/* TODO: add tests for libfvde_encrypted_metadata_read_type_0x0022 */

	/* TODO: add tests for libfvde_encrypted_metadata_read_type_0x0025 */

	/* TODO: add tests for libfvde_encrypted_metadata_read_type_0x0105 */

	/* TODO: add tests for libfvde_encrypted_metadata_read_type_0x0304 */

	/* TODO: add tests for libfvde_encrypted_metadata_read_type_0x0305 */

	/* TODO: add tests for libfvde_encrypted_metadata_read_type_0x0404 */

	/* TODO: add tests for libfvde_encrypted_metadata_read_type_0x0405 */

	/* TODO: add tests for libfvde_encrypted_metadata_read_type_0x0505 */

	FVDE_TEST_RUN(
	 "libfvde_encrypted_metadata_read",
	 fvde_test_encrypted_metadata_read );

	/* TODO: add tests for libfvde_encrypted_metadata_get_volume_master_key */

	FVDE_TEST_RUN(
	 "libfvde_encrypted_metadata_get_number_of_data_area_descriptors",
	 fvde_test_encrypted_metadata_get_number_of_data_area_descriptors );

	/* TODO: add tests for libfvde_encrypted_metadata_get_data_area_descriptor_by_index */

#endif /* defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

