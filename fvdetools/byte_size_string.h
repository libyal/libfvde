/*
 * Byte size string functions
 *
 * Copyright (C) 2011-2022, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _BYTE_SIZE_STRING_H )
#define _BYTE_SIZE_STRING_H

#include <common.h>
#include <types.h>

#include "fvdetools_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum BYTE_SIZE_STRING_UNITS
{
	BYTE_SIZE_STRING_UNIT_MEGABYTE	= 1000,
	BYTE_SIZE_STRING_UNIT_MEBIBYTE	= 1024
};

int byte_size_string_create(
     system_character_t *byte_size_string,
     size_t byte_size_string_length,
     uint64_t size,
     int units,
     libcerror_error_t **error );

int byte_size_string_create_with_decimal_point(
     system_character_t *byte_size_string,
     size_t byte_size_string_length,
     uint64_t size,
     int units,
     int decimal_point,
     libcerror_error_t **error );

int byte_size_string_convert(
     const system_character_t *byte_size_string,
     size_t byte_size_string_length,
     uint64_t *size,
     libcerror_error_t **error );

int byte_size_string_convert_with_decimal_point(
     const system_character_t *byte_size_string,
     size_t byte_size_string_length,
     int decimal_point,
     uint64_t *size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _BYTE_SIZE_STRING_H ) */

