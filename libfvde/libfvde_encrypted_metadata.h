/*
 * Encrypted metadata functions
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

#if !defined( _LIBFVDE_ENCRYPTED_METADATA_H )
#define _LIBFVDE_ENCRYPTED_METADATA_H

#include <common.h>
#include <types.h>

#include "libfvde_encryption_context_plist.h"
#include "libfvde_io_handle.h"
#include "libfvde_keyring.h"
#include "libfvde_libbfio.h"
#include "libfvde_libcdata.h"
#include "libfvde_libcerror.h"
#include "libfvde_logical_volume_descriptor.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfvde_encrypted_metadata libfvde_encrypted_metadata_t;

struct libfvde_encrypted_metadata
{
	/* The encryption context plist (com.apple.corestorage.lvf.encryption.context)
	 */
	libfvde_encryption_context_plist_t *encryption_context_plist;

	/* Value to indicate the encryption context plist file is set
	 */
	uint8_t encryption_context_plist_file_is_set;

	/* The logical volume descriptors
	 */
	libcdata_array_t *logical_volume_descriptors;

	/* The segment descriptors of metadata block 0x0304
	 */
	libcdata_array_t *segment_descriptors_0x0304;

	/* The encryption context plist data
	 */
	uint8_t *encryption_context_plist_data;

	/* The encryption context plist data size
	 */
	size_t encryption_context_plist_data_size;

	/* The compressed data object identifier
	 */
	uint64_t compressed_data_object_identifier;

	/* The compressed data
	 */
	uint8_t *compressed_data;

	/* The compressed data size
	 */
	size_t compressed_data_size;

	/* The compressed data offset
	 */
	size_t compressed_data_offset;

	/* The uncompressed data size
	 */
	size_t uncompressed_data_size;
};

int libfvde_encrypted_metadata_initialize(
     libfvde_encrypted_metadata_t **encrypted_metadata,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_free(
     libfvde_encrypted_metadata_t **encrypted_metadata,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_read_type_0x0010(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_read_type_0x0011(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
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

int libfvde_encrypted_metadata_read_type_0x0024(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     uint64_t object_identifier,
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

int libfvde_encrypted_metadata_read_type_0x0205(
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
     uint64_t object_identifier,
     const uint8_t *block_data,
     size_t block_data_size,
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
     libcerror_error_t **error );

int libfvde_encrypted_metadata_read_type_0x0505(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     uint64_t object_identifier,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_read_type_0x0605(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_read_from_file_io_handle(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     libfvde_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     uint64_t encrypted_metadata_size,
     const uint8_t *key,
     size_t key_bit_size,
     const uint8_t *tweak_key,
     size_t tweak_key_bit_size,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_get_volume_master_key(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     libfvde_encryption_context_plist_t *encryption_context_plist,
     libfvde_keyring_t *keyring,
     const uint8_t *user_password,
     size_t user_password_length,
     const uint8_t *recovery_password,
     size_t recovery_password_length,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_get_number_of_logical_volume_descriptors(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     int *number_of_logical_volume_descriptors,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_get_logical_volume_descriptor_by_index(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     int logical_volume_descriptor_index,
     libfvde_logical_volume_descriptor_t **logical_volume_descriptor,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_get_logical_volume_descriptor_by_object_identifier(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     uint64_t object_identifier,
     libfvde_logical_volume_descriptor_t **logical_volume_descriptor,
     libcerror_error_t **error );

int libfvde_encrypted_metadata_get_last_logical_volume_descriptor(
     libfvde_encrypted_metadata_t *encrypted_metadata,
     libfvde_logical_volume_descriptor_t **logical_volume_descriptor,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVDE_ENCRYPTED_METADATA_H ) */

