/*
 * XML plist key functions
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

#if !defined( _LIBFVDE_XML_PLIST_KEY_H )
#define _LIBFVDE_XML_PLIST_KEY_H

#include <common.h>
#include <types.h>

#include "libfvde_libcerror.h"
#include "libfvde_xml_plist_tag.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfvde_xml_plist_key libfvde_xml_plist_key_t;

struct libfvde_xml_plist_key
{
	/* The XML plist key tag
	 */
	libfvde_xml_plist_tag_t *key_tag;

	/* The XML plist value tag
	 */
	libfvde_xml_plist_tag_t *value_tag;
};

int libfvde_xml_plist_key_initialize(
     libfvde_xml_plist_key_t **key,
     libfvde_xml_plist_tag_t *key_tag,
     libfvde_xml_plist_tag_t *value_tag,
     libcerror_error_t **error );

int libfvde_xml_plist_key_free(
     libfvde_xml_plist_key_t **key,
     libcerror_error_t **error );

int libfvde_xml_plist_key_is_array(
     libfvde_xml_plist_key_t *key,
     libcerror_error_t **error );

int libfvde_xml_plist_key_is_dict(
     libfvde_xml_plist_key_t *key,
     libcerror_error_t **error );

int libfvde_xml_plist_key_get_value_data(
     libfvde_xml_plist_key_t *key,
     uint8_t **data,
     size_t *data_size,
     libcerror_error_t **error );

int libfvde_xml_plist_key_get_value_integer(
     libfvde_xml_plist_key_t *key,
     uint64_t *value_64bit,
     libcerror_error_t **error );

int libfvde_xml_plist_key_get_value_string(
     libfvde_xml_plist_key_t *key,
     uint8_t **string,
     size_t *string_size,
     libcerror_error_t **error );

int libfvde_xml_plist_key_value_uuid_string_copy_to_byte_stream(
     libfvde_xml_plist_key_t *key,
     uint8_t *byte_stream,
     size_t byte_stream_size,
     libcerror_error_t **error );

int libfvde_xml_plist_key_get_array_number_of_entries(
     libfvde_xml_plist_key_t *key,
     int *number_of_entries,
     libcerror_error_t **error );

int libfvde_xml_plist_key_get_array_entry_by_index(
     libfvde_xml_plist_key_t *key,
     int array_entry_index,
     libfvde_xml_plist_key_t **array_entry,
     libcerror_error_t **error );

int libfvde_xml_plist_key_get_sub_key_by_utf8_name(
     libfvde_xml_plist_key_t *key,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfvde_xml_plist_key_t **sub_key,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVDE_XML_PLIST_KEY_H ) */

