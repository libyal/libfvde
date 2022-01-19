/*
 * Python object wrapper of libfvde_volume_group_t
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
#include "pyfvde_guid.h"
#include "pyfvde_libcerror.h"
#include "pyfvde_libfvde.h"
#include "pyfvde_logical_volume.h"
#include "pyfvde_logical_volumes.h"
#include "pyfvde_physical_volume.h"
#include "pyfvde_physical_volumes.h"
#include "pyfvde_python.h"
#include "pyfvde_unused.h"
#include "pyfvde_volume_group.h"

PyMethodDef pyfvde_volume_group_object_methods[] = {

	{ "get_identifier",
	  (PyCFunction) pyfvde_volume_group_get_identifier,
	  METH_NOARGS,
	  "get_identifier() -> Unicode string\n"
	  "\n"
	  "Retrieves the volume group identifier." },

	{ "get_name",
	  (PyCFunction) pyfvde_volume_group_get_name,
	  METH_NOARGS,
	  "get_name() -> Unicode string\n"
	  "\n"
	  "Retrieves the name." },

	{ "get_number_of_physical_volumes",
	  (PyCFunction) pyfvde_volume_group_get_number_of_physical_volumes,
	  METH_NOARGS,
	  "get_number_of_physical_volumes() -> Integer\n"
	  "\n"
	  "Retrieves the number of physical volumes." },

	{ "get_physical_volume",
	  (PyCFunction) pyfvde_volume_group_get_physical_volume,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_physical_volume(volume_index) -> Object\n"
	  "\n"
	  "Retrieves the physical volume specified by the index." },

	{ "get_number_of_logical_volumes",
	  (PyCFunction) pyfvde_volume_group_get_number_of_logical_volumes,
	  METH_NOARGS,
	  "get_number_of_logical_volumes() -> Integer\n"
	  "\n"
	  "Retrieves the number of logical volumes." },

	{ "get_logical_volume",
	  (PyCFunction) pyfvde_volume_group_get_logical_volume,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_logical_volume(volume_index) -> Object\n"
	  "\n"
	  "Retrieves the logical volume specified by the index." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyfvde_volume_group_object_get_set_definitions[] = {

	{ "identifier",
	  (getter) pyfvde_volume_group_get_identifier,
	  (setter) 0,
	  "The volume group identifier.",
	  NULL },

	{ "name",
	  (getter) pyfvde_volume_group_get_name,
	  (setter) 0,
	  "The name.",
	  NULL },

	{ "number_of_physical_volumes",
	  (getter) pyfvde_volume_group_get_number_of_physical_volumes,
	  (setter) 0,
	  "The number of physical volumes.",
	  NULL },

	{ "physical_volumes",
	  (getter) pyfvde_volume_group_get_physical_volumes,
	  (setter) 0,
	  "The physical volumes.",
	  NULL },

	{ "number_of_logical_volumes",
	  (getter) pyfvde_volume_group_get_number_of_logical_volumes,
	  (setter) 0,
	  "The number of logical volumes.",
	  NULL },

	{ "logical_volumes",
	  (getter) pyfvde_volume_group_get_logical_volumes,
	  (setter) 0,
	  "The logical volumes.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyfvde_volume_group_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyfvde.volume_group",
	/* tp_basicsize */
	sizeof( pyfvde_volume_group_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyfvde_volume_group_free,
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
	"pyfvde volume group object (wraps libfvde_volume_group_t)",
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
	pyfvde_volume_group_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pyfvde_volume_group_object_get_set_definitions,
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
	(initproc) pyfvde_volume_group_init,
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

/* Creates a new volume group object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_group_new(
           libfvde_volume_group_t *volume_group,
           PyObject *parent_object )
{
	pyfvde_volume_group_t *pyfvde_volume_group = NULL;
	static char *function                      = "pyfvde_volume_group_new";

	if( volume_group == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume group.",
		 function );

		return( NULL );
	}
	/* PyObject_New does not invoke tp_init
	 */
	pyfvde_volume_group = PyObject_New(
	                       struct pyfvde_volume_group,
	                       &pyfvde_volume_group_type_object );

	if( pyfvde_volume_group == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize volume group.",
		 function );

		goto on_error;
	}
	pyfvde_volume_group->volume_group  = volume_group;
	pyfvde_volume_group->parent_object = parent_object;

	if( pyfvde_volume_group->parent_object != NULL )
	{
		Py_IncRef(
		 pyfvde_volume_group->parent_object );
	}
	return( (PyObject *) pyfvde_volume_group );

