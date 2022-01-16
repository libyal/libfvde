/*
 * Input/Output (IO) handle functions
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

#if !defined( _LIBFVDE_IO_HANDLE_H )
#define _LIBFVDE_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include "libfvde_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfvde_io_handle libfvde_io_handle_t;

struct libfvde_io_handle
{
	/* The serial number
	 */
	uint32_t serial_number;

	/* The number of bytes per sector
	 */
	uint32_t bytes_per_sector;

	/* The block size
	 */
	uint32_t block_size;

	/* The metadata size
	 */
	uint32_t metadata_size;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int libfvde_io_handle_initialize(
     libfvde_io_handle_t **io_handle,
     libcerror_error_t **error );

int libfvde_io_handle_free(
     libfvde_io_handle_t **io_handle,
     libcerror_error_t **error );

int libfvde_io_handle_clear(
     libfvde_io_handle_t *io_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVDE_IO_HANDLE_H ) */

