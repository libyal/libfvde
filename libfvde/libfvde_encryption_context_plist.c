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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libfvde_definitions.h"
#include "libfvde_encryption_context.h"
#include "libfvde_encryption_context_plist.h"
#include "libfvde_libcerror.h"
#include "libfvde_libcnotify.h"
#include "libfvde_libfplist.h"
#include "libfvde_libhmac.h"
#include "libfvde_libuna.h"
#include "libfvde_types.h"

/* Creates an encryption context plist
 * Make sure the value plist is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfvde_encryption_context_plist_initialize(
     libfvde_encryption_context_plist_t **plist,
     libcerror_error_t **error )
{
	libfvde_internal_encryption_context_plist_t *internal_plist = NULL;
	static char *function                                       = "libfvde_encryption_context_plist_initialize";

	if( plist == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid plist.",
		 function );

		return( -1 );
	}
	if( *plist != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid plist value already set.",
		 function );

		return( -1 );
	}
	internal_plist = memory_allocate_structure(
	                  libfvde_internal_encryption_context_plist_t );

	if( internal_plist == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create plist.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_plist,
	     0,
	     sizeof( libfvde_internal_encryption_context_plist_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear plist.",
		 function );
	
		memory_free(
		 internal_plist );

		return( -1 );
	}
	*plist = (libfvde_encryption_context_plist_t *) internal_plist;

	return( 1 );

on_error:
	if( internal_plist != NULL )
	{
		memory_free(
		 internal_plist );
	}
	return( -1 );
}

/* Frees an encryption context plist
 * Returns 1 if successful or -1 on error
 */
