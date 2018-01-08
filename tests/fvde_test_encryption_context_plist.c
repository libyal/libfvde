/*
 * Library encryption_context_plist type test program
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

#include "../libfvde/libfvde_encryption_context_plist.h"

/* Tests the libfvde_encryption_context_plist_initialize function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_encryption_context_plist_initialize(
     void )
{
	libcerror_error_t *error                                     = NULL;
	libfvde_encryption_context_plist_t *encryption_context_plist = NULL;
	int result                                                   = 0;

#if defined( HAVE_FVDE_TEST_MEMORY )
	int number_of_malloc_fail_tests                              = 1;
	int number_of_memset_fail_tests                              = 1;
	int test_number                                              = 0;
#endif

	/* Test regular cases
	 */
	result = libfvde_encryption_context_plist_initialize(
	          &encryption_context_plist,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "encryption_context_plist",
	 encryption_context_plist );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvde_encryption_context_plist_free(
	          &encryption_context_plist,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "encryption_context_plist",
	 encryption_context_plist );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfvde_encryption_context_plist_initialize(
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

	encryption_context_plist = (libfvde_encryption_context_plist_t *) 0x12345678UL;

	result = libfvde_encryption_context_plist_initialize(
	          &encryption_context_plist,
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

	encryption_context_plist = NULL;

#if defined( HAVE_FVDE_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libfvde_encryption_context_plist_initialize with malloc failing
		 */
		fvde_test_malloc_attempts_before_fail = test_number;

		result = libfvde_encryption_context_plist_initialize(
		          &encryption_context_plist,
		          &error );

		if( fvde_test_malloc_attempts_before_fail != -1 )
		{
			fvde_test_malloc_attempts_before_fail = -1;

			if( encryption_context_plist != NULL )
			{
				libfvde_encryption_context_plist_free(
				 &encryption_context_plist,
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
			 "encryption_context_plist",
			 encryption_context_plist );

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
		/* Test libfvde_encryption_context_plist_initialize with memset failing
		 */
		fvde_test_memset_attempts_before_fail = test_number;

		result = libfvde_encryption_context_plist_initialize(
		          &encryption_context_plist,
		          &error );

		if( fvde_test_memset_attempts_before_fail != -1 )
		{
			fvde_test_memset_attempts_before_fail = -1;

			if( encryption_context_plist != NULL )
			{
				libfvde_encryption_context_plist_free(
				 &encryption_context_plist,
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
			 "encryption_context_plist",
			 encryption_context_plist );

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
	if( encryption_context_plist != NULL )
	{
		libfvde_encryption_context_plist_free(
		 &encryption_context_plist,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfvde_encryption_context_plist_free function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_encryption_context_plist_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfvde_encryption_context_plist_free(
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

/* Tests the libfvde_encryption_context_plist_get_data_size function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_encryption_context_plist_get_data_size(
     void )
{
	libcerror_error_t *error                                     = NULL;
	libfvde_encryption_context_plist_t *encryption_context_plist = NULL;
	size64_t data_size                                           = 0;
	int data_size_is_set                                         = 0;
	int result                                                   = 0;

	/* Initialize test
	 */
	result = libfvde_encryption_context_plist_initialize(
	          &encryption_context_plist,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "encryption_context_plist",
	 encryption_context_plist );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libfvde_encryption_context_plist_get_data_size(
	          encryption_context_plist,
	          &data_size,
	          &error );

	FVDE_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	data_size_is_set = result;

	/* Test error cases
	 */
	result = libfvde_encryption_context_plist_get_data_size(
	          NULL,
	          &data_size,
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

	if( data_size_is_set != 0 )
	{
		result = libfvde_encryption_context_plist_get_data_size(
		          encryption_context_plist,
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
	result = libfvde_encryption_context_plist_free(
	          &encryption_context_plist,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "encryption_context_plist",
	 encryption_context_plist );

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
	if( encryption_context_plist != NULL )
	{
		libfvde_encryption_context_plist_free(
		 &encryption_context_plist,
		 NULL );
	}
	return( 0 );
}

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

	FVDE_TEST_RUN(
	 "libfvde_encryption_context_plist_initialize",
	 fvde_test_encryption_context_plist_initialize );

	FVDE_TEST_RUN(
	 "libfvde_encryption_context_plist_free",
	 fvde_test_encryption_context_plist_free );

	FVDE_TEST_RUN(
	 "libfvde_encryption_context_plist_get_data_size",
	 fvde_test_encryption_context_plist_get_data_size );

	/* TODO: add tests for libfvde_encryption_context_plist_copy_data */

#if defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT )

	/* TODO: add tests for libfvde_encryption_context_plist_set_data */

#endif /* defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT ) */

	/* TODO: add tests for libfvde_encryption_context_plist_read_file_io_handle */

	/* TODO: add tests for libfvde_encryption_context_plist_decrypt */

#if defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT )

	/* TODO: add tests for libfvde_encryption_context_plist_read_xml */

	/* TODO: add tests for libfvde_encryption_context_plist_get_conversion_status */

	/* TODO: add tests for libfvde_encryption_context_plist_get_passphrase_wrapped_kek */

	/* TODO: add tests for libfvde_encryption_context_plist_get_kek_wrapped_volume_key */

#endif /* defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

