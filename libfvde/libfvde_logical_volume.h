/*
 * Logical volume functions
 *
 * Copyright (C) 2011-2022, Omar Choudary <choudary.omar@gmail.com>,
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

#if !defined( _LIBFVDE_LOGICAL_VOLUME_H )
#define _LIBFVDE_LOGICAL_VOLUME_H

#include <common.h>
#include <types.h>

#include "libfvde_encrypted_metadata.h"
#include "libfvde_encryption_context_plist.h"
#include "libfvde_extern.h"
#include "libfvde_io_handle.h"
#include "libfvde_keyring.h"
#include "libfvde_libbfio.h"
#include "libfvde_libcerror.h"
#include "libfvde_libcthreads.h"
#include "libfvde_libfcache.h"
#include "libfvde_libfdata.h"
#include "libfvde_logical_volume_descriptor.h"
#include "libfvde_types.h"
#include "libfvde_volume_data_handle.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfvde_internal_logical_volume libfvde_internal_logical_volume_t;

struct libfvde_internal_logical_volume
{
	/* The IO handle
	 */
	libfvde_io_handle_t *io_handle;

	/* The file IO pool
	 */
	libbfio_pool_t *file_io_pool;

	/* The logical volume descriptor
	 */
	libfvde_logical_volume_descriptor_t *logical_volume_descriptor;

	/* The encrypted metadata
	 */
	libfvde_encrypted_metadata_t *encrypted_metadata;

	/* The EncryptedRoot.plist
	 */
	libfvde_encryption_context_plist_t *encrypted_root_plist;

	/* The current offset
	 */
	off64_t current_offset;

	/* The volume size
	 */
	size64_t volume_size;

	/* The volume data handle
	 */
	libfvde_volume_data_handle_t *volume_data_handle;

	/* The sectors vector
	 */
	libfdata_vector_t *sectors_vector;

	/* The sectors cache
	 */
	libfcache_cache_t *sectors_cache;

	/* Value to indicate if the logical volume is locked
	 */
	uint8_t is_locked;

        /* The keyring
	 */
        libfvde_keyring_t *keyring;

	/* Value to indicate the volume master key is set
	 */
	uint8_t volume_master_key_is_set;

	/* The user password
	 */
	uint8_t *user_password;

        /* User password size
	 */
	size_t user_password_size;

	/* The recovery password
	 */
	uint8_t *recovery_password;

        /* Recovery password size
	 */
	size_t recovery_password_size;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

int libfvde_logical_volume_initialize(
     libfvde_logical_volume_t **logical_volume,
     libfvde_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libfvde_logical_volume_descriptor_t *logical_volume_descriptor,
     libfvde_encrypted_metadata_t *encrypted_metadata,
     libfvde_encryption_context_plist_t *encrypted_root_plist,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_logical_volume_free(
     libfvde_logical_volume_t **logical_volume,
     libcerror_error_t **error );

int libfvde_internal_logical_volume_open_read(
     libfvde_internal_logical_volume_t *internal_logical_volume,
     libbfio_pool_t *file_io_pool,
     libcerror_error_t **error );

int libfvde_internal_logical_volume_open_read_keys(
     libfvde_internal_logical_volume_t *internal_logical_volume,
     libcerror_error_t **error );

int libfvde_internal_logical_volume_open_read_volume_header_data(
     libfvde_internal_logical_volume_t *internal_logical_volume,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfvde_internal_logical_volume_open_read_volume_header(
     libfvde_internal_logical_volume_t *internal_logical_volume,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     off64_t file_offset,
     libcerror_error_t **error );

int libfvde_internal_logical_volume_close(
     libfvde_internal_logical_volume_t *internal_logical_volume,
     libcerror_error_t **error );

int libfvde_internal_logical_volume_unlock(
     libfvde_internal_logical_volume_t *internal_logical_volume,
     libbfio_pool_t *file_io_pool,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_logical_volume_unlock(
     libfvde_logical_volume_t *logical_volume,
     libcerror_error_t **error );

ssize_t libfvde_internal_logical_volume_read_buffer_from_file_io_pool(
         libfvde_internal_logical_volume_t *internal_logical_volume,
         libbfio_pool_t *file_io_pool,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBFVDE_EXTERN \
ssize_t libfvde_logical_volume_read_buffer(
         libfvde_logical_volume_t *logical_volume,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBFVDE_EXTERN \
ssize_t libfvde_logical_volume_read_buffer_at_offset(
         libfvde_logical_volume_t *logical_volume,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error );

off64_t libfvde_internal_logical_volume_seek_offset(
         libfvde_internal_logical_volume_t *internal_logical_volume,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBFVDE_EXTERN \
off64_t libfvde_logical_volume_seek_offset(
         libfvde_logical_volume_t *logical_volume,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_logical_volume_get_offset(
     libfvde_logical_volume_t *logical_volume,
     off64_t *offset,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_logical_volume_get_identifier(
     libfvde_logical_volume_t *logical_volume,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_logical_volume_get_utf8_name_size(
     libfvde_logical_volume_t *logical_volume,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_logical_volume_get_utf8_name(
     libfvde_logical_volume_t *logical_volume,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_logical_volume_get_utf16_name_size(
     libfvde_logical_volume_t *logical_volume,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_logical_volume_get_utf16_name(
     libfvde_logical_volume_t *logical_volume,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_logical_volume_get_size(
     libfvde_logical_volume_t *logical_volume,
     size64_t *size,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_logical_volume_is_locked(
     libfvde_logical_volume_t *logical_volume,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_logical_volume_set_key(
     libfvde_logical_volume_t *logical_volume,
     const uint8_t *volume_master_key,
     size_t volume_master_key_size,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_logical_volume_set_utf8_password(
     libfvde_logical_volume_t *logical_volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_logical_volume_set_utf16_password(
     libfvde_logical_volume_t *logical_volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_logical_volume_set_utf8_recovery_password(
     libfvde_logical_volume_t *logical_volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_logical_volume_set_utf16_recovery_password(
     libfvde_logical_volume_t *logical_volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVDE_LOGICAL_VOLUME_H ) */

