/*
 * LVF encryption context and EncryptedRoot.plist file functions
 *
 * Copyright (C) 2011-2022, Omar Choudary <choudary.omar@gmail.com>
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

#if !defined( _LIBFVDE_ENCRYPTION_CONTEXT_PLIST_H )
#define _LIBFVDE_ENCRYPTION_CONTEXT_PLIST_H

#include <common.h>
#include <types.h>

#include "libfvde_extern.h"
#include "libfvde_libbfio.h"
#include "libfvde_libcerror.h"
#include "libfvde_libfplist.h"
#include "libfvde_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libfvde_internal_encryption_context_plist libfvde_internal_encryption_context_plist_t;

struct libfvde_internal_encryption_context_plist
{
        /* The encrypted plist file data
	 */
        uint8_t *data_encrypted;

        /* The decrypted plist file data
	 */
        uint8_t *data_decrypted;

        /* The size of the plist file data
	 */
        size64_t data_size;

        /* The property list
	 */
	libfplist_property_list_t *property_list;

        /* The XML plist conversion info property
	 */
	libfplist_property_t *conversion_info_property;

        /* The XML plist crypto users property
	 */
	libfplist_property_t *crypto_users_property;

	/* The number of crypto users array entries
	 */
	int number_of_crypto_users_entries;

        /* The XML plist wrapped volume keys property
	 */
	libfplist_property_t *wrapped_volume_keys_property;
};

LIBFVDE_EXTERN \
int libfvde_encryption_context_plist_initialize(
     libfvde_encryption_context_plist_t **plist,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_encryption_context_plist_free(
     libfvde_encryption_context_plist_t **plist,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_encryption_context_plist_get_data_size(
     libfvde_encryption_context_plist_t *plist,
     size64_t *data_size,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_encryption_context_plist_copy_data(
     libfvde_encryption_context_plist_t *plist,
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfvde_encryption_context_plist_set_data(
     libfvde_encryption_context_plist_t *plist,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_encryption_context_plist_read_file_io_handle(
     libfvde_encryption_context_plist_t *plist,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

LIBFVDE_EXTERN \
int libfvde_encryption_context_plist_decrypt(
     libfvde_encryption_context_plist_t *plist,
     const uint8_t *key,
     size_t key_bit_size,
     libcerror_error_t **error );

int libfvde_encryption_context_plist_read_xml(
     libfvde_encryption_context_plist_t *plist,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libfvde_encryption_context_plist_get_conversion_status(
     libfvde_encryption_context_plist_t *plist,
     uint8_t **conversion_status,
     size_t *conversion_status_size,
     libcerror_error_t **error );

int libfvde_encryption_context_plist_get_passphrase_wrapped_kek(
     libfvde_encryption_context_plist_t *plist,
     int passphrase_wrapped_kek_index,
     uint8_t **passphrase_wrapped_kek,
     size_t *passphrase_wrapped_kek_size,
     libcerror_error_t **error );

int libfvde_encryption_context_plist_get_kek_wrapped_volume_key(
     libfvde_encryption_context_plist_t *plist,
     uint8_t **kek_wrapped_volume_key,
     size_t *kek_wrapped_volume_key_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVDE_ENCRYPTION_CONTEXT_PLIST_H ) */

