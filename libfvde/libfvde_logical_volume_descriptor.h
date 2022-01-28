/*
 * Logical volume descriptor functions
 *
 * Copyright (C) 2011-2022, Omar Choudary <choudary.omar@gmail.com>
 *                          Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFVDE_LOGICAL_VOLUME_DESCRIPTOR_H )
#define _LIBFVDE_LOGICAL_VOLUME_DESCRIPTOR_H

#include <common.h>
#include <types.h>

#include "libfvde_libcdata.h"
#include "libfvde_libcerror.h"
#include "libfvde_segment_descriptor.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfvde_logical_volume_descriptor libfvde_logical_volume_descriptor_t;

struct libfvde_logical_volume_descriptor
{
	/* The object identifier of the corresponding metadata block 0x001a
	 */
	uint64_t object_identifier;

	/* The identifier
	 */
	uint8_t identifier[ 16 ];

	/* The family identifier
	 */
	uint8_t family_identifier[ 16 ];

	/* The name
	 */
	uint8_t *name;

	/* The name size
	 */
	size_t name_size;

	/* The size
	 */
	size64_t size;

	/* The object identifier of the corresponding metadata block 0x0305
	 */
	uint64_t object_identifier_0x0305;

	/* The object identifier of the corresponding metadata block 0x0505
	 */
	uint64_t object_identifier_0x0505;

	/* The base physical block number
	 */
	uint64_t base_physical_block_number;

	/* The segment descriptors
	 */
	libcdata_array_t *segment_descriptors;
};

int libfvde_logical_volume_descriptor_initialize(
     libfvde_logical_volume_descriptor_t **logical_volume_descriptor,
     libcerror_error_t **error );

int libfvde_logical_volume_descriptor_free(
     libfvde_logical_volume_descriptor_t **logical_volume_descriptor,
     libcerror_error_t **error );

int libfvde_logical_volume_descriptor_get_identifier(
     libfvde_logical_volume_descriptor_t *logical_volume_descriptor,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libcerror_error_t **error );

int libfvde_logical_volume_descriptor_get_family_identifier(
     libfvde_logical_volume_descriptor_t *logical_volume_descriptor,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libcerror_error_t **error );

int libfvde_logical_volume_descriptor_get_utf8_name_size(
     libfvde_logical_volume_descriptor_t *logical_volume_descriptor,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libfvde_logical_volume_descriptor_get_utf8_name(
     libfvde_logical_volume_descriptor_t *logical_volume_descriptor,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libfvde_logical_volume_descriptor_get_utf16_name_size(
     libfvde_logical_volume_descriptor_t *logical_volume_descriptor,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libfvde_logical_volume_descriptor_get_utf16_name(
     libfvde_logical_volume_descriptor_t *logical_volume_descriptor,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libfvde_logical_volume_descriptor_get_size(
     libfvde_logical_volume_descriptor_t *logical_volume_descriptor,
     size64_t *size,
     libcerror_error_t **error );

int libfvde_logical_volume_descriptor_get_first_block_number(
     libfvde_logical_volume_descriptor_t *logical_volume_descriptor,
     uint16_t *volume_index,
     uint64_t *block_number,
     libcerror_error_t **error );

int libfvde_logical_volume_descriptor_get_last_block_number(
     libfvde_logical_volume_descriptor_t *logical_volume_descriptor,
     uint16_t *volume_index,
     uint64_t *block_number,
     libcerror_error_t **error );

int libfvde_logical_volume_descriptor_get_number_of_segment_descriptors(
     libfvde_logical_volume_descriptor_t *logical_volume_descriptor,
     int *number_of_segment_descriptors,
     libcerror_error_t **error );

int libfvde_logical_volume_descriptor_get_segment_descriptor_by_index(
     libfvde_logical_volume_descriptor_t *logical_volume_descriptor,
     int segment_index,
     libfvde_segment_descriptor_t **segment_descriptor,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVDE_LOGICAL_VOLUME_DESCRIPTOR_H ) */

