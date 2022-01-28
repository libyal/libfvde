/*
 * Logical volume functions
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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libfvde_definitions.h"
#include "libfvde_encrypted_metadata.h"
#include "libfvde_encryption_context.h"
#include "libfvde_encryption_context_plist.h"
#include "libfvde_io_handle.h"
#include "libfvde_libbfio.h"
#include "libfvde_libcerror.h"
#include "libfvde_libcnotify.h"
#include "libfvde_libcthreads.h"
#include "libfvde_libhmac.h"
#include "libfvde_libuna.h"
#include "libfvde_logical_volume.h"
#include "libfvde_logical_volume_descriptor.h"
#include "libfvde_password.h"
#include "libfvde_sector_data.h"
#include "libfvde_segment_descriptor.h"
#include "libfvde_types.h"
#include "libfvde_volume_data_handle.h"

/* Creates a logical volume
 * Make sure the value logical_volume is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfvde_logical_volume_initialize(
     libfvde_logical_volume_t **logical_volume,
     libfvde_io_handle_t *io_handle,
     libbfio_pool_t *file_io_pool,
     libfvde_logical_volume_descriptor_t *logical_volume_descriptor,
     libfvde_encrypted_metadata_t *encrypted_metadata,
     libfvde_encryption_context_plist_t *encrypted_root_plist,
     libcerror_error_t **error )
{
	libfvde_internal_logical_volume_t *internal_logical_volume = NULL;
	static char *function                                      = "libfvde_logical_volume_initialize";

	if( logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	if( *logical_volume != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid logical volume value already set.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
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
/* TODO check if encrypted_metadata is set */
/* TODO check if encrypted_root_plist is set */

	internal_logical_volume = memory_allocate_structure(
	                           libfvde_internal_logical_volume_t );

	if( internal_logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create logical volume.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_logical_volume,
	     0,
	     sizeof( libfvde_internal_logical_volume_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear logical volume.",
		 function );

		memory_free(
		 internal_logical_volume );

		return( -1 );
	}
	if( libfvde_keyring_initialize(
	      &( internal_logical_volume->keyring ),
	      error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialise keyring.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_logical_volume->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize read/write lock.",
		 function );

		goto on_error;
	}
#endif
	internal_logical_volume->io_handle                 = io_handle;
	internal_logical_volume->file_io_pool              = file_io_pool;
	internal_logical_volume->logical_volume_descriptor = logical_volume_descriptor;
	internal_logical_volume->encrypted_metadata        = encrypted_metadata;
	internal_logical_volume->encrypted_root_plist      = encrypted_root_plist;
	internal_logical_volume->is_locked                 = 1;

	*logical_volume = (libfvde_logical_volume_t *) internal_logical_volume;

	return( 1 );

on_error:
	if( internal_logical_volume != NULL )
	{
		if( internal_logical_volume->keyring != NULL )
		{
			libfvde_keyring_free(
			 &( internal_logical_volume->keyring ),
			 NULL );
		}
		memory_free(
		 internal_logical_volume );
	}
	return( -1 );
}

/* Frees a logical volume
 * Returns 1 if successful or -1 on error
 */
int libfvde_logical_volume_free(
     libfvde_logical_volume_t **logical_volume,
     libcerror_error_t **error )
{
	libfvde_internal_logical_volume_t *internal_logical_volume = NULL;
	static char *function                                      = "libfvde_logical_volume_free";
	int result                                                 = 1;

	if( logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	if( *logical_volume != NULL )
	{
		internal_logical_volume = (libfvde_internal_logical_volume_t *) *logical_volume;
		*logical_volume         = NULL;

		if( libfvde_internal_logical_volume_close(
		     internal_logical_volume,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close logical volume.",
			 function );

			result = -1;
		}
		if( libfvde_keyring_free(
		     &( internal_logical_volume->keyring ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free keyring.",
			 function );

			result = -1;
		}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_logical_volume->read_write_lock ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free read/write lock.",
			 function );

			result = -1;
		}
#endif
		/* The io_handle, file_io_pool, logical_volume_descriptor, encrypted_metadata and encrypted_root_plist references are freed elsewhere
		 */
		memory_free(
		 internal_logical_volume );
	}
	return( result );
}

/* Opens a logical volume for reading
 * Returns 1 if successful or -1 on error
 */
