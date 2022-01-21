/*
 * Python object wrapper of libfvde_logical_volume_t
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
#include <narrow_string.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyfvde_error.h"
#include "pyfvde_integer.h"
#include "pyfvde_guid.h"
#include "pyfvde_libcerror.h"
#include "pyfvde_libfvde.h"
#include "pyfvde_logical_volume.h"
#include "pyfvde_python.h"
#include "pyfvde_unused.h"

PyMethodDef pyfvde_logical_volume_object_methods[] = {

	{ "unlock",
	  (PyCFunction) pyfvde_logical_volume_unlock,
	  METH_NOARGS,
	  "unlock() -> Boolean\n"
	  "\n"
	  "Unlocks the logical volume." },

	{ "read_buffer",
	  (PyCFunction) pyfvde_logical_volume_read_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "read_buffer(size) -> Binary string\n"
	  "\n"
	  "Reads a buffer of data." },

	{ "read_buffer_at_offset",
	  (PyCFunction) pyfvde_logical_volume_read_buffer_at_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "read_buffer_at_offset(size, offset) -> Binary string\n"
	  "\n"
	  "Reads a buffer of data at a specific offset." },

	{ "seek_offset",
	  (PyCFunction) pyfvde_logical_volume_seek_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "seek_offset(offset, whence) -> None\n"
	  "\n"
	  "Seeks an offset within the data." },

	{ "get_offset",
	  (PyCFunction) pyfvde_logical_volume_get_offset,
	  METH_NOARGS,
	  "get_offset() -> Integer\n"
	  "\n"
	  "Retrieves the current offset of the data." },

	{ "read",
	  (PyCFunction) pyfvde_logical_volume_read_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "read(size) -> Binary string\n"
	  "\n"
	  "Reads a buffer of data." },

	{ "seek",
	  (PyCFunction) pyfvde_logical_volume_seek_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "seek(offset, whence) -> None\n"
	  "\n"
	  "Seeks an offset within the data." },

	{ "tell",
	  (PyCFunction) pyfvde_logical_volume_get_offset,
	  METH_NOARGS,
	  "tell() -> Integer\n"
	  "\n"
	  "Retrieves the current offset of the data." },

	{ "get_identifier",
	  (PyCFunction) pyfvde_logical_volume_get_identifier,
	  METH_NOARGS,
	  "get_identifier() -> Unicode string\n"
	  "\n"
	  "Retrieves the logical volume identifier." },

	{ "get_name",
	  (PyCFunction) pyfvde_logical_volume_get_name,
	  METH_NOARGS,
	  "get_name() -> Unicode string\n"
	  "\n"
	  "Retrieves the name." },

	{ "get_size",
	  (PyCFunction) pyfvde_logical_volume_get_size,
	  METH_NOARGS,
	  "get_size() -> Integer\n"
	  "\n"
	  "Retrieves the volume size." },

	{ "is_locked",
	  (PyCFunction) pyfvde_logical_volume_is_locked,
	  METH_NOARGS,
	  "is_locked() -> Boolean or None\n"
	  "\n"
	  "Determines if the logical volume is locked." },

	{ "set_key",
	  (PyCFunction) pyfvde_logical_volume_set_key,
	  METH_VARARGS | METH_KEYWORDS,
	  "set_key(key) -> None\n"
	  "\n"
	  "Sets the keys." },

	{ "set_password",
	  (PyCFunction) pyfvde_logical_volume_set_password,
	  METH_VARARGS | METH_KEYWORDS,
	  "set_password(password) -> None\n"
	  "\n"
	  "Sets the password." },

	{ "set_recovery_password",
	  (PyCFunction) pyfvde_logical_volume_set_recovery_password,
	  METH_VARARGS | METH_KEYWORDS,
	  "set_recovery_password(password) -> None\n"
	  "\n"
	  "Sets the recovery password." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyfvde_logical_volume_object_get_set_definitions[] = {

	{ "identifier",
	  (getter) pyfvde_logical_volume_get_identifier,
	  (setter) 0,
	  "The logical volume identifier.",
	  NULL },

	{ "name",
	  (getter) pyfvde_logical_volume_get_name,
	  (setter) 0,
	  "The name.",
	  NULL },

	{ "size",
	  (getter) pyfvde_logical_volume_get_size,
	  (setter) 0,
	  "The volume size.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyfvde_logical_volume_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyfvde.logical_volume",
	/* tp_basicsize */
	sizeof( pyfvde_logical_volume_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyfvde_logical_volume_free,
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
	"pyfvde logical volume object (wraps libfvde_logical_volume_t)",
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
	pyfvde_logical_volume_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pyfvde_logical_volume_object_get_set_definitions,
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
	(initproc) pyfvde_logical_volume_init,
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

/* Unlocks the logical volume
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_logical_volume_unlock(
           pyfvde_logical_volume_t *pyfvde_logical_volume,
           PyObject *arguments PYFVDE_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyfvde_logical_volume_unlock";
	int result               = 0;

	PYFVDE_UNREFERENCED_PARAMETER( arguments )

	if( pyfvde_logical_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid logical volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfvde_logical_volume_unlock(
	          pyfvde_logical_volume->logical_volume,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to unlock logical volume.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( result != 0 )
	{
		Py_IncRef(
		 (PyObject *) Py_True );

		return( Py_True );
	}
	Py_IncRef(
	 (PyObject *) Py_False );

	return( Py_False );
}

/* Creates a new logical volume object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_logical_volume_new(
           libfvde_logical_volume_t *logical_volume,
           PyObject *parent_object )
{
	pyfvde_logical_volume_t *pyfvde_logical_volume = NULL;
	static char *function                          = "pyfvde_logical_volume_new";

	if( logical_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid logical volume.",
		 function );

		return( NULL );
	}
	/* PyObject_New does not invoke tp_init
	 */
	pyfvde_logical_volume = PyObject_New(
	                         struct pyfvde_logical_volume,
	                         &pyfvde_logical_volume_type_object );

	if( pyfvde_logical_volume == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize logical volume.",
		 function );

		goto on_error;
	}
	pyfvde_logical_volume->logical_volume = logical_volume;
	pyfvde_logical_volume->parent_object  = parent_object;

	if( pyfvde_logical_volume->parent_object != NULL )
	{
		Py_IncRef(
		 pyfvde_logical_volume->parent_object );
	}
	return( (PyObject *) pyfvde_logical_volume );

