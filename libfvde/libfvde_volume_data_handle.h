/*
 * Volume data handle functions
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

#if !defined( _LIBFVDE_VOLUME_DATA_HANDLE_H )
#define _LIBFVDE_VOLUME_DATA_HANDLE_H

#include <common.h>
#include <types.h>

#include "libfvde_encryption_context.h"
#include "libfvde_io_handle.h"
#include "libfvde_libbfio.h"
#include "libfvde_libcerror.h"
#include "libfvde_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfvde_volume_data_handle libfvde_volume_data_handle_t;

struct libfvde_volume_data_handle
{
	/* The IO handle
	 */
	libfvde_io_handle_t *io_handle;

	/* The logical volume offset
	 */
	off64_t logical_volume_offset;

	/* The encryption context
	 */
	libfvde_encryption_context_t *encryption_context;

	/* Value to indicate the logical volume is encrypted
	 */
	uint8_t is_encrypted;
};

int libfvde_volume_data_handle_initialize(
     libfvde_volume_data_handle_t **volume_data_handle,
     libfvde_io_handle_t *io_handle,
     off64_t logical_volume_offset,
     libcerror_error_t **error );

int libfvde_volume_data_handle_free(
     libfvde_volume_data_handle_t **volume_data_handle,
     libcerror_error_t **error );

int libfvde_volume_data_handle_read_sector(
     libfvde_volume_data_handle_t *volume_data_handle,
     libbfio_pool_t *file_io_pool,
     libfdata_vector_t *vector,
     libfdata_cache_t *cache,
     int element_index,
     int element_data_file_index,
     off64_t element_data_offset,
     size64_t element_data_size,
     uint32_t element_data_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVDE_VOLUME_DATA_HANDLE_H ) */

