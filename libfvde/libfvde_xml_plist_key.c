/*
 * The XML plist key functions
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

#include <libxml/tree.h>

#include "libfvde_libcerror.h"
#include "libfvde_libcnotify.h"
#include "libfvde_libfguid.h"
#include "libfvde_libfvalue.h"
#include "libfvde_libuna.h"
#include "libfvde_xml_plist_key.h"

/* Creates an XML plist key
 * Make sure the value key is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfvde_xml_plist_key_initialize(
    libfvde_xml_plist_key_t **key,
    xmlNode *xml_node,
    uint8_t has_name,
    libcerror_error_t **error )
{
	xmlChar *xml_content    = NULL;
	xmlNode *value_xml_node = NULL;
	static char *function   = "libfvde_xml_plist_key_initialize";

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
	if( has_name == 0 )
	{
		value_xml_node = xml_node;
	}
	else
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

			goto on_error;
		}
		value_xml_node = xml_node->next;

		/* Ignore text nodes
		 */
		while( value_xml_node != NULL )
		{
			if( xmlStrcmp(
			     value_xml_node->name,
			     (const xmlChar *) "text" ) != 0 )
			{
				break;
			}
			value_xml_node = value_xml_node->next;
		}
		if( value_xml_node == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid XML node - missing value XML node.",
			 function );

			goto on_error;
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

			goto on_error;
		}
/* TODO add ID support ? */
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
	if( has_name != 0 )
	{
		( *key )->key_xml_node    = xml_node;
		( *key )->key_xml_content = xml_content;
	}
	( *key )->value_xml_node = value_xml_node;

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

/* Determines if the value is an array
 * Returns 1 if the value isan array, 0 if not or -1 on error
 */
int libfvde_xml_plist_key_is_array(
     libfvde_xml_plist_key_t *key,
     libcerror_error_t **error )
{
	static char *function = "libfvde_xml_plist_key_is_array";

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
	     (const xmlChar *) "array" ) == 0 )
	{
		return( 1 );
	}
	return( 0 );
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

/* Retrieves a data value
 * Returns 1 if successful or -1 on error
 */
int libfvde_xml_plist_key_get_value_data(
     libfvde_xml_plist_key_t *key,
     uint8_t **data,
     size_t *data_size,
     libcerror_error_t **error )
{
	xmlChar *xml_content      = NULL;
	static char *function     = "libfvde_xml_plist_key_get_value_data";
	size_t xml_content_length = 0;
	size_t xml_content_index  = 0;

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
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( *data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data value already set.",
		 function );

		return( -1 );
	}
	if( data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data size.",
		 function );

		return( -1 );
	}
	if( xmlStrcmp(
	     key->value_xml_node->name,
	     (const xmlChar *) "data" ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid key - invalid value XML node - value not a data.",
		 function );

		goto on_error;
	}
	xml_content = xmlNodeGetContent(
		       key->value_xml_node );

	if( xml_content == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: invalid key - invalid value XML node - unable to retrieve content data.",
		 function );

		goto on_error;
	}
	xml_content_length = libcstring_narrow_string_length(
			      (char *) xml_content );

	/* The base64 conversion function doesn't like an empty first line
	 */
	if( xml_content[ 0 ] == '\n' )
	{
		xml_content_index  += 1;
		xml_content_length -= 1;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: base64 encoded data:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) &( xml_content[ xml_content_index ] ),
		 xml_content_length,
		 0 );
	}
