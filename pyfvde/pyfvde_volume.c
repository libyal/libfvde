/*
 * Python object wrapper of libfvde_volume_t
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
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyfvde_datetime.h"
#include "pyfvde_error.h"
#include "pyfvde_file_object_io_handle.h"
#include "pyfvde_file_objects_io_pool.h"
#include "pyfvde_integer.h"
#include "pyfvde_guid.h"
#include "pyfvde_libbfio.h"
#include "pyfvde_libcerror.h"
#include "pyfvde_libclocale.h"
#include "pyfvde_libfvde.h"
#include "pyfvde_python.h"
#include "pyfvde_unused.h"
#include "pyfvde_volume.h"
#include "pyfvde_volume_group.h"

#if !defined( LIBFVDE_HAVE_BFIO )

LIBFVDE_EXTERN \
int libfvde_volume_open_file_io_handle(
     libfvde_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libfvde_error_t **error );

LIBFVDE_EXTERN \
int libfvde_volume_open_physical_volume_files_file_io_pool(
     libfvde_volume_t *volume,
     libbfio_pool_t *file_io_pool,
     libfvde_error_t **error );

#endif /* !defined( LIBFVDE_HAVE_BFIO ) */

PyMethodDef pyfvde_volume_object_methods[] = {

	{ "signal_abort",
	  (PyCFunction) pyfvde_volume_signal_abort,
	  METH_NOARGS,
	  "signal_abort() -> None\n"
	  "\n"
	  "Signals the volume to abort the current activity." },

	/* Functions to access the volume */

	{ "open",
	  (PyCFunction) pyfvde_volume_open,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(filename, mode='r') -> None\n"
	  "\n"
	  "Opens a volume." },

	{ "open_file_object",
	  (PyCFunction) pyfvde_volume_open_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "open_file_object(file_object, mode='r') -> None\n"
	  "\n"
	  "Opens a volume using a file-like object." },

	{ "open_physical_volume_files",
	  (PyCFunction) pyfvde_volume_open_physical_volume_files,
	  METH_VARARGS | METH_KEYWORDS,
	  "open_physical_volume_files() -> None\n"
	  "\n"
	  "Opens the physical volume files." },

	{ "open_physical_volume_files_as_file_objects",
	  (PyCFunction) pyfvde_volume_open_physical_volume_files_as_file_objects,
	  METH_VARARGS | METH_KEYWORDS,
	  "open_physical_volume_files_as_file_objects(file_objects) -> None\n"
	  "\n"
	  "Opens physical volume files using a list of file-like objects." },

	{ "close",
	  (PyCFunction) pyfvde_volume_close,
	  METH_NOARGS,
	  "close() -> None\n"
	  "\n"
	  "Closes a volume." },

	{ "is_locked",
	  (PyCFunction) pyfvde_volume_is_locked,
	  METH_NOARGS,
	  "is_locked() -> Boolean\n"
	  "\n"
	  "Determines if the volume is locked." },

	{ "unlock",
	  (PyCFunction) pyfvde_volume_unlock,
	  METH_NOARGS,
	  "unlock() -> Boolean\n"
	  "\n"
	  "Unlocks the volume." },

	{ "read_buffer",
	  (PyCFunction) pyfvde_volume_read_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "read_buffer(size) -> Binary string\n"
	  "\n"
	  "Reads a buffer of volume data." },

	{ "read_buffer_at_offset",
	  (PyCFunction) pyfvde_volume_read_buffer_at_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "read_buffer_at_offset(size, offset) -> Binary string\n"
	  "\n"
	  "Reads a buffer of volume data at a specific offset." },

	{ "seek_offset",
	  (PyCFunction) pyfvde_volume_seek_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "seek_offset(offset, whence) -> None\n"
	  "\n"
	  "Seeks an offset within the volume data." },

	{ "get_offset",
	  (PyCFunction) pyfvde_volume_get_offset,
	  METH_NOARGS,
	  "get_offset() -> Integer\n"
	  "\n"
	  "Retrieved the current offset within the volume data." },

	/* Some Pythonesque aliases */

	{ "read",
	  (PyCFunction) pyfvde_volume_read_buffer,
	  METH_VARARGS | METH_KEYWORDS,
	  "read(size) -> Binary string\n"
	  "\n"
	  "Reads a buffer of volume data." },

	{ "seek",
	  (PyCFunction) pyfvde_volume_seek_offset,
	  METH_VARARGS | METH_KEYWORDS,
	  "seek(offset, whence) -> None\n"
	  "\n"
	  "Seeks an offset within the volume data." },

	{ "tell",
	  (PyCFunction) pyfvde_volume_get_offset,
	  METH_NOARGS,
	  "tell() -> Integer\n"
	  "\n"
	  "Retrieves the current offset within the volume data." },

	/* Functions to access the volume values */

	{ "get_size",
	  (PyCFunction) pyfvde_volume_get_size,
	  METH_NOARGS,
	  "get_size() -> Integer\n"
	  "\n"
	  "Retrieves the size of the volume data." },

	{ "set_keys",
	  (PyCFunction) pyfvde_volume_set_keys,
	  METH_VARARGS | METH_KEYWORDS,
	  "set_keys(volume_master_key) -> None\n"
	  "\n"
	  "Sets the keys." },

	{ "set_password",
	  (PyCFunction) pyfvde_volume_set_password,
	  METH_VARARGS | METH_KEYWORDS,
	  "set_password(password) -> None\n"
	  "\n"
	  "Sets the password." },

	{ "set_recovery_password",
	  (PyCFunction) pyfvde_volume_set_recovery_password,
	  METH_VARARGS | METH_KEYWORDS,
	  "set_recovery_password(recovery_password) -> None\n"
	  "\n"
	  "Sets the recovery password." },

	{ "read_encrypted_root_plist",
	  (PyCFunction) pyfvde_volume_read_encrypted_root_plist,
	  METH_VARARGS | METH_KEYWORDS,
	  "read_encrypted_root_plist(filename) -> None\n"
	  "\n"
	  "Reads the EncryptedRoot.plist from a file." },

	/* Functions to access the volume group */

	{ "get_volume_group",
	  (PyCFunction) pyfvde_volume_get_volume_group,
	  METH_NOARGS,
	  "get_volume_group() -> Object\n"
	  "\n"
	  "Retrieves the volume group." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyfvde_volume_object_get_set_definitions[] = {

	{ "locked",
	  (getter) pyfvde_volume_is_locked,
	  (setter) 0,
	  "Value to indicate the volume is locked.",
	  NULL },

	{ "size",
	  (getter) pyfvde_volume_get_size,
	  (setter) 0,
	  "The size.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyfvde_volume_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyfvde.volume",
	/* tp_basicsize */
	sizeof( pyfvde_volume_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyfvde_volume_free,
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
	"pyfvde volume object (wraps libfvde_volume_t)",
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
	pyfvde_volume_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pyfvde_volume_object_get_set_definitions,
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
	(initproc) pyfvde_volume_init,
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

/* Initializes a volume object
 * Returns 0 if successful or -1 on error
 */
int pyfvde_volume_init(
     pyfvde_volume_t *pyfvde_volume )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyfvde_volume_init";

	if( pyfvde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( -1 );
	}
	/* Make sure libfvde volume is set to NULL
	 */
	pyfvde_volume->volume         = NULL;
	pyfvde_volume->file_io_handle = NULL;
	pyfvde_volume->file_io_pool   = NULL;

	if( libfvde_volume_initialize(
	     &( pyfvde_volume->volume ),
	     &error ) != 1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to initialize volume.",
		 function );

		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 0 );
}

/* Frees a volume object
 */
void pyfvde_volume_free(
      pyfvde_volume_t *pyfvde_volume )
{
	struct _typeobject *ob_type = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pyfvde_volume_free";
	int result                  = 0;

	if( pyfvde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pyfvde_volume );

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
	if( ( pyfvde_volume->file_io_handle != NULL )
	 || ( pyfvde_volume->file_io_pool != NULL ) )
	{
		if( pyfvde_volume_close(
		     pyfvde_volume,
		     NULL ) == NULL )
		{
			return;
		}
	}
	if( pyfvde_volume->volume != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libfvde_volume_free(
		          &( pyfvde_volume->volume ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyfvde_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free libfvde volume.",
			 function );

			libcerror_error_free(
			 &error );
		}
	}
	ob_type->tp_free(
	 (PyObject*) pyfvde_volume );
}

/* Signals the volume to abort the current activity
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_signal_abort(
           pyfvde_volume_t *pyfvde_volume,
           PyObject *arguments PYFVDE_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyfvde_volume_signal_abort";
	int result               = 0;

	PYFVDE_UNREFERENCED_PARAMETER( arguments )

	if( pyfvde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfvde_volume_signal_abort(
	          pyfvde_volume->volume,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to signal abort.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Opens a volume
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_open(
           pyfvde_volume_t *pyfvde_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *string_object      = NULL;
	libcerror_error_t *error     = NULL;
	static char *function        = "pyfvde_volume_open";
	static char *keyword_list[]  = { "filename", "mode", NULL };
	const char *filename_narrow  = NULL;
	char *mode                   = NULL;
	int result                   = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	const wchar_t *filename_wide = NULL;
#else
	PyObject *utf8_string_object = NULL;
#endif

	if( pyfvde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * On Windows the narrow character strings contains an extended ASCII string with a codepage. Hence we get a conversion
	 * exception. This will also fail if the default encoding is not set correctly. We cannot use "u" here either since that
	 * does not allow us to pass non Unicode string objects and Python (at least 2.7) does not seems to automatically upcast them.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|s",
	     keyword_list,
	     &string_object,
	     &mode ) == 0 )
	{
		return( NULL );
	}
	if( ( mode != NULL )
	 && ( mode[ 0 ] != 'r' ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unsupported mode: %s.",
		 function,
		 mode );

		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyUnicode_Type );

	if( result == -1 )
	{
		pyfvde_error_fetch_and_raise(
	         PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type Unicode.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		filename_wide = (wchar_t *) PyUnicode_AsUnicode(
		                             string_object );
		Py_BEGIN_ALLOW_THREADS

		result = libfvde_volume_open_wide(
		          pyfvde_volume->volume,
		          filename_wide,
		          LIBFVDE_OPEN_READ,
		          &error );

		Py_END_ALLOW_THREADS
#else
		utf8_string_object = PyUnicode_AsUTF8String(
		                      string_object );

		if( utf8_string_object == NULL )
		{
			pyfvde_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to convert Unicode string to UTF-8.",
			 function );

			return( NULL );
		}
#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
		                   utf8_string_object );
#else
		filename_narrow = PyString_AsString(
		                   utf8_string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libfvde_volume_open(
		          pyfvde_volume->volume,
		          filename_narrow,
		          LIBFVDE_OPEN_READ,
		          &error );

		Py_END_ALLOW_THREADS

		Py_DecRef(
		 utf8_string_object );
#endif
		if( result != 1 )
		{
			pyfvde_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to open volume.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyBytes_Type );
#else
	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyString_Type );
#endif
	if( result == -1 )
	{
		pyfvde_error_fetch_and_raise(
	         PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type string.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
		                   string_object );
#else
		filename_narrow = PyString_AsString(
		                   string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libfvde_volume_open(
		          pyfvde_volume->volume,
		          filename_narrow,
		          LIBFVDE_OPEN_READ,
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyfvde_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to open volume.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	PyErr_Format(
	 PyExc_TypeError,
	 "%s: unsupported string object type.",
	 function );

	return( NULL );
}

/* Opens a volume using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_open_file_object(
           pyfvde_volume_t *pyfvde_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *file_object       = NULL;
	libcerror_error_t *error    = NULL;
	char *mode                  = NULL;
	static char *keyword_list[] = { "file_object", "mode", NULL };
	static char *function       = "pyfvde_volume_open_file_object";
	int result                  = 0;

	if( pyfvde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|s",
	     keyword_list,
	     &file_object,
	     &mode ) == 0 )
	{
		return( NULL );
	}
	if( ( mode != NULL )
	 && ( mode[ 0 ] != 'r' ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unsupported mode: %s.",
		 function,
		 mode );

		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_HasAttrString(
	          file_object,
	          "read" );

	if( result != 1 )
	{
		PyErr_Format(
		 PyExc_TypeError,
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
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: unsupported file object - missing seek attribute.",
		 function );

		goto on_error;
	}
	if( pyfvde_volume->file_io_handle != NULL )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: invalid volume - file IO handle already set.",
		 function );

		goto on_error;
	}
	if( pyfvde_file_object_initialize(
	     &( pyfvde_volume->file_io_handle ),
	     file_object,
	     &error ) != 1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to initialize file IO handle.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfvde_volume_open_file_io_handle(
	          pyfvde_volume->volume,
	          pyfvde_volume->file_io_handle,
	          LIBFVDE_OPEN_READ,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to open volume.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );

on_error:
	if( pyfvde_volume->file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &( pyfvde_volume->file_io_handle ),
		 NULL );
	}
	return( NULL );
}

/* Opens the physical volume files
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_open_physical_volume_files(
           pyfvde_volume_t *pyfvde_volume,
           PyObject *arguments,
           PyObject *keywords )
{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	wchar_t **filenames              = NULL;
	wchar_t *filename                = NULL;
	const char *errors               = NULL;
	char *narrow_string              = NULL;
	size_t narrow_string_size        = 0;
	int is_unicode_string            = 0;
#else
	char **filenames                 = NULL;
	char *filename                   = NULL;
#endif
	PyObject *filename_string_object = NULL;
	PyObject *sequence_object        = NULL;
	PyObject *string_object          = NULL;
	libcerror_error_t *error         = NULL;
	static char *keyword_list[]      = { "filenames", NULL };
	static char *function            = "pyfvde_volume_open_physical_volume_files";
	Py_ssize_t sequence_size         = 0;
	size_t filename_length           = 0;
	int filename_index               = 0;
	int number_of_filenames          = 0;
	int result                       = 0;

	if( pyfvde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	      keywords,
	      "O",
	      keyword_list,
	      &sequence_object ) == 0 )
	{
		return( NULL );
	}
	if( PySequence_Check(
	     sequence_object ) == 0 )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: argument: files must be a sequence object.",
		 function );

		return( NULL );
	}
	sequence_size = PySequence_Size(
	                 sequence_object );

	if( sequence_size > (Py_ssize_t) INT_MAX )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sequence size value exceeds maximum.",
		 function );

		goto on_error;
	}
	number_of_filenames = (int) sequence_size;

	if( ( number_of_filenames <= 0 )
	 || ( number_of_filenames > (int) UINT16_MAX ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid number of files.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	filenames = (wchar_t **) PyMem_Malloc(
	                          sizeof( wchar_t * ) * number_of_filenames );
#else
	filenames = (char **) PyMem_Malloc(
	                       sizeof( char * ) * number_of_filenames );
#endif
	if( filenames == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create filenames.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( memory_set(
	     filenames,
	     0,
	     sizeof( wchar_t * ) * number_of_filenames ) == NULL )
#else
	if( memory_set(
	     filenames,
	     0,
	     sizeof( char * ) * number_of_filenames ) == NULL )
#endif
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to clear filenames.",
		 function );

		PyMem_Free(
		 filenames );

		return( NULL );
	}
	for( filename_index = 0;
	     filename_index < number_of_filenames;
	     filename_index++ )
	{
		string_object = PySequence_GetItem(
		                 sequence_object,
		                 filename_index );

		PyErr_Clear();

		result = PyObject_IsInstance(
			  string_object,
			  (PyObject *) &PyUnicode_Type );

		if( result == -1 )
		{
			pyfvde_error_fetch_and_raise(
			 PyExc_ValueError,
			 "%s: unable to determine if the sequence object: %d is of type unicode.",
			 function,
			 filename_index );

			goto on_error;
		}
		else if( result == 0 )
		{
			PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
			result = PyObject_IsInstance(
				  string_object,
				  (PyObject *) &PyBytes_Type );
#else
			result = PyObject_IsInstance(
				  string_object,
				  (PyObject *) &PyString_Type );
#endif
			if( result == -1 )
			{
				pyfvde_error_fetch_and_raise(
				 PyExc_RuntimeError,
				 "%s: unable to determine if string object is of type string.",
				 function );

				goto on_error;
			}
			else if( result == 0 )
			{
				PyErr_Format(
				 PyExc_TypeError,
				 "%s: unsupported string object type",
				 function );

				goto on_error;
			}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			else
			{
				is_unicode_string = 0;
			}
#endif
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		else
		{
			is_unicode_string = 1;
		}
#endif
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( is_unicode_string != 0 )
		{
			filename = (wchar_t *) PyUnicode_AsUnicode(
			                        string_object );
		}
		else
		{
#if PY_MAJOR_VERSION >= 3
			narrow_string = PyBytes_AsString(
			                 string_object );

			narrow_string_size = PyBytes_Size(
			                      string_object );
#else
			narrow_string = PyString_AsString(
			                 string_object );

			narrow_string_size = PyString_Size(
			                      string_object );
#endif
			filename_string_object = PyUnicode_Decode(
						  narrow_string,
						  narrow_string_size,
						  PyUnicode_GetDefaultEncoding(),
						  errors );

			if( filename_string_object == NULL )
			{
				PyErr_Format(
				 PyExc_IOError,
				 "%s: unable to convert filename: %d into Unicode.",
				 function,
				 filename_index );

				goto on_error;
			}
			filename = (wchar_t *) PyUnicode_AsUnicode(
			                        filename_string_object );
		}
		filename_length = wide_string_length(
		                   filename );
#else
		/* A Unicode string object can be converted into UFT-8 formatted narrow string
		 */
#if PY_MAJOR_VERSION >= 3
		filename = PyBytes_AsString(
		            string_object );
#else
		filename = PyString_AsString(
		            string_object );
#endif
		filename_length = narrow_string_length(
		                   filename );
#endif

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		filenames[ filename_index ] = (wchar_t *) PyMem_Malloc(
		                                           sizeof( wchar_t ) * ( filename_length + 1 ) );
#else
		filenames[ filename_index ] = (char *) PyMem_Malloc(
		                                        sizeof( char ) * ( filename_length + 1 ) );
#endif
		if( filenames[ filename_index ] == NULL )
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to create filename: %d.",
			 function,
			 filename_index );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( wide_string_copy(
		     filenames[ filename_index ],
		     filename,
		     filename_length ) == NULL )
#else
		if( narrow_string_copy(
		     filenames[ filename_index ],
		     filename,
		     filename_length ) == NULL )
#endif
		{
			PyErr_Format(
			 PyExc_MemoryError,
			 "%s: unable to set filename: %d.",
			 function,
			 filename_index );

			goto on_error;
		}
		( filenames[ filename_index ] )[ filename_length ] = 0;

		if( filename_string_object != NULL )
		{
			Py_DecRef(
			 filename_string_object );

			filename_string_object = NULL;
		}
		/* The string object was reference by PySequence_GetItem
		 */
		Py_DecRef(
		 string_object );

		string_object = NULL;
	}
	Py_BEGIN_ALLOW_THREADS

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfvde_volume_open_physical_volume_files_wide(
	          pyfvde_volume->volume,
	          filenames,
	          number_of_filenames,
	          &error );
#else
	result = libfvde_volume_open_physical_volume_files(
	          pyfvde_volume->volume,
	          filenames,
	          number_of_filenames,
	          &error );
#endif
	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to open volume.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	for( filename_index = 0;
	     filename_index < number_of_filenames;
	     filename_index++ )
	{
		PyMem_Free(
		 filenames[ filename_index ] );
	}
	PyMem_Free(
	 filenames );

	Py_IncRef(
	 Py_None );

	return( Py_None );

on_error:
	if( filename_string_object != NULL )
	{
		Py_DecRef(
		 filename_string_object );
	}
	if( string_object != NULL )
	{
		Py_DecRef(
		 string_object );
	}
	if( filenames != NULL )
	{
		for( ; filename_index > 0; filename_index-- )
		{
			PyMem_Free(
			 filenames[ filename_index - 1 ] );
		}
		PyMem_Free(
		 filenames );
	}
	return( NULL );
}

