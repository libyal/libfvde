/*
 * Wipekey handle
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

#if !defined( _WIPEKEY_HANDLE_H )
#define _WIPEKEY_HANDLE_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include "fvdetools_libbfio.h"
#include "fvdetools_libcerror.h"
#include "fvdetools_libfvde.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct wipekey_handle wipekey_handle_t;

struct wipekey_handle
{
	/* The EncryptedRoot.plist
	 */
	libfvde_encryption_context_plist_t *encrypted_root_plist;

	/* The volume key data
	 */
	uint8_t volume_key_data[ 16 ];

	/* The libbfio input file IO handle
	 */
	libbfio_handle_t *input_file_io_handle;

	/* The notification output stream
	 */
	FILE *notify_stream;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int wipekey_handle_initialize(
     wipekey_handle_t **wipekey_handle,
     libcerror_error_t **error );

int wipekey_handle_free(
     wipekey_handle_t **wipekey_handle,
     libcerror_error_t **error );

int wipekey_handle_signal_abort(
     wipekey_handle_t *wipekey_handle,
     libcerror_error_t **error );

int wipekey_handle_set_volume_key_data(
     wipekey_handle_t *wipekey_handle,
     const system_character_t *string,
     libcerror_error_t **error );

int wipekey_handle_open_input(
     wipekey_handle_t *wipekey_handle,
     const system_character_t *filename,
     libcerror_error_t **error );

int wipekey_handle_close_input(
     wipekey_handle_t *wipekey_handle,
     libcerror_error_t **error );

int wipekey_handle_wipekey_fprint(
     wipekey_handle_t *wipekey_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _WIPEKEY_HANDLE_H ) */

