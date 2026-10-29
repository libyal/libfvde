/*
 * Sector data vector functions
 *
 * Copyright (C) 2011-2026, Omar Choudary <choudary.omar@gmail.com>
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

#if !defined( _LIBFVDE_SECTOR_DATA_VECTOR_H )
#define _LIBFVDE_SECTOR_DATA_VECTOR_H

#include <common.h>
#include <types.h>

#include "libfvde_encryption_context.h"
#include "libfvde_libbfio.h"
#include "libfvde_libcerror.h"
#include "libfvde_libfcache.h"
#include "libfvde_logical_volume_descriptor.h"
#include "libfvde_sector_data.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfvde_sector_data_vector libfvde_sector_data_vector_t;

struct libfvde_sector_data_vector
{
	/* The cache timestamp
	 */
	int64_t cache_timestamp;

	/* The sector data cache
	 */
	libfcache_cache_t *cache;

	/* The number of bytes per sector
	 */
	uint32_t bytes_per_sector;

	/* The block size
	 */
	uint32_t block_size;

	/* The logical volume descriptor
	 */
	libfvde_logical_volume_descriptor_t *logical_volume_descriptor;

	/* The data size
	 */
	size64_t data_size;
};

int libfvde_sector_data_vector_initialize(
     libfvde_sector_data_vector_t **sector_data_vector,
     uint32_t bytes_per_sector,
     uint32_t block_size,
     libfvde_logical_volume_descriptor_t *logical_volume_descriptor,
     size64_t data_size,
     libcerror_error_t **error );

int libfvde_sector_data_vector_free(
     libfvde_sector_data_vector_t **sector_data_vector,
     libcerror_error_t **error );

int libfvde_sector_data_vector_get_sector_data_at_offset(
     libfvde_sector_data_vector_t *sector_data_vector,
     libbfio_pool_t *file_io_pool,
     libfvde_encryption_context_t *encryption_context,
     uint8_t is_encrypted,
     off64_t offset,
     libfvde_sector_data_t **sector_data,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVDE_SECTOR_DATA_VECTOR_H ) */

