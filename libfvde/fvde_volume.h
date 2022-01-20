/*
 * The MacOS-X File Vault disk encryption (FVDE) volume definition
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

#if !defined( _FVDE_VOLUME_H )
#define _FVDE_VOLUME_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct fvde_volume_header fvde_volume_header_t;

struct fvde_volume_header
{
	/* The checksum
	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];

	/* The initial value (of the checksum)
	 * Consists of 4 bytes
	 */
	uint8_t initial_value[ 4 ];

	/* The format version
	 * Consists of 2 bytes
	 * Contains 1
	 */
	uint8_t format_version[ 2 ];

	/* The block type
	 * Consists of 2 bytes
	 */
	uint8_t block_type[ 2 ];

	/* The serial number
	 * Consists of 4 bytes
	 */
	uint8_t serial_number[ 4 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown2[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown3a[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown3b[ 8 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown3c[ 8 ];

	/* The number of bytes per sector
	 * Consists of 4 bytes
	 */
	uint8_t bytes_per_sector[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown4a[ 4 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown4b[ 8 ];

	/* The size of the (physical) volume (in bytes)
	 * Consists of 8 bytes
	 */
	uint8_t volume_size[ 8 ];

	/* Unknown
	 * Consists of 16 bytes
	 */
/* TODO assumed to be 8-byte resize flags */
	uint8_t unknown5[ 16 ];

	/* The CoreStorage signature
	 * Consists of 2 bytes
	 * Contains "CS"
	 */
	uint8_t core_storage_signature[ 2 ];

	/* The checksum algorithm
	 * Consists of 4 bytes
	 */
	uint8_t checksum_algorithm[ 4 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
/* TODO assumed to be disklabel iterations */
	uint8_t unknown6[ 2 ];

	/* The (volume) block size
	 * Consists of 4 bytes
	 */
	uint8_t block_size[ 4 ];

	/* The (volume) metadata size
	 * Consists of 4 bytes
	 */
	uint8_t metadata_size[ 4 ];

	/* The metadata block numbers
	 * Consists of 4 x 8 = 32 bytes
	 */
	uint8_t metadata_block_numbers[ 32 ];

	/* Unknown
	 * Consists of 32 bytes
	 */
/* TODO assumed to be 8-byte block numbers */
	uint8_t unknown7[ 32 ];

	/* The key data size
	 * Consists of 4 bytes
	 */
	uint8_t key_data_size[ 4 ];

	/* The encryption method
	 * Consists of 4 bytes
	 */
	uint8_t encryption_method[ 4 ];

	/* The key data
	 * Consists of 128 bytes
	 */
	uint8_t key_data[ 128 ];

	/* The physical volume identifier
	 * Consists of 16 bytes
	 * Contains an UUID
	 */
	uint8_t physical_volume_identifier[ 16 ];

	/* The (logical) volume group identifier
	 * Consists of 16 bytes
	 * Contains an UUID
	 */
	uint8_t volume_group_identifier[ 16 ];

	/* Unknown
	 * Consists of 176 bytes
	 */
	uint8_t unknown8[ 176 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _FVDE_VOLUME_H ) */
