/*
 * Library encryption_context_plist type testing program
 *
 * Copyright (C) 2011-2016, Joachim Metz <joachim.metz@gmail.com>
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
#include "fvde_test_libcstring.h"
#include "fvde_test_libfvde.h"
#include "fvde_test_macros.h"
#include "fvde_test_memory.h"
#include "fvde_test_unused.h"

/* Tests the libfvde_encryption_context_plist_initialize function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_encryption_context_plist_initialize(
     void )
{
	libcerror_error_t *error = NULL;
	libfvde_encryption_context_plist_t *encryption_context_plist      = NULL;
	int result               = 0;

	/* Test libfvde_encryption_context_plist_initialize
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

	/* Test libfvde_encryption_context_plist_initialize with malloc failing
	 */
	fvde_test_malloc_attempts_before_fail = 0;

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
	/* Test libfvde_encryption_context_plist_initialize with memset failing
	 */
	fvde_test_memset_attempts_before_fail = 0;

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

/* The main program
 */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
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

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

