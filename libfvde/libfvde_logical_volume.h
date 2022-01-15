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

#include "libfvde_extern.h"
#include "libfvde_libcerror.h"
#include "libfvde_libcthreads.h"
#include "libfvde_logical_volume_descriptor.h"
#include "libfvde_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfvde_internal_logical_volume libfvde_internal_logical_volume_t;

struct libfvde_internal_logical_volume
{
	/* The logical volume descriptor
	 */
	libfvde_logical_volume_descriptor_t *logical_volume_descriptor;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

int libfvde_logical_volume_initialize(
     libfvde_logical_volume_t **logical_volume,
     libfvde_logical_volume_descriptor_t *logical_volume_descriptor,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_logical_volume_free(
     libfvde_logical_volume_t **logical_volume,
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
int libfvde_logical_volume_get_encryption_method(
     libfvde_logical_volume_t *logical_volume,
     uint32_t *encryption_method,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_logical_volume_get_size(
     libfvde_logical_volume_t *logical_volume,
     size64_t *size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVDE_LOGICAL_VOLUME_H ) */

