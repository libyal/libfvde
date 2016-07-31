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

#include "libfvde_libcerror.h"
#include "libfvde_libcnotify.h"
#include "libfvde_libfguid.h"
#include "libfvde_libfvalue.h"
#include "libfvde_libuna.h"
#include "libfvde_xml_plist_key.h"
#include "libfvde_xml_plist_tag.h"

/* Creates an XML plist key
 * Make sure the value key is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfvde_xml_plist_key_initialize(
     libfvde_xml_plist_key_t **key,
     libfvde_xml_plist_tag_t *key_tag,
     libfvde_xml_plist_tag_t *value_tag,
     libcerror_error_t **error )
{
	static char *function = "libfvde_xml_plist_key_initialize";
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
	if( value_tag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid XML plist value tag.",
		 function );

		return( -1 );
	}
	if( key_tag != NULL )
	{
		result = libfvde_xml_plist_tag_compare_name(
		          key_tag,
		          (uint8_t *) "key",
		          3,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to compare name of key tag.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: unsupported key tag: %s.",
			 function,
			 key_tag->name );

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
	( *key )->key_tag   = key_tag;
	( *key )->value_tag = value_tag;

	return( 1 );

on_error:
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
		/* The key_tag and value_tag are referenced and freed elsewhere */

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
	if( key->value_tag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing value XML plist value tag.",
		 function );

		return( -1 );
	}
	result = libfvde_xml_plist_tag_compare_name(
	          key->value_tag,
	          (uint8_t *) "array",
	          5,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to compare name of value tag.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Determines if the value is a dict
 * Returns 1 if the value is a dict, 0 if not or -1 on error
 */
int libfvde_xml_plist_key_is_dict(
     libfvde_xml_plist_key_t *key,
     libcerror_error_t **error )
{
	static char *function = "libfvde_xml_plist_key_is_dict";
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
	if( key->value_tag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing value XML plist value tag.",
		 function );

		return( -1 );
	}
	result = libfvde_xml_plist_tag_compare_name(
	          key->value_tag,
	          (uint8_t *) "dict",
	          4,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to compare name of value tag.",
		 function );

		return( -1 );
	}
	return( result );
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
	static char *function = "libfvde_xml_plist_key_get_value_data";
	size_t value_index    = 0;
	size_t value_length   = 0;
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
	if( key->value_tag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing value XML plist value tag.",
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
	result = libfvde_xml_plist_tag_compare_name(
	          key->value_tag,
	          (uint8_t *) "data",
	          4,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to compare name of value tag.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: unsupported value tag: %s.",
		 function,
		 key->value_tag->name );

		return( -1 );
	}
	value_length = key->value_tag->value_size - 1;

	/* The base64 conversion function doesn't like an empty first line
	 */
	if( ( key->value_tag->value )[ 0 ] == '\n' )
	{
		value_index  += 1;
		value_length -= 1;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: base64 encoded data:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) &( ( key->value_tag->value )[ value_index ] ),
		 value_length,
		 0 );
	}
#endif
	if( libuna_base64_stream_size_to_byte_stream(
	     &( ( key->value_tag->value )[ value_index ] ),
	     value_length,
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
	     &( ( key->value_tag->value )[ value_index ] ),
	     value_length,
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
	static char *function = "libfvde_xml_plist_key_get_value_integer";
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
	if( key->value_tag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing value XML plist value tag.",
		 function );

		return( -1 );
	}
	result = libfvde_xml_plist_tag_compare_name(
	          key->value_tag,
	          (uint8_t *) "integer",
	          7,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to compare name of value tag.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: unsupported value tag: %s.",
		 function,
		 key->value_tag->name );

		return( -1 );
	}
