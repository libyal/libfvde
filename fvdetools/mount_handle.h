/*
 * Mount handle
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

#if !defined( _MOUNT_HANDLE_H )
#define _MOUNT_HANDLE_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include "fvdetools_libbfio.h"
#include "fvdetools_libcdata.h"
#include "fvdetools_libcerror.h"
#include "fvdetools_libfvde.h"
#include "mount_file_entry.h"
#include "mount_file_system.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct mount_handle mount_handle_t;

struct mount_handle
{
	/* The file system
	 */
	mount_file_system_t *file_system;

	/* The encrypted root plist path
	 */
	const system_character_t *encrypted_root_plist_path;

	/* The key data
	 */
	uint8_t key_data[ 16 ];

	/* The key data size
	 */
	size_t key_data_size;

	/* The volume offset
	 */
	off64_t volume_offset;

	/* The recovery password
	 */
	const system_character_t *recovery_password;

	/* The recovery password length
	 */
	size_t recovery_password_length;

	/* The user password
	 */
	const system_character_t *user_password;

	/* The user password length
	 */
	size_t user_password_length;

	/* The libbfio physical volume file IO pool
	 */
	libbfio_pool_t *physical_volume_file_io_pool;

	/* The libfvde volume
	 */
	libfvde_volume_t *volume;

	/* The volume group
	 */
	libfvde_volume_group_t *volume_group;

	/* The notification output stream
	 */
	FILE *notify_stream;

	/* Value to indicate if user interaction is disabled
	 */
	int unattended_mode;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int mount_handle_system_string_copy_from_64_bit_in_decimal(
     const system_character_t *string,
     size_t string_size,
     uint64_t *value_64bit,
     libcerror_error_t **error );

int mount_handle_initialize(
     mount_handle_t **mount_handle,
     int unattended_mode,
     libcerror_error_t **error );

int mount_handle_free(
     mount_handle_t **mount_handle,
     libcerror_error_t **error );

int mount_handle_signal_abort(
     mount_handle_t *mount_handle,
     libcerror_error_t **error );

int mount_handle_set_encrypted_root_plist(
     mount_handle_t *mount_handle,
     const system_character_t *filename,
     libcerror_error_t **error );

int mount_handle_set_key(
     mount_handle_t *mount_handle,
     const system_character_t *string,
     libcerror_error_t **error );

int mount_handle_set_offset(
     mount_handle_t *mount_handle,
     const system_character_t *string,
     libcerror_error_t **error );

int mount_handle_set_password(
     mount_handle_t *mount_handle,
     const system_character_t *string,
     libcerror_error_t **error );

int mount_handle_set_recovery_password(
     mount_handle_t *mount_handle,
     const system_character_t *string,
     libcerror_error_t **error );

int mount_handle_set_path_prefix(
     mount_handle_t *mount_handle,
     const system_character_t *path_prefix,
     size_t path_prefix_size,
     libcerror_error_t **error );

int mount_handle_open(
     mount_handle_t *mount_handle,
     system_character_t * const * filenames,
     int number_of_filenames,
     libcerror_error_t **error );

int mount_handle_close(
     mount_handle_t *mount_handle,
     libcerror_error_t **error );

int mount_handle_get_file_entry_by_path(
     mount_handle_t *mount_handle,
     const system_character_t *path,
     mount_file_entry_t **file_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _MOUNT_HANDLE_H ) */

