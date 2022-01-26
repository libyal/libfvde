/*
 * Logical volume descriptor functions
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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libfvde_libcdata.h"
#include "libfvde_libcerror.h"
#include "libfvde_libuna.h"
#include "libfvde_logical_volume_descriptor.h"
#include "libfvde_segment_descriptor.h"

/* Creates logical volume descriptor
 * Make sure the value logical_volume_descriptor is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfvde_logical_volume_descriptor_initialize(
     libfvde_logical_volume_descriptor_t **logical_volume_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfvde_logical_volume_descriptor_initialize";

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
	if( *logical_volume_descriptor != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid logical volume descriptor value already set.",
		 function );

		return( -1 );
	}
	*logical_volume_descriptor = memory_allocate_structure(
	                              libfvde_logical_volume_descriptor_t );

	if( *logical_volume_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to logical volume descriptor.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *logical_volume_descriptor,
	     0,
	     sizeof( libfvde_logical_volume_descriptor_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear logical volume descriptor.",
		 function );

		memory_free(
		 *logical_volume_descriptor );

		*logical_volume_descriptor = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *logical_volume_descriptor )->segment_descriptors ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create segment descriptors array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *logical_volume_descriptor != NULL )
	{
		memory_free(
		 *logical_volume_descriptor );

		*logical_volume_descriptor = NULL;
	}
	return( -1 );
}

/* Frees logical volume descriptor
 * Returns 1 if successful or -1 on error
 */
int libfvde_logical_volume_descriptor_free(
     libfvde_logical_volume_descriptor_t **logical_volume_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfvde_logical_volume_descriptor_free";
	int result            = 1;

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
	if( *logical_volume_descriptor != NULL )
	{
		if( ( *logical_volume_descriptor )->name != NULL )
		{
			memory_free(
			 ( *logical_volume_descriptor )->name );
		}
		if( libcdata_array_free(
		     &( ( *logical_volume_descriptor )->segment_descriptors ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfvde_segment_descriptor_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free segment descriptors array.",
			 function );

			result = -1;
		}
		memory_free(
		 *logical_volume_descriptor );

		*logical_volume_descriptor = NULL;
	}
	return( result );
}

/* Retrieves the identifier
 * The identifier is a UUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
int libfvde_logical_volume_descriptor_get_identifier(
     libfvde_logical_volume_descriptor_t *logical_volume_descriptor,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libcerror_error_t **error )
{
	static char *function = "libfvde_logical_volume_descriptor_get_identifier";

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
	if( uuid_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UUID data.",
		 function );

		return( -1 );
	}
	if( ( uuid_data_size < 16 )
	 || ( uuid_data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UUID data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     uuid_data,
	     logical_volume_descriptor->identifier,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy identifier.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the family identifier
 * The identifier is a UUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
int libfvde_logical_volume_descriptor_get_family_identifier(
     libfvde_logical_volume_descriptor_t *logical_volume_descriptor,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libcerror_error_t **error )
{
	static char *function = "libfvde_logical_volume_descriptor_get_family_identifier";

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
	if( uuid_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UUID data.",
		 function );

		return( -1 );
	}
	if( ( uuid_data_size < 16 )
	 || ( uuid_data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid UUID data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     uuid_data,
	     logical_volume_descriptor->family_identifier,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy family identifier.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded volume group name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfvde_logical_volume_descriptor_get_utf8_name_size(
     libfvde_logical_volume_descriptor_t *logical_volume_descriptor,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfvde_logical_volume_descriptor_get_utf8_name_size";

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
	if( ( logical_volume_descriptor->name == NULL )
	 || ( logical_volume_descriptor->name_size == 0 ) )
	{
		return( 0 );
	}
	if( libuna_utf8_string_size_from_utf8_stream(
	     logical_volume_descriptor->name,
	     logical_volume_descriptor->name_size,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded volume group name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfvde_logical_volume_descriptor_get_utf8_name(
     libfvde_logical_volume_descriptor_t *logical_volume_descriptor,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfvde_logical_volume_descriptor_get_utf8_name";

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
	if( ( logical_volume_descriptor->name == NULL )
	 || ( logical_volume_descriptor->name_size == 0 ) )
	{
		return( 0 );
	}
	if( libuna_utf8_string_copy_from_utf8_stream(
	     utf8_string,
	     utf8_string_size,
	     logical_volume_descriptor->name,
	     logical_volume_descriptor->name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded volume group name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfvde_logical_volume_descriptor_get_utf16_name_size(
     libfvde_logical_volume_descriptor_t *logical_volume_descriptor,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfvde_logical_volume_descriptor_get_utf16_name_size";

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
	if( ( logical_volume_descriptor->name == NULL )
	 || ( logical_volume_descriptor->name_size == 0 ) )
	{
		return( 0 );
	}
	if( libuna_utf16_string_size_from_utf8_stream(
	     logical_volume_descriptor->name,
	     logical_volume_descriptor->name_size,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded volume group name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfvde_logical_volume_descriptor_get_utf16_name(
     libfvde_logical_volume_descriptor_t *logical_volume_descriptor,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	static char *function = "libfvde_logical_volume_descriptor_get_utf16_name";

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
	if( ( logical_volume_descriptor->name == NULL )
	 || ( logical_volume_descriptor->name_size == 0 ) )
	{
		return( 0 );
	}
	if( libuna_utf16_string_copy_from_utf8_stream(
	     utf16_string,
	     utf16_string_size,
	     logical_volume_descriptor->name,
	     logical_volume_descriptor->name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size
 * Returns 1 if successful or -1 on error
 */
int libfvde_logical_volume_descriptor_get_size(
     libfvde_logical_volume_descriptor_t *logical_volume_descriptor,
     size64_t *size,
     libcerror_error_t **error )
{
	static char *function = "libfvde_logical_volume_descriptor_get_size";

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
	*size = logical_volume_descriptor->size;

	return( 1 );
}

/* Retrieves the first block number
 * Returns 1 if successful or -1 on error
 */
int libfvde_logical_volume_descriptor_get_first_block_number(
     libfvde_logical_volume_descriptor_t *logical_volume_descriptor,
     uint16_t *volume_index,
     uint64_t *block_number,
     libcerror_error_t **error )
{
	libfvde_segment_descriptor_t *segment_descriptor = NULL;
	static char *function                            = "libfvde_logical_volume_descriptor_get_first_block_number";

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
	if( volume_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume index.",
		 function );

		return( -1 );
	}
	if( block_number == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block number.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     logical_volume_descriptor->segment_descriptors,
	     0,
	     (intptr_t **) &segment_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment descriptor: 0 from array.",
		 function );

		return( -1 );
	}
	if( segment_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing segment descriptor: 0.",
		 function );

		return( -1 );
	}
	*volume_index = segment_descriptor->physical_volume_index;
	*block_number = logical_volume_descriptor->base_physical_block_number + segment_descriptor->physical_block_number;

	return( 1 );
}

