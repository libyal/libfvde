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
 * Make sure the value key is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfvde_xml_plist_key_initialize(
    libfvde_xml_plist_key_t **key,
    xmlNode *xml_node,
    uint8_t is_root,
    libcerror_error_t **error )
{
	xmlChar *xml_content  = NULL;
	static char *function = "libfvde_xml_plist_key_initialize";

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
	if( xml_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid XML node.",
		 function );

		return( -1 );
	}
	if( is_root == 0 )
	{
		if( xmlStrcmp(
		     xml_node->name,
		     (const xmlChar *) "key" ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: invalid XML node - unsupported name.",
			 function );

			return( -1 );
		}
/* TODO support comment nodes in between key and value? */
		if( xml_node->next == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid XML node - missing next XML node.",
			 function );

			return( -1 );
		}
		xml_content = xmlNodeGetContent(
			       xml_node );

		if( xml_content == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid XML node - missing content.",
			 function );

			return( -1 );
		}
	}
	*key = memory_allocate_structure(
	        libfvde_xml_plist_key_t );

	if( *key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create key.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *key,
	     0,
	     sizeof( libfvde_xml_plist_key_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear key.",
		 function );
	
		goto on_error;
	}
	if( is_root == 0 )
	{
		( *key )->key_xml_node    = xml_node;
		( *key )->key_xml_content = xml_content;
		( *key )->value_xml_node  = xml_node->next;
	}
	else
	{
		( *key )->value_xml_node = xml_node;
	}
	return( 1 );

on_error:
	if( xml_content != NULL )
	{
		xmlFree(
		 xml_content );
	}
	if( *key != NULL )
	{
		memory_free(
		 *key );

		*key = NULL;
	}
	return( -1 );
}

/* Frees an XML plist key
 * Returns 1 if successful or -1 on error
 */
int libfvde_xml_plist_key_free(
    libfvde_xml_plist_key_t **key,
    libcerror_error_t **error )
{
	static char *function = "libfvde_xml_plist_key_free";

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
		/* The key_xml_node and value_xml_node are referenced and freed elsewhere */
		if( ( *key )->key_xml_content != NULL )
		{
			xmlFree(
			 ( *key )->key_xml_content );
		}
		memory_free(
		 *key );

		*key = NULL;
	}
	return( 1 );
}

/* Determines if the value is a dict
 * Returns 1 if the value is a dict, 0 if not or -1 on error
 */
int libfvde_xml_plist_key_is_dict(
     libfvde_xml_plist_key_t *key,
     libcerror_error_t **error )
{
	static char *function = "libfvde_xml_plist_key_is_dict";

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
	if( key->value_xml_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing value XML node.",
		 function );

		return( -1 );
	}
	if( xmlStrcmp(
	     key->value_xml_node->name,
	     (const xmlChar *) "dict" ) == 0 )
	{
		return( 1 );
	}
	return( 0 );
}

/* Retrieves the sub key for the specific UTF-8 encoded name
 * Returns 1 if successful, 0 if no such sub key or -1 on error
 */
int libfvde_xml_plist_key_get_sub_key_by_utf8_name(
     libfvde_xml_plist_key_t *key,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfvde_xml_plist_key_t **sub_key,
     libcerror_error_t **error )
{
	xmlChar *xml_content  = NULL;
	xmlNode *xml_node     = NULL;
	static char *function = "libfvde_xml_plist_key_get_sub_key_by_utf8_name";
	int result            = 0;

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
	if( key->value_xml_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing value XML node.",
		 function );

		return( -1 );
	}
	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( sub_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub key.",
		 function );

		return( -1 );
	}
	if( *sub_key != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid sub key value already set.",
		 function );

		return( -1 );
	}
	xml_node = key->value_xml_node->children;

	while( xml_node != NULL )
	{
		if( xmlStrcmp(
		     xml_node->name,
		     (const xmlChar *) "key" ) == 0 )
		{
			xml_content = xmlNodeGetContent(
			               xml_node );

			if( xml_content != NULL )
			{
				result = xmlStrcmp(
				          xml_content,
				          (const xmlChar *) utf8_string );

				xmlFree(
				 xml_content );

				if( result == 0 )
				{
					break;
				}
			}
		}
		xml_node = xml_node->next;
	}
	if( xml_node == NULL )
	{
		return( 0 );
	}
	if( libfvde_xml_plist_key_initialize(
	     sub_key,
	     xml_node,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sub key.",
		 function );

		return( -1 );
	}
	return( 1 );
}

