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

#if !defined( _PYFVDE_LOGICAL_VOLUME_H )
#define _PYFVDE_LOGICAL_VOLUME_H

#include <common.h>
#include <types.h>

#include "pyfvde_libfvde.h"
#include "pyfvde_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyfvde_logical_volume pyfvde_logical_volume_t;

struct pyfvde_logical_volume
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libfvde logical volume
	 */
	libfvde_logical_volume_t *logical_volume;

	/* The parent object
	 */
	PyObject *parent_object;
};

extern PyMethodDef pyfvde_logical_volume_object_methods[];
extern PyTypeObject pyfvde_logical_volume_type_object;

PyObject *pyfvde_logical_volume_new(
           libfvde_logical_volume_t *logical_volume,
           PyObject *parent_object );

int pyfvde_logical_volume_init(
     pyfvde_logical_volume_t *pyfvde_logical_volume );

void pyfvde_logical_volume_free(
      pyfvde_logical_volume_t *pyfvde_logical_volume );

PyObject *pyfvde_logical_volume_unlock(
           pyfvde_logical_volume_t *pyfvde_logical_volume,
           PyObject *arguments );

PyObject *pyfvde_logical_volume_read_buffer(
           pyfvde_logical_volume_t *pyfvde_logical_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfvde_logical_volume_read_buffer_at_offset(
           pyfvde_logical_volume_t *pyfvde_logical_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfvde_logical_volume_seek_offset(
           pyfvde_logical_volume_t *pyfvde_logical_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfvde_logical_volume_get_offset(
           pyfvde_logical_volume_t *pyfvde_logical_volume,
           PyObject *arguments );

PyObject *pyfvde_logical_volume_get_identifier(
           pyfvde_logical_volume_t *pyfvde_logical_volume,
           PyObject *arguments );

PyObject *pyfvde_logical_volume_get_name(
           pyfvde_logical_volume_t *pyfvde_logical_volume,
           PyObject *arguments );

PyObject *pyfvde_logical_volume_get_size(
           pyfvde_logical_volume_t *pyfvde_logical_volume,
           PyObject *arguments );

PyObject *pyfvde_logical_volume_is_locked(
           pyfvde_logical_volume_t *pyfvde_logical_volume,
           PyObject *arguments );

PyObject *pyfvde_logical_volume_set_key(
           pyfvde_logical_volume_t *pyfvde_logical_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfvde_logical_volume_set_password(
           pyfvde_logical_volume_t *pyfvde_logical_volume,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfvde_logical_volume_set_recovery_password(
           pyfvde_logical_volume_t *pyfvde_logical_volume,
           PyObject *arguments,
           PyObject *keywords );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYFVDE_LOGICAL_VOLUME_H ) */

