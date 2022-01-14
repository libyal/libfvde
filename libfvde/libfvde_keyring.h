/*
 * Keyring functions
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

#if !defined( _LIBFVDE_KEYRING_H )
#define _LIBFVDE_KEYRING_H

#include <common.h>
#include <types.h>

#include "libfvde_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfvde_keyring libfvde_keyring_t;

struct libfvde_keyring
{
        /* The volume master key
	 */
        uint8_t volume_master_key[ 16 ];

        /* The volume tweak key
	 */
        uint8_t volume_tweak_key[ 32 ];
};

int libfvde_keyring_initialize(
     libfvde_keyring_t **keyring,
     libcerror_error_t **error );

int libfvde_keyring_free(
     libfvde_keyring_t **keyring,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVDE_KEYRING_H ) */