on_error:
	if( pyfvde_volume_group != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyfvde_volume_group );
	}
	return( NULL );
}

/* Initializes a volume group object
 * Returns 0 if successful or -1 on error
 */
int pyfvde_volume_group_init(
     pyfvde_volume_group_t *pyfvde_volume_group )
{
	static char *function = "pyfvde_volume_group_init";

	if( pyfvde_volume_group == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume group.",
		 function );

		return( -1 );
	}
	/* Make sure libfvde volume group is set to NULL
	 */
	pyfvde_volume_group->volume_group = NULL;

	PyErr_Format(
	 PyExc_NotImplementedError,
	 "%s: initialize of volume group not supported.",
	 function );

	return( -1 );
}

/* Frees a volume group object
 */
void pyfvde_volume_group_free(
      pyfvde_volume_group_t *pyfvde_volume_group )
{
	struct _typeobject *ob_type = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pyfvde_volume_group_free";
	int result                  = 0;

	if( pyfvde_volume_group == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume group.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pyfvde_volume_group );

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
	if( pyfvde_volume_group->volume_group != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libfvde_volume_group_free(
		          &( pyfvde_volume_group->volume_group ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyfvde_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free libfvde volume group.",
			 function );

			libcerror_error_free(
			 &error );
		}
	}
	if( pyfvde_volume_group->parent_object != NULL )
	{
		Py_DecRef(
		 pyfvde_volume_group->parent_object );
	}
	ob_type->tp_free(
	 (PyObject*) pyfvde_volume_group );
}

/* Retrieves the volume group identifier
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_group_get_identifier(
           pyfvde_volume_group_t *pyfvde_volume_group,
           PyObject *arguments PYFVDE_ATTRIBUTE_UNUSED )
{
	uint8_t uuid_data[ 16 ];

	PyObject *string_object  = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfvde_volume_group_get_identifier";
	int result               = 0;

	PYFVDE_UNREFERENCED_PARAMETER( arguments )

	if( pyfvde_volume_group == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume group.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfvde_volume_group_get_identifier(
	          pyfvde_volume_group->volume_group,
	          uuid_data,
	          16,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve volume group identifier.",
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

/* Retrieves the name
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_group_get_name(
           pyfvde_volume_group_t *pyfvde_volume_group,
           PyObject *arguments PYFVDE_ATTRIBUTE_UNUSED )
{
	PyObject *string_object  = NULL;
	libcerror_error_t *error = NULL;
	const char *errors       = NULL;
	static char *function    = "pyfvde_volume_group_get_name";
	char *utf8_string        = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	PYFVDE_UNREFERENCED_PARAMETER( arguments )

	if( pyfvde_volume_group == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume group.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfvde_volume_group_get_utf8_name_size(
	          pyfvde_volume_group->volume_group,
	          &utf8_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to determine size of name as UTF-8 string.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( ( result == 0 )
	      || ( utf8_string_size == 0 ) )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	utf8_string = (char *) PyMem_Malloc(
	                        sizeof( char ) * utf8_string_size );

	if( utf8_string == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create UTF-8 string.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfvde_volume_group_get_utf8_name(
	          pyfvde_volume_group->volume_group,
	          (uint8_t *) utf8_string,
	          utf8_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve name as UTF-8 string.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Pass the string length to PyUnicode_DecodeUTF8 otherwise it makes
	 * the end of string character is part of the string.
	 */
	string_object = PyUnicode_DecodeUTF8(
	                 utf8_string,
	                 (Py_ssize_t) utf8_string_size - 1,
	                 errors );

	if( string_object == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to convert UTF-8 string into Unicode object.",
		 function );

		goto on_error;
	}
	PyMem_Free(
	 utf8_string );

	return( string_object );

