/*
 * Segment descriptor functions
 *
 * Copyright (C) 2011-2022, Omar Choudary <choudary.omar@gmail.com>
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
#include <memory.h>
#include <types.h>

#include "libfvde_libcdata.h"
#include "libfvde_libcerror.h"
#include "libfvde_segment_descriptor.h"

/* Creates data area descriptor
 * Make sure the value segment_descriptor is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfvde_segment_descriptor_initialize(
     libfvde_segment_descriptor_t **segment_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfvde_segment_descriptor_initialize";

	if( segment_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data area descriptor.",
		 function );

		return( -1 );
	}
	if( *segment_descriptor != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data area descriptor value already set.",
		 function );

		return( -1 );
	}
	*segment_descriptor = memory_allocate_structure(
	                         libfvde_segment_descriptor_t );

	if( *segment_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to data area descriptor.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *segment_descriptor,
	     0,
	     sizeof( libfvde_segment_descriptor_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data area descriptor.",
		 function );

		memory_free(
		 *segment_descriptor );

		*segment_descriptor = NULL;

		return( -1 );
	}
	return( 1 );

on_error:
	if( *segment_descriptor != NULL )
	{
		memory_free(
		 *segment_descriptor );

		*segment_descriptor = NULL;
	}
	return( -1 );
}

/* Frees data area descriptor
 * Returns 1 if successful or -1 on error
 */
int libfvde_segment_descriptor_free(
     libfvde_segment_descriptor_t **segment_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfvde_segment_descriptor_free";

	if( segment_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data area descriptor.",
		 function );

		return( -1 );
	}
	if( *segment_descriptor != NULL )
	{
		memory_free(
		 *segment_descriptor );

		*segment_descriptor = NULL;
	}
	return( 1 );
}

/* Compares two resource node entries
 * Returns LIBCDATA_COMPARE_LESS, LIBCDATA_COMPARE_EQUAL, LIBCDATA_COMPARE_GREATER if successful or -1 on error
 */
int libfvde_segment_descriptor_compare(
     libfvde_segment_descriptor_t *first_segment_descriptor,
     libfvde_segment_descriptor_t *second_segment_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfvde_segment_descriptor_compare";

	if( first_segment_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid first resource node entry.",
		 function );

		return( -1 );
	}
	if( second_segment_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid second resource node entry.",
		 function );

		return( -1 );
	}
	if( first_segment_descriptor->logical_block_number < second_segment_descriptor->logical_block_number )
	{
		return( LIBCDATA_COMPARE_LESS );
	}
	else if( first_segment_descriptor->logical_block_number > second_segment_descriptor->logical_block_number )
	{
		return( LIBCDATA_COMPARE_GREATER );
	}
	return( LIBCDATA_COMPARE_EQUAL );
}

