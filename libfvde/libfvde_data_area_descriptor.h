/*
 * Data area descriptor functions
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

#if !defined( _LIBFVDE_DATA_AREA_DESCRIPTOR_H )
#define _LIBFVDE_DATA_AREA_DESCRIPTOR_H

#include <common.h>
#include <types.h>

#include "libfvde_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfvde_data_area_descriptor libfvde_data_area_descriptor_t;

struct libfvde_data_area_descriptor
{
	/* The offset
	 */
	off64_t offset;

	/* The size
	 */
	size64_t size;

	/* The data type
	 */
	uint64_t data_type;

	/* The mapped offset
	 */
	off64_t mapped_offset;
};

int libfvde_data_area_descriptor_initialize(
     libfvde_data_area_descriptor_t **data_area_descriptor,
     libcerror_error_t **error );

int libfvde_data_area_descriptor_free(
     libfvde_data_area_descriptor_t **data_area_descriptor,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVDE_DATA_AREA_DESCRIPTOR_H ) */

