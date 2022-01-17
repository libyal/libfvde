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

#if !defined( _PYFVDE_PHYSICAL_VOLUME_H )
#define _PYFVDE_PHYSICAL_VOLUME_H

#include <common.h>
#include <types.h>

#include "pyfvde_libfvde.h"
#include "pyfvde_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyfvde_physical_volume pyfvde_physical_volume_t;

struct pyfvde_physical_volume
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libfvde physical volume
	 */
	libfvde_physical_volume_t *physical_volume;

	/* The parent object
	 */
	PyObject *parent_object;
};

extern PyMethodDef pyfvde_physical_volume_object_methods[];
extern PyTypeObject pyfvde_physical_volume_type_object;

PyObject *pyfvde_physical_volume_new(
           libfvde_physical_volume_t *physical_volume,
           PyObject *parent_object );

int pyfvde_physical_volume_init(
     pyfvde_physical_volume_t *pyfvde_physical_volume );

void pyfvde_physical_volume_free(
      pyfvde_physical_volume_t *pyfvde_physical_volume );

PyObject *pyfvde_physical_volume_get_identifier(
           pyfvde_physical_volume_t *pyfvde_physical_volume,
           PyObject *arguments );

PyObject *pyfvde_physical_volume_get_encryption_method(
           pyfvde_physical_volume_t *pyfvde_physical_volume,
           PyObject *arguments );

PyObject *pyfvde_physical_volume_get_size(
           pyfvde_physical_volume_t *pyfvde_physical_volume,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYFVDE_PHYSICAL_VOLUME_H ) */

