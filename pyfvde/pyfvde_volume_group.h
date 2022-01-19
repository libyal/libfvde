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

#if !defined( _PYFVDE_VOLUME_GROUP_H )
#define _PYFVDE_VOLUME_GROUP_H

#include <common.h>
#include <types.h>

#include "pyfvde_libfvde.h"
#include "pyfvde_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyfvde_volume_group pyfvde_volume_group_t;

struct pyfvde_volume_group
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libfvde volume group
	 */
	libfvde_volume_group_t *volume_group;

	/* The parent object
	 */
	PyObject *parent_object;
};

extern PyMethodDef pyfvde_volume_group_object_methods[];
extern PyTypeObject pyfvde_volume_group_type_object;

PyObject *pyfvde_volume_group_new(
           libfvde_volume_group_t *volume_group,
           PyObject *parent_object );

int pyfvde_volume_group_init(
     pyfvde_volume_group_t *pyfvde_volume_group );

void pyfvde_volume_group_free(
      pyfvde_volume_group_t *pyfvde_volume_group );

PyObject *pyfvde_volume_group_get_identifier(
           pyfvde_volume_group_t *pyfvde_volume_group,
           PyObject *arguments );

PyObject *pyfvde_volume_group_get_name(
           pyfvde_volume_group_t *pyfvde_volume_group,
           PyObject *arguments );

PyObject *pyfvde_volume_group_get_number_of_physical_volumes(
           pyfvde_volume_group_t *pyfvde_volume_group,
           PyObject *arguments );

PyObject *pyfvde_volume_group_get_physical_volume_by_index(
           PyObject *pyfvde_volume_group,
           int physical_volume_index );

PyObject *pyfvde_volume_group_get_physical_volume(
           pyfvde_volume_group_t *pyfvde_volume_group,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfvde_volume_group_get_physical_volumes(
           pyfvde_volume_group_t *pyfvde_volume_group,
           PyObject *arguments );

PyObject *pyfvde_volume_group_get_number_of_logical_volumes(
           pyfvde_volume_group_t *pyfvde_volume_group,
           PyObject *arguments );

PyObject *pyfvde_volume_group_get_logical_volume_by_index(
           PyObject *pyfvde_volume_group,
           int logical_volume_index );

PyObject *pyfvde_volume_group_get_logical_volume(
           pyfvde_volume_group_t *pyfvde_volume_group,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyfvde_volume_group_get_logical_volumes(
           pyfvde_volume_group_t *pyfvde_volume_group,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYFVDE_VOLUME_GROUP_H ) */

