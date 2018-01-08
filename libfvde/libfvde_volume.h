/*
 * Volume functions
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

#if !defined( _LIBFVDE_VOLUME_H )
#define _LIBFVDE_VOLUME_H

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
#include "libfvde_libuna.h"
#include "libfvde_metadata.h"
#include "libfvde_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfvde_internal_volume libfvde_internal_volume_t;

struct libfvde_internal_volume
{
	/* The current (storage media) offset
	 */
	off64_t current_offset;

	/* The primary metadata
	 */
	libfvde_metadata_t *primary_metadata;

	/* The secondary metadata
	 */
	libfvde_metadata_t *secondary_metadata;

	/* The tertiary metadata
	 */
	libfvde_metadata_t *tertiary_metadata;

	/* The quaternary metadata
	 */
	libfvde_metadata_t *quaternary_metadata;

	/* The primary encrypted metadata
	 */
	libfvde_encrypted_metadata_t *primary_encrypted_metadata;

	/* The secondary encrypted metadata
	 */
	libfvde_encrypted_metadata_t *secondary_encrypted_metadata;

	/* The EncryptedRoot.plist
	 */
	libfvde_encryption_context_plist_t *encrypted_root_plist;

	/* Value to indicate the encrypted root plist file is set
	 */
	uint8_t encrypted_root_plist_file_is_set;

	/* Value to indicate the encrypted root plist file is decrypted
	 */
	uint8_t encrypted_root_plist_file_is_decrypted;

	/* Value to indicate the volume master key is set
	 */
	uint8_t volume_master_key_is_set;

	/* The sectors vector
	 */
	libfdata_vector_t *sectors_vector;

	/* The sectors cache
	 */
	libfcache_cache_t *sectors_cache;

        /* The keyring
	 */
        libfvde_keyring_t *keyring;

	/* The IO handle
	 */
	libfvde_io_handle_t *io_handle;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* Value to indicate if the file IO handle was created inside the library
	 */
	uint8_t file_io_handle_created_in_library;

	/* Value to indicate if the file IO handle was opened inside the library
	 */
	uint8_t file_io_handle_opened_in_library;

	/* Value to indicate if the volume is locked
	 */
	uint8_t is_locked;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

LIBFVDE_EXTERN \
int libfvde_volume_initialize(
     libfvde_volume_t **volume,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_free(
     libfvde_volume_t **volume,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_signal_abort(
     libfvde_volume_t *volume,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_open(
     libfvde_volume_t *volume,
     const char *filename,
     int access_flags,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

LIBFVDE_EXTERN \
int libfvde_volume_open_wide(
     libfvde_volume_t *volume,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

LIBFVDE_EXTERN \
int libfvde_volume_open_file_io_handle(
     libfvde_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_close(
     libfvde_volume_t *volume,
     libcerror_error_t **error );

int libfvde_volume_open_read(
     libfvde_internal_volume_t *internal_volume,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libfvde_volume_open_read_keys_from_encrypted_metadata(
     libfvde_internal_volume_t *internal_volume,
     libfvde_encrypted_metadata_t *encrypted_metadata,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_is_locked(
     libfvde_volume_t *volume,
     libcerror_error_t **error );

ssize_t libfvde_internal_volume_read_buffer_from_file_io_handle(
         libfvde_internal_volume_t *internal_volume,
         libbfio_handle_t *file_io_handle,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBFVDE_EXTERN \
ssize_t libfvde_volume_read_buffer(
         libfvde_volume_t *volume,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBFVDE_EXTERN \
ssize_t libfvde_volume_read_buffer_at_offset(
         libfvde_volume_t *volume,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error );

#ifdef TODO_WRITE_SUPPORT

LIBFVDE_EXTERN \
ssize_t libfvde_volume_write_buffer(
         libfvde_volume_t *volume,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error );

LIBFVDE_EXTERN \
ssize_t libfvde_volume_write_buffer_at_offset(
         libfvde_volume_t *volume,
         const void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error );

#endif /* TODO_WRITE_SUPPORT */

off64_t libfvde_internal_volume_seek_offset(
         libfvde_internal_volume_t *internal_volume,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBFVDE_EXTERN \
off64_t libfvde_volume_seek_offset(
         libfvde_volume_t *volume,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_get_offset(
     libfvde_volume_t *volume,
     off64_t *offset,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_get_logical_volume_size(
     libfvde_volume_t *volume,
     size64_t *size,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_get_logical_volume_encryption_method(
     libfvde_volume_t *volume,
     uint32_t *encryption_method,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_get_logical_volume_identifier(
     libfvde_volume_t *volume,
     uint8_t *identifier,
     size_t size,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_get_logical_volume_group_identifier(
     libfvde_volume_t *volume,
     uint8_t *group_identifier,
     size_t size,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_get_physical_volume_size(
     libfvde_volume_t *volume,
     size64_t *size,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_get_physical_volume_encryption_method(
     libfvde_volume_t *volume,
     uint32_t *encryption_method,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_get_physical_volume_identifier(
     libfvde_volume_t *volume,
     uint8_t *identifier,
     size_t size,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_set_keys(
     libfvde_volume_t *volume,
     const uint8_t *volume_master_key,
     size_t volume_master_key_size,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_set_utf8_password(
     libfvde_volume_t *volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_set_utf16_password(
     libfvde_volume_t *volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_set_utf8_recovery_password(
     libfvde_volume_t *volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_set_utf16_recovery_password(
     libfvde_volume_t *volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_read_encrypted_root_plist(
     libfvde_volume_t *volume,
     const char *filename,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

LIBFVDE_EXTERN \
int libfvde_volume_read_encrypted_root_plist_wide(
     libfvde_volume_t *volume,
     const wchar_t *filename,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

LIBFVDE_EXTERN \
int libfvde_volume_read_encrypted_root_plist_file_io_handle(
     libfvde_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVDE_VOLUME_H ) */

