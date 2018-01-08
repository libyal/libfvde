/*
 * Mount handle
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

#if !defined( _MOUNT_HANDLE_H )
#define _MOUNT_HANDLE_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include "fvdetools_libbfio.h"
#include "fvdetools_libcerror.h"
#include "fvdetools_libcnotify.h"
#include "fvdetools_libfvde.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct mount_handle mount_handle_t;

struct mount_handle
{
	/* The volume offset
	 */
	off64_t volume_offset;

	/* The libbfio input file IO handle
	 */
	libbfio_handle_t *input_file_io_handle;

	/* The libfvde input volume
	 */
	libfvde_volume_t *input_volume;

	/* The notification output stream
	 */
	FILE *notify_stream;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int fvdetools_system_string_copy_from_64_bit_in_decimal(
     const system_character_t *string,
     size_t string_size,
     uint64_t *value_64bit,
     libcerror_error_t **error );

int mount_handle_initialize(
     mount_handle_t **mount_handle,
     libcerror_error_t **error );

int mount_handle_free(
     mount_handle_t **mount_handle,
     libcerror_error_t **error );

int mount_handle_signal_abort(
     mount_handle_t *mount_handle,
     libcerror_error_t **error );

int mount_handle_set_keys(
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

int mount_handle_read_encrypted_root_plist(
     mount_handle_t *mount_handle,
     const system_character_t *filename,
     libcerror_error_t **error );

int mount_handle_set_volume_offset(
     mount_handle_t *mount_handle,
     const system_character_t *string,
     libcerror_error_t **error );

int mount_handle_open_input(
     mount_handle_t *mount_handle,
     const system_character_t *filename,
     libcerror_error_t **error );

int mount_handle_close_input(
     mount_handle_t *mount_handle,
     libcerror_error_t **error );

ssize_t mount_handle_read_buffer(
         mount_handle_t *mount_handle,
         uint8_t *buffer,
         size_t size,
         libcerror_error_t **error );

off64_t mount_handle_seek_offset(
         mount_handle_t *mount_handle,
         off64_t offset,
         int whence,
         libcerror_error_t **error );

int mount_handle_get_size(
     mount_handle_t *mount_handle,
     size64_t *size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _MOUNT_HANDLE_H ) */

