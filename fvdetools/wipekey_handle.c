/*
 * Wipekey handle
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
#include <byte_stream.h>
#include <file_stream.h>
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "fvdetools_libbfio.h"
#include "fvdetools_libcerror.h"
#include "fvdetools_libfvde.h"
#include "fvdetools_libuna.h"
#include "wipekey_handle.h"

#if !defined( LIBFVDE_HAVE_BFIO )

LIBFVDE_EXTERN \
int libfvde_encryption_context_plist_read_file_io_handle(
     libfvde_encryption_context_plist_t *plist,
     libbfio_handle_t *file_io_handle,
     libfvde_error_t **error );

#endif /* !defined( LIBFVDE_HAVE_BFIO ) */

#define INFO_HANDLE_NOTIFY_STREAM		stdout

/* Creates a wipekey handle
 * Make sure the value wipekey_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int wipekey_handle_initialize(
     wipekey_handle_t **wipekey_handle,
     libcerror_error_t **error )
{
	static char *function = "wipekey_handle_initialize";

	if( wipekey_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid wipekey handle.",
		 function );

		return( -1 );
	}
	if( *wipekey_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid wipekey handle value already set.",
		 function );

		return( -1 );
	}
	*wipekey_handle = memory_allocate_structure(
	                   wipekey_handle_t );

	if( *wipekey_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create wipekey handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *wipekey_handle,
	     0,
	     sizeof( wipekey_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear wipekey handle.",
		 function );

		goto on_error;
	}
	if( libbfio_file_initialize(
	     &( ( *wipekey_handle )->input_file_io_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize input file IO handle.",
		 function );

		goto on_error;
	}
	( *wipekey_handle )->notify_stream = INFO_HANDLE_NOTIFY_STREAM;

	return( 1 );

on_error:
	if( *wipekey_handle != NULL )
	{
		memory_free(
		 *wipekey_handle );

		*wipekey_handle = NULL;
	}
	return( -1 );
}

/* Frees a wipekey handle
 * Returns 1 if successful or -1 on error
 */
int wipekey_handle_free(
     wipekey_handle_t **wipekey_handle,
     libcerror_error_t **error )
{
	static char *function = "wipekey_handle_free";
	int result            = 1;

	if( wipekey_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid wipekey handle.",
		 function );

		return( -1 );
	}
	if( *wipekey_handle != NULL )
	{
		if( ( *wipekey_handle )->encrypted_root_plist != NULL )
		{
			if( libfvde_encryption_context_plist_free(
			     &( ( *wipekey_handle )->encrypted_root_plist ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free encrypted root plist.",
				 function );

				result = -1;
			}
		}
		if( libbfio_handle_free(
		     &( ( *wipekey_handle )->input_file_io_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free input file IO handle.",
			 function );

			result = -1;
		}
		memory_free(
		 *wipekey_handle );

		*wipekey_handle = NULL;
	}
	return( result );
}

/* Signals the wipekey handle to abort
 * Returns 1 if successful or -1 on error
 */
int wipekey_handle_signal_abort(
     wipekey_handle_t *wipekey_handle,
     libcerror_error_t **error )
{
	static char *function = "wipekey_handle_signal_abort";

	if( wipekey_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid wipekey handle.",
		 function );

		return( -1 );
	}
	wipekey_handle->abort = 1;

	return( 1 );
}

/* Sets the volume key data
 * Returns 1 if successful or -1 on error
 */