int libfvde_encryption_context_plist_free(
     libfvde_encryption_context_plist_t **plist,
     libcerror_error_t **error )
{
	libfvde_internal_encryption_context_plist_t *internal_plist = NULL;
	static char *function                                       = "libfvde_encryption_context_plist_free";
	int result                                                  = 1;

	if( plist == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid plist.",
		 function );

		return( -1 );
	}
	if( *plist != NULL )
	{
		internal_plist = (libfvde_internal_encryption_context_plist_t *) *plist;
		*plist         = NULL;

		if( internal_plist->data_encrypted != NULL )
		{
			memory_free(
			 internal_plist->data_encrypted );
		}
		if( internal_plist->data_decrypted != NULL )
		{
			memory_free(
			 internal_plist->data_decrypted );
		}
		if( internal_plist->wrapped_volume_keys_property != NULL )
		{
			if( libfplist_property_free(
			     &( internal_plist->wrapped_volume_keys_property ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free WrappedVolumeKeys property.",
				 function );

				result = -1;
			}
		}
		if( internal_plist->crypto_users_property != NULL )
		{
			if( libfplist_property_free(
			     &( internal_plist->crypto_users_property ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free CryptoUsers property.",
				 function );

				result = -1;
			}
		}
		if( internal_plist->conversion_info_property != NULL )
		{
			if( libfplist_property_free(
			     &( internal_plist->conversion_info_property ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free ConversionInfo property.",
				 function );

				result = -1;
			}
		}
		if( internal_plist->property_list != NULL )
		{
			if( libfplist_property_list_free(
			     &( internal_plist->property_list ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free property list.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 internal_plist );
	}
	return( result );
}

/* Retrieves the (un)encrypted data size of an encryption context plist
 * Returns 1 if successful or -1 on error
 */
int libfvde_encryption_context_plist_get_data_size(
     libfvde_encryption_context_plist_t *plist,
     size64_t *data_size,
     libcerror_error_t **error )
{
	libfvde_internal_encryption_context_plist_t *internal_plist = NULL;
	static char *function                                       = "libfvde_encryption_context_plist_get_data_size";

	if( plist == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid plist.",
		 function );

		return( -1 );
	}
	internal_plist = (libfvde_internal_encryption_context_plist_t *) plist;

	if( data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data size.",
		 function );

		return( -1 );
	}
	*data_size = internal_plist->data_size;

	return( 1 );
}

/* Copies the unencrypted data of an encryption context plist
 * Returns 1 if successful or -1 on error
 */
int libfvde_encryption_context_plist_copy_data(
     libfvde_encryption_context_plist_t *plist,
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfvde_internal_encryption_context_plist_t *internal_plist = NULL;
	static char *function                                       = "libfvde_encryption_context_plist_copy_data";

	if( plist == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid plist.",
		 function );

		return( -1 );
	}
	internal_plist = (libfvde_internal_encryption_context_plist_t *) plist;

	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( (size64_t) data_size < internal_plist->data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid data size value too small.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     data,
	     internal_plist->data_decrypted,
	     (size_t) internal_plist->data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy plist data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the unencrypted data of an encryption context plist
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libfvde_encryption_context_plist_set_data(
     libfvde_encryption_context_plist_t *plist,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfvde_internal_encryption_context_plist_t *internal_plist = NULL;
	static char *function                                       = "libfvde_encryption_context_plist_set_data";
	int result                                                  = 0;

	if( plist == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid plist.",
		 function );

		return( -1 );
	}
	internal_plist = (libfvde_internal_encryption_context_plist_t *) plist;

	if( internal_plist->data_decrypted != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid plist - decrypted data already set.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( ( data_size == 0 )
	 || ( data_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( data[ data_size - 1 ] != 0 )
	{
		data_size += 1;
	}
	internal_plist->data_decrypted = memory_allocate(
	                                  sizeof( uint8_t ) * data_size );

	if( internal_plist->data_decrypted == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create plist data.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     internal_plist->data_decrypted,
	     data,
	     data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy plist data.",
		 function );

		goto on_error;
	}
	/* Make sure the plist data has a trailing 0 byte before passing it
	 * to libfvde_encryption_context_plist_read_xml
	 */
	internal_plist->data_decrypted[ data_size - 1 ] = 0;

	internal_plist->data_size = (size64_t) data_size;

	result = libfvde_encryption_context_plist_read_xml(
	          plist,
	          internal_plist->data_decrypted,
	          internal_plist->data_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve XML.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		memory_free(
		 internal_plist->data_decrypted );

		internal_plist->data_decrypted = NULL;
		internal_plist->data_size      = 0;
	}
	return( result );

on_error:
	if( internal_plist->data_decrypted != NULL )
	{
		memory_free(
		 internal_plist->data_decrypted );

		internal_plist->data_decrypted = NULL;
	}
	internal_plist->data_size = 0;

	return( -1 );
}

/* Reads an encryption context plist file (EncryptedRoot.plist) using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libfvde_encryption_context_plist_read_file_io_handle(
     libfvde_encryption_context_plist_t *plist,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libfvde_internal_encryption_context_plist_t *internal_plist = NULL;
	static char *function                                       = "libfvde_encryption_context_plist_read_file_io_handle";
	ssize_t read_count                                          = 0;
	int file_io_handle_is_open                                  = 0;

	if( plist == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid plist.",
		 function );

		return( -1 );
	}
	internal_plist = (libfvde_internal_encryption_context_plist_t *) plist;

	file_io_handle_is_open = libbfio_handle_is_open(
	                          file_io_handle,
	                          error );

	if( file_io_handle_is_open == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.",
		 function );

		goto on_error;
	}
	else if( file_io_handle_is_open == 0 )
	{
		if( libbfio_handle_open(
		     file_io_handle,
		     LIBBFIO_OPEN_READ,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open file.",
			 function );

			goto on_error;
		}
	}
	if( libbfio_handle_get_size(
	     file_io_handle,
	     &( internal_plist->data_size ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine file size.",
		 function );

		goto on_error;
	}
	if( ( internal_plist->data_size == 0 )
	 || ( internal_plist->data_size > (size64_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid plist file size value out of bounds.",
		 function );

		goto on_error;
	}
	internal_plist->data_encrypted = memory_allocate(
	                                  sizeof( uint8_t ) * (size_t) internal_plist->data_size );

	if( internal_plist->data_encrypted == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create encrypted plist data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading file of size: %" PRIu64 "\n",
		 function,
		 internal_plist->data_size );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              internal_plist->data_encrypted,
	              (size_t) internal_plist->data_size,
	              0,
	              error );

	if( read_count != (ssize_t) internal_plist->data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read encrypted plist file at offset: 0 (0x00000000).",
		 function );

		goto on_error;
	}
	if( file_io_handle_is_open == 0 )
	{
		if( libbfio_handle_close(
		     file_io_handle,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close file.",
			 function );

			goto on_error;
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );

on_error:
	if( internal_plist->data_encrypted != NULL )
	{
		memory_free(
		 internal_plist->data_encrypted );

		internal_plist->data_encrypted = NULL;
	}
	if( file_io_handle_is_open == 0 )
	{
		libbfio_handle_close(
		 file_io_handle,
		 NULL );
	}
	return( -1 );
}

/* Decrypts the previously read encryption context plist file (EncryptedRoot.plist) with the given key.
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libfvde_encryption_context_plist_decrypt(
     libfvde_encryption_context_plist_t *plist,
     const uint8_t *key,
     size_t key_bit_size,
     libcerror_error_t **error )
{
	uint8_t tweak_key[ 16 ];

	libfvde_encryption_context_t *encryption_context            = NULL;
	libfvde_internal_encryption_context_plist_t *internal_plist = NULL;
	static char *function                                       = "libfvde_encryption_context_plist_decrypt";
	size_t xml_data_length                                      = 0;
	int result                                                  = 0;

	if( plist == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid plist.",
		 function );

		return( -1 );
	}
	internal_plist = (libfvde_internal_encryption_context_plist_t *) plist;

	if( ( internal_plist->data_encrypted == NULL )
	 || ( internal_plist->data_size == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid plist - missing encrypted data.",
		 function );

		return( -1 );
	}
	if( ( internal_plist->data_size == 0 )
	 || ( internal_plist->data_size > (size64_t) ( MEMORY_MAXIMUM_ALLOCATION_SIZE - 1 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid plist - data size value out of bounds.",
		 function );

		goto on_error;
	}
	if( internal_plist->data_decrypted != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid plist - decrypted data already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: decrypting plist data:\n",
		 function );
	}
#endif
	if( libfvde_encryption_context_initialize(
	     &encryption_context,
	     LIBFVDE_ENCRYPTION_METHOD_AES_128_XTS,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
		 LIBCERROR_ENCRYPTION_ERROR_GENERIC,
		 "%s: unable to initialize encryption context.",
		 function);

		goto on_error;
	}
	/* The tweak key is 0
	 */
	if( memory_set(
	     tweak_key,
	     0,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear tweak key.",
		 function );

		goto on_error;
	}
	if( libfvde_encryption_context_set_keys(
	     encryption_context,
	     key,
	     key_bit_size,
	     tweak_key,
	     128,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to set keys in encryption context.",
		 function);

		goto on_error;
	}
	internal_plist->data_decrypted = (uint8_t *) memory_allocate(
	                                              sizeof( uint8_t ) * (size_t) ( internal_plist->data_size + 1 ) );

	if( internal_plist->data_decrypted == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create decrypted plist block.",
		 function );

		goto on_error;
	}
	/* The tweak value is 0 and the block size is the size of the data
	 */
	if( libfvde_encryption_context_crypt(
	     encryption_context,
	     LIBFVDE_ENCRYPTION_CRYPT_MODE_DECRYPT,
	     internal_plist->data_encrypted,
	     (size_t) internal_plist->data_size,
	     internal_plist->data_decrypted,
	     (size_t) internal_plist->data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
		 LIBCERROR_ENCRYPTION_ERROR_GENERIC,
		 "%s: unable to decrypt data.",
		 function );

		goto on_error;
	}
	internal_plist->data_decrypted[ internal_plist->data_size ] = 0;

	if( libfvde_encryption_context_free(
	     &encryption_context,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free encryption context.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: unencrypted encryption context plist data:\n",
		 function );
		libcnotify_print_data(
		 internal_plist->data_decrypted,
		 internal_plist->data_size + 1,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( ( internal_plist->data_decrypted[ 0 ] == (uint8_t) '<' )
	 && ( internal_plist->data_decrypted[ 1 ] == (uint8_t) '?' )
	 && ( internal_plist->data_decrypted[ 2 ] == (uint8_t) 'x' )
	 && ( internal_plist->data_decrypted[ 3 ] == (uint8_t) 'm' )
	 && ( internal_plist->data_decrypted[ 4 ] == (uint8_t) 'l' ) )
	{
		/* Make sure the plist data has only one trailing 0 byte before passing it
		 * to libfvde_encryption_context_plist_read_xml
		 */
		xml_data_length = internal_plist->data_size;

		while( xml_data_length > 0 )
		{
			if( internal_plist->data_decrypted[ xml_data_length ] != 0 )
			{
				break;
			}
			xml_data_length--;
		}
		result = libfvde_encryption_context_plist_read_xml(
			  plist,
			  internal_plist->data_decrypted,
			  xml_data_length + 1,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve XML.",
			 function );

			goto on_error;
		}
	}
	if( result == 0 )
	{
		memory_free(
		 internal_plist->data_decrypted );

		internal_plist->data_decrypted = NULL;
		internal_plist->data_size      = 0;
	}
	return( result );

on_error:
	if( internal_plist->property_list != NULL )
	{
		libfplist_property_list_free(
		 &( internal_plist->property_list ),
		 NULL );
	}
	if( internal_plist->data_decrypted != NULL )
	{
		memory_free(
		 internal_plist->data_decrypted );

		internal_plist->data_decrypted = NULL;
	}
	if( encryption_context != NULL )
	{
		libfvde_encryption_context_free(
		 &encryption_context,
		 NULL );
	}
	return( -1 );
}

/* Reads the plist XML data
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libfvde_encryption_context_plist_read_xml(
     libfvde_encryption_context_plist_t *plist,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libfvde_internal_encryption_context_plist_t *internal_plist = NULL;
	libfplist_property_t *encryption_context_property           = NULL;
	libfplist_property_t *root_property                         = NULL;
	static char *function                                       = "libfvde_encryption_context_plist_read_xml";
	int result                                                  = 0;

	if( plist == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid plist.",
		 function );

		return( -1 );
	}
	internal_plist = (libfvde_internal_encryption_context_plist_t *) plist;

	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( ( data_size == 0 )
	 || ( data_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( internal_plist->property_list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid plist - XML plist already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: decrypted data:\n%s\n\n",
		 function,
		 data );
	}
#endif
	if( internal_plist->data_size > (size64_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid plist file size value out of bounds.",
		 function );

		goto on_error;
	}
	if( libfplist_property_list_initialize(
	     &( internal_plist->property_list ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create property list.",
		 function );

		goto on_error;
	}
	if( libfplist_property_list_copy_from_byte_stream(
	     internal_plist->property_list,
	     data,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy property list from byte stream.",
		 function );

		goto on_error;
	}
	if( libfplist_property_list_get_root_property(
	     internal_plist->property_list,
	     &root_property,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve root property.",
		 function );

		goto on_error;
	}
	result = libfplist_property_get_sub_property_by_utf8_name(
	          root_property,
	          (uint8_t *) "com.apple.corestorage.lvf.encryption.context",
	          44,
	          &encryption_context_property,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve encryption context property.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		encryption_context_property = root_property;
	}
	else
	{
		result = libfplist_property_list_has_plist_root_element(
		          internal_plist->property_list,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if property list has plist root element.",
			 function );

			goto on_error;
		}
		else if( result == 1 )
		{
			if( libfplist_property_free(
			     &encryption_context_property,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free encryption context property.",
				 function );

				goto on_error;
			}
			if( libfplist_property_list_free(
			     &( internal_plist->property_list ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free properyt list.",
				 function );

				goto on_error;
			}
			return( 0 );
		}
	}
	result = libfplist_property_get_sub_property_by_utf8_name(
	          encryption_context_property,
	          (uint8_t *) "ConversionInfo",
	          14,
	          &( internal_plist->conversion_info_property ),
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve ConversionInfo property.",
		 function );

		goto on_error;
	}
	result = libfplist_property_get_sub_property_by_utf8_name(
	          encryption_context_property,
	          (uint8_t *) "CryptoUsers",
	          11,
	          &( internal_plist->crypto_users_property ),
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve CryptoUsers property.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libfplist_property_get_array_number_of_entries(
		     internal_plist->crypto_users_property,
		     &( internal_plist->number_of_crypto_users_entries ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of CryptoUsers entries.",
			 function );

			goto on_error;
		}
	}
	result = libfplist_property_get_sub_property_by_utf8_name(
	          encryption_context_property,
	          (uint8_t *) "WrappedVolumeKeys",
	          17,
	          &( internal_plist->wrapped_volume_keys_property ),
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve WrappedVolumeKeys sub property.",
		 function );

		goto on_error;
	}
	if( encryption_context_property != root_property )
	{
		if( libfplist_property_free(
		     &encryption_context_property,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free encryption context property.",
			 function );

			goto on_error;
		}
	}
	if( libfplist_property_free(
	     &root_property,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free root property.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( internal_plist->wrapped_volume_keys_property != NULL )
	{
		libfplist_property_free(
		 &( internal_plist->wrapped_volume_keys_property ),
		 NULL );
	}
	if( internal_plist->crypto_users_property != NULL )
	{
		libfplist_property_free(
		 &( internal_plist->crypto_users_property ),
		 NULL );
	}
	if( internal_plist->conversion_info_property != NULL )
	{
		libfplist_property_free(
		 &( internal_plist->conversion_info_property ),
		 NULL );
	}
	if( ( encryption_context_property != NULL )
	 && ( encryption_context_property != root_property ) )
	{
		libfplist_property_free(
		 &encryption_context_property,
		 NULL );
	}
	if( root_property != NULL )
	{
		libfplist_property_free(
		 &root_property,
		 NULL );
	}
	if( internal_plist->property_list != NULL )
	{
		libfplist_property_list_free(
		 &( internal_plist->property_list ),
		 NULL );
	}
	return( -1 );
}

/* Retrieves the conversion status from the given plist data.
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libfvde_encryption_context_plist_get_conversion_status(
     libfvde_encryption_context_plist_t *plist,
     uint8_t **conversion_status,
     size_t *conversion_status_size,
     libcerror_error_t **error )
{
	libfplist_property_t *sub_property                          = NULL;
	libfvde_internal_encryption_context_plist_t *internal_plist = NULL;
	static char *function                                       = "libfvde_encryption_context_plist_get_conversion_status";
	int result                                                  = 0;

	if( plist == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid plist.",
		 function );

		return( -1 );
	}
	internal_plist = (libfvde_internal_encryption_context_plist_t *) plist;

	if( conversion_status == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid conversion status.",
		 function );

		return( -1 );
	}
	if( *conversion_status != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid conversion status value already set.",
		 function );

		return( -1 );
	}
	if( conversion_status_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid conversion status size.",
		 function );

		return( -1 );
	}
	if( internal_plist->conversion_info_property != NULL )
	{
		if( libfplist_property_get_sub_property_by_utf8_name(
		     internal_plist->conversion_info_property,
		     (uint8_t *) "ConversionStatus",
		     16,
		     &sub_property,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve ConversionStatus sub property.",
			 function );

			goto on_error;
		}
		if( libfplist_property_get_value_string(
		     sub_property,
		     conversion_status,
		     conversion_status_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve conversation status.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: conversion status:\n",
			 function );
			libcnotify_print_data(
			 *conversion_status,
			 *conversion_status_size,
			 0 );
		}
#endif
		if( libfplist_property_free(
		     &sub_property,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free ConversionStatus property.",
			 function );

			goto on_error;
		}
		result = 1;
	}
	return( result );

on_error:
	if( sub_property != NULL )
	{
		libfplist_property_free(
		 &sub_property,
		 NULL );
	}
	if( *conversion_status != NULL )
	{
		memory_free(
		 *conversion_status );

		*conversion_status = NULL;
	}
	*conversion_status_size = 0;

	return( -1 );
}

/* Retrieves the index-specified passphrase wrapped kek from the given plist data.
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libfvde_encryption_context_plist_get_passphrase_wrapped_kek(
     libfvde_encryption_context_plist_t *plist,
     int passphrase_wrapped_kek_index,
     uint8_t **passphrase_wrapped_kek,
     size_t *passphrase_wrapped_kek_size,
     libcerror_error_t **error )
{
	libfplist_property_t *array_entry_property                  = NULL;
	libfplist_property_t *sub_property                          = NULL;
	libfvde_internal_encryption_context_plist_t *internal_plist = NULL;
	uint8_t *safe_passphrase_wrapped_kek                        = NULL;
	static char *function                                       = "libfvde_encryption_context_plist_get_passphrase_wrapped_kek";
	size_t safe_passphrase_wrapped_kek_size                     = 0;
	int result                                                  = 0;

	if( plist == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid plist.",
		 function );

		return( -1 );
	}
	internal_plist = (libfvde_internal_encryption_context_plist_t *) plist;

	if( internal_plist->crypto_users_property == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid plist - missing XML plist crypto users property.",
		 function );

		return( -1 );
	}
	if( passphrase_wrapped_kek_index < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid passphrase wrapped KEK index value out of bounds.",
		 function );

		return( -1 );
	}
	if( passphrase_wrapped_kek == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid passphrase wrapped KEK.",
		 function );

		return( -1 );
	}
	if( *passphrase_wrapped_kek != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid passphrase wrapped KEK value already set.",
		 function );

		return( -1 );
	}
	if( passphrase_wrapped_kek_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid passphrase wrapped KEK size.",
		 function );

		return( -1 );
	}
	if( passphrase_wrapped_kek_index >= internal_plist->number_of_crypto_users_entries )
	{
		return( 0 );
	}
	if( libfplist_property_get_array_entry_by_index(
	     internal_plist->crypto_users_property,
	     passphrase_wrapped_kek_index,
	     &array_entry_property,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve crypto users array entry: %d.",
		 function,
		 passphrase_wrapped_kek_index );

		goto on_error;
	}
	result = libfplist_property_get_sub_property_by_utf8_name(
	          array_entry_property,
	          (uint8_t *) "PassphraseWrappedKEKStruct",
	          26,
	          &sub_property,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve PassphraseWrappedKEKStruct sub property.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libfplist_property_get_value_data_size(
		     sub_property,
		     &safe_passphrase_wrapped_kek_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve passphrase wrapped kek data size.",
			 function );

			goto on_error;
		}
		if( ( safe_passphrase_wrapped_kek_size == 0 )
		 || ( safe_passphrase_wrapped_kek_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid passphrase wrapped kek data size value exceeds maximum.",
			 function );

			return( -1 );
		}
		safe_passphrase_wrapped_kek = (uint8_t *) memory_allocate(
		                                           sizeof( uint8_t ) * safe_passphrase_wrapped_kek_size );

		if( safe_passphrase_wrapped_kek == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create passphrase wrapped kek.",
			 function );

			goto on_error;
		}
		if( libfplist_property_get_value_data(
		     sub_property,
		     safe_passphrase_wrapped_kek,
		     safe_passphrase_wrapped_kek_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve passphrase wrapped kek data.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: passphrase wrapped KEK:\n",
			 function );
			libcnotify_print_data(
			 safe_passphrase_wrapped_kek,
			 safe_passphrase_wrapped_kek_size,
			 0 );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		if( libfplist_property_free(
		     &sub_property,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free PassphraseWrappedKEKStruct property.",
			 function );

			goto on_error;
		}
	}
	if( libfplist_property_free(
	     &array_entry_property,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free crypto users array entry: %d.",
		 function,
		 passphrase_wrapped_kek_index );

		goto on_error;
	}
	if( result != 0 )
	{
		*passphrase_wrapped_kek      = safe_passphrase_wrapped_kek;
		*passphrase_wrapped_kek_size = safe_passphrase_wrapped_kek_size;
	}
	return( result );

on_error:
	if( sub_property != NULL )
	{
		libfplist_property_free(
		 &sub_property,
		 NULL );
	}
	if( array_entry_property != NULL )
	{
		libfplist_property_free(
		 &array_entry_property,
		 NULL );
	}
	if( safe_passphrase_wrapped_kek != NULL )
	{
		memory_free(
		 safe_passphrase_wrapped_kek );
	}
	return( -1 );
}

/* Retrieves the KEK wrapped volume key structure from the given plist data.
 * Returns 1 if successful or -1 on error
 */
int libfvde_encryption_context_plist_get_kek_wrapped_volume_key(
     libfvde_encryption_context_plist_t *plist,
     uint8_t **kek_wrapped_volume_key,
     size_t *kek_wrapped_volume_key_size,
     libcerror_error_t **error )
{
	libfplist_property_t *array_entry_property                  = NULL;
	libfplist_property_t *sub_property                          = NULL;
	libfvde_internal_encryption_context_plist_t *internal_plist = NULL;
	uint8_t *safe_kek_wrapped_volume_key                        = NULL;
	static char *function                                       = "libfvde_encryption_context_plist_get_kek_wrapped_volume_key";
	size_t safe_kek_wrapped_volume_key_size                     = 0;

	if( plist == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid plist.",
		 function );

		return( -1 );
	}
	internal_plist = (libfvde_internal_encryption_context_plist_t *) plist;

	if( internal_plist->wrapped_volume_keys_property == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid plist - missing XML plist wrapped volume keys property.",
		 function );

		return( -1 );
	}
	if( kek_wrapped_volume_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid KEK wrapped volume key.",
		 function );

		return( -1 );
	}
	if( *kek_wrapped_volume_key != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid KEK wrapped volume key value already set.",
		 function );

		return( -1 );
	}
	if( kek_wrapped_volume_key_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid KEK wrapped volume key size.",
		 function );

		return( -1 );
	}
	if( libfplist_property_get_array_entry_by_index(
	     internal_plist->wrapped_volume_keys_property,
	     1,
	     &array_entry_property,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve crypto users array entry: 1.",
		 function );

		goto on_error;
	}
	if( libfplist_property_get_sub_property_by_utf8_name(
	     array_entry_property,
	     (uint8_t *) "KEKWrappedVolumeKeyStruct",
	     25,
	     &sub_property,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve KEKWrappedVolumeKeyStruct sub property.",
		 function );

		goto on_error;
	}
	if( libfplist_property_get_value_data_size(
	     sub_property,
	     &safe_kek_wrapped_volume_key_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve kek wrapped volume key data size.",
		 function );

		goto on_error;
	}
	if( ( safe_kek_wrapped_volume_key_size == 0 )
	 || ( safe_kek_wrapped_volume_key_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid kek wrapped volume key data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	safe_kek_wrapped_volume_key = (uint8_t *) memory_allocate(
	                                           sizeof( uint8_t ) * safe_kek_wrapped_volume_key_size );

	if( safe_kek_wrapped_volume_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create kek wrapped volume key.",
		 function );

		goto on_error;
	}
	if( libfplist_property_get_value_data(
	     sub_property,
	     safe_kek_wrapped_volume_key,
	     safe_kek_wrapped_volume_key_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve kek wrapped volume property.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: KEK wrapped volume key:\n",
		 function );
		libcnotify_print_data(
		 safe_kek_wrapped_volume_key,
		 safe_kek_wrapped_volume_key_size,
		 0 );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( libfplist_property_free(
	     &sub_property,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free KEKWrappedVolumeKeyStruct property.",
		 function );

		goto on_error;
	}
	if( libfplist_property_free(
	     &array_entry_property,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free crypto users array entry: 1.",
		 function );

		goto on_error;
	}
	*kek_wrapped_volume_key      = safe_kek_wrapped_volume_key;
	*kek_wrapped_volume_key_size = safe_kek_wrapped_volume_key_size;

	return( 1 );

on_error:
	if( sub_property != NULL )
	{
		libfplist_property_free(
		 &sub_property,
		 NULL );
	}
	if( array_entry_property != NULL )
	{
		libfplist_property_free(
		 &array_entry_property,
		 NULL );
	}
	if( safe_kek_wrapped_volume_key != NULL )
	{
		memory_free(
		 safe_kek_wrapped_volume_key );
	}
	return( -1 );
}

