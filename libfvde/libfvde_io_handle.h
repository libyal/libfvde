/*
 * Input/Output (IO) handle functions
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

#if !defined( _LIBFVDE_IO_HANDLE_H )
#define _LIBFVDE_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include "libfvde_libbfio.h"
#include "libfvde_libcaes.h"
#include "libfvde_libcerror.h"
#include "libfvde_libfcache.h"
#include "libfvde_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern const char *libfvde_core_storage_signature;

typedef struct libfvde_io_handle libfvde_io_handle_t;

struct libfvde_io_handle
{
	/* The version
	 */
	uint16_t version;

	/* The serial number
	 */
	uint32_t serial_number;

	/* The number of bytes per sector
	 */
	uint32_t bytes_per_sector;

	/* The block size
	 */
	uint32_t block_size;

	/* The number of sectors per block
	 */
	uint32_t sectors_per_block;

	/* The metadata size
	 */
	uint32_t metadata_size;

	/* The first metadata offset
	 */
	uint64_t first_metadata_offset;

	/* The second metadata offset
	 */
	uint64_t second_metadata_offset;

	/* The third metadata offset
	 */
	uint64_t third_metadata_offset;

	/* The fourth metadata offset
	 */
	uint64_t fourth_metadata_offset;

	/* The checksum algorithm
	 */
	uint32_t checksum_algorithm;

        /* The logical volume offset
         */
        off64_t logical_volume_offset;

	/* The logical volume size
	 */
	size64_t logical_volume_size;

	/* The encryption method of the logical volume
	 */
	uint32_t logical_volume_encryption_method;

	/* The logical volume identifier
	 */
	uint8_t logical_volume_identifier[ 16 ];

	/* The logical volume group identifier
	 */
	uint8_t logical_volume_group_identifier[ 16 ];

	/* The physical volume size
	 */
	uint64_t physical_volume_size;

	/* The encryption method of the physical volume
	 */
	uint32_t physical_volume_encryption_method;

	/* The physical volume identifier
	 */
	uint8_t physical_volume_identifier[ 16 ];

	/* The key data
         */
	uint8_t key_data[ 16 ];

	/* Value to indicate the volume is encrpted
	 */
	uint8_t is_encrypted;

	/* The XTS context
	 */
	libcaes_tweaked_context_t *xts_context;

	/* The user password
	 */
	uint8_t *user_password;

        /* User password size
	 */
	size_t user_password_size;

	/* Value to indicate the user password is set
	 */
	uint8_t user_password_is_set;

	/* The recovery password
	 */
	uint8_t *recovery_password;

        /* Recovery password size
	 */
	size_t recovery_password_size;

	/* Value to indicate the recovery password is set
	 */
	uint8_t recovery_password_is_set;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int libfvde_io_handle_initialize(
     libfvde_io_handle_t **io_handle,
     libcerror_error_t **error );

int libfvde_io_handle_free(
     libfvde_io_handle_t **io_handle,
     libcerror_error_t **error );

int libfvde_io_handle_clear(
     libfvde_io_handle_t *io_handle,
     libcerror_error_t **error );

int libfvde_io_handle_read_volume_header(
     libfvde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

int libfvde_io_handle_read_sector(
     libfvde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfcache_cache_t *cache,
     int element_index,
     int element_data_file_index,
     off64_t element_data_offset,
     size64_t element_data_size,
     uint32_t element_data_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

int libfvde_io_handle_read_logical_volume_header(
     libfvde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVDE_IO_HANDLE_H ) */