/* TODO add size support ? */
	if( libfvalue_utf8_string_copy_to_integer(
	     key->value_tag->value,
	     key->value_tag->value_size - 1,
	     (uint64_t *) value_64bit,
	     64,
	     LIBFVALUE_INTEGER_FORMAT_TYPE_HEXADECIMAL | LIBFVALUE_INTEGER_FORMAT_FLAG_UNSIGNED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to convert value to integer.",
		 function );

		return( -1 );
	}
	return( 1 );
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
	static char *function = "libfvde_xml_plist_key_get_value_string";
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
	if( key->value_tag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing value XML plist value tag.",
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
	result = libfvde_xml_plist_tag_compare_name(
	          key->value_tag,
	          (uint8_t *) "string",
	          6,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to compare name of value tag.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: unsupported value tag: %s.",
		 function,
		 key->value_tag->name );

		return( -1 );
	}
	*string_size = key->value_tag->value_size;

	*string = memory_allocate(
	           sizeof( uint8_t ) * key->value_tag->value_size );

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
	     key->value_tag->value,
	     key->value_tag->value_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy string.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
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
	libfvde_xml_plist_tag_t *element_tag = NULL;
	static char *function                = "libfvde_xml_plist_key_get_array_number_of_entries";
	int element_index                    = 0;
	int number_of_elements               = 0;
	int number_of_nodes                  = 0;
	int result                           = 0;

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
	if( key->value_tag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing value XML plist value tag.",
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
	result = libfvde_xml_plist_tag_compare_name(
	          key->value_tag,
	          (uint8_t *) "array",
	          5,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to compare name of value tag.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: unsupported value tag: %s.",
		 function,
		 key->value_tag->name );

		return( -1 );
	}
	if( libfvde_xml_plist_tag_get_number_of_elements(
	     key->value_tag,
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
		     key->value_tag,
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
		/* Ignore text nodes
		 */
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
			 "%s: unable to compare name of value tag.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			number_of_nodes++;
		}
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
	libfvde_xml_plist_tag_t *value_tag = NULL;
	static char *function              = "libfvde_xml_plist_key_get_array_entry_by_index";
	int entry_index                    = 0;
	int element_index                  = 0;
	int number_of_elements             = 0;
	int result                         = 0;

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
	if( key->value_tag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing value XML plist value tag.",
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
	result = libfvde_xml_plist_tag_compare_name(
	          key->value_tag,
	          (uint8_t *) "array",
	          5,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to compare name of value tag.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: unsupported value tag: %s.",
		 function,
		 key->value_tag->name );

		return( -1 );
	}
	if( libfvde_xml_plist_tag_get_number_of_elements(
	     key->value_tag,
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
	entry_index = array_entry_index;

	element_index = 0;

	while( element_index < number_of_elements )
	{
		if( libfvde_xml_plist_tag_get_element(
		     key->value_tag,
		     element_index,
		     &value_tag,
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
		/* Ignore text nodes
		 */
		result = libfvde_xml_plist_tag_compare_name(
		          value_tag,
		          (uint8_t *) "text",
		          4,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to compare name of key tag.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			if( entry_index == 0 )
			{
				break;
			}
			entry_index--;
		}
		element_index++;
	}
	if( element_index >= number_of_elements )
	{
		return( 0 );
	}
	if( libfvde_xml_plist_key_initialize(
	     array_entry,
	     NULL,
	     value_tag,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create array entry: %d.",
		 function,
		 array_entry_index );

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
	libfvde_xml_plist_tag_t *key_tag   = NULL;
	libfvde_xml_plist_tag_t *value_tag = NULL;
	static char *function              = "libfvde_xml_plist_key_get_sub_key_by_utf8_name";
	int element_index                  = 0;
	int number_of_elements             = 0;
	int result                         = 0;

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
	if( key->value_tag == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing value XML plist value tag.",
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
	result = libfvde_xml_plist_tag_compare_name(
	          key->value_tag,
	          (uint8_t *) "dict",
	          4,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to compare name of value tag.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: unsupported value tag: %s.",
		 function,
		 key->value_tag->name );

		return( -1 );
	}
	if( libfvde_xml_plist_tag_get_number_of_elements(
	     key->value_tag,
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
	element_index = 0;

	while( element_index < number_of_elements )
	{
		if( libfvde_xml_plist_tag_get_element(
		     key->value_tag,
		     element_index,
		     &key_tag,
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
		          key_tag,
		          (uint8_t *) "key",
		          3,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to compare name of key tag.",
			 function );

			return( -1 );
		}
		else if( result != 0 )
		{
			if( ( key_tag->value_size == ( utf8_string_length + 1 ) )
			 && libcstring_narrow_string_compare(
			     key_tag->value,
			     utf8_string,
			     utf8_string_length ) == 0 )
			{
				break;
			}
		}
		element_index++;
	}
	if( element_index >= number_of_elements )
	{
		return( 0 );
	}
	element_index++;

	while( element_index < number_of_elements )
	{
		if( libfvde_xml_plist_tag_get_element(
		     key->value_tag,
		     element_index,
		     &value_tag,
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
		/* Ignore text nodes
		 */
		result = libfvde_xml_plist_tag_compare_name(
		          value_tag,
		          (uint8_t *) "text",
		          4,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to compare name of value tag.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			break;
		}
		element_index++;
	}
	if( element_index >= number_of_elements )
	{
		return( 0 );
	}
	if( libfvde_xml_plist_key_initialize(
	     sub_key,
	     key_tag,
	     value_tag,
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

