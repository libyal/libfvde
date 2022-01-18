/*
 * Info handle
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

#if !defined( _INFO_HANDLE_H )
#define _INFO_HANDLE_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include "fvdetools_libbfio.h"
#include "fvdetools_libcdata.h"
#include "fvdetools_libcerror.h"
#include "fvdetools_libfvde.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct info_handle info_handle_t;

struct info_handle
{
	/* The volume master key
	 */
	uint8_t volume_master_key[ 16 ];

	/* Value to indicate the volume master key is set
	 */
	uint8_t volume_master_key_is_set;

	/* The recovery password
	 */
	system_character_t *recovery_password;

	/* The recovery password size
	 */
	size_t recovery_password_size;

	/* The user password
	 */
	system_character_t *user_password;

	/* The user password size
	 */
	size_t user_password_size;

	/* The volume offset
	 */
	off64_t volume_offset;

	/* The libbfio physical volume file IO pool
	 */
	libbfio_pool_t *physical_volume_file_io_pool;

	/* The libfvde input volume
	 */
	libfvde_volume_t *input_volume;

	/* The volume group
	 */
	libfvde_volume_group_t *volume_group;

	/* The logical_volumes array
	 */
	libcdata_array_t *logical_volumes_array;

	/* The notification output stream
	 */
	FILE *notify_stream;

	/* Value to indicate if user interaction is disabled
	 */
	int unattend_mode;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int fvdetools_system_string_copy_from_64_bit_in_decimal(
     const system_character_t *string,
     size_t string_size,
     uint64_t *value_64bit,
     libcerror_error_t **error );

int info_handle_initialize(
     info_handle_t **info_handle,
     int unattend_mode,
     libcerror_error_t **error );

int info_handle_free(
     info_handle_t **info_handle,
     libcerror_error_t **error );

int info_handle_signal_abort(
     info_handle_t *info_handle,
     libcerror_error_t **error );

int info_handle_set_keys(
     info_handle_t *info_handle,
     const system_character_t *string,
     libcerror_error_t **error );

int info_handle_set_password(
     info_handle_t *info_handle,
     const system_character_t *string,
     libcerror_error_t **error );

int info_handle_set_recovery_password(
     info_handle_t *info_handle,
     const system_character_t *string,
     libcerror_error_t **error );

int info_handle_read_encrypted_root_plist(
     info_handle_t *info_handle,
     const system_character_t *filename,
     libcerror_error_t **error );

int info_handle_set_volume_offset(
     info_handle_t *info_handle,
     const system_character_t *string,
     libcerror_error_t **error );

int info_handle_open_input(
     info_handle_t *info_handle,
     system_character_t * const * filenames,
     int number_of_filenames,
     libcerror_error_t **error );

int info_handle_close_input(
     info_handle_t *info_handle,
     libcerror_error_t **error );

int info_handle_uuid_value_fprint(
     info_handle_t *info_handle,
     const char *value_name,
     const uint8_t *uuid_data,
     libcerror_error_t **error );

int info_handle_physical_volume_fprint(
     info_handle_t *info_handle,
     int physical_volume_index,
     libfvde_physical_volume_t *physical_volume,
     libcerror_error_t **error );

int info_handle_logical_volume_fprint(
     info_handle_t *info_handle,
     int logical_volume_index,
     libfvde_logical_volume_t *logical_volume,
     libcerror_error_t **error );

int info_handle_volume_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _INFO_HANDLE_H ) */

