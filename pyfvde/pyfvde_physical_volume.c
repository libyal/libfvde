/*
 * Python object wrapper of libfvde_physical_volume_t
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

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyfvde_error.h"
#include "pyfvde_integer.h"
#include "pyfvde_guid.h"
#include "pyfvde_libcerror.h"
#include "pyfvde_libfvde.h"
#include "pyfvde_physical_volume.h"
#include "pyfvde_python.h"
#include "pyfvde_unused.h"

PyMethodDef pyfvde_physical_volume_object_methods[] = {

	{ "get_identifier",
	  (PyCFunction) pyfvde_physical_volume_get_identifier,
	  METH_NOARGS,
	  "get_identifier() -> Unicode string\n"
	  "\n"
	  "Retrieves the physical volume identifier." },

	{ "get_encryption_method",
	  (PyCFunction) pyfvde_physical_volume_get_encryption_method,
	  METH_NOARGS,
	  "get_encryption_method() -> Integer\n"
	  "\n"
	  "Retrieves the volume encryption method." },

	{ "get_size",
	  (PyCFunction) pyfvde_physical_volume_get_size,
	  METH_NOARGS,
	  "get_size() -> Integer\n"
	  "\n"
	  "Retrieves the volume size." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyfvde_physical_volume_object_get_set_definitions[] = {

	{ "identifier",
	  (getter) pyfvde_physical_volume_get_identifier,
	  (setter) 0,
	  "The physical volume identifier.",
	  NULL },

	{ "encryption_method",
	  (getter) pyfvde_physical_volume_get_encryption_method,
	  (setter) 0,
	  "The volume encryption method.",
	  NULL },

	{ "size",
	  (getter) pyfvde_physical_volume_get_size,
	  (setter) 0,
	  "The volume size.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyfvde_physical_volume_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyfvde.physical_volume",
	/* tp_basicsize */
	sizeof( pyfvde_physical_volume_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyfvde_physical_volume_free,
	/* tp_print */
	0,
	/* tp_getattr */
	0,
	/* tp_setattr */
	0,
	/* tp_compare */
	0,
	/* tp_repr */
	0,
	/* tp_as_number */
	0,
	/* tp_as_sequence */
	0,
	/* tp_as_mapping */
	0,
	/* tp_hash */
	0,
	/* tp_call */
	0,
	/* tp_str */
	0,
	/* tp_getattro */
	0,
	/* tp_setattro */
	0,
	/* tp_as_buffer */
	0,
	/* tp_flags */
	Py_TPFLAGS_DEFAULT,
	/* tp_doc */
	"pyfvde physical volume object (wraps libfvde_physical_volume_t)",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	0,
	/* tp_iternext */
	0,
	/* tp_methods */
	pyfvde_physical_volume_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pyfvde_physical_volume_object_get_set_definitions,
	/* tp_base */
	0,
	/* tp_dict */
	0,
	/* tp_descr_get */
	0,
	/* tp_descr_set */
	0,
	/* tp_dictoffset */
	0,
	/* tp_init */
	(initproc) pyfvde_physical_volume_init,
	/* tp_alloc */
	0,
	/* tp_new */
	0,
	/* tp_free */
	0,
	/* tp_is_gc */
	0,
	/* tp_bases */
	NULL,
	/* tp_mro */
	NULL,
	/* tp_cache */
	NULL,
	/* tp_subclasses */
	NULL,
	/* tp_weaklist */
	NULL,
	/* tp_del */
	0
};

/* Creates a new physical volume object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_physical_volume_new(
           libfvde_physical_volume_t *physical_volume,
           PyObject *parent_object )
{
	pyfvde_physical_volume_t *pyfvde_physical_volume = NULL;
	static char *function                            = "pyfvde_physical_volume_new";

	if( physical_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid physical volume.",
		 function );

		return( NULL );
	}
	/* PyObject_New does not invoke tp_init
	 */
	pyfvde_physical_volume = PyObject_New(
	                          struct pyfvde_physical_volume,
	                          &pyfvde_physical_volume_type_object );

	if( pyfvde_physical_volume == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize physical volume.",
		 function );

		goto on_error;
	}
	pyfvde_physical_volume->physical_volume = physical_volume;
	pyfvde_physical_volume->parent_object   = parent_object;

	if( pyfvde_physical_volume->parent_object != NULL )
	{
		Py_IncRef(
		 pyfvde_physical_volume->parent_object );
	}
	return( (PyObject *) pyfvde_physical_volume );

