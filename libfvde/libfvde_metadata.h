/*
 * Metadata functions
 *
 * Copyright (C) 2011-2018, Omar Choudary <choudary.omar@gmail.com>
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

#if !defined( _LIBFVDE_METADATA_H )
#define _LIBFVDE_METADATA_H

#include <common.h>
#include <types.h>

#include "libfvde_io_handle.h"
#include "libfvde_libbfio.h"
#include "libfvde_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfvde_metadata libfvde_metadata_t;

struct libfvde_metadata
{
	/* The encrypted metadata size
	 */
	uint64_t encrypted_metadata_size;

	/* The primary encrypted metadata offset
	 */
	uint64_t primary_encrypted_metadata_offset;

	/* The secondary encrypted metadata offset
	 */
	uint64_t secondary_encrypted_metadata_offset;
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
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error );

int libfvde_metadata_read_core_storage_plist(
     libfvde_metadata_t *metadata,
     const uint8_t *xml_plist_data,
     libcerror_error_t **error );

int libfvde_metadata_read(
     libfvde_metadata_t *metadata,
     libfvde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVDE_METADATA_H ) */

