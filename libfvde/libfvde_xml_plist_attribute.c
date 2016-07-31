/*
 * XML plist attribute functions
 *
 * Copyright (C) 2011-2016, Omar Choudary <choudary.omar@gmail.com>
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

#include "libfvde_libcerror.h"
#include "libfvde_xml_plist_attribute.h"

/* Creates a XML plist attribute
 * Make sure the value attribute is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfvde_xml_plist_attribute_initialize(
     libfvde_xml_plist_attribute_t **attribute,
     const uint8_t *name,
     size_t name_length,
     const uint8_t *value,
     size_t value_length,
     libcerror_error_t **error )
{
	static char *function = "libfvde_xml_plist_attribute_initialize";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid XML plist attribute.",
		 function );

		return( -1 );
	}
	if( *attribute != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid XML plist attribute value already set.",
		 function );

		return( -1 );
	}
	if( name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name.",
		 function );

		return( -1 );
	}
	if( name_length > (size_t) ( SSIZE_MAX - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid name length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	if( value_length > (size_t) ( SSIZE_MAX - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid value length value exceeds maximum.",
		 function );

		return( -1 );
	}
	*attribute = memory_allocate_structure(
	              libfvde_xml_plist_attribute_t );

	if( *attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create XML plist attribute.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *attribute,
	     0,
	     sizeof( libfvde_xml_plist_attribute_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear XML plist attribute.",
		 function );

		memory_free(
		 *attribute );

		*attribute = NULL;

		return( -1 );
	}
	( *attribute )->name_size = name_length + 1;

	( *attribute )->name = (uint8_t *) memory_allocate(
	                                    sizeof( uint8_t ) * ( *attribute )->name_size );

	if( ( *attribute )->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create name.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     ( *attribute )->name,
	     name,
	     name_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy name.",
		 function );

		goto on_error;
	}
	( *attribute )->name[ name_length ] = 0;

	( *attribute )->value_size = value_length + 1;

	( *attribute )->value = (uint8_t *) memory_allocate(
	                                     sizeof( uint8_t ) * ( *attribute )->value_size );

	if( ( *attribute )->value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create value.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     ( *attribute )->value,
	     value,
	     value_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy value.",
		 function );

		goto on_error;
	}
	( *attribute )->value[ value_length ] = 0;

	return( 1 );

on_error:
	if( *attribute != NULL )
	{
		if( ( *attribute )->value != NULL )
		{
			memory_free(
			 ( *attribute )->value );
		}
		if( ( *attribute )->name != NULL )
		{
			memory_free(
			 ( *attribute )->name );
		}
		memory_free(
		 *attribute );

		*attribute = NULL;
	}
	return( -1 );
}

/* Frees a XML plist attribute
 * Returns 1 if successful or -1 on error
 */
int libfvde_xml_plist_attribute_free(
     libfvde_xml_plist_attribute_t **attribute,
     libcerror_error_t **error )
{
	static char *function = "libfvde_xml_plist_attribute_free";

	if( attribute == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid XML plist attribute.",
		 function );

		return( -1 );
	}
	if( *attribute != NULL )
	{
		if( ( *attribute )->value != NULL )
		{
			memory_free(
			 ( *attribute )->value );
		}
		if( ( *attribute )->name != NULL )
		{
			memory_free(
			 ( *attribute )->name );
		}
		memory_free(
		 *attribute );

		*attribute = NULL;
	}
	return( 1 );
}

