/*
 * Function to read password from stdin
 *
 * Copyright (C) 2017, Rob Wu <rob@robwu.nl>
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

#if !defined( _FVDETOOLS_ASKPASSWORD_H )
#define _FVDETOOLS_ASKPASSWORD_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

int fvdetools_ask_password(
     system_character_t ** asked_password,
     const system_character_t * prompt );

void fvdetools_free_password(
      system_character_t* asked_password );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FVDETOOLS_ASKPASSWORD_H ) */
