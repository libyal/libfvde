/*
 * Library volume type test program
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
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( TIME_WITH_SYS_TIME )
#include <sys/time.h>
#include <time.h>
#elif defined( HAVE_SYS_TIME_H )
#include <sys/time.h>
#else
#include <time.h>
#endif

#include "fvde_test_functions.h"
#include "fvde_test_getopt.h"
#include "fvde_test_libbfio.h"
#include "fvde_test_libcerror.h"
#include "fvde_test_libfvde.h"
#include "fvde_test_macros.h"
#include "fvde_test_memory.h"

#include "../libfvde/libfvde_volume.h"

#if defined( HAVE_WIDE_SYSTEM_CHARACTER ) && SIZEOF_WCHAR_T != 2 && SIZEOF_WCHAR_T != 4
#error Unsupported size of wchar_t
#endif

/* Define to make fvde_test_volume generate verbose output
#define FVDE_TEST_VOLUME_VERBOSE
 */

#define FVDE_TEST_VOLUME_READ_BUFFER_SIZE	4096

#if !defined( LIBFVDE_HAVE_BFIO )

LIBFVDE_EXTERN \
int libfvde_check_volume_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_open_file_io_handle(
     libfvde_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libfvde_error_t **error );

#endif /* !defined( LIBFVDE_HAVE_BFIO ) */

/* Creates and opens a source volume
 * Returns 1 if successful or -1 on error
 */
int fvde_test_volume_open_source(
     libfvde_volume_t **volume,
     libbfio_handle_t *file_io_handle,
     const system_character_t *password,
     libcerror_error_t **error )
{
	static char *function = "fvde_test_volume_open_source";
	size_t string_length  = 0;
	int result            = 0;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( libfvde_volume_initialize(
	     volume,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize volume.",
		 function );

		goto on_error;
	}
	if( password != NULL )
	{
		string_length = system_string_length(
		                 password );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfvde_volume_set_utf16_password(
		          *volume,
		          (uint16_t *) password,
		          string_length,
		          error );
#else
		result = libfvde_volume_set_utf8_password(
		          *volume,
		          (uint8_t *) password,
		          string_length,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set password.",
			 function );

			goto on_error;
		}
	}
	result = libfvde_volume_open_file_io_handle(
	          *volume,
	          file_io_handle,
	          LIBFVDE_OPEN_READ,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open volume.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *volume != NULL )
	{
		libfvde_volume_free(
		 volume,
		 NULL );
	}
	return( -1 );
}

/* Closes and frees a source volume
 * Returns 1 if successful or -1 on error
 */