#endif
	if( libuna_base64_stream_size_to_byte_stream(
	     &( xml_content[ xml_content_index ] ),
	     xml_content_length,
	     data_size,
	     LIBUNA_BASE64_VARIANT_ALPHABET_NORMAL | LIBUNA_BASE64_VARIANT_CHARACTER_LIMIT_NONE | LIBUNA_BASE64_VARIANT_PADDING_REQUIRED,
	     LIBUNA_BASE64_FLAG_STRIP_WHITESPACE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to determine size of base64 encoded data.",
		 function );

		goto on_error;
	}
	*data = (uint8_t *) memory_allocate(
	                     sizeof( uint8_t ) * *data_size );

	if( *data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data.",
		 function );

		goto on_error;
	}
	if( libuna_base64_stream_copy_to_byte_stream(
	     &( xml_content[ xml_content_index ] ),
	     xml_content_length,
	     *data,
	     *data_size,
	     LIBUNA_BASE64_VARIANT_ALPHABET_NORMAL | LIBUNA_BASE64_VARIANT_CHARACTER_LIMIT_NONE | LIBUNA_BASE64_VARIANT_PADDING_REQUIRED,
	     LIBUNA_BASE64_FLAG_STRIP_WHITESPACE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy base64 encoded data to byte stream.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( xml_content != NULL )
	{
		xmlFree(
		 xml_content );
	}
	if( *data != NULL )
	{
		memory_free(
		 *data );

		*data = NULL;
	}
	*data_size = 0;

	return( -1 );
}

/* Retrieves an integer value
 * Returns 1 if successful or -1 on error
 */
int libfvde_xml_plist_key_get_value_integer(
     libfvde_xml_plist_key_t *key,
     uint64_t *value_64bit,
     libcerror_error_t **error )
{
	xmlChar *xml_content      = NULL;
	static char *function     = "libfvde_xml_plist_key_get_value_integer";
	size_t xml_content_length = 0;

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
	     (const xmlChar *) "integer" ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid key - invalid value XML node - value not an integer.",
		 function );

		goto on_error;
	}
/* TODO add size support ? */
	xml_content = xmlNodeGetContent(
		       key->value_xml_node );

	if( xml_content == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: invalid key - invalid value XML node - unable to retrieve content string.",
		 function );

		goto on_error;
	}
	xml_content_length = libcstring_narrow_string_length(
			      (char *) xml_content );

	if( libfvalue_utf8_string_copy_to_integer(
	     xml_content,
	     xml_content_length,
	     (uint64_t *) value_64bit,
	     64,
	     LIBFVALUE_INTEGER_FORMAT_TYPE_HEXADECIMAL | LIBFVALUE_INTEGER_FORMAT_FLAG_UNSIGNED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to content string to integer.",
		 function );

		goto on_error;
	}
	xmlFree(
	 xml_content );

	xml_content = NULL;

	return( 1 );

on_error:
	if( xml_content != NULL )
	{
		xmlFree(
		 xml_content );
	}
	return( -1 );
}

/* Retrieves a string value
 * Returns 1 if successful or -1 on error
 */
int libfvde_xml_plist_key_get_value_string(
     libfvde_xml_plist_key_t *key,
     uint8_t **string,
     size_t *string_size,
     libcerror_error_t **error )
{
	xmlChar *xml_content      = NULL;
	static char *function     = "libfvde_xml_plist_key_get_value_string";
	size_t xml_content_length = 0;

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
	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( *string != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid string value already set.",
		 function );

		return( -1 );
	}
	if( string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string size.",
		 function );

		return( -1 );
	}
	if( xmlStrcmp(
	     key->value_xml_node->name,
	     (const xmlChar *) "string" ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid key - invalid value XML node - value not a string.",
		 function );

		goto on_error;
	}
	xml_content = xmlNodeGetContent(
		       key->value_xml_node );

	if( xml_content == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: invalid key - invalid value XML node - unable to retrieve content string.",
		 function );

		goto on_error;
	}
	xml_content_length = libcstring_narrow_string_length(
			      (char *) xml_content );

	*string_size = xml_content_length + 1;

	*string = memory_allocate(
	           sizeof( uint8_t ) * *string_size );

	if( *string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create string.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     *string,
	     xml_content,
	     xml_content_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy string.",
		 function );

		goto on_error;
	}
	( *string )[ xml_content_length ] = 0;

	return( 1 );

on_error:
	if( xml_content != NULL )
	{
		xmlFree(
		 xml_content );
	}
	if( *string != NULL )
	{
		memory_free(
		 *string );

		*string = NULL;
	}
	*string_size = 0;

	return( -1 );
}

/* Copies an UUID string value to a byte stream
 * Returns 1 if successful or -1 on error
 */