int wipekey_handle_set_volume_key_data(
     wipekey_handle_t *wipekey_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function   = "wipekey_handle_set_volume_key_data";
	size_t string_length    = 0;
	uint32_t base16_variant = 0;

	if( wipekey_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid wipekey handle.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	if( memory_set(
	     wipekey_handle->volume_key_data,
	     0,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear volume key data.",
		 function );

		goto on_error;
	}
	base16_variant = LIBUNA_BASE16_VARIANT_RFC4648;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( _BYTE_STREAM_HOST_IS_ENDIAN_BIG )
	{
		base16_variant |= LIBUNA_BASE16_VARIANT_ENCODING_UTF16_BIG_ENDIAN;
	}
	else
	{
		base16_variant |= LIBUNA_BASE16_VARIANT_ENCODING_UTF16_LITTLE_ENDIAN;
	}
#endif
	if( string_length != 32 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string length.",
		 function );

		goto on_error;
	}
	if( libuna_base16_stream_copy_to_byte_stream(
	     (uint8_t *) string,
	     string_length,
	     wipekey_handle->volume_key_data,
	     16,
	     base16_variant,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy volume key data.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	memory_set(
	 wipekey_handle->volume_key_data,
	 0,
	 16 );

	return( -1 );
}

/* Opens the wipekey handle
 * Returns 1 if successful, 0 if the keys could not be read or -1 on error
 */
int wipekey_handle_open_input(
     wipekey_handle_t *wipekey_handle,
     const system_character_t *filename,
     libcerror_error_t **error )
{
	static char *function  = "wipekey_handle_open_input";
	size_t filename_length = 0;
	int result             = 0;

	if( wipekey_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid wipekey handle.",
		 function );

		return( -1 );
	}
	filename_length = system_string_length(
	                   filename );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libbfio_file_set_name_wide(
	     wipekey_handle->input_file_io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
#else
	if( libbfio_file_set_name(
	     wipekey_handle->input_file_io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open set file name.",
		 function );

		goto on_error;
	}
	if( libbfio_handle_open(
	     wipekey_handle->input_file_io_handle,
	     LIBBFIO_OPEN_READ,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open input file.",
		 function );

		goto on_error;
	}
	if( libfvde_encryption_context_plist_initialize(
	     &( wipekey_handle->encrypted_root_plist ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create encrypted root plist.",
		 function );

		goto on_error;
	}
	if( libfvde_encryption_context_plist_read_file_io_handle(
	     wipekey_handle->encrypted_root_plist,
	     wipekey_handle->input_file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_GENERIC,
		 "%s: unable to read encrypted root plist.",
		 function );

		goto on_error;
	}
	if( libbfio_handle_close(
	     wipekey_handle->input_file_io_handle,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close input file IO handle.",
		 function );

		goto on_error;
	}
	result = libfvde_encryption_context_plist_decrypt(
	          wipekey_handle->encrypted_root_plist,
	          wipekey_handle->volume_key_data,
	          128,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ENCRYPTION,
		 LIBCERROR_ENCRYPTION_ERROR_DECRYPT_FAILED,
		 "%s: unable to decrypt encrypted root plist.",
		 function );

		 goto on_error;
	}
	if( memory_set(
	     wipekey_handle->volume_key_data,
	     0,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear volume key data.",
		 function );

		goto on_error;
	}
	return( result );

on_error:
	if( wipekey_handle->encrypted_root_plist != NULL )
	{
		libfvde_encryption_context_plist_free(
		 &( wipekey_handle->encrypted_root_plist ),
		 NULL );
	}
	return( -1 );
}

/* Closes the wipekey handle
 * Returns the 0 if succesful or -1 on error
 */
int wipekey_handle_close_input(
     wipekey_handle_t *wipekey_handle,
     libcerror_error_t **error )
{
	static char *function = "wipekey_handle_close_input";

	if( wipekey_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid wipekey handle.",
		 function );

		return( -1 );
	}
	if( wipekey_handle->encrypted_root_plist != NULL )
	{
		if( libfvde_encryption_context_plist_free(
		     &( wipekey_handle->encrypted_root_plist ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free encrypted root plist.",
			 function );

			return( -1 );
		}
	}
	return( 0 );
}

/* Prints the EncryptedRoot.plist.wipekey information to a stream
 * Returns 1 if successful or -1 on error
 */
int wipekey_handle_wipekey_fprint(
     wipekey_handle_t *wipekey_handle,
     libcerror_error_t **error )
{
	uint8_t *data         = NULL;
	static char *function = "wipekey_handle_wipekey_fprint";
	size64_t data_size    = 0;

	if( wipekey_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid wipekey handle.",
		 function );

		return( -1 );
	}
	if( libfvde_encryption_context_plist_get_data_size(
	     wipekey_handle->encrypted_root_plist,
	     &data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve encrypted root plist data size.",
		 function );

		goto on_error;
	}
	if( data_size > (size64_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid encrypted root plist data size value exceeds maximum.",
		 function );

		goto on_error;
	}
	fprintf(
	 wipekey_handle->notify_stream,
	 "Decrypted EncryptedRoot.plist.wipekey:\n" );

	if( data_size > 0 )
	{
		data = (uint8_t *) memory_allocate(
		                    sizeof( uint8_t ) * (size_t) data_size );

		if( data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create encrypted root plist data.",
			 function );

			goto on_error;
		}
		if( libfvde_encryption_context_plist_copy_data(
		     wipekey_handle->encrypted_root_plist,
		     data,
		     (size_t) data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy encrypted root plist data.",
			 function );

			goto on_error;
		}
		fprintf(
		 wipekey_handle->notify_stream,
		 "%s",
		 data );

		memory_free(
		 data );

		data = NULL;
	}
	fprintf(
	 wipekey_handle->notify_stream,
	 "\n" );

	return( 1 );

on_error:
	if( data != NULL )
	{
		memory_free(
		 data );
	}
	return( -1 );
}

