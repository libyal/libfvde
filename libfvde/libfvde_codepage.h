/*
 * Codepage functions
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

#if !defined( _LIBFVDE_INTERNAL_CODEPAGE_H )
#define _LIBFVDE_INTERNAL_CODEPAGE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

/* Define HAVE_LOCAL_LIBFVDE for local use of libfvde
 * The definitions in <libfvde/codepage.h> are copied here
 * for local use of libfvde
 */
#if !defined( HAVE_LOCAL_LIBFVDE )

#include <libfvde/codepage.h>

#else

/* The codepage definitions
 */
enum LIBFVDE_CODEPAGES
{
	LIBFVDE_CODEPAGE_ASCII		= 20127,

	LIBFVDE_CODEPAGE_ISO_8859_1	= 28591,
	LIBFVDE_CODEPAGE_ISO_8859_2	= 28592,
	LIBFVDE_CODEPAGE_ISO_8859_3	= 28593,
	LIBFVDE_CODEPAGE_ISO_8859_4	= 28594,
	LIBFVDE_CODEPAGE_ISO_8859_5	= 28595,
	LIBFVDE_CODEPAGE_ISO_8859_6	= 28596,
	LIBFVDE_CODEPAGE_ISO_8859_7	= 28597,
	LIBFVDE_CODEPAGE_ISO_8859_8	= 28598,
	LIBFVDE_CODEPAGE_ISO_8859_9	= 28599,
	LIBFVDE_CODEPAGE_ISO_8859_10	= 28600,
	LIBFVDE_CODEPAGE_ISO_8859_11	= 28601,
	LIBFVDE_CODEPAGE_ISO_8859_13	= 28603,
	LIBFVDE_CODEPAGE_ISO_8859_14	= 28604,
	LIBFVDE_CODEPAGE_ISO_8859_15	= 28605,
	LIBFVDE_CODEPAGE_ISO_8859_16	= 28606,

	LIBFVDE_CODEPAGE_KOI8_R		= 20866,
	LIBFVDE_CODEPAGE_KOI8_U		= 21866,

	LIBFVDE_CODEPAGE_WINDOWS_874	= 874,
	LIBFVDE_CODEPAGE_WINDOWS_932	= 932,
	LIBFVDE_CODEPAGE_WINDOWS_936	= 936,
	LIBFVDE_CODEPAGE_WINDOWS_949	= 949,
	LIBFVDE_CODEPAGE_WINDOWS_950	= 950,
	LIBFVDE_CODEPAGE_WINDOWS_1250	= 1250,
	LIBFVDE_CODEPAGE_WINDOWS_1251	= 1251,
	LIBFVDE_CODEPAGE_WINDOWS_1252	= 1252,
	LIBFVDE_CODEPAGE_WINDOWS_1253	= 1253,
	LIBFVDE_CODEPAGE_WINDOWS_1254	= 1254,
	LIBFVDE_CODEPAGE_WINDOWS_1255	= 1255,
	LIBFVDE_CODEPAGE_WINDOWS_1256	= 1256,
	LIBFVDE_CODEPAGE_WINDOWS_1257	= 1257,
	LIBFVDE_CODEPAGE_WINDOWS_1258	= 1258
};

#endif /* !defined( HAVE_LOCAL_LIBFVDE ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVDE_INTERNAL_CODEPAGE_H ) */

