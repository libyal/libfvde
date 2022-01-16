/*
 * Metadata block functions
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

#if !defined( _LIBFVDE_METADATA_BLOCK_H )
#define _LIBFVDE_METADATA_BLOCK_H

#include <common.h>
#include <types.h>

#include "libfvde_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfvde_metadata_block libfvde_metadata_block_t;

struct libfvde_metadata_block
{
	/* The type
	 */
	uint16_t type;

	/* The serial number
	 */
	uint32_t serial_number;

	/* The transaction identifier
	 */
	uint64_t transaction_identifier;

	/* The object identifier
	 */
	uint64_t object_identifier;

	/* The number
	 */
	uint64_t number;

	/* The data
	 */
	const uint8_t *data;

	/* The data size
	 */
	size_t data_size;

	/* Value to indicate the block starts with LVFwiped
	 */
	uint8_t is_lvf_wiped;
};

int libfvde_metadata_block_initialize(
     libfvde_metadata_block_t **metadata_block,
     libcerror_error_t **error );

int libfvde_metadata_block_free(
     libfvde_metadata_block_t **metadata_block,
     libcerror_error_t **error );

int libfvde_metadata_block_check_for_empty_block(
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfvde_metadata_block_read_data(
     libfvde_metadata_block_t *metadata_block,
     const uint8_t *block_data,
     size_t block_data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVDE_METADATA_BLOCK_H ) */

