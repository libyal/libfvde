/*
 * The internal type definitions
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

#if !defined( _LIBFVDE_INTERNAL_TYPES_H )
#define _LIBFVDE_INTERNAL_TYPES_H

#include <common.h>
#include <types.h>

/* Define HAVE_LOCAL_LIBFVDE for local use of libfvde
 * The definitions in <libfvde/types.h> are copied here
 * for local use of libfvde
 */
#if defined( HAVE_LOCAL_LIBFVDE )

/* The following type definitions hide internal data structures
 */
#if defined( HAVE_DEBUG_OUTPUT ) && !defined( WINAPI )
typedef struct libfvde_encryption_context_plist {}	libfvde_encryption_context_plist_t;
typedef struct libfvde_logical_volume {}		libfvde_logical_volume_t;
typedef struct libfvde_physical_volume {}		libfvde_physical_volume_t;
typedef struct libfvde_volume {}			libfvde_volume_t;
typedef struct libfvde_volume_group {}			libfvde_volume_group_t;

#else
typedef intptr_t libfvde_encryption_context_plist_t;
typedef intptr_t libfvde_logical_volume_t;
typedef intptr_t libfvde_physical_volume_t;
typedef intptr_t libfvde_volume_t;
typedef intptr_t libfvde_volume_group_t;

#endif /* defined( HAVE_DEBUG_OUTPUT ) && !defined( WINAPI ) */

#endif /* defined( HAVE_LOCAL_LIBFVDE ) */

/* The largest primary (or scalar) available
 * supported by a single load and store instruction
 */
typedef unsigned long int libfvde_aligned_t;

#endif /* !defined( _LIBFVDE_INTERNAL_TYPES_H ) */

