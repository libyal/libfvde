/*
 * Deflate (zlib) (un)compression functions
 *
 * Copyright (C) 2011-2016, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFVDE_DEFLATE_COMPRESSION_H )
#define _LIBFVDE_DEFLATE_COMPRESSION_H

#include <common.h>
#include <types.h>

#include "libfvde_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

/* The block types
 */
enum LIBFVDE_DEFLATE_BLOCK_TYPES
{
	LIBFVDE_DEFLATE_BLOCK_TYPE_UNCOMPRESSED		= 0x00,
	LIBFVDE_DEFLATE_BLOCK_TYPE_HUFFMAN_FIXED	= 0x01,
	LIBFVDE_DEFLATE_BLOCK_TYPE_HUFFMAN_DYNAMIC	= 0x02,
	LIBFVDE_DEFLATE_BLOCK_TYPE_RESERVED		= 0x03
};

typedef struct libfvde_deflate_bit_stream libfvde_deflate_bit_stream_t;

struct libfvde_deflate_bit_stream
{
	/* The byte stream
	 */
	const uint8_t *byte_stream;

	/* The byte stream size
	 */
	size_t byte_stream_size;

	/* The byte stream offset
	 */
	size_t byte_stream_offset;

	/* The bit buffer
	 */
	uint32_t bit_buffer;

	/* The number of bits remaining in the bit buffer
	 */
	uint8_t bit_buffer_size;
};

typedef struct libfvde_deflate_huffman_table libfvde_deflate_huffman_table_t;

struct libfvde_deflate_huffman_table
{
	/* The maximum number of bits representable by the Huffman table
	 */
	uint8_t maximum_number_of_bits;

/* TODO create initialize and set size? */
	/* The codes array
	 */
	int codes_array[ 288 ];

	/* The code counts array
	 */
	int code_counts_array[ 16 ];

	/* The number of codes
	 */
	int number_of_codes;
};

int libfvde_deflate_bit_stream_get_value(
     libfvde_deflate_bit_stream_t *bit_stream,
     uint8_t number_of_bits,
     uint32_t *value_32bit,
     libcerror_error_t **error );

int libfvde_deflate_huffman_table_construct(
     libfvde_deflate_huffman_table_t *table,
     const uint16_t *code_sizes_array,
     int number_of_code_sizes,
     libcerror_error_t **error );

int libfvde_deflate_bit_stream_get_huffman_encoded_value(
     libfvde_deflate_bit_stream_t *bit_stream,
     libfvde_deflate_huffman_table_t *table,
     uint32_t *value_32bit,
     libcerror_error_t **error );

int libfvde_deflate_bit_stream_get_huffman_encoded_value(
     libfvde_deflate_bit_stream_t *bit_stream,
     libfvde_deflate_huffman_table_t *table,
     uint32_t *value_32bit,
     libcerror_error_t **error );

int libfvde_deflate_bit_stream_get_huffman_encoded_codes_array(
     libfvde_deflate_bit_stream_t *bit_stream,
     libfvde_deflate_huffman_table_t *code_size_table,
     uint16_t *codes_array,
     uint32_t maximum_number_of_codes,
     uint32_t number_of_codes,
     libcerror_error_t **error );

int libfvde_deflate_calculate_adler32(
     uint32_t *checksum_value,
     const uint8_t *buffer,
     size_t size,
     uint32_t initial_value,
     libcerror_error_t **error );

int libfvde_deflate_decompress(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     uint8_t *uncompressed_data,
     size_t *uncompressed_data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVDE_DEFLATE_COMPRESSION_H ) */