int fvde_test_volume_close_source(
     libfvde_volume_t **volume,
     libcerror_error_t **error )
{
	static char *function = "fvde_test_volume_close_source";
	int result            = 0;

	if( volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	if( libfvde_volume_close(
	     *volume,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close volume.",
		 function );

		result = -1;
	}
	if( libfvde_volume_free(
	     volume,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free volume.",
		 function );

		result = -1;
	}
	return( result );
}

/* Tests the libfvde_volume_initialize function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_volume_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libfvde_volume_t *volume        = NULL;
	int result                      = 0;

#if defined( HAVE_FVDE_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libfvde_volume_initialize(
	          &volume,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "volume",
	 volume );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvde_volume_free(
	          &volume,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "volume",
	 volume );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfvde_volume_initialize(
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

	volume = (libfvde_volume_t *) 0x12345678UL;

	result = libfvde_volume_initialize(
	          &volume,
	          &error );

	volume = NULL;

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
		/* Test libfvde_volume_initialize with malloc failing
		 */
		fvde_test_malloc_attempts_before_fail = test_number;

		result = libfvde_volume_initialize(
		          &volume,
		          &error );

		if( fvde_test_malloc_attempts_before_fail != -1 )
		{
			fvde_test_malloc_attempts_before_fail = -1;

			if( volume != NULL )
			{
				libfvde_volume_free(
				 &volume,
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
			 "volume",
			 volume );

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
		/* Test libfvde_volume_initialize with memset failing
		 */
		fvde_test_memset_attempts_before_fail = test_number;

		result = libfvde_volume_initialize(
		          &volume,
		          &error );

		if( fvde_test_memset_attempts_before_fail != -1 )
		{
			fvde_test_memset_attempts_before_fail = -1;

			if( volume != NULL )
			{
				libfvde_volume_free(
				 &volume,
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
			 "volume",
			 volume );

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
	if( volume != NULL )
	{
		libfvde_volume_free(
		 &volume,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfvde_volume_free function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_volume_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfvde_volume_free(
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

/* Tests the libfvde_volume_open function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_volume_open(
     const system_character_t *source,
     const system_character_t *password )
{
	char narrow_source[ 256 ];

	libcerror_error_t *error = NULL;
	libfvde_volume_t *volume = NULL;
	size_t string_length     = 0;
	int result               = 0;

	/* Initialize test
	 */
	result = fvde_test_get_narrow_source(
	          source,
	          narrow_source,
	          256,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvde_volume_initialize(
	          &volume,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "volume",
	 volume );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( password != NULL )
	{
		string_length = system_string_length(
		                 password );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfvde_volume_set_utf16_password(
		          volume,
		          (uint16_t *) password,
		          string_length,
		          &error );
#else
		result = libfvde_volume_set_utf8_password(
		          volume,
		          (uint8_t *) password,
		          string_length,
		          &error );
#endif
		FVDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        FVDE_TEST_ASSERT_IS_NULL(
	         "error",
		 error );
	}
	/* Test open
	 */
	result = libfvde_volume_open(
	          volume,
	          narrow_source,
	          LIBFVDE_OPEN_READ,
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
	result = libfvde_volume_open(
	          NULL,
	          narrow_source,
	          LIBFVDE_OPEN_READ,
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

	result = libfvde_volume_open(
	          volume,
	          NULL,
	          LIBFVDE_OPEN_READ,
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

	result = libfvde_volume_open(
	          volume,
	          narrow_source,
	          -1,
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

	/* Test open when already opened
	 */
	result = libfvde_volume_open(
	          volume,
	          narrow_source,
	          LIBFVDE_OPEN_READ,
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
	result = libfvde_volume_free(
	          &volume,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "volume",
	 volume );

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
	if( volume != NULL )
	{
		libfvde_volume_free(
		 &volume,
		 NULL );
	}
	return( 0 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Tests the libfvde_volume_open_wide function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_volume_open_wide(
     const system_character_t *source,
     const system_character_t *password )
{
	wchar_t wide_source[ 256 ];

	libcerror_error_t *error = NULL;
	libfvde_volume_t *volume = NULL;
	size_t string_length     = 0;
	int result               = 0;

	/* Initialize test
	 */
	result = fvde_test_get_wide_source(
	          source,
	          wide_source,
	          256,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvde_volume_initialize(
	          &volume,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "volume",
	 volume );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( password != NULL )
	{
		string_length = system_string_length(
		                 password );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfvde_volume_set_utf16_password(
		          volume,
		          (uint16_t *) password,
		          string_length,
		          &error );
#else
		result = libfvde_volume_set_utf8_password(
		          volume,
		          (uint8_t *) password,
		          string_length,
		          &error );
#endif
		FVDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        FVDE_TEST_ASSERT_IS_NULL(
	         "error",
		 error );
	}
	/* Test open
	 */
	result = libfvde_volume_open_wide(
	          volume,
	          wide_source,
	          LIBFVDE_OPEN_READ,
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
	result = libfvde_volume_open_wide(
	          NULL,
	          wide_source,
	          LIBFVDE_OPEN_READ,
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

	result = libfvde_volume_open_wide(
	          volume,
	          NULL,
	          LIBFVDE_OPEN_READ,
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

	result = libfvde_volume_open_wide(
	          volume,
	          wide_source,
	          -1,
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

	/* Test open when already opened
	 */
	result = libfvde_volume_open_wide(
	          volume,
	          wide_source,
	          LIBFVDE_OPEN_READ,
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
	result = libfvde_volume_free(
	          &volume,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "volume",
	 volume );

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
	if( volume != NULL )
	{
		libfvde_volume_free(
		 &volume,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Tests the libfvde_volume_open_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_volume_open_file_io_handle(
     const system_character_t *source,
     const system_character_t *password )
{
	libbfio_handle_t *file_io_handle = NULL;
	libcerror_error_t *error         = NULL;
	libfvde_volume_t *volume         = NULL;
	size_t string_length             = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libbfio_file_initialize(
	          &file_io_handle,
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

	string_length = system_string_length(
	                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libbfio_file_set_name_wide(
	          file_io_handle,
	          source,
	          string_length,
	          &error );
#else
	result = libbfio_file_set_name(
	          file_io_handle,
	          source,
	          string_length,
	          &error );
#endif
	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvde_volume_initialize(
	          &volume,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "volume",
	 volume );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( password != NULL )
	{
		string_length = system_string_length(
		                 password );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfvde_volume_set_utf16_password(
		          volume,
		          (uint16_t *) password,
		          string_length,
		          &error );
#else
		result = libfvde_volume_set_utf8_password(
		          volume,
		          (uint8_t *) password,
		          string_length,
		          &error );
#endif
		FVDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        FVDE_TEST_ASSERT_IS_NULL(
	         "error",
		 error );
	}
	/* Test open
	 */
	result = libfvde_volume_open_file_io_handle(
	          volume,
	          file_io_handle,
	          LIBFVDE_OPEN_READ,
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
	result = libfvde_volume_open_file_io_handle(
	          NULL,
	          file_io_handle,
	          LIBFVDE_OPEN_READ,
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

	result = libfvde_volume_open_file_io_handle(
	          volume,
	          NULL,
	          LIBFVDE_OPEN_READ,
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

	result = libfvde_volume_open_file_io_handle(
	          volume,
	          file_io_handle,
	          -1,
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

	/* Test open when already opened
	 */
	result = libfvde_volume_open_file_io_handle(
	          volume,
	          file_io_handle,
	          LIBFVDE_OPEN_READ,
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
	result = libfvde_volume_free(
	          &volume,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "volume",
	 volume );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbfio_handle_free(
	          &file_io_handle,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "file_io_handle",
	 file_io_handle );

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
	if( volume != NULL )
	{
		libfvde_volume_free(
		 &volume,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfvde_volume_close function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_volume_close(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libfvde_volume_close(
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

/* Tests the libfvde_volume_open and libfvde_volume_close functions
 * Returns 1 if successful or 0 if not
 */
int fvde_test_volume_open_close(
     const system_character_t *source,
     const system_character_t *password )
{
	libcerror_error_t *error = NULL;
	libfvde_volume_t *volume = NULL;
	size_t string_length     = 0;
	int result               = 0;

	/* Initialize test
	 */
	result = libfvde_volume_initialize(
	          &volume,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "volume",
	 volume );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( password != NULL )
	{
		string_length = system_string_length(
		                 password );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfvde_volume_set_utf16_password(
		          volume,
		          (uint16_t *) password,
		          string_length,
		          &error );
#else
		result = libfvde_volume_set_utf8_password(
		          volume,
		          (uint8_t *) password,
		          string_length,
		          &error );
#endif
		FVDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        FVDE_TEST_ASSERT_IS_NULL(
	         "error",
		 error );
	}
	/* Test open and close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfvde_volume_open_wide(
	          volume,
	          source,
	          LIBFVDE_OPEN_READ,
	          &error );
#else
	result = libfvde_volume_open(
	          volume,
	          source,
	          LIBFVDE_OPEN_READ,
	          &error );
#endif

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvde_volume_close(
	          volume,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open and close a second time to validate clean up on close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfvde_volume_open_wide(
	          volume,
	          source,
	          LIBFVDE_OPEN_READ,
	          &error );
#else
	result = libfvde_volume_open(
	          volume,
	          source,
	          LIBFVDE_OPEN_READ,
	          &error );
#endif

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfvde_volume_close(
	          volume,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libfvde_volume_free(
	          &volume,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "volume",
	 volume );

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
	if( volume != NULL )
	{
		libfvde_volume_free(
		 &volume,
		 NULL );
	}
	return( 0 );
}

/* Tests the libfvde_volume_signal_abort function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_volume_signal_abort(
     libfvde_volume_t *volume )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfvde_volume_signal_abort(
	          volume,
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
	result = libfvde_volume_signal_abort(
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

/* Tests the libfvde_volume_is_locked function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_volume_is_locked(
     libfvde_volume_t *volume )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfvde_volume_is_locked(
	          NULL,
	          &error );

	FVDE_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libfvde_volume_is_locked(
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

/* Tests the libfvde_volume_read_buffer function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_volume_read_buffer(
     libfvde_volume_t *volume )
{
	uint8_t buffer[ FVDE_TEST_VOLUME_READ_BUFFER_SIZE ];

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
	result = libfvde_volume_get_logical_volume_size(
	          volume,
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
	offset = libfvde_volume_seek_offset(
	          volume,
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
	read_size = FVDE_TEST_VOLUME_READ_BUFFER_SIZE;

	if( size < FVDE_TEST_VOLUME_READ_BUFFER_SIZE )
	{
		read_size = (size_t) size;
	}
	read_count = libfvde_volume_read_buffer(
	              volume,
	              buffer,
	              FVDE_TEST_VOLUME_READ_BUFFER_SIZE,
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
		offset = libfvde_volume_seek_offset(
		          volume,
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
		read_count = libfvde_volume_read_buffer(
		              volume,
		              buffer,
		              FVDE_TEST_VOLUME_READ_BUFFER_SIZE,
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
		read_count = libfvde_volume_read_buffer(
		              volume,
		              buffer,
		              FVDE_TEST_VOLUME_READ_BUFFER_SIZE,
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

	offset = libfvde_volume_seek_offset(
	          volume,
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

		read_size = (size_t) random_number % FVDE_TEST_VOLUME_READ_BUFFER_SIZE;

#if defined( FVDE_TEST_VOLUME_VERBOSE )
		fprintf(
		 stdout,
		 "libfvde_volume_read_buffer: at offset: %" PRIi64 " (0x%08" PRIx64 ") of size: %" PRIzd "\n",
		 read_offset,
		 read_offset,
		 read_size );
#endif
		read_count = libfvde_volume_read_buffer(
		              volume,
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

		result = libfvde_volume_get_offset(
		          volume,
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
			offset = libfvde_volume_seek_offset(
			          volume,
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
	offset = libfvde_volume_seek_offset(
	          volume,
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
	read_count = libfvde_volume_read_buffer(
	              NULL,
	              buffer,
	              FVDE_TEST_VOLUME_READ_BUFFER_SIZE,
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

	read_count = libfvde_volume_read_buffer(
	              volume,
	              NULL,
	              FVDE_TEST_VOLUME_READ_BUFFER_SIZE,
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

	read_count = libfvde_volume_read_buffer(
	              volume,
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

#if defined( HAVE_FVDE_TEST_RWLOCK )

	/* Test libfvde_volume_read_buffer with pthread_rwlock_wrlock failing in libcthreads_read_write_lock_grab_for_write
	 */
	fvde_test_pthread_rwlock_wrlock_attempts_before_fail = 0;

	read_count = libfvde_volume_read_buffer(
	              volume,
	              buffer,
	              FVDE_TEST_PARTITION_READ_BUFFER_SIZE,
	              &error );

	if( fvde_test_pthread_rwlock_wrlock_attempts_before_fail != -1 )
	{
		fvde_test_pthread_rwlock_wrlock_attempts_before_fail = -1;
	}
	else
	{
		FVDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) -1 );

		FVDE_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Test libfvde_volume_read_buffer with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_write
	 */
	fvde_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	read_count = libfvde_volume_read_buffer(
	              volume,
	              buffer,
	              FVDE_TEST_PARTITION_READ_BUFFER_SIZE,
	              &error );

	if( fvde_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		fvde_test_pthread_rwlock_unlock_attempts_before_fail = -1;
	}
	else
	{
		FVDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) -1 );

		FVDE_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_FVDE_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfvde_volume_read_buffer_at_offset function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_volume_read_buffer_at_offset(
     libfvde_volume_t *volume )
{
	uint8_t buffer[ FVDE_TEST_VOLUME_READ_BUFFER_SIZE ];

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
	result = libfvde_volume_get_logical_volume_size(
	          volume,
	          &size,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	read_size = FVDE_TEST_VOLUME_READ_BUFFER_SIZE;

	if( size < FVDE_TEST_VOLUME_READ_BUFFER_SIZE )
	{
		read_size = (size_t) size;
	}
	read_count = libfvde_volume_read_buffer_at_offset(
	              volume,
	              buffer,
	              FVDE_TEST_VOLUME_READ_BUFFER_SIZE,
	              0,
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
		/* Read buffer on size boundary
		 */
		read_count = libfvde_volume_read_buffer_at_offset(
		              volume,
		              buffer,
		              FVDE_TEST_VOLUME_READ_BUFFER_SIZE,
		              size - 8,
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
		read_count = libfvde_volume_read_buffer_at_offset(
		              volume,
		              buffer,
		              FVDE_TEST_VOLUME_READ_BUFFER_SIZE,
		              size + 8,
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

	for( test_number = 0;
	     test_number < number_of_tests;
	     test_number++ )
	{
		random_number = rand();

		FVDE_TEST_ASSERT_GREATER_THAN_INT(
		 "random_number",
		 random_number,
		 -1 );

		if( size > 0 )
		{
			read_offset = (off64_t) random_number % size;
		}
		read_size = (size_t) random_number % FVDE_TEST_VOLUME_READ_BUFFER_SIZE;

#if defined( FVDE_TEST_VOLUME_VERBOSE )
		fprintf(
		 stdout,
		 "libfvde_volume_read_buffer_at_offset: at offset: %" PRIi64 " (0x%08" PRIx64 ") of size: %" PRIzd "\n",
		 read_offset,
		 read_offset,
		 read_size );
#endif
		read_count = libfvde_volume_read_buffer_at_offset(
		              volume,
		              buffer,
		              read_size,
		              read_offset,
		              &error );

		remaining_size = size - read_offset;

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

		result = libfvde_volume_get_offset(
		          volume,
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
	}
	/* Test error cases
	 */
	read_count = libfvde_volume_read_buffer_at_offset(
	              NULL,
	              buffer,
	              FVDE_TEST_VOLUME_READ_BUFFER_SIZE,
	              0,
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

	read_count = libfvde_volume_read_buffer_at_offset(
	              volume,
	              NULL,
	              FVDE_TEST_VOLUME_READ_BUFFER_SIZE,
	              0,
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

	read_count = libfvde_volume_read_buffer_at_offset(
	              volume,
	              buffer,
	              (size_t) SSIZE_MAX + 1,
	              0,
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

	read_count = libfvde_volume_read_buffer_at_offset(
	              volume,
	              buffer,
	              FVDE_TEST_VOLUME_READ_BUFFER_SIZE,
	              -1,
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

#if defined( HAVE_FVDE_TEST_RWLOCK )

	/* Test libfvde_volume_read_buffer_at_offset with pthread_rwlock_wrlock failing in libcthreads_read_write_lock_grab_for_write
	 */
	fvde_test_pthread_rwlock_wrlock_attempts_before_fail = 0;

	read_count = libfvde_volume_read_buffer_at_offset(
	              volume,
	              buffer,
	              FVDE_TEST_VOLUME_READ_BUFFER_SIZE,
	              0,
	              &error );

	if( fvde_test_pthread_rwlock_wrlock_attempts_before_fail != -1 )
	{
		fvde_test_pthread_rwlock_wrlock_attempts_before_fail = -1;
	}
	else
	{
		FVDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) -1 );

		FVDE_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Test libfvde_volume_read_buffer_at_offset with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_write
	 */
	fvde_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	read_count = libfvde_volume_read_buffer_at_offset(
	              volume,
	              buffer,
	              FVDE_TEST_VOLUME_READ_BUFFER_SIZE,
	              0,
	              &error );

	if( fvde_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		fvde_test_pthread_rwlock_unlock_attempts_before_fail = -1;
	}
	else
	{
		FVDE_TEST_ASSERT_EQUAL_SSIZE(
		 "read_count",
		 read_count,
		 (ssize_t) -1 );

		FVDE_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_FVDE_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfvde_volume_seek_offset function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_volume_seek_offset(
     libfvde_volume_t *volume )
{
	libcerror_error_t *error = NULL;
	size64_t size            = 0;
	off64_t offset           = 0;

	/* Test regular cases
	 */
	offset = libfvde_volume_seek_offset(
	          volume,
	          0,
	          SEEK_END,
	          &error );

	FVDE_TEST_ASSERT_NOT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	size = (size64_t) offset;

	offset = libfvde_volume_seek_offset(
	          volume,
	          1024,
	          SEEK_SET,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 1024 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	offset = libfvde_volume_seek_offset(
	          volume,
	          -512,
	          SEEK_CUR,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) 512 );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	offset = libfvde_volume_seek_offset(
	          volume,
	          (off64_t) ( size + 512 ),
	          SEEK_SET,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) ( size + 512 ) );

	FVDE_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Reset offset to 0
	 */
	offset = libfvde_volume_seek_offset(
	          volume,
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
	offset = libfvde_volume_seek_offset(
	          NULL,
	          0,
	          SEEK_SET,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	offset = libfvde_volume_seek_offset(
	          volume,
	          -1,
	          SEEK_SET,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	offset = libfvde_volume_seek_offset(
	          volume,
	          -1,
	          SEEK_CUR,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	offset = libfvde_volume_seek_offset(
	          volume,
	          (off64_t) ( -1 * ( size + 1 ) ),
	          SEEK_END,
	          &error );

	FVDE_TEST_ASSERT_EQUAL_INT64(
	 "offset",
	 offset,
	 (int64_t) -1 );

	FVDE_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_FVDE_TEST_RWLOCK )

	/* Test libfvde_volume_seek_offset with pthread_rwlock_wrlock failing in libcthreads_read_write_lock_grab_for_write
	 */
	fvde_test_pthread_rwlock_wrlock_attempts_before_fail = 0;

	offset = libfvde_volume_seek_offset(
	          volume,
	          0,
	          SEEK_SET,
	          &error );

	if( fvde_test_pthread_rwlock_wrlock_attempts_before_fail != -1 )
	{
		fvde_test_pthread_rwlock_wrlock_attempts_before_fail = -1;
	}
	else
	{
		FVDE_TEST_ASSERT_EQUAL_INT64(
		 "offset",
		 (int64_t) offset,
		 (int64_t) -1 );

		FVDE_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Test libfvde_volume_seek_offset with pthread_rwlock_unlock failing in libcthreads_read_write_lock_release_for_write
	 */
	fvde_test_pthread_rwlock_unlock_attempts_before_fail = 0;

	offset = libfvde_volume_seek_offset(
	          volume,
	          0,
	          SEEK_SET,
	          &error );

	if( fvde_test_pthread_rwlock_unlock_attempts_before_fail != -1 )
	{
		fvde_test_pthread_rwlock_unlock_attempts_before_fail = -1;
	}
	else
	{
		FVDE_TEST_ASSERT_EQUAL_INT64(
		 "offset",
		 (int64_t) offset,
		 (int64_t) -1 );

		FVDE_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_FVDE_TEST_RWLOCK ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libfvde_volume_get_offset function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_volume_get_offset(
     libfvde_volume_t *volume )
{
	libcerror_error_t *error = NULL;
	off64_t offset           = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libfvde_volume_get_offset(
	          volume,
	          &offset,
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
	result = libfvde_volume_get_offset(
	          NULL,
	          &offset,
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

	result = libfvde_volume_get_offset(
	          volume,
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

/* Tests the libfvde_volume_get_logical_volume_size function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_volume_get_logical_volume_size(
     libfvde_volume_t *volume )
{
	libcerror_error_t *error     = NULL;
	size64_t logical_volume_size = 0;
	int result                   = 0;

	/* Test regular cases
	 */
	result = libfvde_volume_get_logical_volume_size(
	          volume,
	          &logical_volume_size,
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
	result = libfvde_volume_get_logical_volume_size(
	          NULL,
	          &logical_volume_size,
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

	result = libfvde_volume_get_logical_volume_size(
	          volume,
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

/* Tests the libfvde_volume_get_logical_volume_encryption_method function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_volume_get_logical_volume_encryption_method(
     libfvde_volume_t *volume )
{
	libcerror_error_t *error                  = NULL;
	uint32_t logical_volume_encryption_method = 0;
	int result                                = 0;

	/* Test regular cases
	 */
	result = libfvde_volume_get_logical_volume_encryption_method(
	          volume,
	          &logical_volume_encryption_method,
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
	result = libfvde_volume_get_logical_volume_encryption_method(
	          NULL,
	          &logical_volume_encryption_method,
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

	result = libfvde_volume_get_logical_volume_encryption_method(
	          volume,
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

/* Tests the libfvde_volume_get_physical_volume_size function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_volume_get_physical_volume_size(
     libfvde_volume_t *volume )
{
	libcerror_error_t *error      = NULL;
	size64_t physical_volume_size = 0;
	int result                    = 0;

	/* Test regular cases
	 */
	result = libfvde_volume_get_physical_volume_size(
	          volume,
	          &physical_volume_size,
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
	result = libfvde_volume_get_physical_volume_size(
	          NULL,
	          &physical_volume_size,
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

	result = libfvde_volume_get_physical_volume_size(
	          volume,
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

/* Tests the libfvde_volume_get_physical_volume_encryption_method function
 * Returns 1 if successful or 0 if not
 */
int fvde_test_volume_get_physical_volume_encryption_method(
     libfvde_volume_t *volume )
{
	libcerror_error_t *error                   = NULL;
	uint32_t physical_volume_encryption_method = 0;
	int result                                 = 0;

	/* Test regular cases
	 */
	result = libfvde_volume_get_physical_volume_encryption_method(
	          volume,
	          &physical_volume_encryption_method,
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
	result = libfvde_volume_get_physical_volume_encryption_method(
	          NULL,
	          &physical_volume_encryption_method,
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

	result = libfvde_volume_get_physical_volume_encryption_method(
	          volume,
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
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc,
     wchar_t * const argv[] )
#else
int main(
     int argc,
     char * const argv[] )
#endif
{
	libbfio_handle_t *file_io_handle    = NULL;
	libcerror_error_t *error            = NULL;
	libfvde_volume_t *volume            = NULL;
	system_character_t *option_offset   = NULL;
	system_character_t *option_password = NULL;
	system_character_t *source          = NULL;
	system_integer_t option             = 0;
	size_t string_length                = 0;
	off64_t volume_offset               = 0;
	int result                          = 0;

	while( ( option = fvde_test_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "o:p:" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM ".\n",
				 argv[ optind - 1 ] );

				return( EXIT_FAILURE );

			case (system_integer_t) 'o':
				option_offset = optarg;

				break;

			case (system_integer_t) 'p':
				option_password = optarg;

				break;
		}
	}
	if( optind < argc )
	{
		source = argv[ optind ];
	}
	if( option_offset != NULL )
	{
		string_length = system_string_length(
		                 option_offset );

		result = fvde_test_system_string_copy_from_64_bit_in_decimal(
		          option_offset,
		          string_length + 1,
		          (uint64_t *) &volume_offset,
		          &error );

		FVDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        FVDE_TEST_ASSERT_IS_NULL(
	         "error",
	         error );
	}
#if defined( HAVE_DEBUG_OUTPUT ) && defined( FVDE_TEST_VOLUME_VERBOSE )
	libfvde_notify_set_verbose(
	 1 );
	libfvde_notify_set_stream(
	 stderr,
	 NULL );
#endif

	FVDE_TEST_RUN(
	 "libfvde_volume_initialize",
	 fvde_test_volume_initialize );

	FVDE_TEST_RUN(
	 "libfvde_volume_free",
	 fvde_test_volume_free );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )
	if( source != NULL )
	{
		result = libbfio_file_range_initialize(
		          &file_io_handle,
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

		string_length = system_string_length(
		                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbfio_file_range_set_name_wide(
		          file_io_handle,
		          source,
		          string_length,
		          &error );
#else
		result = libbfio_file_range_set_name(
		          file_io_handle,
		          source,
		          string_length,
		          &error );
#endif
		FVDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        FVDE_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

		result = libbfio_file_range_set(
		          file_io_handle,
		          volume_offset,
		          0,
		          &error );

		FVDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        FVDE_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

		result = libfvde_check_volume_signature_file_io_handle(
		          file_io_handle,
		          &error );

		FVDE_TEST_ASSERT_NOT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		FVDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	if( ( result != 0 )
	 && ( volume_offset == 0 ) )
	{
		FVDE_TEST_RUN_WITH_ARGS(
		 "libfvde_volume_open",
		 fvde_test_volume_open,
		 source,
		 option_password );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

		FVDE_TEST_RUN_WITH_ARGS(
		 "libfvde_volume_open_wide",
		 fvde_test_volume_open_wide,
		 source,
		 option_password );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

		FVDE_TEST_RUN_WITH_ARGS(
		 "libfvde_volume_open_file_io_handle",
		 fvde_test_volume_open_file_io_handle,
		 source,
		 option_password );

		FVDE_TEST_RUN(
		 "libfvde_volume_close",
		 fvde_test_volume_close );

		FVDE_TEST_RUN_WITH_ARGS(
		 "libfvde_volume_open_close",
		 fvde_test_volume_open_close,
		 source,
		 option_password );

	}
	if( result != 0 )
	{
		/* Initialize volume for tests
		 */
		result = fvde_test_volume_open_source(
		          &volume,
		          file_io_handle,
		          option_password,
		          &error );

		FVDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		FVDE_TEST_ASSERT_IS_NOT_NULL(
		 "volume",
		 volume );

		FVDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		FVDE_TEST_RUN_WITH_ARGS(
		 "libfvde_volume_signal_abort",
		 fvde_test_volume_signal_abort,
		 volume );

#if defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT )

		/* TODO: add tests for libfvde_internal_volume_open_read */

#endif /* defined( __GNUC__ ) && !defined( LIBFVDE_DLL_IMPORT ) */

		FVDE_TEST_RUN_WITH_ARGS(
		 "libfvde_volume_is_locked",
		 fvde_test_volume_is_locked,
		 volume );

		FVDE_TEST_RUN_WITH_ARGS(
		 "libfvde_volume_read_buffer",
		 fvde_test_volume_read_buffer,
		 volume );

		FVDE_TEST_RUN_WITH_ARGS(
		 "libfvde_volume_read_buffer_at_offset",
		 fvde_test_volume_read_buffer_at_offset,
		 volume );

		FVDE_TEST_RUN_WITH_ARGS(
		 "libfvde_volume_seek_offset",
		 fvde_test_volume_seek_offset,
		 volume );

		FVDE_TEST_RUN_WITH_ARGS(
		 "libfvde_volume_get_offset",
		 fvde_test_volume_get_offset,
		 volume );

		FVDE_TEST_RUN_WITH_ARGS(
		 "libfvde_volume_get_logical_volume_size",
		 fvde_test_volume_get_logical_volume_size,
		 volume );

		FVDE_TEST_RUN_WITH_ARGS(
		 "libfvde_volume_get_logical_volume_encryption_method",
		 fvde_test_volume_get_logical_volume_encryption_method,
		 volume );

		/* TODO: add tests for libfvde_volume_get_logical_volume_identifier */

		/* TODO: add tests for libfvde_volume_get_logical_volume_group_identifier */

		FVDE_TEST_RUN_WITH_ARGS(
		 "libfvde_volume_get_physical_volume_size",
		 fvde_test_volume_get_physical_volume_size,
		 volume );

		FVDE_TEST_RUN_WITH_ARGS(
		 "libfvde_volume_get_physical_volume_encryption_method",
		 fvde_test_volume_get_physical_volume_encryption_method,
		 volume );

		/* TODO: add tests for libfvde_volume_get_physical_volume_identifier */

		/* TODO: add tests for libfvde_volume_set_keys */

		/* TODO: add tests for libfvde_volume_set_utf8_password */

		/* TODO: add tests for libfvde_volume_set_utf16_password */

		/* TODO: add tests for libfvde_volume_set_utf8_recovery_password */

		/* TODO: add tests for libfvde_volume_set_utf16_recovery_password */

		/* TODO: add tests for libfvde_volume_read_encrypted_root_plist */

#if defined( HAVE_WIDE_CHARACTER_TYPE )

		/* TODO: add tests for libfvde_volume_read_encrypted_root_plist_wide */

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

		/* TODO: add tests for libfvde_volume_read_encrypted_root_plist_file_io_handle */

		/* Clean up
		 */
		result = fvde_test_volume_close_source(
		          &volume,
		          &error );

		FVDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 0 );

		FVDE_TEST_ASSERT_IS_NULL(
		 "volume",
		 volume );

		FVDE_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	if( file_io_handle != NULL )
	{
		result = libbfio_handle_free(
		          &file_io_handle,
		          &error );

		FVDE_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		FVDE_TEST_ASSERT_IS_NULL(
	         "file_io_handle",
	         file_io_handle );

	        FVDE_TEST_ASSERT_IS_NULL(
	         "error",
	         error );
	}
#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( volume != NULL )
	{
		libfvde_volume_free(
		 &volume,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

