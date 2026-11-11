/*
 * OSS-Fuzz target for libfvde compression type
 *
 * Copyright (C) 2026, Joachim Metz <joachim.metz@gmail.com>
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

#include <stddef.h>
#include <stdint.h>

/* Note that some of the OSS-Fuzz engines use C++
 */
extern "C" {

#include "ossfuzz_libbfio.h"
#include "ossfuzz_libfvde.h"

#include "../libfvde/libfvde_compression.h"
#include "../libfvde/libfvde_definitions.h"

int LLVMFuzzerTestOneInput(
     const uint8_t *data,
     size_t size )
{
	uint8_t uncompressed_data[ 64 * 1024 ];

	size_t uncompressed_data_size = sizeof( uncompressed_data );

	libfvde_decompress_data(
	 data,
	 size,
	 LIBFVDE_COMPRESSION_METHOD_DEFLATE,
	 uncompressed_data,
	 &uncompressed_data_size,
	 NULL );

	uncompressed_data_size = sizeof( uncompressed_data );

	libfvde_decompress_data(
	 data,
	 size,
	 LIBFVDE_COMPRESSION_METHOD_NONE,
	 uncompressed_data,
	 &uncompressed_data_size,
	 NULL );
	return( 0 );
}

} /* extern "C" */