/* Opens physical volume files using a list of file-like objects
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_open_physical_volume_files_as_file_objects(
           pyfvde_volume_t *pyfvde_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *file_objects      = NULL;
	libcerror_error_t *error    = NULL;
	static char *keyword_list[] = { "file_object", NULL };
	static char *function       = "pyfvde_volume_open_physical_volume_files_as_file_objects";
	int result                  = 0;

	if( pyfvde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O",
	     keyword_list,
	     &file_objects ) == 0 )
	{
		return( NULL );
	}
	if( pyfvde_file_objects_pool_initialize(
	     &( pyfvde_volume->file_io_pool ),
	     file_objects,
	     LIBBFIO_OPEN_READ,
	     &error ) != 1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to initialize file IO pool.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfvde_volume_open_physical_volume_files_file_io_pool(
	          pyfvde_volume->volume,
	          pyfvde_volume->file_io_pool,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to open physical volume files.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );

on_error:
	if( pyfvde_volume->file_io_pool != NULL )
	{
		libbfio_pool_free(
		 &( pyfvde_volume->file_io_pool ),
		 NULL );
	}
	return( NULL );
}

/* Closes a volume
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_close(
           pyfvde_volume_t *pyfvde_volume,
           PyObject *arguments PYFVDE_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyfvde_volume_close";
	int result               = 0;

	PYFVDE_UNREFERENCED_PARAMETER( arguments )

	if( pyfvde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfvde_volume_close(
	          pyfvde_volume->volume,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 0 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to close volume.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( pyfvde_volume->file_io_handle != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libbfio_handle_free(
		          &( pyfvde_volume->file_io_handle ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyfvde_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free libbfio file IO handle.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	if( pyfvde_volume->file_io_pool != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libbfio_pool_free(
		          &( pyfvde_volume->file_io_pool ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyfvde_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to free libbfio file IO pool.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Determines if the volume is locked
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_is_locked(
           pyfvde_volume_t *pyfvde_volume,
           PyObject *arguments PYFVDE_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyfvde_volume_is_locked";
	int result               = 0;

	PYFVDE_UNREFERENCED_PARAMETER( arguments )

	if( PyErr_WarnEx(
	     PyExc_DeprecationWarning,
	     "Call to deprecated function: is_locked",
	     1 ) < 0 )
	{
		return( NULL );
	}
	if( pyfvde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfvde_volume_is_locked(
	          pyfvde_volume->volume,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to determine if volume is locked.",
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

/* Unlocks the volume
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_unlock(
           pyfvde_volume_t *pyfvde_volume,
           PyObject *arguments PYFVDE_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyfvde_volume_unlock";
	int result               = 0;

	PYFVDE_UNREFERENCED_PARAMETER( arguments )

	if( PyErr_WarnEx(
	     PyExc_DeprecationWarning,
	     "Call to deprecated function: unlock",
	     1 ) < 0 )
	{
		return( NULL );
	}
	if( pyfvde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfvde_volume_unlock(
	          pyfvde_volume->volume,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to unlock volume.",
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

/* Reads (volume) data at the current offset into a buffer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_read_buffer(
           pyfvde_volume_t *pyfvde_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	PyObject *string_object     = NULL;
	static char *function       = "pyfvde_volume_read_buffer";
	static char *keyword_list[] = { "size", NULL };
	char *buffer                = NULL;
	ssize_t read_count          = 0;
	int read_size               = -1;

	if( PyErr_WarnEx(
	     PyExc_DeprecationWarning,
	     "Call to deprecated function: read_buffer",
	     1 ) < 0 )
	{
		return( NULL );
	}
	if( pyfvde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|i",
	     keyword_list,
	     &read_size ) == 0 )
	{
		return( NULL );
	}
	if( read_size < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value less than zero.",
		 function );

		return( NULL );
	}
	/* Make sure the data fits into the memory buffer
	 */
	if( read_size > INT_MAX )
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
	                 read_size );

	buffer = PyBytes_AsString(
	          string_object );
