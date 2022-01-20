/*
 * Volume functions
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

#if !defined( _LIBFVDE_VOLUME_H )
#define _LIBFVDE_VOLUME_H

#include <common.h>
#include <types.h>

#include "libfvde_encrypted_metadata.h"
#include "libfvde_encryption_context_plist.h"
#include "libfvde_extern.h"
#include "libfvde_io_handle.h"
#include "libfvde_logical_volume.h"
#include "libfvde_libbfio.h"
#include "libfvde_libcerror.h"
#include "libfvde_libcthreads.h"
#include "libfvde_libuna.h"
#include "libfvde_metadata.h"
#include "libfvde_types.h"
#include "libfvde_volume_header.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfvde_internal_volume libfvde_internal_volume_t;

struct libfvde_internal_volume
{
	/* The volume header
	 */
	libfvde_volume_header_t *volume_header;

	/* The metadata
	 */
	libfvde_metadata_t *metadata;

	/* The encrypted metadata 1
	 */
	libfvde_encrypted_metadata_t *encrypted_metadata1;

	/* The encrypted metadata 2
	 */
	libfvde_encrypted_metadata_t *encrypted_metadata2;

	/* The EncryptedRoot.plist
	 */
	libfvde_encryption_context_plist_t *encrypted_root_plist;

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

	/* The physical volume file IO pool
	 */
	libbfio_pool_t *physical_volume_file_io_pool;

	/* Value to indicate if the file IO pool was created inside the library
	 */
	uint8_t physical_volume_file_io_pool_created_in_library;

	/* The access flags
	 */
	int access_flags;

	/* The maximum number of open handles in the pool
	 */
	int maximum_number_of_open_handles;

	/* The file IO pool for backwards compatibility
	 */
	libbfio_pool_t *legacy_file_io_pool;

	/* Logical volume for backwards compatibility
	 */
	libfvde_logical_volume_t *legacy_logical_volume;

        /* The volume master key for backwards compatibility
	 */
        uint8_t legacy_volume_master_key[ 16 ];

	/* Value to indicate the volume master key is set for backwards compatibility
	 */
	uint8_t legacy_volume_master_key_is_set;

	/* The user password for backwards compatibility
	 */
	uint8_t *legacy_user_password;

        /* User password size for backwards compatibility
	 */
	size_t legacy_user_password_size;

	/* The recovery password for backwards compatibility
	 */
	uint8_t *legacy_recovery_password;

        /* Recovery password size for backwards compatibility
	 */
	size_t legacy_recovery_password_size;

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
int libfvde_volume_open_physical_volume_files(
     libfvde_volume_t *volume,
     char * const filenames[],
     int number_of_filenames,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

LIBFVDE_EXTERN \
int libfvde_volume_open_physical_volume_files_wide(
     libfvde_volume_t *volume,
     wchar_t * const filenames[],
     int number_of_filenames,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

LIBFVDE_EXTERN \
int libfvde_volume_open_physical_volume_files_file_io_pool(
     libfvde_volume_t *volume,
     libbfio_pool_t *file_io_pool,
     libcerror_error_t **error );

int libfvde_internal_volume_open_physical_volume_file(
     libfvde_internal_volume_t *internal_volume,
     libbfio_pool_t *file_io_pool,
     int physical_volume_index,
     const char *filename,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

int libfvde_internal_volume_open_physical_volume_file_wide(
     libfvde_internal_volume_t *internal_volume,
     libbfio_pool_t *file_io_pool,
     int physical_volume_index,
     const wchar_t *filename,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

int libfvde_internal_volume_open_physical_volume_file_io_handle(
     libfvde_internal_volume_t *internal_volume,
     libbfio_pool_t *file_io_pool,
     int physical_volume_index,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_close(
     libfvde_volume_t *volume,
     libcerror_error_t **error );

int libfvde_internal_volume_open_read(
     libfvde_internal_volume_t *internal_volume,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libfvde_internal_volume_open_read_physical_volume_files(
     libfvde_internal_volume_t *internal_volume,
     libbfio_pool_t *file_io_pool,
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

LIBFVDE_EXTERN \
int libfvde_volume_get_volume_group(
     libfvde_volume_t *volume,
     libfvde_volume_group_t **volume_group,
     libcerror_error_t **error );

/* The following functions have been deprecated and will be removed
 */

LIBFVDE_EXTERN \
int libfvde_volume_unlock(
     libfvde_volume_t *volume,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_is_locked(
     libfvde_volume_t *volume,
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
int libfvde_volume_get_size(
     libfvde_volume_t *volume,
     size64_t *size,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_get_encryption_method(
     libfvde_volume_t *volume,
     uint32_t *encryption_method,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_get_logical_volume_group_identifier(
     libfvde_volume_t *volume,
     uint8_t *uuid_data,
     size_t uuid_data_size,
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
     uint8_t *uuid_data,
     size_t uuid_data_size,
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
     uint8_t *uuid_data,
     size_t uuid_data_size,
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

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVDE_VOLUME_H ) */

