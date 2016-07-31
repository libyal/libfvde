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

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlversion.h>

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
		/* The plist_xml_node, root_xml_node and dict_xml_node are referenced and freed elsewhere */
		if( ( *plist )->xml_document != NULL )
		{
			xmlFreeDoc(
			 ( *plist )->xml_document );
		}
		memory_free(
		 *plist );

		*plist = NULL;
	}
	/* Cleanup memory allocated by libxml
	*/
	xmlCleanupParser();

	return( 1 );
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
	if( plist->xml_document != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid plist - XML document already set.",
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

	/* Get the root node
	 */
	plist->root_xml_node = xmlDocGetRootElement(
	                        plist->xml_document );

	if( plist->root_xml_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve root XML node.",
		 function );

		goto on_error;
	}
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
	xmlNode *xml_node     = NULL;
	static char *function = "libfvde_xml_plist_get_root_key";

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
	if( plist->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid plist - missing XML document.",
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
	if( plist->root_xml_node == NULL )
	{
		return( 0 );
	}
	if( plist->dict_xml_node == NULL )
	{
		if( xmlStrcmp(
		     plist->root_xml_node->name,
		     (const xmlChar *) "dict" ) == 0 )
		{
			plist->dict_xml_node = plist->root_xml_node;
		}
		/* Ignore the plist XML node
		 * <plist version="1.0">
		 */
		else if( xmlStrcmp(
		          plist->root_xml_node->name,
		          (const xmlChar *) "plist" ) == 0 )
		{
			/* TODO: determine plist version
			 */
			plist->plist_xml_node = xml_node;

			xml_node = plist->root_xml_node->children;

			/* Ignore text nodes
			 */
			while( xml_node != NULL )
			{
				if( xmlStrcmp(
				     xml_node->name,
				     (const xmlChar *) "text" ) != 0 )
				{
					break;
				}
				xml_node = xml_node->next;
			}
			if( ( xml_node != NULL )
			 && ( xmlStrcmp(
			       xml_node->name,
			       (const xmlChar *) "dict" ) == 0 ) )
			{
				plist->dict_xml_node = xml_node;
			}
		}
	}
	if( plist->dict_xml_node == NULL )
	{
		return( 0 );
	}
	if( libfvde_xml_plist_key_initialize(
	     key,
	     plist->dict_xml_node,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create key.",
		 function );

		return( -1 );
	}
	return( 1 );
}

