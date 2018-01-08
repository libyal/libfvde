/*
 * The internal libfvde header
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

#if !defined( _FVDETOOLS_LIBFVDE_H )
#define _FVDETOOLS_LIBFVDE_H

#include <common.h>

/* If Cygwin libtool DLL support is enabled set LIBFVDE_DLL_IMPORT
 * before including libfvde.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT ) && !defined( HAVE_STATIC_EXECUTABLES )
#define LIBFVDE_DLL_IMPORT
#endif

#include <libfvde.h>

#endif /* !defined( _FVDETOOLS_LIBFVDE_H ) */

