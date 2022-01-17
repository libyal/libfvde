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

	/* Logical volume for backwards compatibility
	 */
	libfvde_logical_volume_t *legacy_logical_volume;

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
int libfvde_volume_close(
     libfvde_volume_t *volume,
     libcerror_error_t **error );

int libfvde_internal_volume_open_read(
     libfvde_internal_volume_t *internal_volume,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_get_logical_volume_encryption_method(
     libfvde_volume_t *volume,
     uint32_t *encryption_method,
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

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVDE_VOLUME_H ) */

