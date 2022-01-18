/*
 * Volume group functions
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

#if !defined( _LIBFVDE_VOLUME_GROUP_H )
#define _LIBFVDE_VOLUME_GROUP_H

#include <common.h>
#include <types.h>

#include "libfvde_encrypted_metadata.h"
#include "libfvde_encryption_context_plist.h"
#include "libfvde_extern.h"
#include "libfvde_io_handle.h"
#include "libfvde_libbfio.h"
#include "libfvde_libcerror.h"
#include "libfvde_libcthreads.h"
#include "libfvde_metadata.h"
#include "libfvde_types.h"
#include "libfvde_volume_header.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfvde_internal_volume_group libfvde_internal_volume_group_t;

struct libfvde_internal_volume_group
{
	/* The IO handle
	 */
	libfvde_io_handle_t *io_handle;

	/* The file IO pool
	 */
	libbfio_pool_t *file_io_pool;

	/* The volume header
	 */
	libfvde_volume_header_t *volume_header;

	/* The metadata
	 */
	libfvde_metadata_t *metadata;

	/* The encrypted metadata
	 */
	libfvde_encrypted_metadata_t *encrypted_metadata;

	/* The EncryptedRoot.plist
	 */
	libfvde_encryption_context_plist_t *encrypted_root_plist;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

int libfvde_volume_group_initialize(
     libfvde_volume_group_t **volume_group,
     libfvde_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libfvde_volume_header_t *volume_header,
     libfvde_metadata_t *metadata,
     libfvde_encrypted_metadata_t *encrypted_metadata,
     libfvde_encryption_context_plist_t *encrypted_root_plist,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_group_free(
     libfvde_volume_group_t **volume_group,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_group_get_identifier(
     libfvde_volume_t *volume,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_group_get_utf8_name_size(
     libfvde_volume_group_t *volume_group,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_group_get_utf8_name(
     libfvde_volume_group_t *volume_group,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_group_get_utf16_name_size(
     libfvde_volume_group_t *volume_group,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_group_get_utf16_name(
     libfvde_volume_group_t *volume_group,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_group_get_number_of_physical_volumes(
     libfvde_volume_group_t *volume_group,
     int *number_of_physical_volumes,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_group_get_physical_volume_by_index(
     libfvde_volume_group_t *volume_group,
     int volume_index,
     libfvde_physical_volume_t **physical_volume,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_group_get_number_of_logical_volumes(
     libfvde_volume_group_t *volume_group,
     int *number_of_logical_volumes,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_group_get_logical_volume_by_index(
     libfvde_volume_group_t *volume_group,
     int volume_index,
     libfvde_logical_volume_t **logical_volume,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVDE_VOLUME_GROUP_H ) */

