/*
 * Library sector_data type test program
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

#include "../libfvde/libfvde_sector_data.h"

#if defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT )

/* Tests the libfvde_sector_data_initialize function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_sector_data_initialize(
     void )
{
	libcerror_error_t *error           = NULL;
	libfvde_sector_data_t *sector_data = NULL;
	int result                         = 0;

#if defined( HAVE_FVDE_TEST_MEMORY )
	int number_of_malloc_fail_tests    = 2;
	int number_of_memset_fail_tests    = 1;
	int test_number                    = 0;
#endif

	/* Test regular cases
	 */
	result = libfvde_sector_data_initialize(
	          &sector_data,
	          512,
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

	result = libfvde_sector_data_free(
	          &sector_data,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "sector_data",
	 sector_data );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfvde_sector_data_initialize(
	          NULL,
	          512,
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

	sector_data = (libfvde_sector_data_t *) 0x12345678UL;

	result = libfvde_sector_data_initialize(
	          &sector_data,
	          512,
	          &error );

	sector_data = NULL;

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libfvde_sector_data_initialize(
	          &sector_data,
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

	result = libfvde_sector_data_initialize(
	          &sector_data,
	          (size_t) SSIZE_MAX + 1,
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
		/* Test libfvde_sector_data_initialize with malloc failing
		 */
		fvde_test_malloc_attempts_before_fail = test_number;

		result = libfvde_sector_data_initialize(
		          &sector_data,
		          512,
		          &error );

		if( fvde_test_malloc_attempts_before_fail != -1 )
		{
			fvde_test_malloc_attempts_before_fail = -1;

			if( sector_data != NULL )
			{
				libfvde_sector_data_free(
				 &sector_data,
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
			 "sector_data",
			 sector_data );

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
		/* Test libfvde_sector_data_initialize with memset failing
		 */
		fvde_test_memset_attempts_before_fail = test_number;

		result = libfvde_sector_data_initialize(
		          &sector_data,
		          512,
		          &error );

		if( fvde_test_memset_attempts_before_fail != -1 )
		{
			fvde_test_memset_attempts_before_fail = -1;

			if( sector_data != NULL )
			{
				libfvde_sector_data_free(
				 &sector_data,
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
			 "sector_data",
			 sector_data );

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
	if( sector_data != NULL )
	{
		libfvde_sector_data_free(
		 &sector_data,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfvde_sector_data_free function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_sector_data_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfvde_sector_data_free(
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
	 "libfvde_sector_data_initialize",
	 fvde_test_sector_data_initialize );

	FVDE_TEST_RUN(
	 "libfvde_sector_data_free",
	 fvde_test_sector_data_free );

	/* TODO: add tests for libfvde_sector_data_read */

#endif /* defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT ) */
}