int libfvde_xml_plist_key_value_uuid_string_copy_to_byte_stream(
     libfvde_xml_plist_key_t *key,
     uint8_t *byte_stream,
     size_t byte_stream_size,
     libcerror_error_t **error )
{
	libfguid_identifier_t *guid = NULL;
	uint8_t *string             = NULL;
	static char *function       = "libfvde_xml_plist_key_value_uuid_string_copy_to_byte_stream";
	size_t string_size          = 0;
	int result                  = 0;

	if( libfvde_xml_plist_key_get_value_string(
	     key,
	     &string,
	     &string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve logical volume family identifier.",
		 function );

		goto on_error;
	}
	if( string_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid string size value out of bounds.",
		 function );

		return( -1 );
	}
	if( libfguid_identifier_initialize(
	     &guid,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create GUID.",
		 function );

		goto on_error;
	}
	result = libfguid_identifier_copy_from_utf8_string(
		  guid,
		  (uint8_t *) string,
		  string_size - 1,
		  LIBFGUID_STRING_FORMAT_FLAG_USE_MIXED_CASE,
		  error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy GUID from string.",
		 function );

		goto on_error;
	}
	memory_free(
	 string );

	string = NULL;

	if( libfguid_identifier_copy_to_byte_stream(
	     guid,
	     byte_stream,
	     byte_stream_size,
	     LIBFGUID_ENDIAN_BIG,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to copy GUID to byte stream.",
		 function );

		goto on_error;
	}
	if( libfguid_identifier_free(
	     &guid,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free GUID.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( string != NULL )
	{
		memory_free(
		 string );
	}
#endif
	if( guid != NULL )
	{
		libfguid_identifier_free(
		 &guid,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the number of array entries
 * Returns 1 if successful or -1 on error
 */
int libfvde_xml_plist_key_get_array_number_of_entries(
     libfvde_xml_plist_key_t *key,
     int *number_of_entries,
     libcerror_error_t **error )
{
	xmlNode *xml_node     = NULL;
	static char *function = "libfvde_xml_plist_key_get_array_number_of_entries";
	int number_of_nodes   = 0;

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
	if( number_of_entries == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of entries.",
		 function );

		return( -1 );
	}
	if( xmlStrcmp(
	     key->value_xml_node->name,
	     (const xmlChar *) "array" ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid key - invalid value XML node - unsupported type.",
		 function );

		return( -1 );
	}
	xml_node = key->value_xml_node->children;

	while( xml_node != NULL )
	{
		/* Ignore text nodes
		 */
		if( xmlStrcmp(
		     xml_node->name,
		     (const xmlChar *) "text" ) != 0 )
		{
			number_of_nodes++;
		}
		xml_node = xml_node->next;
	}
	*number_of_entries = number_of_nodes;

	return( 1 );
}

/* Retrieves a specific array entry
 * Returns 1 if successful or -1 on error
 */
int libfvde_xml_plist_key_get_array_entry_by_index(
     libfvde_xml_plist_key_t *key,
     int array_entry_index,
     libfvde_xml_plist_key_t **array_entry,
     libcerror_error_t **error )
{
	xmlNode *xml_node     = NULL;
	static char *function = "libfvde_xml_plist_key_get_array_entry_by_index";

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
	if( array_entry_index < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid array entry index value out of bounds.",
		 function );

		return( -1 );
	}
	if( array_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid array entry.",
		 function );

		return( -1 );
	}
	if( *array_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid array entry value already set.",
		 function );

		return( -1 );
	}
	if( xmlStrcmp(
	     key->value_xml_node->name,
	     (const xmlChar *) "array" ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid key - invalid value XML node - unsupported type.",
		 function );

		return( -1 );
	}
	xml_node = key->value_xml_node->children;

	while( xml_node != NULL )
	{
		/* Ignore text nodes
		 */
		if( xmlStrcmp(
		     xml_node->name,
		     (const xmlChar *) "text" ) != 0 )
		{
			if( array_entry_index == 0 )
			{
				break;
			}
			array_entry_index--;
		}
		xml_node = xml_node->next;
	}
	if( xml_node == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid array entry index value out of bounds.",
		 function );

		return( -1 );
	}
	if( libfvde_xml_plist_key_initialize(
	     array_entry,
	     xml_node,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create array entry.",
		 function );

		return( -1 );
	}
	return( 1 );
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
	if( xmlStrcmp(
	     key->value_xml_node->name,
	     (const xmlChar *) "dict" ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid key - invalid value XML node - unsupported type.",
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
	     1,
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

