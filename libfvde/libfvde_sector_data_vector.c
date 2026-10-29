/*
 * Sector data vector functions
 *
 * Copyright (C) 2011-2026, Omar Choudary <choudary.omar@gmail.com>
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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libfvde_definitions.h"
#include "libfvde_encryption_context.h"
#include "libfvde_libbfio.h"
#include "libfvde_libcerror.h"
#include "libfvde_libcnotify.h"
#include "libfvde_libfcache.h"
#include "libfvde_logical_volume_descriptor.h"
#include "libfvde_sector_data.h"
#include "libfvde_sector_data_vector.h"
#include "libfvde_segment_descriptor.h"

/* Creates a sector data vector
 * Make sure the value sector data vector is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfvde_sector_data_vector_initialize(
     libfvde_sector_data_vector_t **sector_data_vector,
     uint32_t bytes_per_sector,
     uint32_t block_size,
     libfvde_logical_volume_descriptor_t *logical_volume_descriptor,
     size64_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libfvde_sector_data_vector_initialize";

	if( sector_data_vector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector data vector.",
		 function );

		return( -1 );
	}
	if( *sector_data_vector != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid sector data vector value already set.",
		 function );

		return( -1 );
	}
	if( ( bytes_per_sector == 0 )
	 || ( bytes_per_sector > (uint32_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid bytes per sector value out of bounds.",
		 function );

		return( -1 );
	}
	/* A sector should not span multiple blocks
	 */
	if( ( block_size == 0 )
	 || ( ( block_size % bytes_per_sector ) != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid block size value out of bounds.",
		 function );

		return( -1 );
	}
	if( logical_volume_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume descriptor.",
		 function );

		return( -1 );
	}
	if( data_size > (size64_t) INT64_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	*sector_data_vector = memory_allocate_structure(
	                       libfvde_sector_data_vector_t );

	if( *sector_data_vector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create sector data vector.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *sector_data_vector,
	     0,
	     sizeof( libfvde_sector_data_vector_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear sector data vector.",
		 function );

		memory_free(
		 *sector_data_vector );

		*sector_data_vector = NULL;

		return( -1 );
	}
	if( libfcache_date_time_get_timestamp(
	     &( ( *sector_data_vector )->cache_timestamp ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve cache timestamp.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( ( *sector_data_vector )->cache ),
	     LIBFVDE_MAXIMUM_CACHE_ENTRIES_SECTORS,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sectors cache.",
		 function );

		goto on_error;
	}
	( *sector_data_vector )->bytes_per_sector          = bytes_per_sector;
	( *sector_data_vector )->block_size                = block_size;
	( *sector_data_vector )->logical_volume_descriptor = logical_volume_descriptor;
	( *sector_data_vector )->data_size                 = data_size;

	return( 1 );

on_error:
	if( *sector_data_vector != NULL )
	{
		memory_free(
		 *sector_data_vector );

		*sector_data_vector = NULL;
	}
	return( -1 );
}

/* Frees a sector data vector
 * Returns 1 if successful or -1 on error
 */
int libfvde_sector_data_vector_free(
     libfvde_sector_data_vector_t **sector_data_vector,
     libcerror_error_t **error )
{
	static char *function = "libfvde_sector_data_vector_free";
	int result            = 1;

	if( sector_data_vector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector data vector.",
		 function );

		return( -1 );
	}
	if( *sector_data_vector != NULL )
	{
		/* The logical_volume_descriptor is referenced and freed elsewhere
		 */
		if( libfcache_cache_free(
		     &( ( *sector_data_vector )->cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free cache.",
			 function );

			result = -1;
		}
		memory_free(
		 *sector_data_vector );

		*sector_data_vector = NULL;
	}
	return( result );
}

/* Retrieves the sector data at a specific (sector aligned) offset in the logical volume
 * The sector number relative to the start of the logical volume is used as
 * the encryption tweak value
 * Returns 1 if successful or -1 on error
 */
int libfvde_sector_data_vector_get_sector_data_at_offset(
     libfvde_sector_data_vector_t *sector_data_vector,
     libbfio_pool_t *file_io_pool,
     libfvde_encryption_context_t *encryption_context,
     uint8_t is_encrypted,
     off64_t offset,
     libfvde_sector_data_t **sector_data,
     libcerror_error_t **error )
{
	libfcache_cache_value_t *cache_value             = NULL;
	libfvde_sector_data_t *safe_sector_data          = NULL;
	libfvde_segment_descriptor_t *segment_descriptor = NULL;
	static char *function                            = "libfvde_sector_data_vector_get_sector_data_at_offset";
	off64_t sector_data_offset                       = 0;
	uint64_t logical_block_number                    = 0;
	uint64_t maximum_block_number                    = 0;
	uint64_t physical_block_number                   = 0;
	uint64_t sector_number                           = 0;
	int result                                       = 0;

	if( sector_data_vector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector data vector.",
		 function );

		return( -1 );
	}
	if( sector_data_vector->bytes_per_sector == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid sector data vector - bytes per sector value out of bounds.",
		 function );

		return( -1 );
	}
	if( sector_data_vector->block_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid sector data vector - block size value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( offset < 0 )
	 || ( (size64_t) offset >= sector_data_vector->data_size )
	 || ( ( (uint64_t) offset % sector_data_vector->bytes_per_sector ) != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( sector_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sector data.",
		 function );

		return( -1 );
	}
	result = libfcache_cache_get_value_by_identifier(
	          sector_data_vector->cache,
	          0,
	          offset,
	          sector_data_vector->cache_timestamp,
	          &cache_value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value from cache.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: cache: 0x%08" PRIjx " hit for offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
			 function,
			 (intptr_t) sector_data_vector->cache,
			 offset,
			 offset );
		}
