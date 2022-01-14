/*
 * Error functions
 *
 * Copyright (C) 2011-2022, Omar Choudary <choudary.omar@gmail.com>,
 *                          Joachim Metz <joachim.metz@gmail.com>
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

#include "libfvde_error.h"
#include "libfvde_libcerror.h"

#if !defined( HAVE_LOCAL_LIBFVDE )

/* Free an error and its elements
 */
void libfvde_error_free(
      libfvde_error_t **error )
{
	libcerror_error_free(
	 (libcerror_error_t **) error );
}

/* Prints a descriptive string of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
int libfvde_error_fprint(
     libfvde_error_t *error,
     FILE *stream )
{
	int print_count = 0;

	print_count = libcerror_error_fprint(
	               (libcerror_error_t *) error,
	               stream );

	return( print_count );
}

/* Prints a descriptive string of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
int libfvde_error_sprint(
     libfvde_error_t *error,
     char *string,
     size_t size )
{
	int print_count = 0;

	print_count = libcerror_error_sprint(
	               (libcerror_error_t *) error,
	               string,
	               size );

	return( print_count );
}

/* Prints a backtrace of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
int libfvde_error_backtrace_fprint(
     libfvde_error_t *error,
      FILE *stream )
{
	int print_count = 0;

	print_count = libcerror_error_backtrace_fprint(
	               (libcerror_error_t *) error,
	               stream );

	return( print_count );
}

/* Prints a backtrace of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
int libfvde_error_backtrace_sprint(
     libfvde_error_t *error,
     char *string,
     size_t size )
{
	int print_count = 0;

	print_count = libcerror_error_backtrace_sprint(
	               (libcerror_error_t *) error,
	               string,
	               size );

	return( print_count );
}

#endif /* !defined( HAVE_LOCAL_LIBFVDE ) */