#else
	string_object = PyString_FromStringAndSize(
	                 NULL,
	                 read_size );

	buffer = PyString_AsString(
	          string_object );
#endif
	Py_BEGIN_ALLOW_THREADS

	read_count = libfvde_volume_read_buffer(
	              pyfvde_volume->volume,
	              (uint8_t *) buffer,
	              (size_t) read_size,
	              &error );

	Py_END_ALLOW_THREADS

	if( read_count <= -1 )
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

/* Reads (volume) data at a specific offset
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_read_buffer_at_offset(
           pyfvde_volume_t *pyfvde_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	PyObject *string_object     = NULL;
	static char *function       = "pyfvde_volume_read_buffer_at_offset";
	static char *keyword_list[] = { "size", "offset", NULL };
	char *buffer                = NULL;
	off64_t read_offset         = 0;
	ssize_t read_count          = 0;
	int read_size               = 0;

	if( PyErr_WarnEx(
	     PyExc_DeprecationWarning,
	     "Call to deprecated function: read_buffer_at_offset",
	     1 ) < 0 )
	{
		return( NULL );
	}
	if( pyfvde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i|L",
	     keyword_list,
	     &read_size,
	     &read_offset ) == 0 )
	{
		return( NULL );
	}
	if( read_size < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid argument read size value less than zero.",
		 function );

		return( NULL );
	}
	/* Make sure the data fits into the memory buffer
	 */
	if( read_size > INT_MAX )
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
		 "%s: invalid argument read offset value less than zero.",
		 function );

		return( NULL );
	}
	/* Make sure the data fits into the memory buffer
	 */
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromStringAndSize(
	                 NULL,
	                 read_size );

	buffer = PyBytes_AsString(
	          string_object );
