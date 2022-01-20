/*
 * Metadata functions
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

#if !defined( _LIBFVDE_METADATA_H )
#define _LIBFVDE_METADATA_H

#include <common.h>
#include <types.h>

#include "libfvde_io_handle.h"
#include "libfvde_libbfio.h"
#include "libfvde_libcdata.h"
#include "libfvde_libcerror.h"
#include "libfvde_physical_volume_descriptor.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfvde_metadata libfvde_metadata_t;

struct libfvde_metadata
{
	/* The transaction identifier
	 */
	uint64_t transaction_identifier;

	/* The physical volume index
	 */
	uint16_t physical_volume_index;

	/* The encrypted metadata size
	 */
	uint64_t encrypted_metadata_size;

	/* The encrypted metadata 1 offset
	 */
	uint64_t encrypted_metadata1_offset;

	/* The encrypted metadata 1 (physical) volume index
	 */
	uint16_t encrypted_metadata1_volume_index;

	/* The encrypted metadata 2 offset
	 */
	uint64_t encrypted_metadata2_offset;

	/* The encrypted metadata 2 (physical) volume index
	 */
	uint16_t encrypted_metadata2_volume_index;

	/* The volume group name
	 */
	uint8_t *volume_group_name;

	/* The volume group name size
	 */
	size_t volume_group_name_size;

	/* The physical volume descriptors
	 */
	libcdata_array_t *physical_volume_descriptors;
};

int libfvde_metadata_initialize(
     libfvde_metadata_t **metadata,
     libcerror_error_t **error );

int libfvde_metadata_free(
     libfvde_metadata_t **metadata,
     libcerror_error_t **error );

int libfvde_metadata_read_type_0x0011(
     libfvde_metadata_t *metadata,
     libfvde_io_handle_t *io_handle,
     uint64_t transaction_identifier,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error );

int libfvde_metadata_read_volume_group_plist(
     libfvde_metadata_t *metadata,
     const uint8_t *xml_plist_data,
     libcerror_error_t **error );

int libfvde_metadata_read_file_io_handle(
     libfvde_metadata_t *metadata,
     libfvde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

int libfvde_metadata_get_utf8_volume_group_name_size(
     libfvde_metadata_t *metadata,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libfvde_metadata_get_utf8_volume_group_name(
     libfvde_metadata_t *metadata,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libfvde_metadata_get_utf16_volume_group_name_size(
     libfvde_metadata_t *metadata,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libfvde_metadata_get_utf16_volume_group_name(
     libfvde_metadata_t *metadata,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

int libfvde_metadata_get_number_of_physical_volume_descriptors(
     libfvde_metadata_t *metadata,
     int *number_of_physical_volume_descriptors,
     libcerror_error_t **error );

int libfvde_metadata_get_physical_volume_descriptor_by_index(
     libfvde_metadata_t *metadata,
     int physical_volume_descriptor_index,
     libfvde_physical_volume_descriptor_t **physical_volume_descriptor,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVDE_METADATA_H ) */