on_error:
	if( utf8_string != NULL )
	{
		PyMem_Free(
		 utf8_string );
	}
	return( NULL );
}

/* Retrieves the number of physical volumes
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_group_get_number_of_physical_volumes(
           pyfvde_volume_group_t *pyfvde_volume_group,
           PyObject *arguments PYFVDE_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object       = NULL;
	libcerror_error_t *error       = NULL;
	static char *function          = "pyfvde_volume_group_get_number_of_physical_volumes";
	int number_of_physical_volumes = 0;
	int result                     = 0;

	PYFVDE_UNREFERENCED_PARAMETER( arguments )

	if( pyfvde_volume_group == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume group.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfvde_volume_group_get_number_of_physical_volumes(
	          pyfvde_volume_group->volume_group,
	          &number_of_physical_volumes,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of physical volumes.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) number_of_physical_volumes );
#else
	integer_object = PyInt_FromLong(
	                  (long) number_of_physical_volumes );
#endif
	return( integer_object );
}

/* Retrieves a specific physical volume by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_group_get_physical_volume_by_index(
           PyObject *pyfvde_volume_group,
           int physical_volume_index )
{
	PyObject *physical_volume_object           = NULL;
	libcerror_error_t *error                   = NULL;
	libfvde_physical_volume_t *physical_volume = NULL;
	static char *function                      = "pyfvde_volume_group_get_physical_volume_by_index";
	int result                                 = 0;

	if( pyfvde_volume_group == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume group.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfvde_volume_group_get_physical_volume_by_index(
	          ( (pyfvde_volume_group_t *) pyfvde_volume_group )->volume_group,
	          physical_volume_index,
	          &physical_volume,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve physical volume: %d.",
		 function,
		 physical_volume_index );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	physical_volume_object = pyfvde_physical_volume_new(
	                          physical_volume,
	                          pyfvde_volume_group );

	if( physical_volume_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create physical volume object.",
		 function );

		goto on_error;
	}
	return( physical_volume_object );

on_error:
	if( physical_volume != NULL )
	{
		libfvde_physical_volume_free(
		 &physical_volume,
		 NULL );
	}
	return( NULL );
}

/* Retrieves a specific physical volume
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_group_get_physical_volume(
           pyfvde_volume_group_t *pyfvde_volume_group,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *physical_volume_object = NULL;
	static char *keyword_list[]      = { "physical_volume_index", NULL };
	int physical_volume_index        = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &physical_volume_index ) == 0 )
	{
		return( NULL );
	}
	physical_volume_object = pyfvde_volume_group_get_physical_volume_by_index(
	                          (PyObject *) pyfvde_volume_group,
	                          physical_volume_index );

	return( physical_volume_object );
}

/* Retrieves a sequence and iterator object for the physical volumes
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_group_get_physical_volumes(
           pyfvde_volume_group_t *pyfvde_volume_group,
           PyObject *arguments PYFVDE_ATTRIBUTE_UNUSED )
{
	PyObject *sequence_object      = NULL;
	libcerror_error_t *error       = NULL;
	static char *function          = "pyfvde_volume_group_get_physical_volumes";
	int number_of_physical_volumes = 0;
	int result                     = 0;

	PYFVDE_UNREFERENCED_PARAMETER( arguments )

	if( pyfvde_volume_group == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume group.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfvde_volume_group_get_number_of_physical_volumes(
	          pyfvde_volume_group->volume_group,
	          &number_of_physical_volumes,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of physical volumes.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	sequence_object = pyfvde_physical_volumes_new(
	                   (PyObject *) pyfvde_volume_group,
	                   &pyfvde_volume_group_get_physical_volume_by_index,
	                   number_of_physical_volumes );

	if( sequence_object == NULL )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to create sequence object.",
		 function );

		return( NULL );
	}
	return( sequence_object );
}

/* Retrieves the number of logical volumes
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_group_get_number_of_logical_volumes(
           pyfvde_volume_group_t *pyfvde_volume_group,
           PyObject *arguments PYFVDE_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object      = NULL;
	libcerror_error_t *error      = NULL;
	static char *function         = "pyfvde_volume_group_get_number_of_logical_volumes";
	int number_of_logical_volumes = 0;
	int result                    = 0;

	PYFVDE_UNREFERENCED_PARAMETER( arguments )

	if( pyfvde_volume_group == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume group.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfvde_volume_group_get_number_of_logical_volumes(
	          pyfvde_volume_group->volume_group,
	          &number_of_logical_volumes,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of logical volumes.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) number_of_logical_volumes );
#else
	integer_object = PyInt_FromLong(
	                  (long) number_of_logical_volumes );
#endif
	return( integer_object );
}

/* Retrieves a specific logical volume by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_group_get_logical_volume_by_index(
           PyObject *pyfvde_volume_group,
           int logical_volume_index )
{
	PyObject *logical_volume_object          = NULL;
	libcerror_error_t *error                 = NULL;
	libfvde_logical_volume_t *logical_volume = NULL;
	static char *function                    = "pyfvde_volume_group_get_logical_volume_by_index";
	int result                               = 0;

	if( pyfvde_volume_group == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume group.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfvde_volume_group_get_logical_volume_by_index(
	          ( (pyfvde_volume_group_t *) pyfvde_volume_group )->volume_group,
	          logical_volume_index,
	          &logical_volume,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve logical volume: %d.",
		 function,
		 logical_volume_index );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	logical_volume_object = pyfvde_logical_volume_new(
	                         logical_volume,
	                         pyfvde_volume_group );

	if( logical_volume_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create logical volume object.",
		 function );

		goto on_error;
	}
	return( logical_volume_object );

on_error:
	if( logical_volume != NULL )
	{
		libfvde_logical_volume_free(
		 &logical_volume,
		 NULL );
	}
	return( NULL );
}

/* Retrieves a specific logical volume
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_group_get_logical_volume(
           pyfvde_volume_group_t *pyfvde_volume_group,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *logical_volume_object = NULL;
	static char *keyword_list[]     = { "logical_volume_index", NULL };
	int logical_volume_index        = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &logical_volume_index ) == 0 )
	{
		return( NULL );
	}
	logical_volume_object = pyfvde_volume_group_get_logical_volume_by_index(
	                         (PyObject *) pyfvde_volume_group,
	                         logical_volume_index );

	return( logical_volume_object );
}

/* Retrieves a sequence and iterator object for the logical volumes
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_group_get_logical_volumes(
           pyfvde_volume_group_t *pyfvde_volume_group,
           PyObject *arguments PYFVDE_ATTRIBUTE_UNUSED )
{
	PyObject *sequence_object     = NULL;
	libcerror_error_t *error      = NULL;
	static char *function         = "pyfvde_volume_group_get_logical_volumes";
	int number_of_logical_volumes = 0;
	int result                    = 0;

	PYFVDE_UNREFERENCED_PARAMETER( arguments )

	if( pyfvde_volume_group == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume group.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfvde_volume_group_get_number_of_logical_volumes(
	          pyfvde_volume_group->volume_group,
	          &number_of_logical_volumes,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of logical volumes.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	sequence_object = pyfvde_logical_volumes_new(
	                   (PyObject *) pyfvde_volume_group,
	                   &pyfvde_volume_group_get_logical_volume_by_index,
	                   number_of_logical_volumes );

	if( sequence_object == NULL )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to create sequence object.",
		 function );

		return( NULL );
	}
	return( sequence_object );
}