int libfvde_internal_logical_volume_open_read(
     libfvde_internal_logical_volume_t *internal_logical_volume,
     libbfio_pool_t *file_io_pool,
     libcerror_error_t **error )
{
	uint8_t volume_header_data[ 512 ];

	libfvde_segment_descriptor_t *segment_descriptor = NULL;
	static char *function                            = "libfvde_internal_logical_volume_open_read";
	size64_t segment_size                            = 0;
	ssize_t read_count                               = 0;
	off64_t segment_offset                           = 0;
	off64_t volume_offset                            = 0;
	uint64_t expected_logical_block_number           = 0;
	uint32_t segment_flags                           = 0;
	int file_io_pool_entry                           = 0;
	int number_of_segment_descriptors                = 0;
	int result                                       = 0;
	int segment_descriptor_index                     = 0;
	int segment_index                                = 0;

	if( internal_logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	if( internal_logical_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid logical volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_logical_volume->volume_data_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid logical volume - volume data handle value already set.",
		 function );

		return( -1 );
	}
	if( internal_logical_volume->sectors_vector != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid logical volume - sectors vector value already set.",
		 function );

		return( -1 );
	}
	if( internal_logical_volume->sectors_cache != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid logical volume - sectors cache value already set.",
		 function );

		return( -1 );
	}
	if( libfvde_logical_volume_descriptor_get_first_block_number(
	     internal_logical_volume->logical_volume_descriptor,
	     (uint16_t *) &file_io_pool_entry,
	     (uint64_t *) &volume_offset,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve first block number from logical volume descriptor.",
		 function );

		goto on_error;
	}
	volume_offset *= internal_logical_volume->io_handle->block_size;

	if( libfvde_logical_volume_descriptor_get_size(
	     internal_logical_volume->logical_volume_descriptor,
	     &( internal_logical_volume->volume_size ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve logical volume size from descriptor.",
		 function );

		result = -1;
	}
/* TODO remove after debugging unsupported format variants
	if( internal_logical_volume->volume_size == 0 )
	{
		if( libfvde_logical_volume_descriptor_get_last_block_number(
		     internal_logical_volume->logical_volume_descriptor,
		     (uint16_t *) &file_io_pool_entry,
		     (uint64_t *) &( internal_logical_volume->volume_size ),
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve last block number from logical volume descriptor.",
			 function );

			goto on_error;
		}
		internal_logical_volume->volume_size *= internal_logical_volume->io_handle->block_size;
	}
*/
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading logical volume header at offset: %" PRIi64 " (0x%08" PRIx64 ").\n",
		 volume_offset + 1024,
		 volume_offset + 1024 );
	}