#else
	string_object = PyString_FromStringAndSize(
	                 NULL,
	                 read_size );

	buffer = PyString_AsString(
	          string_object );
#endif
	Py_BEGIN_ALLOW_THREADS

	read_count = libfvde_volume_read_buffer_at_offset(
	              pyfvde_volume->volume,
	              (uint8_t *) buffer,
	              (size_t) read_size,
	              (off64_t) read_offset,
	              &error );

	Py_END_ALLOW_THREADS

	if( read_count <= -1 )
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

/* Seeks a certain offset in the (volume) data
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_seek_offset(
           pyfvde_volume_t *pyfvde_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	static char *function       = "pyfvde_volume_seek_offset";
	static char *keyword_list[] = { "offset", "whence", NULL };
	off64_t offset              = 0;
	int whence                  = 0;

	if( PyErr_WarnEx(
	     PyExc_DeprecationWarning,
	     "Call to deprecated function: seek_offset",
	     1 ) < 0 )
	{
		return( NULL );
	}
	if( pyfvde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid volume.",
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

	offset = libfvde_volume_seek_offset(
	          pyfvde_volume->volume,
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

/* Retrieves the current offset in the (volume) data
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_get_offset(
           pyfvde_volume_t *pyfvde_volume,
           PyObject *arguments PYFVDE_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyfvde_volume_get_offset";
	off64_t current_offset   = 0;
	int result               = 0;

	PYFVDE_UNREFERENCED_PARAMETER( arguments )

	if( PyErr_WarnEx(
	     PyExc_DeprecationWarning,
	     "Call to deprecated function: get_offset",
	     1 ) < 0 )
	{
		return( NULL );
	}
	if( pyfvde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfvde_volume_get_offset(
	          pyfvde_volume->volume,
	          &current_offset,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve offset.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyfvde_integer_signed_new_from_64bit(
	                  (int64_t) current_offset );

	return( integer_object );
}

/* Retrieves the size
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_get_size(
           pyfvde_volume_t *pyfvde_volume,
           PyObject *arguments PYFVDE_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyfvde_volume_get_size";
	size64_t size            = 0;
	int result               = 0;

	PYFVDE_UNREFERENCED_PARAMETER( arguments )

	if( PyErr_WarnEx(
	     PyExc_DeprecationWarning,
	     "Call to deprecated function: get_size",
	     1 ) < 0 )
	{
		return( NULL );
	}
	if( pyfvde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfvde_volume_get_logical_volume_size(
	          pyfvde_volume->volume,
	          &size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: failed to retrieve size.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyfvde_integer_unsigned_new_from_64bit(
	                  (uint64_t) size );

	return( integer_object );
}

/* Sets the keys
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_set_keys(
           pyfvde_volume_t *pyfvde_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error                   = NULL;
	char *volume_encryption_key_string         = NULL;
	static char *keyword_list[]                = { "volume_master_key", NULL };
	static char *function                      = "pyfvde_volume_set_keys";
	size_t volume_encryption_key_string_length = 0;
	int result                                 = 0;

	if( PyErr_WarnEx(
	     PyExc_DeprecationWarning,
	     "Call to deprecated function: set_keys",
	     1 ) < 0 )
	{
		return( NULL );
	}
	if( pyfvde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s|s",
	     keyword_list,
	     &volume_encryption_key_string ) == 0 )
	{
		return( NULL );
	}
	if( volume_encryption_key_string == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume encryption key string.",
		 function );

		return( NULL );
	}
	volume_encryption_key_string_length = narrow_string_length(
	                                       volume_encryption_key_string );

	Py_BEGIN_ALLOW_THREADS

	result = libfvde_volume_set_keys(
	          pyfvde_volume->volume,
	          (uint8_t *) volume_encryption_key_string,
	          volume_encryption_key_string_length,
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

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )

/* Sets the password
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_set_password(
           pyfvde_volume_t *pyfvde_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *string_object             = NULL;
	libcerror_error_t *error            = NULL;
	static char *function               = "pyfvde_volume_set_password";
	static char *keyword_list[]         = { "password", NULL };
	const wchar_t *password_string_wide = NULL;
	const char *password_string_narrow  = NULL;
	size_t password_string_length       = 0;
	int result                          = 0;

	if( PyErr_WarnEx(
	     PyExc_DeprecationWarning,
	     "Call to deprecated function: set_password",
	     1 ) < 0 )
	{
		return( NULL );
	}
	if( pyfvde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * On Windows the narrow character strings contains an extended ASCII string with a codepage. Hence we get a conversion
	 * exception. We cannot use "u" here either since that does not allow us to pass non Unicode string objects and
	 * Python (at least 2.7) does not seems to automatically upcast them.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O",
	     keyword_list,
	     &string_object ) == 0 )
	{
		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyUnicode_Type );

	if( result == -1 )
	{
		pyfvde_error_fetch_and_raise(
	         PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type Unicode.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

		password_string_wide = (wchar_t *) PyUnicode_AsUnicode(
		                                    string_object );

		password_string_length = wide_string_length(
		                          password_string_wide );

		Py_BEGIN_ALLOW_THREADS

		result = libfvde_volume_set_utf16_password(
		          pyfvde_volume->volume,
		          (uint16_t *) password_string_wide,
		          password_string_length,
		          &error );

		Py_END_ALLOW_THREADS

		if( result == -1 )
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
	PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyBytes_Type );
#else
	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyString_Type );
#endif
	if( result == -1 )
	{
		pyfvde_error_fetch_and_raise(
	         PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type string.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
		password_string_narrow = PyBytes_AsString(
		                          string_object );
#else
		password_string_narrow = PyString_AsString(
		                          string_object );
#endif
		password_string_length = narrow_string_length(
		                          password_string_narrow );

		Py_BEGIN_ALLOW_THREADS

		result = libfvde_volume_set_utf8_password(
		          pyfvde_volume->volume,
		          (uint8_t *) password_string_narrow,
		          password_string_length,
		          &error );

		Py_END_ALLOW_THREADS

		if( result == -1 )
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
	PyErr_Format(
	 PyExc_TypeError,
	 "%s: unsupported string object type",
	 function );

	return( NULL );
}

#else

/* Sets the password
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_set_password(
           pyfvde_volume_t *pyfvde_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error      = NULL;
	char *password_string         = NULL;
	static char *keyword_list[]   = { "password", NULL };
	static char *function         = "pyfvde_volume_set_password";
	size_t password_string_length = 0;
	int result                    = 0;

	if( PyErr_WarnEx(
	     PyExc_DeprecationWarning,
	     "Call to deprecated function: set_password",
	     1 ) < 0 )
	{
		return( NULL );
	}
	if( pyfvde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * For systems that support UTF-8 this works for Unicode string objects as well.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s",
	     keyword_list,
	     &password_string ) == 0 )
	{
		return( NULL );
	}
	if( password_string == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid password string.",
		 function );

		return( NULL );
	}
	password_string_length = narrow_string_length(
	                          password_string );

	Py_BEGIN_ALLOW_THREADS

	result = libfvde_volume_set_utf8_password(
	          pyfvde_volume->volume,
	          (uint8_t *) password_string,
	          password_string_length,
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

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )

/* Sets the recovery password
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_set_recovery_password(
           pyfvde_volume_t *pyfvde_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *string_object                      = NULL;
	libcerror_error_t *error                     = NULL;
	static char *function                        = "pyfvde_volume_set_recovery_password";
	static char *keyword_list[]                  = { "recovery_password", NULL };
	const wchar_t *recovery_password_string_wide = NULL;
	const char *recovery_password_string_narrow  = NULL;
	size_t recovery_password_string_length       = 0;
	int result                                   = 0;

	if( PyErr_WarnEx(
	     PyExc_DeprecationWarning,
	     "Call to deprecated function: set_recovery_password",
	     1 ) < 0 )
	{
		return( NULL );
	}
	if( pyfvde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * On Windows the narrow character strings contains an extended ASCII string with a codepage. Hence we get a conversion
	 * exception. We cannot use "u" here either since that does not allow us to pass non Unicode string objects and
	 * Python (at least 2.7) does not seems to automatically upcast them.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O",
	     keyword_list,
	     &string_object ) == 0 )
	{
		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyUnicode_Type );

	if( result == -1 )
	{
		pyfvde_error_fetch_and_raise(
	         PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type Unicode.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

		recovery_password_string_wide = (wchar_t *) PyUnicode_AsUnicode(
		                                             string_object );

		recovery_password_string_length = wide_string_length(
		                                   recovery_password_string_wide );

		Py_BEGIN_ALLOW_THREADS

		result = libfvde_volume_set_utf16_recovery_password(
		          pyfvde_volume->volume,
		          (uint16_t *) recovery_password_string_wide,
		          recovery_password_string_length,
		          &error );

		Py_END_ALLOW_THREADS

		if( result == -1 )
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
	PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyBytes_Type );
#else
	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyString_Type );
#endif
	if( result == -1 )
	{
		pyfvde_error_fetch_and_raise(
	         PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type string.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
		recovery_password_string_narrow = PyBytes_AsString(
		                                   string_object );
#else
		recovery_password_string_narrow = PyString_AsString(
		                                   string_object );
#endif
		recovery_password_string_length = narrow_string_length(
		                                   recovery_password_string_narrow );

		Py_BEGIN_ALLOW_THREADS

		result = libfvde_volume_set_utf8_recovery_password(
		          pyfvde_volume->volume,
		          (uint8_t *) recovery_password_string_narrow,
		          recovery_password_string_length,
		          &error );

		Py_END_ALLOW_THREADS

		if( result == -1 )
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
	PyErr_Format(
	 PyExc_TypeError,
	 "%s: unsupported string object type",
	 function );

	return( NULL );
}

#else

/* Sets the recovery password
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_set_recovery_password(
           pyfvde_volume_t *pyfvde_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error               = NULL;
	char *recovery_password_string         = NULL;
	static char *keyword_list[]            = { "recovery_password", NULL };
	static char *function                  = "pyfvde_volume_set_recovery_password";
	size_t recovery_password_string_length = 0;
	int result                             = 0;

	if( PyErr_WarnEx(
	     PyExc_DeprecationWarning,
	     "Call to deprecated function: set_recovery_password",
	     1 ) < 0 )
	{
		return( NULL );
	}
	if( pyfvde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * For systems that support UTF-8 this works for Unicode string objects as well.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s",
	     keyword_list,
	     &recovery_password_string ) == 0 )
	{
		return( NULL );
	}
	if( recovery_password_string == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid recovery password string.",
		 function );

		return( NULL );
	}
	recovery_password_string_length = narrow_string_length(
	                                   recovery_password_string );

	Py_BEGIN_ALLOW_THREADS

	result = libfvde_volume_set_utf8_recovery_password(
	          pyfvde_volume->volume,
	          (uint8_t *) recovery_password_string,
	          recovery_password_string_length,
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

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )

/* Reads the EncryptedRoot.plist from a file
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_read_encrypted_root_plist(
           pyfvde_volume_t *pyfvde_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *string_object      = NULL;
	libcerror_error_t *error     = NULL;
	static char *function        = "pyfvde_volume_read_encrypted_root_plist";
	static char *keyword_list[]  = { "filename", NULL };
	const wchar_t *filename_wide = NULL;
	const char *filename_narrow  = NULL;
	int result                   = 0;

	if( pyfvde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * On Windows the narrow character strings contains an extended ASCII string with a codepage. Hence we get a conversion
	 * exception. We cannot use "u" here either since that does not allow us to pass non Unicode string objects and
	 * Python (at least 2.7) does not seems to automatically upcast them.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O",
	     keyword_list,
	     &string_object ) == 0 )
	{
		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyUnicode_Type );

	if( result == -1 )
	{
		pyfvde_error_fetch_and_raise(
	         PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type Unicode.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

		filename_wide = (wchar_t *) PyUnicode_AsUnicode(
		                             string_object );
		Py_BEGIN_ALLOW_THREADS

		result = libfvde_volume_read_encrypted_root_plist_wide(
		          pyfvde_volume->volume,
		          filename_wide,
		          &error );

		Py_END_ALLOW_THREADS

		if( result == -1 )
		{
			pyfvde_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to read EncryptedRoot.plist.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyBytes_Type );
#else
	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyString_Type );
#endif
	if( result == -1 )
	{
		pyfvde_error_fetch_and_raise(
	         PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type string.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
		                   string_object );
#else
		filename_narrow = PyString_AsString(
		                   string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libfvde_volume_read_encrypted_root_plist(
		          pyfvde_volume->volume,
		          filename_narrow,
		          &error );

		Py_END_ALLOW_THREADS

		if( result == -1 )
		{
			pyfvde_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to read EncryptedRoot.plist.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	PyErr_Format(
	 PyExc_TypeError,
	 "%s: unsupported string object type",
	 function );

	return( NULL );
}

#else

/* Reads the EncryptedRoot.plist from a file
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_read_encrypted_root_plist(
           pyfvde_volume_t *pyfvde_volume,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	char *filename              = NULL;
	static char *keyword_list[] = { "filename", NULL };
	static char *function       = "pyfvde_volume_read_encrypted_root_plist";
	int result                  = 0;

	if( pyfvde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * For systems that support UTF-8 this works for Unicode string objects as well.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s",
	     keyword_list,
	     &filename ) == 0 )
	{
		return( NULL );
	}
	if( filename == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid filename.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfvde_volume_read_encrypted_root_plist(
	          pyfvde_volume->volume,
	          filename,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to read EncryptedRoot.plist.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

/* Retrieves the volume group
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyfvde_volume_get_volume_group(
           pyfvde_volume_t *pyfvde_volume,
           PyObject *arguments PYFVDE_ATTRIBUTE_UNUSED )
{
	PyObject *volume_group_object        = NULL;
	libcerror_error_t *error             = NULL;
	libfvde_volume_group_t *volume_group = NULL;
	static char *function                = "pyfvde_volume_get_volume_group";
	int result                           = 0;

	PYFVDE_UNREFERENCED_PARAMETER( arguments )

	if( pyfvde_volume == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid volume.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libfvde_volume_get_volume_group(
	          pyfvde_volume->volume,
	          &volume_group,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyfvde_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve volume group.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	volume_group_object = pyfvde_volume_group_new(
	                       volume_group,
	                       (PyObject *) pyfvde_volume );

	if( volume_group_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create volume group object.",
		 function );

		goto on_error;
	}
	return( volume_group_object );

on_error:
	if( volume_group != NULL )
	{
		libfvde_volume_group_free(
		 &volume_group,
		 NULL );
	}
	return( NULL );
}

