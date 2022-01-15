/*
 * Physical volume descriptor functions
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

#if !defined( _LIBFVDE_PHYSICAL_VOLUME_DESCRIPTOR_H )
#define _LIBFVDE_PHYSICAL_VOLUME_DESCRIPTOR_H

#include <common.h>
#include <types.h>

#include "libfvde_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfvde_physical_volume_descriptor libfvde_physical_volume_descriptor_t;

struct libfvde_physical_volume_descriptor
{
	/* The identifier
	 */
	uint8_t identifier[ 16 ];
};

int libfvde_physical_volume_descriptor_initialize(
     libfvde_physical_volume_descriptor_t **physical_volume_descriptor,
     libcerror_error_t **error );

int libfvde_physical_volume_descriptor_free(
     libfvde_physical_volume_descriptor_t **physical_volume_descriptor,
     libcerror_error_t **error );

int libfvde_physical_volume_descriptor_get_identifier(
     libfvde_physical_volume_descriptor_t *physical_volume_descriptor,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVDE_PHYSICAL_VOLUME_DESCRIPTOR_H ) */