#endif
	read_count = libbfio_pool_read_buffer_at_offset(
		      file_io_pool,
		      file_io_pool_entry,
		      volume_header_data,
		      512,
		      volume_offset + 1024,
		      error );

	if( read_count != 512 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume header data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 volume_offset + 1024,
		 volume_offset + 1024 );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: volume header data:\n",
		 function );
		libcnotify_print_data(
		 volume_header_data,
		 512,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	result = libfvde_internal_logical_volume_open_read_volume_header_data(
	          internal_logical_volume,
	          volume_header_data,
	          512,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume header.",
		 function );

		goto on_error;
	}
	internal_logical_volume->is_locked = ( result == 0 );

	if( libfvde_volume_data_handle_initialize(
	     &( internal_logical_volume->volume_data_handle ),
	     internal_logical_volume->io_handle,
	     volume_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create volume data handle.",
		 function );

		goto on_error;
	}
	if( internal_logical_volume->is_locked != 0 )
	{
		result = libfvde_internal_logical_volume_unlock(
		          internal_logical_volume,
		          file_io_pool,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to unlock volume.",
			 function );

			goto on_error;
		}
	}
	if( libfdata_vector_initialize(
	     &( internal_logical_volume->sectors_vector ),
	     (size64_t) internal_logical_volume->io_handle->bytes_per_sector,
	     (intptr_t *) internal_logical_volume->volume_data_handle,
	     NULL,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_vector_t *, libfdata_cache_t *, int, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libfvde_volume_data_handle_read_sector,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sectors vector.",
		 function );

		goto on_error;
	}
	if( libfvde_logical_volume_descriptor_get_number_of_segment_descriptors(
	     internal_logical_volume->logical_volume_descriptor,
	     &number_of_segment_descriptors,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of segment descriptors from logical volume descriptor.",
		 function );

		goto on_error;
	}
	expected_logical_block_number = 0;

	for( segment_descriptor_index = 0;
	     segment_descriptor_index < number_of_segment_descriptors;
	     segment_descriptor_index++ )
	{
		if( libfvde_logical_volume_descriptor_get_segment_descriptor_by_index(
		     internal_logical_volume->logical_volume_descriptor,
		     segment_descriptor_index,
		     &segment_descriptor,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve segment descriptor: %d from logical volume descriptor.",
			 function,
			 segment_descriptor_index );

			goto on_error;
		}
		if( segment_descriptor == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing segment descriptor: %d.",
			 function,
			 segment_descriptor_index );

			goto on_error;
		}
		if( segment_descriptor->logical_block_number < expected_logical_block_number )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported logical block number of segment descriptor: %d.",
			 function,
			 segment_descriptor_index );

			goto on_error;
		}
		else if( segment_descriptor->logical_block_number > expected_logical_block_number )
		{
			segment_size  = segment_descriptor->logical_block_number - expected_logical_block_number;
			segment_size *= internal_logical_volume->io_handle->block_size;

			if( libfdata_vector_append_segment(
			     internal_logical_volume->sectors_vector,
			     &segment_index,
			     0,
			     0,
			     segment_size,
			     LIBFVDE_RANGE_FLAG_IS_SPARSE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append sparse segment to sectors vector.",
				 function );

				goto on_error;
			}
			expected_logical_block_number = segment_descriptor->logical_block_number;
		}
		segment_offset  = internal_logical_volume->logical_volume_descriptor->base_physical_block_number;
		segment_offset += segment_descriptor->physical_block_number;
		segment_offset *= internal_logical_volume->io_handle->block_size;
		segment_size    = segment_descriptor->number_of_blocks;
		segment_size   *= internal_logical_volume->io_handle->block_size;

		if( internal_logical_volume->volume_data_handle->is_encrypted != 0 )
		{
			segment_flags = LIBFVDE_RANGE_FLAG_IS_ENCRYPTED;
		}
		else
		{
			segment_flags = 0;
		}
		if( libfdata_vector_append_segment(
		     internal_logical_volume->sectors_vector,
		     &segment_index,
		     (int) segment_descriptor->physical_volume_index,
		     segment_offset,
		     segment_size,
		     segment_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append segment to sectors vector.",
			 function );

			goto on_error;
		}
		expected_logical_block_number += segment_descriptor->number_of_blocks;
	}
	segment_size = (size64_t) expected_logical_block_number * internal_logical_volume->io_handle->block_size;

	if( segment_size < internal_logical_volume->volume_size )
	{
		segment_size = internal_logical_volume->volume_size - segment_size;

		if( libfdata_vector_append_segment(
		     internal_logical_volume->sectors_vector,
		     &segment_index,
		     0,
		     0,
		     segment_size,
		     LIBFVDE_RANGE_FLAG_IS_SPARSE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append sparse segment to sectors vector.",
			 function );

			goto on_error;
		}
	}
	if( libfcache_cache_initialize(
	     &( internal_logical_volume->sectors_cache ),
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
	return( 1 );

on_error:
	if( internal_logical_volume->sectors_cache != NULL )
	{
		libfcache_cache_free(
		 &( internal_logical_volume->sectors_cache ),
		 NULL );
	}
	if( internal_logical_volume->sectors_vector != NULL )
	{
		libfdata_vector_free(
		 &( internal_logical_volume->sectors_vector ),
		 NULL );
	}
	if( internal_logical_volume->volume_data_handle != NULL )
	{
		libfvde_volume_data_handle_free(
		 &( internal_logical_volume->volume_data_handle ),
		 NULL );
	}
	return( -1 );
}

/* Reads the keys from when unlocking the volume
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libfvde_internal_logical_volume_open_read_keys(
     libfvde_internal_logical_volume_t *internal_logical_volume,
     libcerror_error_t **error )
{
	uint8_t tweak_key_data[ 32 ];

	static char *function = "libfvde_internal_logical_volume_open_read_keys";
	int result            = 0;

	if( internal_logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	if( internal_logical_volume->logical_volume_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid logical volume - missing logical volume descriptor.",
		 function );

		return( -1 );
	}
	if( internal_logical_volume->encrypted_metadata == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid logical volume - missing encrypted metadata.",
		 function );

		return( -1 );
	}
	if( internal_logical_volume->keyring == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid logical volume - missing keyring handle.",
		 function );

		return( -1 );
	}
	if( internal_logical_volume->volume_data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid logical volume - missing volume data handle.",
		 function );

		return( -1 );
	}
	if( internal_logical_volume->volume_master_key_is_set == 0 )
	{
		if( internal_logical_volume->encrypted_metadata->encryption_context_plist_file_is_set != 0 )
		{
			result = libfvde_encrypted_metadata_get_volume_master_key(
				  internal_logical_volume->encrypted_metadata,
				  internal_logical_volume->encrypted_metadata->encryption_context_plist,
				  internal_logical_volume->keyring,
			          internal_logical_volume->user_password,
			          internal_logical_volume->user_password_size - 1,
			          internal_logical_volume->recovery_password,
			          internal_logical_volume->recovery_password_size - 1,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve volume master key from encrypted metadata.",
				 function );

				goto on_error;
			}
			else if( result != 0 )
			{
				internal_logical_volume->volume_master_key_is_set = 1;
			}
		}
		else if( internal_logical_volume->encrypted_root_plist != NULL )
		{
			result = libfvde_encrypted_metadata_get_volume_master_key(
				  internal_logical_volume->encrypted_metadata,
				  internal_logical_volume->encrypted_root_plist,
				  internal_logical_volume->keyring,
			          internal_logical_volume->user_password,
			          internal_logical_volume->user_password_size - 1,
			          internal_logical_volume->recovery_password,
			          internal_logical_volume->recovery_password_size - 1,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve volume master key from encrypted metadata.",
				 function );

				goto on_error;
			}
			else if( result != 0 )
			{
				internal_logical_volume->volume_master_key_is_set = 1;
			}
		}
	}
	if( ( internal_logical_volume->volume_master_key_is_set != 0 )
	 && ( internal_logical_volume->volume_data_handle->encryption_context == NULL ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: volume master key:\n",
			 function );
			libcnotify_print_data(
			 internal_logical_volume->keyring->volume_master_key,
			 16,
			 0 );
		}
#endif
		if( memory_copy(
		     &( tweak_key_data[ 0 ] ),
		     internal_logical_volume->keyring->volume_master_key,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy volume master key to tweak key data.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     &( tweak_key_data[ 16 ] ),
		     internal_logical_volume->logical_volume_descriptor->family_identifier,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy logical volume family identifier to tweak key data.",
			 function );

			goto on_error;
		}
		if( libhmac_sha256_calculate(
		     tweak_key_data,
		     32,
		     internal_logical_volume->keyring->volume_tweak_key,
		     32,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to calculate SHA-256 of tweak key data.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: volume tweak key:\n",
			 function );
			libcnotify_print_data(
			 internal_logical_volume->keyring->volume_tweak_key,
			 16,
			 0 );
		}
#endif
		if( libfvde_encryption_context_initialize(
		     &( internal_logical_volume->volume_data_handle->encryption_context ),
		     LIBFVDE_ENCRYPTION_METHOD_AES_128_XTS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create encryption context.",
			 function );

			goto on_error;
		}
		if( libfvde_encryption_context_set_keys(
		     internal_logical_volume->volume_data_handle->encryption_context,
		     internal_logical_volume->keyring->volume_master_key,
		     128,
		     internal_logical_volume->keyring->volume_tweak_key,
		     128,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set keys in encryption context.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     tweak_key_data,
		     0,
		     32 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear tweak key data.",
			 function );

			goto on_error;
		}
		internal_logical_volume->volume_data_handle->is_encrypted = 1;
	}
	if( internal_logical_volume->volume_data_handle->encryption_context != NULL )
	{
		return( 1 );
	}
	return( 0 );

on_error:
	memory_set(
	 tweak_key_data,
	 0,
	 32 );

	return( -1 );
}

/* Reads the logical volume header
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libfvde_internal_logical_volume_open_read_volume_header_data(
     libfvde_internal_logical_volume_t *internal_logical_volume,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function     = "libfvde_internal_logical_volume_open_read_volume_header_data";
	uint32_t block_size       = 0;
	uint32_t number_of_blocks = 0;
	uint16_t volume_signature = 0;
	int result                = 0;

	if( internal_logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( ( data_size < 512 )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint16_big_endian(
	 data,
	 volume_signature );

	if( ( volume_signature == 0x482b )
	 || ( volume_signature == 0x4858 ) )
	{
		byte_stream_copy_to_uint32_big_endian(
		 &( data[ 40 ] ),
		 block_size );

		byte_stream_copy_to_uint32_big_endian(
		 &( data[ 44 ] ),
		 number_of_blocks );

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: block size\t\t: %" PRIu32 "\n",
			 function,
			 block_size );

			libcnotify_printf(
			 "%s: number of blocks\t\t: %" PRIu32 "\n",
			 function,
			 number_of_blocks );

			libcnotify_printf(
			 "\n" );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		result = 1;
	}
	return( result );
}

/* Reads the logical volume header
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libfvde_internal_logical_volume_open_read_volume_header(
     libfvde_internal_logical_volume_t *internal_logical_volume,
     libbfio_pool_t *file_io_pool,
     int file_io_pool_entry,
     off64_t file_offset,
     libcerror_error_t **error )
{
	libfvde_sector_data_t *sector_data = NULL;
	static char *function              = "libfvde_internal_logical_volume_open_read_volume_header";
	uint64_t block_number              = 0;
	int result                         = 0;

	if( internal_logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	if( internal_logical_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid logical volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_logical_volume->volume_data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid logical volume - missing volume data handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading logical volume header at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	if( libfvde_sector_data_initialize(
	     &sector_data,
	     (size_t) internal_logical_volume->io_handle->bytes_per_sector,
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
	block_number = (uint64_t) ( file_offset - internal_logical_volume->volume_data_handle->logical_volume_offset ) / internal_logical_volume->io_handle->bytes_per_sector;

	if( libfvde_sector_data_read(
	     sector_data,
	     internal_logical_volume->volume_data_handle->encryption_context,
	     file_io_pool,
	     file_io_pool_entry,
	     file_offset,
	     block_number,
	     internal_logical_volume->volume_data_handle->is_encrypted,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume header sector data.",
		 function );

		goto on_error;
	}
	result = libfvde_internal_logical_volume_open_read_volume_header_data(
	          internal_logical_volume,
	          sector_data->data,
	          sector_data->data_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume header.",
		 function );

		goto on_error;
	}
	if( libfvde_sector_data_free(
	     &sector_data,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free sector data.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( sector_data != NULL )
	{
		libfvde_sector_data_free(
		 &sector_data,
		 NULL );
	}
	return( -1 );
}

/* Closes a logical volume
 * Returns 0 if successful or -1 on error
 */
int libfvde_internal_logical_volume_close(
     libfvde_internal_logical_volume_t *internal_logical_volume,
     libcerror_error_t **error )
{
	static char *function = "libfvde_internal_logical_volume_close";
	int result            = 0;

	if( internal_logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	internal_logical_volume->current_offset = 0;
	internal_logical_volume->is_locked      = 1;

	if( internal_logical_volume->user_password != NULL )
	{
		if( memory_set(
		     internal_logical_volume->user_password,
		     0,
		     internal_logical_volume->user_password_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear user password.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_logical_volume->user_password );

		internal_logical_volume->user_password      = NULL;
		internal_logical_volume->user_password_size = 0;
	}
	if( internal_logical_volume->recovery_password != NULL )
	{
		if( memory_set(
		     internal_logical_volume->recovery_password,
		     0,
		     internal_logical_volume->recovery_password_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear recovery password.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_logical_volume->recovery_password );

		internal_logical_volume->recovery_password      = NULL;
		internal_logical_volume->recovery_password_size = 0;
	}
	if( internal_logical_volume->volume_data_handle != NULL )
	{
		if( libfvde_volume_data_handle_free(
		     &( internal_logical_volume->volume_data_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free volume data handle.",
			 function );

			result = -1;
		}
	}
	if( internal_logical_volume->sectors_vector != NULL )
	{
		if( libfdata_vector_free(
		     &( internal_logical_volume->sectors_vector ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free sectors vector.",
			 function );

			result = -1;
		}
	}
	if( internal_logical_volume->sectors_cache != NULL )
	{
		if( libfcache_cache_free(
		     &( internal_logical_volume->sectors_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free sectors cache.",
			 function );

			result = -1;
		}
	}
	return( result );
}

/* Unlocks the logical volume
 * Returns 1 if the volume is unlocked, 0 if not or -1 on error
 */
int libfvde_internal_logical_volume_unlock(
     libfvde_internal_logical_volume_t *internal_logical_volume,
     libbfio_pool_t *file_io_pool,
     libcerror_error_t **error )
{
	static char *function  = "libfvde_internal_logical_volume_unlock";
	off64_t volume_offset  = 0;
	int file_io_pool_entry = 0;
	int result             = 0;

	if( internal_logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	if( internal_logical_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid logical volume - missing IO handle.",
		 function );

		return( -1 );
	}
	result = libfvde_internal_logical_volume_open_read_keys(
		  internal_logical_volume,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read keys.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvde_logical_volume_descriptor_get_first_block_number(
		     internal_logical_volume->logical_volume_descriptor,
		     (uint16_t *) &file_io_pool_entry,
		     (uint64_t *) &volume_offset,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve first block number from logical volume descriptor.",
			 function );

			return( -1 );
		}
		volume_offset *= internal_logical_volume->io_handle->block_size;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "Reading logical volume header at offset: %" PRIi64 " (0x%08" PRIx64 "):\n",
			 volume_offset + 1024,
			 volume_offset + 1024 );
		}
#endif
		result = libfvde_internal_logical_volume_open_read_volume_header(
			  internal_logical_volume,
			  file_io_pool,
			  file_io_pool_entry,
			  volume_offset + 1024,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read logical volume header.",
			 function );

			return( -1 );
		}
		else if( result != 0 )
		{
			internal_logical_volume->is_locked = 0;
		}
	}
	return( result );
}

/* Unlocks the logical volume
 * Returns 1 if the volume is unlocked, 0 if not or -1 on error
 */
int libfvde_logical_volume_unlock(
     libfvde_logical_volume_t *logical_volume,
     libcerror_error_t **error )
{
	libfvde_internal_logical_volume_t *internal_logical_volume = NULL;
	static char *function                                      = "libfvde_logical_volume_unlock";
	int result                                                 = 1;

	if( logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	internal_logical_volume = (libfvde_internal_logical_volume_t *) logical_volume;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_logical_volume->is_locked != 0 )
	{
		result = libfvde_internal_logical_volume_unlock(
		          internal_logical_volume,
		          internal_logical_volume->file_io_pool,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to unlock logical volume.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Reads data from the last current into a buffer using a Basic File IO (bfio) pool
 * This function is not multi-thread safe acquire write lock before call
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfvde_internal_logical_volume_read_buffer_from_file_io_pool(
         libfvde_internal_logical_volume_t *internal_logical_volume,
         libbfio_pool_t *file_io_pool,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libfvde_sector_data_t *sector_data = NULL;
	static char *function              = "libfvde_internal_logical_volume_read_buffer_from_file_io_pool";
	off64_t element_data_offset        = 0;
	size_t buffer_offset               = 0;
	size_t read_size                   = 0;
	size_t sector_data_offset          = 0;

	if( internal_logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	if( internal_logical_volume->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid logical volume - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_logical_volume->logical_volume_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid logical volume - missing logical volume descriptor.",
		 function );

		return( -1 );
	}
	if( internal_logical_volume->current_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid logical volume - current offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( internal_logical_volume->is_locked != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid logical volume - volume is locked.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( (size64_t) internal_logical_volume->current_offset >= internal_logical_volume->logical_volume_descriptor->size )
	{
		return( 0 );
	}
	if( (size64_t) buffer_size > ( internal_logical_volume->logical_volume_descriptor->size - internal_logical_volume->current_offset ) )
	{
		buffer_size = (size_t) ( internal_logical_volume->logical_volume_descriptor->size - internal_logical_volume->current_offset );
	}
	sector_data_offset = (size_t) ( internal_logical_volume->current_offset % internal_logical_volume->io_handle->bytes_per_sector );

	while( buffer_offset < buffer_size )
	{
		if( libfdata_vector_get_element_value_at_offset(
		     internal_logical_volume->sectors_vector,
		     (intptr_t *) file_io_pool,
		     (libfdata_cache_t *) internal_logical_volume->sectors_cache,
		     internal_logical_volume->current_offset,
		     &element_data_offset,
		     (intptr_t **) &sector_data,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sector data at offset: %" PRIi64 ".",
			 function,
			 internal_logical_volume->current_offset );

			return( -1 );
		}
		if( sector_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing sector data at offset: %" PRIi64 ".",
			 function,
			 internal_logical_volume->current_offset );

			return( -1 );
		}
		read_size = sector_data->data_size - sector_data_offset;

		if( read_size > ( buffer_size - buffer_offset ) )
		{
			read_size = buffer_size - buffer_offset;
		}
		if( read_size == 0 )
		{
			break;
		}
		if( memory_copy(
		     &( ( (uint8_t *) buffer )[ buffer_offset ] ),
		     &( ( sector_data->data )[ sector_data_offset ] ),
		     read_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy sector data to buffer.",
			 function );

			return( -1 );
		}
		buffer_offset     += read_size;
		sector_data_offset = 0;

		internal_logical_volume->current_offset += (off64_t) read_size;

		if( (size64_t) internal_logical_volume->current_offset >= internal_logical_volume->logical_volume_descriptor->size )
		{
			break;
		}
		if( internal_logical_volume->io_handle->abort != 0 )
		{
			break;
		}
	}
	return( (ssize_t) buffer_offset );
}

/* Reads data at the current offset into a buffer
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfvde_logical_volume_read_buffer(
         libfvde_logical_volume_t *logical_volume,
         void *buffer,
         size_t buffer_size,
         libcerror_error_t **error )
{
	libfvde_internal_logical_volume_t *internal_logical_volume = NULL;
	static char *function                                      = "libfvde_logical_volume_read_buffer";
	ssize_t read_count                                         = 0;

	if( logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	internal_logical_volume = (libfvde_internal_logical_volume_t *) logical_volume;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	read_count = libfvde_internal_logical_volume_read_buffer_from_file_io_pool(
		      internal_logical_volume,
		      internal_logical_volume->file_io_pool,
		      buffer,
		      buffer_size,
		      error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read buffer.",
		 function );

		read_count = -1;
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( read_count );
}

/* Reads data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
ssize_t libfvde_logical_volume_read_buffer_at_offset(
         libfvde_logical_volume_t *logical_volume,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error )
{
	libfvde_internal_logical_volume_t *internal_logical_volume = NULL;
	static char *function                                      = "libfvde_logical_volume_read_buffer_at_offset";
	ssize_t read_count                                         = 0;

	if( logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	internal_logical_volume = (libfvde_internal_logical_volume_t *) logical_volume;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libfvde_internal_logical_volume_seek_offset(
	     internal_logical_volume,
	     offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		read_count = -1;
	}
	else
	{
		read_count = libfvde_internal_logical_volume_read_buffer_from_file_io_pool(
			      internal_logical_volume,
			      internal_logical_volume->file_io_pool,
			      buffer,
			      buffer_size,
			      error );

		if( read_count == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read buffer.",
			 function );

			read_count = -1;
		}
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( read_count );
}

/* Seeks a certain offset of the data
 * This function is not multi-thread safe acquire write lock before call
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libfvde_internal_logical_volume_seek_offset(
         libfvde_internal_logical_volume_t *internal_logical_volume,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	static char *function = "libfvde_internal_logical_volume_seek_offset";

	if( internal_logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	if( internal_logical_volume->logical_volume_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid logical volume - missing logical volume descriptor.",
		 function );

		return( -1 );
	}
	if( internal_logical_volume->is_locked != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid logical volume - volume is locked.",
		 function );

		return( -1 );
	}
	if( ( whence != SEEK_CUR )
	 && ( whence != SEEK_END )
	 && ( whence != SEEK_SET ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported whence.",
		 function );

		return( -1 );
	}
	if( whence == SEEK_CUR )
	{	
		offset += internal_logical_volume->current_offset;
	}
	else if( whence == SEEK_END )
	{	
		offset += (off64_t) internal_logical_volume->logical_volume_descriptor->size;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: seeking media data offset: %" PRIi64 ".\n",
		 function,
		 offset );
	}
#endif
	if( offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid offset value out of bounds.",
		 function );

		return( -1 );
	}
	internal_logical_volume->current_offset = offset;

	return( offset );
}

/* Seeks a certain offset of the data
 * Returns the offset if seek is successful or -1 on error
 */
off64_t libfvde_logical_volume_seek_offset(
         libfvde_logical_volume_t *logical_volume,
         off64_t offset,
         int whence,
         libcerror_error_t **error )
{
	libfvde_internal_logical_volume_t *internal_logical_volume = NULL;
	static char *function                                      = "libfvde_logical_volume_seek_offset";

	if( logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	internal_logical_volume = (libfvde_internal_logical_volume_t *) logical_volume;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	offset = libfvde_internal_logical_volume_seek_offset(
	          internal_logical_volume,
	          offset,
	          whence,
	          error );

	if( offset == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek offset.",
		 function );

		offset = -1;
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( offset );
}

/* Retrieves the current offset of the data
 * Returns 1 if successful or -1 on error
 */
int libfvde_logical_volume_get_offset(
     libfvde_logical_volume_t *logical_volume,
     off64_t *offset,
     libcerror_error_t **error )
{
	libfvde_internal_logical_volume_t *internal_logical_volume = NULL;
	static char *function                                      = "libfvde_logical_volume_get_offset";

	if( logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	internal_logical_volume = (libfvde_internal_logical_volume_t *) logical_volume;

	if( offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*offset = internal_logical_volume->current_offset;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves the logical volume identifier
 * The identifier is a UUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
int libfvde_logical_volume_get_identifier(
     libfvde_logical_volume_t *logical_volume,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libcerror_error_t **error )
{
	libfvde_internal_logical_volume_t *internal_logical_volume = NULL;
	static char *function                                      = "libfvde_logical_volume_get_identifier";
	int result                                                 = 1;

	if( logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	internal_logical_volume = (libfvde_internal_logical_volume_t *) logical_volume;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( libfvde_logical_volume_descriptor_get_identifier(
	     internal_logical_volume->logical_volume_descriptor,
	     uuid_data,
	     uuid_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve logical volume identifier from descriptor.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfvde_logical_volume_get_utf8_name_size(
     libfvde_logical_volume_t *logical_volume,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libfvde_internal_logical_volume_t *internal_logical_volume = NULL;
	static char *function                                      = "libfvde_logical_volume_get_utf8_name_size";
	int result                                                 = 0;

	if( logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	internal_logical_volume = (libfvde_internal_logical_volume_t *) logical_volume;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libfvde_logical_volume_descriptor_get_utf8_name_size(
	          internal_logical_volume->logical_volume_descriptor,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-8 volume group name from descriptor.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfvde_logical_volume_get_utf8_name(
     libfvde_logical_volume_t *logical_volume,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libfvde_internal_logical_volume_t *internal_logical_volume = NULL;
	static char *function                                      = "libfvde_logical_volume_get_utf8_name";
	int result                                                 = 0;

	if( logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	internal_logical_volume = (libfvde_internal_logical_volume_t *) logical_volume;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libfvde_logical_volume_descriptor_get_utf8_name(
	          internal_logical_volume->logical_volume_descriptor,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 volume group name from descriptor.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfvde_logical_volume_get_utf16_name_size(
     libfvde_logical_volume_t *logical_volume,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libfvde_internal_logical_volume_t *internal_logical_volume = NULL;
	static char *function                                      = "libfvde_logical_volume_get_utf16_name_size";
	int result                                                 = 0;

	if( logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	internal_logical_volume = (libfvde_internal_logical_volume_t *) logical_volume;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libfvde_logical_volume_descriptor_get_utf16_name_size(
	          internal_logical_volume->logical_volume_descriptor,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of UTF-16 volume group name from descriptor.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfvde_logical_volume_get_utf16_name(
     libfvde_logical_volume_t *logical_volume,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libfvde_internal_logical_volume_t *internal_logical_volume = NULL;
	static char *function                                      = "libfvde_logical_volume_get_utf16_name";
	int result                                                 = 0;

	if( logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	internal_logical_volume = (libfvde_internal_logical_volume_t *) logical_volume;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libfvde_logical_volume_descriptor_get_utf16_name(
	          internal_logical_volume->logical_volume_descriptor,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 volume group name from descriptor.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the volume size
 * Returns 1 if successful or -1 on error
 */
int libfvde_logical_volume_get_size(
     libfvde_logical_volume_t *logical_volume,
     size64_t *size,
     libcerror_error_t **error )
{
	libfvde_internal_logical_volume_t *internal_logical_volume = NULL;
	static char *function                                      = "libfvde_logical_volume_get_size";

	if( logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	if( size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid size.",
		 function );

		return( -1 );
	}
	internal_logical_volume = (libfvde_internal_logical_volume_t *) logical_volume;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*size = internal_logical_volume->volume_size;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Determines if the logical volume is locked
 * Returns 1 if locked, 0 if not or -1 on error
 */
int libfvde_logical_volume_is_locked(
     libfvde_logical_volume_t *logical_volume,
     libcerror_error_t **error )
{
	libfvde_internal_logical_volume_t *internal_logical_volume = NULL;
	static char *function                                      = "libfvde_logical_volume_is_locked";
	uint8_t is_locked                                          = 0;

	if( logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	internal_logical_volume = (libfvde_internal_logical_volume_t *) logical_volume;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	is_locked = internal_logical_volume->is_locked;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( is_locked );
}

/* Sets the key
 * This function needs to be used before the unlock function
 * Returns 1 if successful or -1 on error
 */
int libfvde_logical_volume_set_key(
     libfvde_logical_volume_t *logical_volume,
     const uint8_t *volume_master_key,
     size_t volume_master_key_size,
     libcerror_error_t **error )
{
	libfvde_internal_logical_volume_t *internal_logical_volume = NULL;
	static char *function                                      = "libfvde_logical_volume_set_key";
	int result                                                 = 1;

	if( logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	internal_logical_volume = (libfvde_internal_logical_volume_t *) logical_volume;

	if( internal_logical_volume->keyring == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid logical volume - missing keyring handle.",
		 function );

		return( -1 );
	}
	if( volume_master_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume master key.",
		 function );

		return( -1 );
	}
	if( volume_master_key_size != 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported volume master key size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( memory_copy(
	     internal_logical_volume->keyring->volume_master_key,
	     volume_master_key,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy volume master key to keyring.",
		 function );

		result = -1;
	}
	else
	{
		internal_logical_volume->volume_master_key_is_set = 1;
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Sets an UTF-8 formatted password
 * This function needs to be used before the unlock function
 * Returns 1 if successful, 0 if password is invalid or -1 on error
 */
int libfvde_logical_volume_set_utf8_password(
     libfvde_logical_volume_t *logical_volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	libfvde_internal_logical_volume_t *internal_logical_volume = NULL;
	static char *function                                      = "libfvde_logical_volume_set_utf8_password";
	int result                                                 = 1;

	if( logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	internal_logical_volume = (libfvde_internal_logical_volume_t *) logical_volume;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_logical_volume->user_password != NULL )
	{
		if( memory_set(
		     internal_logical_volume->user_password,
		     0,
		     internal_logical_volume->user_password_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear user password.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_logical_volume->user_password );

		internal_logical_volume->user_password      = NULL;
		internal_logical_volume->user_password_size = 0;
	}
	if( result == 1 )
	{
		if( libfvde_password_copy_from_utf8_string(
		     &( internal_logical_volume->user_password ),
		     &( internal_logical_volume->user_password_size ),
		     utf8_string,
		     utf8_string_length,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set user password.",
			 function );

			result = -1;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		else if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: user password: %s\n",
			 function,
			 internal_logical_volume->user_password );
		}
#endif
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Sets an UTF-16 formatted password
 * This function needs to be used before the unlock function
 * Returns 1 if successful, 0 if password is invalid or -1 on error
 */
int libfvde_logical_volume_set_utf16_password(
     libfvde_logical_volume_t *logical_volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	libfvde_internal_logical_volume_t *internal_logical_volume = NULL;
	static char *function                                      = "libfvde_logical_volume_set_utf16_password";
	int result                                                 = 1;

	if( logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	internal_logical_volume = (libfvde_internal_logical_volume_t *) logical_volume;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_logical_volume->user_password != NULL )
	{
		if( memory_set(
		     internal_logical_volume->user_password,
		     0,
		     internal_logical_volume->user_password_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear user password.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_logical_volume->user_password );

		internal_logical_volume->user_password      = NULL;
		internal_logical_volume->user_password_size = 0;
	}
	if( result == 1 )
	{
		if( libfvde_password_copy_from_utf16_string(
		     &( internal_logical_volume->user_password ),
		     &( internal_logical_volume->user_password_size ),
		     utf16_string,
		     utf16_string_length,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set user password.",
			 function );

			result = -1;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		else if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: user password: %s\n",
			 function,
			 internal_logical_volume->user_password );
		}
#endif
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Sets an UTF-8 formatted recovery password
 * This function needs to be used before the unlock function
 * Returns 1 if successful, 0 if recovery password is invalid or -1 on error
 */
int libfvde_logical_volume_set_utf8_recovery_password(
     libfvde_logical_volume_t *logical_volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	libfvde_internal_logical_volume_t *internal_logical_volume = NULL;
	static char *function                                      = "libfvde_logical_volume_set_utf8_recovery_password";
	int result                                                 = 1;

	if( logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	internal_logical_volume = (libfvde_internal_logical_volume_t *) logical_volume;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_logical_volume->recovery_password != NULL )
	{
		if( memory_set(
		     internal_logical_volume->recovery_password,
		     0,
		     internal_logical_volume->recovery_password_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear recovery password.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_logical_volume->recovery_password );

		internal_logical_volume->recovery_password      = NULL;
		internal_logical_volume->recovery_password_size = 0;
	}
	if( result == 1 )
	{
		if( libfvde_password_copy_from_utf8_string(
		     &( internal_logical_volume->recovery_password ),
		     &( internal_logical_volume->recovery_password_size ),
		     utf8_string,
		     utf8_string_length,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set recovery password.",
			 function );

			result = -1;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		else if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: recovery password: %s\n",
			 function,
			 internal_logical_volume->recovery_password );
		}
#endif
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Sets an UTF-16 formatted recovery password
 * This function needs to be used before the unlock function
 * Returns 1 if successful, 0 if recovery password is invalid or -1 on error
 */
int libfvde_logical_volume_set_utf16_recovery_password(
     libfvde_logical_volume_t *logical_volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	libfvde_internal_logical_volume_t *internal_logical_volume = NULL;
	static char *function                                      = "libfvde_logical_volume_set_utf16_recovery_password";
	int result                                                 = 1;

	if( logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	internal_logical_volume = (libfvde_internal_logical_volume_t *) logical_volume;

#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( internal_logical_volume->recovery_password != NULL )
	{
		if( memory_set(
		     internal_logical_volume->recovery_password,
		     0,
		     internal_logical_volume->recovery_password_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear recovery password.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_logical_volume->recovery_password );

		internal_logical_volume->recovery_password      = NULL;
		internal_logical_volume->recovery_password_size = 0;
	}
	if( result == 1 )
	{
		if( libfvde_password_copy_from_utf16_string(
		     &( internal_logical_volume->recovery_password ),
		     &( internal_logical_volume->recovery_password_size ),
		     utf16_string,
		     utf16_string_length,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set recovery password.",
			 function );

			result = -1;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		else if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: recovery password: %s\n",
			 function,
			 internal_logical_volume->recovery_password );
		}
#endif
	}
#if defined( HAVE_LIBFVDE_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_logical_volume->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

