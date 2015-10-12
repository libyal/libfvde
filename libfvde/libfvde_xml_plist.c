/*
 * The XML plist functions
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

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlversion.h>

#include "libfvde_libcerror.h"
#include "libfvde_libcnotify.h"
#include "libfvde_xml_plist.h"
#include "libfvde_xml_plist_key.h"

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
	static char *function = "libfvde_xml_plist_copy_from_byte_stream";

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
	plist->xml_document = xmlReadMemory(
			       (char *) byte_stream,
			       (int) byte_stream_size,
			       "plist.xml",
			       NULL,
			       0 );

	if( plist->xml_document == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create XML document.",
		 function );

		goto on_error;
	}
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
	if( plist->xml_document != NULL )
	{
		xmlFreeDoc(
		 plist->xml_document );

		plist->xml_document = NULL;
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
	if( plist->dict_xml_node == NULL )
	{
		/* Find the main dict XML node
		 */
		if( plist->root_xml_node != NULL )
		{
			xml_node = plist->root_xml_node->children;

			/* Skip the plist XML node
			 * <plist version="1.0">
			 */
			if( ( xml_node != NULL )
			 && ( xmlStrcmp(
			       xml_node->name,
			       (const xmlChar *) "plist" ) == 0 ) )
			{
				/* TODO: determine version
				 */
				xml_node = xml_node->next;
			}
			/* TODO: check for comment in between plist and dict XML nodes?
			 */
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
	     1,
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

