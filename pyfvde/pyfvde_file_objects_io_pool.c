/*
 * Python file objects IO pool functions
 *
 * Copyright (C) 2011-2022, Joachim Metz <joachim.metz@gmail.com>
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
#include <types.h>

#include "pyfvde_file_object_io_handle.h"
#include "pyfvde_file_objects_io_pool.h"
#include "pyfvde_integer.h"
#include "pyfvde_libbfio.h"
#include "pyfvde_libcerror.h"
#include "pyfvde_python.h"

/* Initializes the file objects IO pool
 * Returns 1 if successful or -1 on error
 */
int pyfvde_file_objects_pool_initialize(
     libbfio_pool_t **pool,
     PyObject *sequence_object,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle = NULL;
	PyObject *file_object            = NULL;
	static char *function            = "pyfvde_file_objects_pool_initialize";
	Py_ssize_t sequence_size         = 0;
	int element_index                = 0;
	int file_io_pool_entry           = 0;
	int number_of_elements           = 0;
	int result                       = 0;

	if( pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid pool.",
		 function );

		return( -1 );
	}
	if( *pool != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid pool value already set.",
		 function );

		return( -1 );
	}
	sequence_size = PySequence_Size(
	                 sequence_object );

	if( sequence_size > (Py_ssize_t) INT_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid sequence size value exceeds maximum.",
		 function );

		goto on_error;
	}
	number_of_elements = (int) sequence_size;

	if( libbfio_pool_initialize(
	     pool,
	     0,
	     LIBBFIO_POOL_UNLIMITED_NUMBER_OF_OPEN_HANDLES,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create pool.",
		 function );

		goto on_error;
	}
	for( element_index = 0;
	     element_index < number_of_elements;
	     element_index++ )
	{
		file_object = PySequence_GetItem(
		               sequence_object,
		               element_index );

		if( file_object == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing file object IO handle.",
			 function );

			goto on_error;
		}
		PyErr_Clear();

		result = PyObject_HasAttrString(
		          file_object,
		          "read" );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unsupported file object - missing read attribute.",
			 function );

			goto on_error;
		}
		PyErr_Clear();

		result = PyObject_HasAttrString(
		          file_object,
		          "seek" );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unsupported file object - missing seek attribute.",
			 function );

			goto on_error;
		}
		if( pyfvde_file_object_initialize(
		     &file_io_handle,
		     file_object,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create file IO handle.",
			 function );

			goto on_error;
		}
		/* Remove the reference created by PySequence_GetItem
		 */
		Py_DecRef(
		 (PyObject *) file_object );

		file_object = NULL;

		if( libbfio_pool_append_handle(
		     *pool,
		     &file_io_pool_entry,
		     file_io_handle,
		     access_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append file IO handle to pool.",
			 function );

			goto on_error;
		}
		file_io_handle = NULL;
	}
	return( 1 );

on_error:
	if( file_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) file_object );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	if( *pool != NULL )
	{
		libbfio_pool_free(
		 pool,
		 NULL );
	}
	return( -1 );
}

