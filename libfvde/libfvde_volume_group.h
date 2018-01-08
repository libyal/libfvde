/*
 * Volume group functions
 *
 * Copyright (C) 2011-2018, Omar Choudary <choudary.omar@gmail.com>,
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

#if !defined( _LIBFVDE_INTERNAL_VOLUME_GROUP_H )
#define _LIBFVDE_INTERNAL_VOLUME_GROUP_H

#include <common.h>
#include <types.h>

#include "libfvde_extern.h"
#include "libfvde_libcdata.h"
#include "libfvde_libcerror.h"
#include "libfvde_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfvde_internal_volume_group libfvde_internal_volume_group_t;

struct libfvde_internal_volume_group
{
	/* The name
	 */
	char *name;

	/* The name size
	 */
	size_t name_size;

	/* The identfier
	 */
	char identifier[ 39 ];

	/* The physical volumes array
	 */
	libcdata_array_t *physical_volumes_array;

	/* The logical volumes array
	 */
	libcdata_array_t *logical_volumes_array;
};

int libfvde_volume_group_initialize(
     libfvde_volume_group_t **volume_group,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_group_free(
     libfvde_volume_group_t **volume_group,
     libcerror_error_t **error );

int libfvde_internal_volume_group_free(
     libfvde_internal_volume_group_t **internal_volume_group,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVDE_INTERNAL_VOLUME_GROUP_H ) */

