/*
 * The FVDE volume header functions
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

#if !defined( _LIBFVDE_VOLUME_HEADER_H )
#define _LIBFVDE_VOLUME_HEADER_H

#include <common.h>
#include <types.h>

#include "libfvde_libbfio.h"
#include "libfvde_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfvde_volume_header libfvde_volume_header_t;

struct libfvde_volume_header
{
	/* The serial number
	 */
	uint32_t serial_number;

	/* The number of bytes per sector
	 */
	uint32_t bytes_per_sector;

	/* The physical volume size
	 */
	uint64_t physical_volume_size;

	/* The block size
	 */
	uint32_t block_size;

	/* The metadata size
	 */
	uint32_t metadata_size;

	/* The metadata offsets
	 */
	uint64_t metadata_offsets[ 4 ];

	/* The encryption method of the physical volume
	 */
	uint32_t physical_volume_encryption_method;

	/* The key data
         */
	uint8_t key_data[ 16 ];

	/* The physical volume identifier
	 */
	uint8_t physical_volume_identifier[ 16 ];

	/* The volume group identifier
	 */
	uint8_t volume_group_identifier[ 16 ];
};

int libfvde_volume_header_initialize(
     libfvde_volume_header_t **volume_header,
     libcerror_error_t **error );

int libfvde_volume_header_free(
     libfvde_volume_header_t **volume_header,
     libcerror_error_t **error );

int libfvde_volume_header_read_data(
     libfvde_volume_header_t *volume_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfvde_volume_header_read_file_io_handle(
     libfvde_volume_header_t *volume_header,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

int libfvde_volume_header_get_volume_group_identifier(
     libfvde_volume_header_t *volume_header,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libcerror_error_t **error );

int libfvde_volume_header_get_physical_volume_identifier(
     libfvde_volume_header_t *volume_header,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libcerror_error_t **error );

int libfvde_volume_header_get_physical_volume_encryption_method(
     libfvde_volume_header_t *volume_header,
     uint32_t *encryption_method,
     libcerror_error_t **error );

int libfvde_volume_header_get_physical_volume_size(
     libfvde_volume_header_t *volume_header,
     size64_t *size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVDE_VOLUME_HEADER_H ) */

