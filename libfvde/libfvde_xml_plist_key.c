/*
 * The XML plist key functions
 *
 * Copyright (C) 2011-2015, Omar Choudary <choudary.omar@gmail.com>
 *                          Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>
#include <types.h>

#include <libxml/tree.h>

#include "libfvde_libcerror.h"
#include "libfvde_libcnotify.h"
#include "libfvde_xml_plist_key.h"

/* Creates an XML plist key
 * Make sure the value plist_key is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfvde_xml_plist_key_initialize(
    libfvde_xml_plist_key_t **plist_key,
    libcerror_error_t **error )
{
	static char *function = "libfvde_xml_plist_key_initialize";

	if( plist_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid plist key.",
		 function );

		return( -1 );
	}
	if( *plist_key != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid plist key value already set.",
		 function );

		return( -1 );
	}
	*plist_key = memory_allocate_structure(
	              libfvde_xml_plist_key_t );

	if( *plist_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create plist key.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *plist_key,
	     0,
	     sizeof( libfvde_xml_plist_key_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear plist key.",
		 function );
	
		goto on_error;
	}
	return( 1 );

on_error:
	if( *plist_key != NULL )
	{
		memory_free(
		 *plist_key );

		*plist_key = NULL;
	}
	return( -1 );
}

/* Frees an XML plist key
 * Returns 1 if successful or -1 on error
 */
int libfvde_xml_plist_key_free(
    libfvde_xml_plist_key_t **plist_key,
    libcerror_error_t **error )
{
	static char *function = "libfvde_xml_plist_key_free";

	if( plist_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid plist key.",
		 function );

		return( -1 );
	}
	if( *plist_key != NULL )
	{
		memory_free(
		 *plist_key );

		*plist_key = NULL;
	}
	return( 1 );
}


