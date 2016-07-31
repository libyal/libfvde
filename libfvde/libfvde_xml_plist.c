/*
 * The XML plist functions
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
#include "libfvde_libcnotify.h"
#include "libfvde_xml_plist.h"
#include "libfvde_xml_plist_key.h"
#include "libfvde_xml_plist_parser.h"

extern int xml_plist_parser_parse_buffer(
            libfvde_xml_plist_t *xml_plist,
            const uint8_t *buffer,
            size_t buffer_size,
            libcerror_error_t **error );

/* Creates an XML plist
 * Make sure the value plist is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfvde_xml_plist_initialize(
    libfvde_xml_plist_t **plist,
    libcerror_error_t **error )
{
	static char *function = "libfvde_xml_plist_initialize";

	if( plist == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid plist.",
		 function );

		return( -1 );
	}
	if( *plist != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid plist value already set.",
		 function );

		return( -1 );
	}
	*plist = memory_allocate_structure(
	          libfvde_xml_plist_t );

	if( *plist == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create plist.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *plist,
	     0,
	     sizeof( libfvde_xml_plist_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear plist.",
		 function );
	
		goto on_error;
	}
	return( 1 );

on_error:
	if( *plist != NULL )
	{
		memory_free(
		 *plist );

		*plist = NULL;
	}
	return( -1 );
}

/* Frees an XML plist
 * Returns 1 if successful or -1 on error
 */
int libfvde_xml_plist_free(
    libfvde_xml_plist_t **plist,
    libcerror_error_t **error )
{
	static char *function = "libfvde_xml_plist_free";
	int result            = 1;

	if( plist == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid plist.",
		 function );

		return( -1 );
	}
	if( *plist != NULL )
	{
		/* The root_tag and dict_tag are referenced and freed elsewhere */

		if( libfvde_xml_plist_tag_free(
		     &( ( *plist )->root_tag ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free XML plist root tag.",
			 function );

			result = -1;
		}
		memory_free(
		 *plist );

		*plist = NULL;
	}
	return( result );
}

/* Copies the XML plist from the byte stream
 * Returns 1 if successful, 0 if not a valid plist or -1 on error
 */
int libfvde_xml_plist_copy_from_byte_stream(
     libfvde_xml_plist_t *plist,
     const uint8_t *byte_stream,
     size_t byte_stream_size,
     libcerror_error_t **error )
{
	uint8_t *buffer       = NULL;
	static char *function = "libfvde_xml_plist_copy_from_byte_stream";
	size_t buffer_size    = 0;
	int result            = 0;

	if( plist == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid plist.",
		 function );

		return( -1 );
	}
	if( plist->root_tag != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid plist - XML plist root tag already set.",
		 function );

		return( -1 );
	}
	if( byte_stream == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid byte stream.",
		 function );

		return( -1 );
	}
	if( byte_stream_size > (size_t) INT_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid byte stream size value out of bounds.",
		 function );

		return( -1 );
	}
	/* Lex wants 2 zero bytes at the end of the buffer
	 */
	buffer_size = byte_stream_size + 1;

	buffer = (uint8_t *) memory_allocate(
	                      sizeof( uint8_t ) * buffer_size );

	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create buffer.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     buffer,
	     byte_stream,
	     byte_stream_size - 1 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy byte stream.",
		 function );
	
		goto on_error;
	}
	/* Lex wants 2 zero bytes at the end of the buffer
	 */
	buffer[ buffer_size - 2 ] = 0;
	buffer[ buffer_size - 1 ] = 0;

	result = xml_plist_parser_parse_buffer(
	          plist,
	          buffer,
	          buffer_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to parse XML plist.",
		 function );

		goto on_error;
	}
	memory_free(
	 buffer );

	buffer = NULL;

	return( 1 );

on_error:
	if( buffer != NULL )
	{
		memory_free(
		 buffer );
	}
	return( -1 );
}

/* Retrieves the root key
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfvde_xml_plist_get_root_key(
     libfvde_xml_plist_t *plist,
     libfvde_xml_plist_key_t **key,
     libcerror_error_t **error )
{
	libfvde_xml_plist_tag_t *element_tag = NULL;
	static char *function                = "libfvde_xml_plist_get_root_key";
	int element_index                    = 0;
	int number_of_elements               = 0;
	int result                           = 0;

	if( plist == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid plist.",
		 function );

		return( -1 );
	}
	if( plist->root_tag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid plist - missing XML plist root tag.",
		 function );

		return( -1 );
	}
	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	if( *key != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid key value already set.",
		 function );

		return( -1 );
	}
	if( plist->root_tag == NULL )
	{
		return( 0 );
	}
	if( plist->dict_tag == NULL )
	{
		result = libfvde_xml_plist_tag_compare_name(
		          plist->root_tag,
		          (uint8_t *) "dict",
		          4,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to compare name of root tag.",
			 function );

			return( -1 );
		}
		else if( result != 0 )
		{
			plist->dict_tag = plist->root_tag;
		}
	}
	if( plist->dict_tag == NULL )
	{
		/* Ignore the plist XML node
		 * <plist version="1.0">
		 */
		result = libfvde_xml_plist_tag_compare_name(
		          plist->root_tag,
		          (uint8_t *) "plist",
		          5,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to compare name of root tag.",
			 function );

			return( -1 );
		}
		else if( result != 0 )
		{
			plist->plist_tag = plist->root_tag;

			if( libfvde_xml_plist_tag_get_number_of_elements(
			     plist->root_tag,
			     &number_of_elements,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve number of elements.",
				 function );

				return( -1 );
			}
			for( element_index = 0;
			     element_index < number_of_elements;
			     element_index++ )
			{
				if( libfvde_xml_plist_tag_get_element(
				     plist->root_tag,
				     element_index,
				     &element_tag,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve element: %d.",
					 function,
					 element_index );

					return( -1 );
				}
				result = libfvde_xml_plist_tag_compare_name(
				          element_tag,
				          (uint8_t *) "text",
				          4,
				          error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to compare name of element tag: %d.",
					 function,
					 element_index );

					return( -1 );
				}
				else if( result != 0 )
				{
					/* Ignore text nodes
					 */
					continue;
				}
				result = libfvde_xml_plist_tag_compare_name(
				          element_tag,
				          (uint8_t *) "dict",
				          4,
				          error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to compare name of element tag: %d.",
					 function,
					 element_index );

					return( -1 );
				}
				else if( result != 0 )
				{
					plist->dict_tag = plist->root_tag;
				}
				else
				{
					break;
				}
			}
		}
	}
	if( plist->dict_tag == NULL )
	{
		return( 0 );
	}
	if( libfvde_xml_plist_key_initialize(
	     key,
	     NULL,
	     plist->dict_tag,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create XML plist key.",
		 function );

		return( -1 );
	}
	return( 1 );
}

