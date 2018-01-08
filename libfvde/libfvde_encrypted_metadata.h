/*
 * Encrypted metadata functions
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

#if !defined( _LIBFVDE_ENCRYPTED_METADATA_H )
#define _LIBFVDE_ENCRYPTED_METADATA_H

#include <common.h>
#include <types.h>

#include "libfvde_data_area_descriptor.h"
#include "libfvde_encryption_context_plist.h"
#include "libfvde_io_handle.h"
#include "libfvde_keyring.h"
#include "libfvde_libbfio.h"
#include "libfvde_libcdata.h"
#include "libfvde_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfvde_encrypted_metadata libfvde_encrypted_metadata_t;

struct libfvde_encrypted_metadata
{
        /* The logical volume offset
         */
        off64_t logical_volume_offset;

	/* The logical volume size
	 */
	size64_t logical_volume_size;

	/* The logical volume identifier
	 * Contains an UUID
	 */
	uint8_t logical_volume_identifier[ 16 ];

	/* The logical volume family identifier
	 * Contains an UUID
	 */
	uint8_t logical_volume_family_identifier[ 16 ];

	/* The encryption context plist (com.apple.corestorage.lvf.encryption.context)
	 */
	libfvde_encryption_context_plist_t *encryption_context_plist;

	/* Value to indicate the encryption context plist file is set
	 */
	uint8_t encryption_context_plist_file_is_set;

	/* The group number of the metadata block 0x0305
	 */
	uint64_t block_group_0x0305;

	/* The group number of the metadata block 0x0405
	 */
	uint64_t block_group_0x0405;

	/* The group number of the metadata block 0x0505
	 */
	uint64_t block_group_0x0505;

	/* The logical volume block number as defined by metadata block 0x0305
	 */
	uint64_t logical_volume_block_number_0x0305;

	/* The logical volume number of blocks as defined by metadata block 0x0305
	 */
	uint64_t logical_volume_number_of_blocks_0x0305;

	/* The logical volume block number as defined by metadata block 0x0405
	 */
	uint64_t logical_volume_block_number_0x0405;

	/* The logical volume number of blocks as defined by metadata block 0x0405
	 */
	uint64_t logical_volume_number_of_blocks_0x0405;

	/* The logical volume block number as defined by metadata block 0x0505
	 */
	uint64_t logical_volume_block_number_0x0505;

	/* The logical volume number of blocks as defined by metadata block 0x0505
	 */
	uint64_t logical_volume_number_of_blocks_0x0505;

	/* The segment descriptors
	 */
	libcdata_array_t *segment_descriptors;

	/* The data area descriptors
	 */
	libcdata_array_t *data_area_descriptors;
};

int libfvde_encrypted_metadata_initialize(
     libfvde_encrypted_metadata_t **encrypted_metadata,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_free(
     libfvde_encrypted_metadata_t **encrypted_metadata,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_read_block_header(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t *block_data_size,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_read_type_0x0012(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_read_type_0x0013(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_read_type_0x0014(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_read_type_0x0016(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_read_type_0x0017(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_read_type_0x0018(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_read_type_0x0019(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_read_type_0x001a(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_read_type_0x001c(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_read_type_0x001d(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_read_type_0x0021(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_read_type_0x0022(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_read_type_0x0025(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_read_type_0x0105(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_read_type_0x0304(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_read_type_0x0305(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     uint64_t block_group,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_read_type_0x0404(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     libfvde_io_handle_t *io_handle,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_read_type_0x0405(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     libfvde_io_handle_t *io_handle,
     const uint8_t *block_data,
     size_t block_data_size,
     uint64_t block_group,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_read_type_0x0505(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     uint64_t block_group,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_read(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     libfvde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     uint64_t encrypted_metadata_size,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_get_volume_master_key(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     libfvde_io_handle_t *io_handle,
     libfvde_encryption_context_plist_t *encryption_context_plist,
     libfvde_keyring_t *keyring,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_get_number_of_data_area_descriptors(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     int *data_area_descriptors,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_get_data_area_descriptor_by_index(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     int data_area_descriptor_index,
     libfvde_data_area_descriptor_t **data_area_descriptor,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVDE_ENCRYPTED_METADATA_H ) */

