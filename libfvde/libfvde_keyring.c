/*
 * Keyring functions
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

#include "libfvde_keyring.h"
#include "libfvde_libcerror.h"

/* Creates a keyring
 * Make sure the value keyring is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfvde_keyring_initialize(
     libfvde_keyring_t **keyring,
     libcerror_error_t **error )
{
	static char *function = "libfvde_keyring_initialize";

	if( keyring == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid keyring.",
		 function );

		return( -1 );
	}
	if( *keyring != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid keyring value already set.",
		 function );

		return( -1 );
	}
	*keyring = memory_allocate_structure(
	            libfvde_keyring_t );

	if( *keyring == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create keyring.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *keyring,
	     0,
	     sizeof( libfvde_keyring_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear keyring.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *keyring != NULL )
	{
		memory_free(
		 *keyring );

		*keyring = NULL;
	}
	return( -1 );
}

/* Frees a  keyring
 * Returns 1 if successful or -1 on error
 */
int libfvde_keyring_free(
     libfvde_keyring_t **keyring,
     libcerror_error_t **error )
{
	static char *function = "libfvde_keyring_free";
	int result            = 1;

	if( keyring == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid keyring.",
		 function );

		return( -1 );
	}
	if( *keyring != NULL )
	{
		if( memory_set(
		     *keyring,
		     0,
		     sizeof( libfvde_keyring_t ) ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear keyring.",
			 function );

			result = -1;
		}
		memory_free(
		 *keyring );

		*keyring = NULL;
	}
	return( result );
}