on_error:
	if( pyfvde_physical_volume != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyfvde_physical_volume );
	}
	return( NULL );
}

/* Initializes a physical volume object
 * Returns 0 if successful or -1 on error
 */
int pyfvde_physical_volume_init(
     pyfvde_physical_volume_t *pyfvde_physical_volume )
{
	static char *function = "pyfvde_physical_volume_init";

	if( pyfvde_physical_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid physical volume.",
		 function );

		return( -1 );
	}
	/* Make sure libfvde physical volume is set to NULL
	 */
	pyfvde_physical_volume->physical_volume = NULL;

	PyErr_Format(
	 PyExc_NotImplementedError,
	 "%s: initialize of physical volume not supported.",
	 function );

	return( -1 );
}

/* Frees a physical volume object
 */
void pyfvde_physical_volume_free(
      pyfvde_physical_volume_t *pyfvde_physical_volume )
{
	struct _typeobject *ob_type = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pyfvde_physical_volume_free";
	int result                  = 0;

	if( pyfvde_physical_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid physical volume.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pyfvde_physical_volume );

	if( ob_type == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: missing ob_type.",
		 function );

		return;
	}
	if( ob_type->tp_free == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid ob_type - missing tp_free.",
		 function );

		return;
	}
	if( pyfvde_physical_volume->physical_volume != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libfvde_physical_volume_free(
		          &( pyfvde_physical_volume->physical_volume ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyfvde_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free libfvde physical volume.",
			 function );

			libcerror_error_free(
			 &error );
		}
	}
	if( pyfvde_physical_volume->parent_object != NULL )
	{
		Py_DecRef(
		 pyfvde_physical_volume->parent_object );
	}
	ob_type->tp_free(
	 (PyObject*) pyfvde_physical_volume );
}

/* Retrieves the physical volume identifier
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_physical_volume_get_identifier(
           pyfvde_physical_volume_t *pyfvde_physical_volume,
           PyObject *arguments PYFVDE_ATTRIBUTE_UNUSED )
{
	uint8_t uuid_data[ 16 ];

	PyObject *string_object  = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfvde_physical_volume_get_identifier";
	int result               = 0;

	PYFVDE_UNREFERENCED_PARAMETER( arguments )

	if( pyfvde_physical_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid physical volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfvde_physical_volume_get_identifier(
	          pyfvde_physical_volume->physical_volume,
	          uuid_data,
	          16,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve physical volume identifier.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	string_object = pyfvde_string_new_from_guid(
	                 uuid_data,
	                 16 );

	if( string_object == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to convert UUID into Unicode object.",
		 function );

		return( NULL );
	}
	return( string_object );
}

/* Retrieves the volume encryption method
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_physical_volume_get_encryption_method(
           pyfvde_physical_volume_t *pyfvde_physical_volume,
           PyObject *arguments PYFVDE_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfvde_physical_volume_get_encryption_method";
	uint32_t value_32bit     = 0;
	int result               = 0;

	PYFVDE_UNREFERENCED_PARAMETER( arguments )

	if( pyfvde_physical_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid physical volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfvde_physical_volume_get_encryption_method(
	          pyfvde_physical_volume->physical_volume,
	          &value_32bit,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve volume encryption method.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = PyLong_FromUnsignedLong(
	                  (unsigned long) value_32bit );

	return( integer_object );
}

/* Retrieves the volume size
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_physical_volume_get_size(
           pyfvde_physical_volume_t *pyfvde_physical_volume,
           PyObject *arguments PYFVDE_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfvde_physical_volume_get_size";
	size64_t size            = 0;
	int result               = 0;

	PYFVDE_UNREFERENCED_PARAMETER( arguments )

	if( pyfvde_physical_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid physical volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfvde_physical_volume_get_size(
	          pyfvde_physical_volume->physical_volume,
	          &size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: failed to retrieve volume size.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyfvde_integer_unsigned_new_from_64bit(
	                  (uint64_t) size );

	return( integer_object );
}

