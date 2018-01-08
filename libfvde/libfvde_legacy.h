/*
 * Legacy functions
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

#if !defined( _LIBFVDE_LEGACY_H )
#define _LIBFVDE_LEGACY_H

#include <common.h>
#include <types.h>

#include "libfvde_extern.h"
#include "libfvde_legacy.h"
#include "libfvde_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

LIBFVDE_EXTERN \
int libfvde_volume_get_size(
     libfvde_volume_t *volume,
     size64_t *size,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_get_encryption_method(
     libfvde_volume_t *volume,
     uint32_t *encryption_method,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVDE_LEGACY_H ) */

