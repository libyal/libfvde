/*
 * Library logical_volume_descriptor type test program
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
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fvde_test_libcdata.h"
#include "fvde_test_libcerror.h"
#include "fvde_test_libfvde.h"
#include "fvde_test_macros.h"
#include "fvde_test_memory.h"
#include "fvde_test_unused.h"

#include "../libfvde/libfvde_logical_volume_descriptor.h"
#include "../libfvde/libfvde_segment_descriptor.h"

#if defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT )

/* Tests the libfvde_logical_volume_descriptor_initialize function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_logical_volume_descriptor_initialize(
     void )
{
	libcerror_error_t *error                                       = NULL;
	libfvde_logical_volume_descriptor_t *logical_volume_descriptor = NULL;
	int result                                                     = 0;

#if defined( HAVE_FVDE_TEST_MEMORY )
	int number_of_malloc_fail_tests                                = 2;
	int number_of_memset_fail_tests                                = 1;
	int test_number                                                = 0;
#endif

	/* Test regular cases
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

	/* Test error cases
	 */
	result = libfvde_logical_volume_descriptor_initialize(
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

	logical_volume_descriptor = (libfvde_logical_volume_descriptor_t *) 0x12345678UL;

	result = libfvde_logical_volume_descriptor_initialize(
	          &logical_volume_descriptor,
	          &error );

	logical_volume_descriptor = NULL;

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
		/* Test libfvde_logical_volume_descriptor_initialize with malloc failing
		 */
		fvde_test_malloc_attempts_before_fail = test_number;

		result = libfvde_logical_volume_descriptor_initialize(
		          &logical_volume_descriptor,
		          &error );

		if( fvde_test_malloc_attempts_before_fail != -1 )
		{
			fvde_test_malloc_attempts_before_fail = -1;

			if( logical_volume_descriptor != NULL )
			{
				libfvde_logical_volume_descriptor_free(
				 &logical_volume_descriptor,
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
			 "logical_volume_descriptor",
			 logical_volume_descriptor );

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
		/* Test libfvde_logical_volume_descriptor_initialize with memset failing
		 */
		fvde_test_memset_attempts_before_fail = test_number;

		result = libfvde_logical_volume_descriptor_initialize(
		          &logical_volume_descriptor,
		          &error );

		if( fvde_test_memset_attempts_before_fail != -1 )
		{
			fvde_test_memset_attempts_before_fail = -1;

			if( logical_volume_descriptor != NULL )
			{
				libfvde_logical_volume_descriptor_free(
				 &logical_volume_descriptor,
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
			 "logical_volume_descriptor",
			 logical_volume_descriptor );

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
	if( logical_volume_descriptor != NULL )
	{
		libfvde_logical_volume_descriptor_free(
		 &logical_volume_descriptor,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfvde_logical_volume_descriptor_free function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_logical_volume_descriptor_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfvde_logical_volume_descriptor_free(
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

/* Tests the libfvde_logical_volume_descriptor_get_segment_descriptor_by_logical_block_number function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_logical_volume_descriptor_get_segment_descriptor_by_logical_block_number(
     void )
{
	uint64_t logical_block_numbers[ 3 ]                            = { 16, 64, 0x100000000UL };
	uint64_t number_of_blocks[ 3 ]                                 = { 16, 64, 0x80000000UL };
	libcerror_error_t *error                                       = NULL;
	libfvde_logical_volume_descriptor_t *logical_volume_descriptor = NULL;
	libfvde_segment_descriptor_t *found_segment_descriptor         = NULL;
	libfvde_segment_descriptor_t *segment_descriptor               = NULL;
	int entry_index                                                = 0;
	int result                                                     = 0;
	int segment_index                                              = 0;

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

	for( segment_index = 0;
	     segment_index < 3;
	     segment_index++ )
	{
		result = libfvde_segment_descriptor_initialize(
		          &segment_descriptor,
		          &error );

		FVDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		FVDE_TEST_ASSERT_IS_NOT_NULL(
		 "segment_descriptor",
		 segment_descriptor );

		FVDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		segment_descriptor->logical_block_number  = logical_block_numbers[ segment_index ];
		segment_descriptor->number_of_blocks      = number_of_blocks[ segment_index ];
		segment_descriptor->physical_block_number = 1000 * ( segment_index + 1 );

		result = libcdata_array_append_entry(
		          logical_volume_descriptor->segment_descriptors,
		          &entry_index,
		          (intptr_t *) segment_descriptor,
		          &error );

		FVDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		FVDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		segment_descriptor = NULL;
	}
	/* Test regular cases
	 */
	result = libfvde_logical_volume_descriptor_get_segment_descriptor_by_logical_block_number(
	          logical_volume_descriptor,
	          16,
	          &found_segment_descriptor,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "found_segment_descriptor",
	 found_segment_descriptor );

	FVDE_TEST_ASSERT_EQUAL_UINT64(
	 "found_segment_descriptor->physical_block_number",
	 found_segment_descriptor->physical_block_number,
	 (uint64_t) 1000 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	found_segment_descriptor = NULL;

	result = libfvde_logical_volume_descriptor_get_segment_descriptor_by_logical_block_number(
	          logical_volume_descriptor,
	          127,
	          &found_segment_descriptor,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "found_segment_descriptor",
	 found_segment_descriptor );

	FVDE_TEST_ASSERT_EQUAL_UINT64(
	 "found_segment_descriptor->physical_block_number",
	 found_segment_descriptor->physical_block_number,
	 (uint64_t) 2000 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	found_segment_descriptor = NULL;

	/* Logical block numbers beyond 32-bit
	 */
	result = libfvde_logical_volume_descriptor_get_segment_descriptor_by_logical_block_number(
	          logical_volume_descriptor,
	          0x17fffffffUL,
	          &found_segment_descriptor,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "found_segment_descriptor",
	 found_segment_descriptor );

	FVDE_TEST_ASSERT_EQUAL_UINT64(
	 "found_segment_descriptor->physical_block_number",
	 found_segment_descriptor->physical_block_number,
	 (uint64_t) 3000 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	found_segment_descriptor = NULL;

	/* Logical block numbers that are not mapped
	 */
	result = libfvde_logical_volume_descriptor_get_segment_descriptor_by_logical_block_number(
	          logical_volume_descriptor,
	          15,
	          &found_segment_descriptor,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvde_logical_volume_descriptor_get_segment_descriptor_by_logical_block_number(
	          logical_volume_descriptor,
	          32,
	          &found_segment_descriptor,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvde_logical_volume_descriptor_get_segment_descriptor_by_logical_block_number(
	          logical_volume_descriptor,
	          0x180000000UL,
	          &found_segment_descriptor,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	FVDE_TEST_ASSERT_IS_NULL(
	 "found_segment_descriptor",
	 found_segment_descriptor );

	/* Test error cases
	 */
	result = libfvde_logical_volume_descriptor_get_segment_descriptor_by_logical_block_number(
	          NULL,
	          16,
	          &found_segment_descriptor,
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

	result = libfvde_logical_volume_descriptor_get_segment_descriptor_by_logical_block_number(
	          logical_volume_descriptor,
	          16,
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
	if( segment_descriptor != NULL )
	{
		libfvde_segment_descriptor_free(
		 &segment_descriptor,
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
	 "libfvde_logical_volume_descriptor_initialize",
	 fvde_test_logical_volume_descriptor_initialize );

	FVDE_TEST_RUN(
	 "libfvde_logical_volume_descriptor_free",
	 fvde_test_logical_volume_descriptor_free );

	FVDE_TEST_RUN(
	 "libfvde_logical_volume_descriptor_get_segment_descriptor_by_logical_block_number",
	 fvde_test_logical_volume_descriptor_get_segment_descriptor_by_logical_block_number );

#endif /* defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT ) */
}

