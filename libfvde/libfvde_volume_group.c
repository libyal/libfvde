/*
 * Volume group functions
 *
 * Copyright (C) 2011-2018, Omar Choudary <choudary.omar@gmail.com>,
 *                          Joachim Metz <joachim.metz@gmail.com>
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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libfvde_libcdata.h"
#include "libfvde_libcerror.h"
#include "libfvde_types.h"
#include "libfvde_volume_group.h"

/* Creates a volume group
 * Make sure the value volume_group is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_group_initialize(
     libfvde_volume_group_t **volume_group,
     libcerror_error_t **error )
{
	libfvde_internal_volume_group_t *internal_volume_group = NULL;
	static char *function                                  = "libfvde_volume_group_initialize";

	if( volume_group == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume group.",
		 function );

		return( -1 );
	}
	if( *volume_group != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume group value already set.",
		 function );

		return( -1 );
	}
	internal_volume_group = memory_allocate_structure(
	                         libfvde_internal_volume_group_t );

	if( internal_volume_group == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create volume group.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_volume_group,
	     0,
	     sizeof( libfvde_internal_volume_group_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear volume group.",
		 function );

		memory_free(
		 internal_volume_group );

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( internal_volume_group->physical_volumes_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create physical volumes array.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( internal_volume_group->logical_volumes_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create logical volumes array.",
		 function );

		goto on_error;
	}
	*volume_group = (libfvde_volume_group_t *) internal_volume_group;

	return( 1 );

on_error:
	if( internal_volume_group != NULL )
	{
		if( internal_volume_group->physical_volumes_array != NULL )
		{
			libcdata_array_free(
			 &( internal_volume_group->physical_volumes_array ),
			 NULL,
			 NULL );
		}
		memory_free(
		 internal_volume_group );
	}
	return( -1 );
}

/* Frees a volume group
 * Returns 1 if successful or -1 on error
 */
int libfvde_volume_group_free(
     libfvde_volume_group_t **volume_group,
     libcerror_error_t **error )
{
	static char *function = "libfvde_volume_group_free";

	if( volume_group == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume group.",
		 function );

		return( -1 );
	}
	if( *volume_group != NULL )
	{
		*volume_group = NULL;
	}
	return( 1 );
}

/* Frees a volume group
 * Returns 1 if successful or -1 on error
 */
int libfvde_internal_volume_group_free(
     libfvde_internal_volume_group_t **internal_volume_group,
     libcerror_error_t **error )
{
	static char *function = "libfvde_internal_volume_group_free";
	int result            = 1;

	if( internal_volume_group == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume group.",
		 function );

		return( -1 );
	}
	if( *internal_volume_group != NULL )
	{
		/* The io_handle and physical_volume_file_io_pool references are freed elsewhere
		 */
		if( ( *internal_volume_group )->name != NULL )
		{
			memory_free(
			 ( *internal_volume_group )->name );
		}
/* TODO add free function */
		if( libcdata_array_free(
		     &( ( *internal_volume_group )->physical_volumes_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free physical volumes array.",
			 function );

			result = -1;
		}
/* TODO add free function */
		if( libcdata_array_free(
		     &( ( *internal_volume_group )->logical_volumes_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free logical volumes array.",
			 function );

			result = -1;
		}
		memory_free(
		 *internal_volume_group );

		*internal_volume_group = NULL;
	}
	return( result );
}