on_error:
	if( pyfvde_logical_volume != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyfvde_logical_volume );
	}
	return( NULL );
}

/* Initializes a logical volume object
 * Returns 0 if successful or -1 on error
 */
int pyfvde_logical_volume_init(
     pyfvde_logical_volume_t *pyfvde_logical_volume )
{
	static char *function = "pyfvde_logical_volume_init";

	if( pyfvde_logical_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	/* Make sure libfvde logical volume is set to NULL
	 */
	pyfvde_logical_volume->logical_volume = NULL;

	PyErr_Format(
	 PyExc_NotImplementedError,
	 "%s: initialize of logical volume not supported.",
	 function );

	return( -1 );
}

/* Frees a logical volume object
 */
void pyfvde_logical_volume_free(
      pyfvde_logical_volume_t *pyfvde_logical_volume )
{
	struct _typeobject *ob_type = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pyfvde_logical_volume_free";
	int result                  = 0;

	if( pyfvde_logical_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid logical volume.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pyfvde_logical_volume );

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
	if( pyfvde_logical_volume->logical_volume != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libfvde_logical_volume_free(
		          &( pyfvde_logical_volume->logical_volume ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyfvde_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free libfvde logical volume.",
			 function );

			libcerror_error_free(
			 &error );
		}
	}
	if( pyfvde_logical_volume->parent_object != NULL )
	{
		Py_DecRef(
		 pyfvde_logical_volume->parent_object );
	}
	ob_type->tp_free(
	 (PyObject*) pyfvde_logical_volume );
}

/* Reads data at the current offset into a buffer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_logical_volume_read_buffer(
           pyfvde_logical_volume_t *pyfvde_logical_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *integer_object    = NULL;
	PyObject *string_object     = NULL;
	libcerror_error_t *error    = NULL;
	char *buffer                = NULL;
	static char *function       = "pyfvde_logical_volume_read_buffer";
	static char *keyword_list[] = { "size", NULL };
	ssize_t read_count          = 0;
	int64_t read_size           = 0;
	int result                  = 0;

	if( pyfvde_logical_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid logical volume.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|O",
	     keyword_list,
	     &integer_object ) == 0 )
	{
		return( NULL );
	}
	if( integer_object == NULL )
	{
		result = 0;
	}
	else
	{
		result = PyObject_IsInstance(
		          integer_object,
		          (PyObject *) &PyLong_Type );

		if( result == -1 )
		{
			pyfvde_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to determine if integer object is of type long.",
			 function );

			return( NULL );
		}
#if PY_MAJOR_VERSION < 3
		else if( result == 0 )
		{
			PyErr_Clear();

			result = PyObject_IsInstance(
			          integer_object,
			          (PyObject *) &PyInt_Type );

			if( result == -1 )
			{
				pyfvde_error_fetch_and_raise(
				 PyExc_RuntimeError,
				 "%s: unable to determine if integer object is of type int.",
				 function );

				return( NULL );
			}
		}
#endif
	}
	if( result != 0 )
	{
		if( pyfvde_integer_signed_copy_to_64bit(
		     integer_object,
		     &read_size,
		     &error ) != 1 )
		{
			pyfvde_error_raise(
			 error,
			 PyExc_ValueError,
			 "%s: unable to convert integer object into read size.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	else if( ( integer_object == NULL )
	      || ( integer_object == Py_None ) )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libfvde_logical_volume_get_size(
		          pyfvde_logical_volume->logical_volume,
		          (size64_t *) &read_size,
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyfvde_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to retrieve size.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	else
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: unsupported integer object type.",
		 function );

		return( NULL );
	}
	if( read_size == 0 )
	{
#if PY_MAJOR_VERSION >= 3
		string_object = PyBytes_FromString(
		                 "" );
#else
		string_object = PyString_FromString(
		                 "" );
#endif
		return( string_object );
	}
	if( read_size < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid read size value less than zero.",
		 function );

		return( NULL );
	}
	/* Make sure the data fits into a memory buffer
	 */
	if( ( read_size > (int64_t) INT_MAX )
	 || ( read_size > (int64_t) SSIZE_MAX ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value exceeds maximum.",
		 function );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromStringAndSize(
	                 NULL,
	                 (Py_ssize_t) read_size );

	buffer = PyBytes_AsString(
	          string_object );
#else
	/* Note that a size of 0 is not supported
	 */
	string_object = PyString_FromStringAndSize(
	                 NULL,
	                 (Py_ssize_t) read_size );

	buffer = PyString_AsString(
	          string_object );
#endif
	Py_BEGIN_ALLOW_THREADS

	read_count = libfvde_logical_volume_read_buffer(
	              pyfvde_logical_volume->logical_volume,
	              (uint8_t *) buffer,
	              (size_t) read_size,
	              &error );

	Py_END_ALLOW_THREADS

	if( read_count == -1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to read data.",
		 function );

		libcerror_error_free(
		 &error );

		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	/* Need to resize the string here in case read_size was not fully read.
	 */
#if PY_MAJOR_VERSION >= 3
	if( _PyBytes_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#else
	if( _PyString_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#endif
	{
		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	return( string_object );
}

/* Reads data at a specific offset into a buffer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_logical_volume_read_buffer_at_offset(
           pyfvde_logical_volume_t *pyfvde_logical_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *integer_object    = NULL;
	PyObject *string_object     = NULL;
	libcerror_error_t *error    = NULL;
	char *buffer                = NULL;
	static char *function       = "pyfvde_logical_volume_read_buffer_at_offset";
	static char *keyword_list[] = { "size", "offset", NULL };
	ssize_t read_count          = 0;
	off64_t read_offset         = 0;
	int64_t read_size           = 0;
	int result                  = 0;

	if( pyfvde_logical_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid logical volume.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "OL",
	     keyword_list,
	     &integer_object,
	     &read_offset ) == 0 )
	{
		return( NULL );
	}
	result = PyObject_IsInstance(
	          integer_object,
	          (PyObject *) &PyLong_Type );

	if( result == -1 )
	{
		pyfvde_error_fetch_and_raise(
		 PyExc_RuntimeError,
		 "%s: unable to determine if integer object is of type long.",
		 function );

		return( NULL );
	}
#if PY_MAJOR_VERSION < 3
	else if( result == 0 )
	{
		PyErr_Clear();

		result = PyObject_IsInstance(
		          integer_object,
		          (PyObject *) &PyInt_Type );

		if( result == -1 )
		{
			pyfvde_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to determine if integer object is of type int.",
			 function );

			return( NULL );
		}
	}
#endif
	if( result != 0 )
	{
		if( pyfvde_integer_signed_copy_to_64bit(
		     integer_object,
		     &read_size,
		     &error ) != 1 )
		{
			pyfvde_error_raise(
			 error,
			 PyExc_ValueError,
			 "%s: unable to convert integer object into read size.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	else
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: unsupported integer object type.",
		 function );

		return( NULL );
	}
	if( read_size == 0 )
	{
#if PY_MAJOR_VERSION >= 3
		string_object = PyBytes_FromString(
		                 "" );
#else
		string_object = PyString_FromString(
		                 "" );
#endif
		return( string_object );
	}
	if( read_size < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid read size value less than zero.",
		 function );

		return( NULL );
	}
	/* Make sure the data fits into a memory buffer
	 */
	if( ( read_size > (int64_t) INT_MAX )
	 || ( read_size > (int64_t) SSIZE_MAX ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value exceeds maximum.",
		 function );

		return( NULL );
	}
	if( read_offset < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid read offset value less than zero.",
		 function );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromStringAndSize(
	                 NULL,
	                 (Py_ssize_t) read_size );

	buffer = PyBytes_AsString(
	          string_object );
#else
	/* Note that a size of 0 is not supported
	 */
	string_object = PyString_FromStringAndSize(
	                 NULL,
	                 (Py_ssize_t) read_size );

	buffer = PyString_AsString(
	          string_object );
#endif
	Py_BEGIN_ALLOW_THREADS

	read_count = libfvde_logical_volume_read_buffer_at_offset(
	              pyfvde_logical_volume->logical_volume,
	              (uint8_t *) buffer,
	              (size_t) read_size,
	              (off64_t) read_offset,
	              &error );

	Py_END_ALLOW_THREADS

	if( read_count == -1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to read data.",
		 function );

		libcerror_error_free(
		 &error );

		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	/* Need to resize the string here in case read_size was not fully read.
	 */
#if PY_MAJOR_VERSION >= 3
	if( _PyBytes_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#else
	if( _PyString_Resize(
	     &string_object,
	     (Py_ssize_t) read_count ) != 0 )
#endif
	{
		Py_DecRef(
		 (PyObject *) string_object );

		return( NULL );
	}
	return( string_object );
}

/* Seeks a certain offset
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_logical_volume_seek_offset(
           pyfvde_logical_volume_t *pyfvde_logical_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	static char *function       = "pyfvde_logical_volume_seek_offset";
	static char *keyword_list[] = { "offset", "whence", NULL };
	off64_t offset              = 0;
	int whence                  = 0;

	if( pyfvde_logical_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid logical volume.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "L|i",
	     keyword_list,
	     &offset,
	     &whence ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	offset = libfvde_logical_volume_seek_offset(
	          pyfvde_logical_volume->logical_volume,
	          offset,
	          whence,
	          &error );

	Py_END_ALLOW_THREADS

	if( offset == -1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to seek offset.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Retrieves the current offset of the data
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_logical_volume_get_offset(
           pyfvde_logical_volume_t *pyfvde_logical_volume,
           PyObject *arguments PYFVDE_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfvde_logical_volume_get_offset";
	off64_t offset           = 0;
	int result               = 0;

	PYFVDE_UNREFERENCED_PARAMETER( arguments )

	if( pyfvde_logical_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid logical volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfvde_logical_volume_get_offset(
	          pyfvde_logical_volume->logical_volume,
	          &offset,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve current offset of the data.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	integer_object = pyfvde_integer_signed_new_from_64bit(
	                  (int64_t) offset );

	return( integer_object );
}

/* Retrieves the logical volume identifier
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_logical_volume_get_identifier(
           pyfvde_logical_volume_t *pyfvde_logical_volume,
           PyObject *arguments PYFVDE_ATTRIBUTE_UNUSED )
{
	uint8_t uuid_data[ 16 ];

	PyObject *string_object  = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfvde_logical_volume_get_identifier";
	int result               = 0;

	PYFVDE_UNREFERENCED_PARAMETER( arguments )

	if( pyfvde_logical_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid logical volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfvde_logical_volume_get_identifier(
	          pyfvde_logical_volume->logical_volume,
	          uuid_data,
	          16,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve logical volume identifier.",
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
PyObject *pyfvde_logical_volume_get_name(
           pyfvde_logical_volume_t *pyfvde_logical_volume,
           PyObject *arguments PYFVDE_ATTRIBUTE_UNUSED )
{
	PyObject *string_object  = NULL;
	libcerror_error_t *error = NULL;
	const char *errors       = NULL;
	static char *function    = "pyfvde_logical_volume_get_name";
	char *utf8_string        = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	PYFVDE_UNREFERENCED_PARAMETER( arguments )

	if( pyfvde_logical_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid logical volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfvde_logical_volume_get_utf8_name_size(
	          pyfvde_logical_volume->logical_volume,
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

	result = libfvde_logical_volume_get_utf8_name(
	          pyfvde_logical_volume->logical_volume,
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

/* Retrieves the volume size
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_logical_volume_get_size(
           pyfvde_logical_volume_t *pyfvde_logical_volume,
           PyObject *arguments PYFVDE_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyfvde_logical_volume_get_size";
	size64_t size            = 0;
	int result               = 0;

	PYFVDE_UNREFERENCED_PARAMETER( arguments )

	if( pyfvde_logical_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid logical volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfvde_logical_volume_get_size(
	          pyfvde_logical_volume->logical_volume,
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

/* Determines if the logical volume is 
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_logical_volume_is_locked(
           pyfvde_logical_volume_t *pyfvde_logical_volume,
           PyObject *arguments PYFVDE_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyfvde_logical_volume_is_locked";
	int result               = 0;

	PYFVDE_UNREFERENCED_PARAMETER( arguments )

	if( pyfvde_logical_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid logical volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfvde_logical_volume_is_locked(
	          pyfvde_logical_volume->logical_volume,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to determine if logical volume is locked.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( result != 0 )
	{
		Py_IncRef(
		 (PyObject *) Py_True );

		return( Py_True );
	}
	Py_IncRef(
	 (PyObject *) Py_False );

	return( Py_False );
}

/* Sets the key
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_logical_volume_set_key(
           pyfvde_logical_volume_t *pyfvde_logical_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *key_string_object = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pyfvde_logical_volume_set_key";
	char *key_data              = NULL;
	static char *keyword_list[] = { "key", NULL };
	Py_ssize_t key_data_size    = 0;
	int result                  = 0;

	if( pyfvde_logical_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid logical volume.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O",
	     keyword_list,
	     &key_string_object ) == 0 )
	{
		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	key_data = PyBytes_AsString(
	            key_string_object );

	key_data_size = PyBytes_Size(
	                 key_string_object );
#else
	key_data = PyString_AsString(
	            key_string_object );

	key_data_size = PyString_Size(
	                 key_string_object );
#endif
	if( ( key_data_size < 0 )
	 || ( key_data_size > (Py_ssize_t) ( SSIZE_MAX / 8 ) ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key data size value out of bounds.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfvde_logical_volume_set_key(
	          pyfvde_logical_volume->logical_volume,
	          (uint8_t *) key_data,
	          (size_t) ( key_data_size * 8 ),
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to set keys.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Sets the password
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_logical_volume_set_password(
           pyfvde_logical_volume_t *pyfvde_logical_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	static char *function       = "pyfvde_logical_volume_set_password";
	static char *keyword_list[] = { "password", NULL };
	char *utf8_string           = NULL;
	size_t utf8_string_length   = 0;
	int result                  = 0;

	if( pyfvde_logical_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid logical volume.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s",
	     keyword_list,
	     &utf8_string ) == 0 )
	{
		return( NULL );
	}
	if( utf8_string == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid password.",
		 function );

		return( NULL );
	}
	utf8_string_length = narrow_string_length(
	                      utf8_string );

	Py_BEGIN_ALLOW_THREADS

	result = libfvde_logical_volume_set_utf8_password(
	          pyfvde_logical_volume->logical_volume,
	          (uint8_t *) utf8_string,
	          utf8_string_length,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to set password.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Sets the recovery password
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_logical_volume_set_recovery_password(
           pyfvde_logical_volume_t *pyfvde_logical_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	static char *function       = "pyfvde_logical_volume_set_recovery_password";
	static char *keyword_list[] = { "password", NULL };
	char *utf8_string           = NULL;
	size_t utf8_string_length   = 0;
	int result                  = 0;

	if( pyfvde_logical_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid logical volume.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s",
	     keyword_list,
	     &utf8_string ) == 0 )
	{
		return( NULL );
	}
	if( utf8_string == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid recovery password.",
		 function );

		return( NULL );
	}
	utf8_string_length = narrow_string_length(
	                      utf8_string );

	Py_BEGIN_ALLOW_THREADS

	result = libfvde_logical_volume_set_utf8_recovery_password(
	          pyfvde_logical_volume->logical_volume,
	          (uint8_t *) utf8_string,
	          utf8_string_length,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to set recovery password.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

