/*
 * The libfplist header wrapper
 *
 * Copyright (C) 2011-2022, Omar Choudary <choudary.omar@gmail.com>,
 *                          Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBFVDE_LIBFPLIST_H )
#define _LIBFVDE_LIBFPLIST_H

#include <common.h>

/* Define HAVE_LOCAL_LIBFPLIST for local use of libfplist
 */
#if defined( HAVE_LOCAL_LIBFPLIST )

#include <libfplist_definitions.h>
#include <libfplist_property.h>
#include <libfplist_property_list.h>
#include <libfplist_types.h>

#else

/* If libtool DLL support is enabled set LIBFPLIST_DLL_IMPORT
 * before including libfplist.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT )
#define LIBFPLIST_DLL_IMPORT
#endif

#include <libfplist.h>

#endif /* defined( HAVE_LOCAL_LIBFPLIST ) */

#endif /* !defined( _LIBFVDE_LIBFPLIST_H ) */

