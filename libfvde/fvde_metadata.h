/*
 * The MacOS-X File Vault disk encryption (FVDE) metadata definition
 * (also known as CoreStorage)
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

#if !defined( _FVDE_METADATA_H )
#define _FVDE_METADATA_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fvde_metadata_block_header fvde_metadata_block_header_t;

struct fvde_metadata_block_header
{
	/* The checksum
	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];

	/* The initial value (of the checksum)
	 * Consists of 4 bytes
	 */
	uint8_t initial_value[ 4 ];

	/* The version
	 * Consists of 2 bytes
	 * Contains 1
	 */
	uint8_t version[ 2 ];

	/* The type
	 * Consists of 2 bytes
	 */
	uint8_t type[ 2 ];

	/* The serial number
	 * Consists of 4 bytes
	 */
	uint8_t serial_number[ 4 ];

	/* The transaction identifier
	 * Consists of 8 bytes
	 */
	uint8_t transaction_identifier[ 8 ];

	/* The object identifier
	 * Consists of 8 bytes
	 */
	uint8_t object_identifier[ 8 ];

	/* The number
	 * Consists of 8 bytes
	 */
	uint8_t number[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown5[ 8 ];

	/* Block size
	 * Consists of 4 bytes
	 */
	uint8_t block_size[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown6[ 4 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown7[ 8 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FVDE_METADATA_H ) */
