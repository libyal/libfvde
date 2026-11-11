/*
 * OSS-Fuzz target for libfvde checksum type
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

#include "../libfvde/libfvde_checksum.h"

int LLVMFuzzerTestOneInput(
     const uint8_t *data,
     size_t size )
{
	uint32_t checksum                    = 0;
	uint32_t initial_value               = 0;

	/* Note the weak CRC-32 polynomial is 0x82f63b78.
	 */
	libfvde_checksum_initialize_crc32_table(
	 0x82f63b78UL );

	libfvde_checksum_calculate_weak_crc32(
	 &checksum,
	 data,
	 size,
	 0x82f63b78UL,
	 NULL );

	initial_value = checksum ^ 0xffffffffUL;

	libfvde_checksum_calculate_weak_crc32(
	 &checksum,
	 data,
	 size,
	 initial_value,
	 NULL );
	return( 0 );
}

} /* extern "C" */