#endif
		if( libfcache_cache_value_get_value(
		     cache_value,
		     (intptr_t **) sector_data,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sector data from cache value.",
			 function );

			goto on_error;
		}
		return( 1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: cache: 0x%08" PRIjx " miss for offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 (intptr_t) sector_data_vector->cache,
		 offset,
		 offset );
	}
#endif
	sector_number        = (uint64_t) offset / sector_data_vector->bytes_per_sector;
	logical_block_number = (uint64_t) offset / sector_data_vector->block_size;

	result = libfvde_logical_volume_descriptor_get_segment_descriptor_by_logical_block_number(
	          sector_data_vector->logical_volume_descriptor,
	          logical_block_number,
	          &segment_descriptor,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment descriptor for logical block number: %" PRIu64 ".",
		 function,
		 logical_block_number );

		goto on_error;
	}
	if( libfvde_sector_data_initialize(
	     &safe_sector_data,
	     (size_t) sector_data_vector->bytes_per_sector,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sector data.",
		 function );

		goto on_error;
	}
	if( result == 0 )
	{
		/* Data that is not mapped by a segment descriptor is sparse
		 */
		if( memory_set(
		     safe_sector_data->data,
		     0,
		     safe_sector_data->data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear sector data.",
			 function );

			goto on_error;
		}
	}
	else
	{
		if( segment_descriptor == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing segment descriptor.",
			 function );

			goto on_error;
		}
		/* Make sure the physical offset, including the offset within the block, does not exceed INT64_MAX
		 */
		maximum_block_number = ( (uint64_t) INT64_MAX - sector_data_vector->block_size ) / sector_data_vector->block_size;

		physical_block_number = sector_data_vector->logical_volume_descriptor->base_physical_block_number;

		if( ( physical_block_number > maximum_block_number )
		 || ( segment_descriptor->physical_block_number > ( maximum_block_number - physical_block_number ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid physical block number value out of bounds.",
			 function );

			goto on_error;
		}
		physical_block_number += segment_descriptor->physical_block_number;

		if( ( logical_block_number - segment_descriptor->logical_block_number ) > ( maximum_block_number - physical_block_number ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid physical block number value out of bounds.",
			 function );

			goto on_error;
		}
		physical_block_number += logical_block_number - segment_descriptor->logical_block_number;
		sector_data_offset = (off64_t) ( physical_block_number * sector_data_vector->block_size )
		                   + (off64_t) ( (uint64_t) offset % sector_data_vector->block_size );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: reading sector: %" PRIu64 " at offset: %" PRIi64 " (0x%08" PRIx64 ") from physical volume: %" PRIu16 "\n",
			 function,
			 sector_number,
			 sector_data_offset,
			 sector_data_offset,
			 segment_descriptor->physical_volume_index );
		}
#endif
		if( libfvde_sector_data_read(
		     safe_sector_data,
		     encryption_context,
		     file_io_pool,
		     (int) segment_descriptor->physical_volume_index,
		     sector_data_offset,
		     sector_number,
		     is_encrypted,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read sector data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 sector_data_offset,
			 sector_data_offset );

			goto on_error;
		}
	}
	if( libfcache_cache_set_value_by_identifier(
	     sector_data_vector->cache,
	     0,
	     offset,
	     sector_data_vector->cache_timestamp,
	     (intptr_t *) safe_sector_data,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfvde_sector_data_free,
	     LIBFCACHE_CACHE_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set sector data in cache.",
		 function );

		goto on_error;
	}
	*sector_data = safe_sector_data;

	return( 1 );

on_error:
	if( safe_sector_data != NULL )
	{
		libfvde_sector_data_free(
		 &safe_sector_data,
		 NULL );
	}
	return( -1 );
}