/* Retrieves the last block number
 * Returns 1 if successful or -1 on error
 */
int libfvde_logical_volume_descriptor_get_last_block_number(
     libfvde_logical_volume_descriptor_t *logical_volume_descriptor,
     uint16_t *volume_index,
     uint64_t *block_number,
     libcerror_error_t **error )
{
	libfvde_segment_descriptor_t *segment_descriptor = NULL;
	static char *function                            = "libfvde_logical_volume_descriptor_get_last_block_number";
	int number_of_segment_descriptors                = 0;

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
	if( volume_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume index.",
		 function );

		return( -1 );
	}
	if( block_number == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid block number.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     logical_volume_descriptor->segment_descriptors,
	     &number_of_segment_descriptors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of segment descriptors from array.",
		 function );

		return( -1 );
	}
	number_of_segment_descriptors -= 1;

	if( libcdata_array_get_entry_by_index(
	     logical_volume_descriptor->segment_descriptors,
	     number_of_segment_descriptors,
	     (intptr_t **) &segment_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment descriptor: %d from array.",
		 function,
		 number_of_segment_descriptors );

		return( -1 );
	}
	if( segment_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing segment descriptor: %d.",
		 function,
		 number_of_segment_descriptors );

		return( -1 );
	}
	*volume_index = segment_descriptor->physical_volume_index;
	*block_number = logical_volume_descriptor->base_physical_block_number + segment_descriptor->physical_block_number + segment_descriptor->number_of_blocks;

	return( 1 );
}

/* Retrieves the number of segment descriptors
 * Returns 1 if successful or -1 on error
 */
int libfvde_logical_volume_descriptor_get_number_of_segment_descriptors(
     libfvde_logical_volume_descriptor_t *logical_volume_descriptor,
     int *number_of_segment_descriptors,
     libcerror_error_t **error )
{
	static char *function = "libfvde_logical_volume_descriptor_get_number_of_segment_descriptors";

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
	if( libcdata_array_get_number_of_entries(
	     logical_volume_descriptor->segment_descriptors,
	     number_of_segment_descriptors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of segment descriptors from array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific segment descriptor
 * Returns 1 if successful or -1 on error
 */
int libfvde_logical_volume_descriptor_get_segment_descriptor_by_index(
     libfvde_logical_volume_descriptor_t *logical_volume_descriptor,
     int segment_index,
     libfvde_segment_descriptor_t **segment_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libfvde_logical_volume_descriptor_get_segment_descriptor_by_index";

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
	if( libcdata_array_get_entry_by_index(
	     logical_volume_descriptor->segment_descriptors,
	     segment_index,
	     (intptr_t **) segment_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment descriptor: %d from array.",
		 function,
		 segment_index );

		return( -1 );
	}
	return( 1 );
}
