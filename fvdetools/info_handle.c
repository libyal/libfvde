/*
 * Info handle
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

#include "byte_size_string.h"
#include "fvdetools_input.h"
#include "fvdetools_libbfio.h"
#include "fvdetools_libcdata.h"
#include "fvdetools_libcerror.h"
#include "fvdetools_libfguid.h"
#include "fvdetools_libfvde.h"
#include "fvdetools_libuna.h"
#include "info_handle.h"

#if !defined( LIBFVDE_HAVE_BFIO )

extern \
int libfvde_volume_open_file_io_handle(
     libfvde_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libfvde_error_t **error );

extern \
int libfvde_volume_open_physical_volume_files_file_io_pool(
     libfvde_volume_t *handle,
     libbfio_pool_t *file_io_pool,
     libcerror_error_t **error );

#endif /* !defined( LIBFVDE_HAVE_BFIO ) */

#define INFO_HANDLE_NOTIFY_STREAM		stdout

/* Copies a string of a decimal value to a 64-bit value
 * Returns 1 if successful or -1 on error
 */
int fvdetools_system_string_copy_from_64_bit_in_decimal(
     const system_character_t *string,
     size_t string_size,
     uint64_t *value_64bit,
     libcerror_error_t **error )
{
	static char *function              = "fvdetools_system_string_copy_from_64_bit_in_decimal";
	size_t string_index                = 0;
	system_character_t character_value = 0;
	uint8_t maximum_string_index       = 20;
	int8_t sign                        = 1;

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( value_64bit == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value 64-bit.",
		 function );

		return( -1 );
	}
	*value_64bit = 0;

	if( string[ string_index ] == (system_character_t) '-' )
	{
		string_index++;
		maximum_string_index++;

		sign = -1;
	}
	else if( string[ string_index ] == (system_character_t) '+' )
	{
		string_index++;
		maximum_string_index++;
	}
	while( string_index < string_size )
	{
		if( string[ string_index ] == 0 )
		{
			break;
		}
		if( string_index > (size_t) maximum_string_index )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_LARGE,
			 "%s: string too large.",
			 function );

			return( -1 );
		}
		*value_64bit *= 10;

		if( ( string[ string_index ] >= (system_character_t) '0' )
		 && ( string[ string_index ] <= (system_character_t) '9' ) )
		{
			character_value = (system_character_t) ( string[ string_index ] - (system_character_t) '0' );
		}
		else
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported character value: %" PRIc_SYSTEM " at index: %d.",
			 function,
			 string[ string_index ],
			 string_index );

			return( -1 );
		}
		*value_64bit += character_value;

		string_index++;
	}
	if( sign == -1 )
	{
		*value_64bit *= (uint64_t) -1;
	}
	return( 1 );
}

/* Creates an info handle
 * Make sure the value info_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int info_handle_initialize(
     info_handle_t **info_handle,
     int unattended_mode,
     libcerror_error_t **error )
{
	static char *function = "info_handle_initialize";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( *info_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid info handle value already set.",
		 function );

		return( -1 );
	}
	*info_handle = memory_allocate_structure(
	                info_handle_t );

	if( *info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create info handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *info_handle,
	     0,
	     sizeof( info_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear info handle.",
		 function );

		memory_free(
		 *info_handle );

		*info_handle = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *info_handle )->logical_volumes_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize logical volumes array.",
		 function );

		goto on_error;
	}
	( *info_handle )->notify_stream   = INFO_HANDLE_NOTIFY_STREAM;
	( *info_handle )->unattended_mode = unattended_mode;

	return( 1 );

on_error:
	if( *info_handle != NULL )
	{
		memory_free(
		 *info_handle );

		*info_handle = NULL;
	}
	return( -1 );
}

/* Frees an info handle
 * Returns 1 if successful or -1 on error
 */
int info_handle_free(
     info_handle_t **info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_free";
	int result            = 1;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( *info_handle != NULL )
	{
		if( ( *info_handle )->physical_volume_file_io_pool != NULL )
		{
			if( info_handle_close(
			     *info_handle,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close info handle.",
				 function );

				result = -1;
			}
		}
		if( libcdata_array_free(
		     &( ( *info_handle )->logical_volumes_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfvde_logical_volume_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free logical volumes array.",
			 function );

			result = -1;
		}
		if( memory_set(
		     ( *info_handle )->key_data,
		     0,
		     16 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear key data.",
			 function );

			result = -1;
		}
		memory_free(
		 *info_handle );

		*info_handle = NULL;
	}
	return( result );
}

/* Signals the info handle to abort
 * Returns 1 if successful or -1 on error
 */
int info_handle_signal_abort(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_signal_abort";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->volume != NULL )
	{
		if( libfvde_volume_signal_abort(
		     info_handle->volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal volume to abort.",
			 function );

			return( -1 );
		}
	}
	info_handle->abort = 1;

	return( 1 );
}

/* Sets the key
 * Returns 1 if successful or -1 on error
 */
int info_handle_set_key(
     info_handle_t *info_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function   = "info_handle_set_key";
	size_t string_length    = 0;
	uint32_t base16_variant = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	if( memory_set(
	     info_handle->key_data,
	     0,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear key data.",
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
	     info_handle->key_data,
	     16,
	     base16_variant,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy key data.",
		 function );

		goto on_error;
	}
	info_handle->key_data_size = 16;

	return( 1 );

on_error:
	memory_set(
	 info_handle->key_data,
	 0,
	 16 );

	info_handle->key_data_size = 0;

	return( -1 );
}

/* Sets the password
 * Returns 1 if successful or -1 on error
 */
int info_handle_set_password(
     info_handle_t *info_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "info_handle_set_password";
	size_t string_length  = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	info_handle->user_password        = string;
	info_handle->user_password_length = string_length;

	return( 1 );
}

/* Sets the recovery password
 * Returns 1 if successful or -1 on error
 */
int info_handle_set_recovery_password(
     info_handle_t *info_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "info_handle_set_recovery_password";
	size_t string_length  = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	info_handle->recovery_password        = string;
	info_handle->recovery_password_length = string_length;

	return( 1 );
}

/* Sets the path of the EncryptedRoot.plist.wipekey file
 * Returns 1 if successful or -1 on error
 */
int info_handle_set_encrypted_root_plist(
     info_handle_t *info_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "info_handle_set_encrypted_root_plist";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	info_handle->encrypted_root_plist_path = string;

	return( 1 );
}

/* Sets the volume offset
 * Returns 1 if successful or -1 on error
 */
int info_handle_set_volume_offset(
     info_handle_t *info_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "info_handle_set_volume_offset";
	size_t string_length  = 0;
	uint64_t value_64bit  = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	if( fvdetools_system_string_copy_from_64_bit_in_decimal(
	     string,
	     string_length + 1,
	     &value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy string to 64-bit decimal.",
		 function );

		return( -1 );
	}
	info_handle->volume_offset = (off64_t) value_64bit;

	return( 1 );
}

/* Opens the info handle
 * Returns 1 if successful or -1 on error
 */
int info_handle_open(
     info_handle_t *info_handle,
     system_character_t * const * filenames,
     int number_of_filenames,
     libcerror_error_t **error )
{
	system_character_t password[ 64 ];

	libbfio_handle_t *file_io_handle         = NULL;
	libfvde_logical_volume_t *logical_volume = NULL;
	static char *function                    = "info_handle_open";
	size_t filename_length                   = 0;
	size_t password_length                   = 0;
	int entry_index                          = 0;
	int filename_index                       = 0;
	int logical_volume_index                 = 0;
	int number_of_logical_volumes            = 0;
	int result                               = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->physical_volume_file_io_pool != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid info handle - physical volume file IO pool value already set.",
		 function );

		return( -1 );
	}
	if( info_handle->volume != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid info handle - volume value already set.",
		 function );

		return( -1 );
	}
	if( number_of_filenames <= 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_ZERO_OR_LESS,
		 "%s: invalid number of filenames.",
		 function );

		return( -1 );
	}
	if( libbfio_file_range_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize file IO handle: 0.",
		 function );

		goto on_error;
	}
	filename_length = system_string_length(
	                   filenames[ 0 ] );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libbfio_file_range_set_name_wide(
	     file_io_handle,
	     filenames[ 0 ],
	     filename_length,
	     error ) != 1 )
#else
	if( libbfio_file_range_set_name(
	     file_io_handle,
	     filenames[ 0 ],
	     filename_length,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to set name of file IO handle: 0.",
		 function );

		goto on_error;
	}
	if( libbfio_file_range_set(
	     file_io_handle,
	     info_handle->volume_offset,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to set volume offset of file IO handle: 0.",
		 function );

		goto on_error;
	}
	if( libfvde_volume_initialize(
	     &( info_handle->volume ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize volume.",
		 function );

		goto on_error;
	}
	if( info_handle->encrypted_root_plist_path != NULL )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libfvde_volume_read_encrypted_root_plist_wide(
		     info_handle->volume,
		     info_handle->encrypted_root_plist_path,
		     error ) != 1 )
#else
		if( libfvde_volume_read_encrypted_root_plist(
		     info_handle->volume,
		     info_handle->encrypted_root_plist_path,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read EncryptedRoot.plist.wipekey file.",
			 function );

			goto on_error;
		}
	}
	if( libfvde_volume_open_file_io_handle(
	     info_handle->volume,
	     file_io_handle,
	     LIBFVDE_OPEN_READ,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open volume.",
		 function );

		goto on_error;
	}
/* TODO control maximum number of open handles */
	if( libbfio_pool_initialize(
	     &( info_handle->physical_volume_file_io_pool ),
	     number_of_filenames,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize physical volume file IO pool.",
		 function );

		goto on_error;
	}
	if( libbfio_pool_set_handle(
	     info_handle->physical_volume_file_io_pool,
	     0,
	     file_io_handle,
	     LIBBFIO_OPEN_READ,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set file IO handle: 0 in pool.",
		 function );

		goto on_error;
	}
	/* The file IO pool takes over management of the file IO handle
	 */
	file_io_handle = NULL;

	for( filename_index = 1;
	     filename_index < number_of_filenames;
	     filename_index++ )
	{
		if( libbfio_file_range_initialize(
		     &file_io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize file IO handle: %d.",
			 function,
			 filename_index );

			goto on_error;
		}
		filename_length = system_string_length(
		                   filenames[ filename_index ] );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libbfio_file_range_set_name_wide(
		     file_io_handle,
		     filenames[ filename_index ],
		     filename_length,
		     error ) != 1 )
#else
		if( libbfio_file_range_set_name(
		     file_io_handle,
		     filenames[ filename_index ],
		     filename_length,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to set name of file IO handle: %d.",
			 function,
			 filename_index );

			goto on_error;
		}
		if( libbfio_file_range_set(
		     file_io_handle,
		     info_handle->volume_offset,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to set volume offset of file IO handle: %d.",
			 function,
			 filename_index );

			goto on_error;
		}
		if( libbfio_pool_set_handle(
		     info_handle->physical_volume_file_io_pool,
		     filename_index,
		     file_io_handle,
		     LIBBFIO_OPEN_READ,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set file IO handle: %d in pool.",
			 function,
			 filename_index );

			goto on_error;
		}
		/* The file IO pool takes over management of the file IO handle
		 */
		file_io_handle = NULL;
	}
	if( libfvde_volume_open_physical_volume_files_file_io_pool(
	     info_handle->volume,
	     info_handle->physical_volume_file_io_pool,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open physical volume files.",
		 function );

		goto on_error;
	}
	if( libfvde_volume_get_volume_group(
	     info_handle->volume,
	     &( info_handle->volume_group ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve volume group.",
		 function );

		goto on_error;
	}
	if( libfvde_volume_group_get_number_of_logical_volumes(
	     info_handle->volume_group,
	     &number_of_logical_volumes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of logical volumes.",
		 function );

		goto on_error;
	}
	for( logical_volume_index = 0;
	     logical_volume_index < number_of_logical_volumes;
	     logical_volume_index++ )
	{
		if( libfvde_volume_group_get_logical_volume_by_index(
		     info_handle->volume_group,
		     logical_volume_index,
		     &logical_volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve logical volume: %d.",
			 function,
			 logical_volume_index );

			goto on_error;
		}
		if( info_handle->key_data_size != 0 )
		{
			if( libfvde_logical_volume_set_key(
			     logical_volume,
			     info_handle->key_data,
			     16,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set key.",
				 function );

				goto on_error;
			}
		}
		if( info_handle->user_password != NULL )
		{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			if( libfvde_logical_volume_set_utf16_password(
			     logical_volume,
			     (uint16_t *) info_handle->user_password,
			     info_handle->user_password_length,
			     error ) != 1 )
#else
			if( libfvde_logical_volume_set_utf8_password(
			     logical_volume,
			     (uint8_t *) info_handle->user_password,
			     info_handle->user_password_length,
			     error ) != 1 )
#endif
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set password.",
				 function );

				goto on_error;
			}
		}
		if( info_handle->recovery_password != NULL )
		{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			if( libfvde_logical_volume_set_utf16_recovery_password(
			     logical_volume,
			     (uint16_t *) info_handle->recovery_password,
			     info_handle->recovery_password_length,
			     error ) != 1 )
#else
			if( libfvde_logical_volume_set_utf8_recovery_password(
			     logical_volume,
			     (uint8_t *) info_handle->recovery_password,
			     info_handle->recovery_password_length,
			     error ) != 1 )
#endif
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set recovery password.",
				 function );

				goto on_error;
			}
		}
		result = libfvde_logical_volume_unlock(
		          logical_volume,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to unlock logical volume.",
			 function );

			goto on_error;
		}
		else if( ( result == 0 )
		      && ( info_handle->unattended_mode == 0 ) )
		{
/* TODO print logical volume identifier and/or name */
			fprintf(
			 stdout,
			 "Logical volume: %d is locked and a password is needed to unlock it.\n\n",
			 logical_volume_index + 1 );

			if( fvdetools_prompt_for_password(
			     stdout,
			     "Password",
			     password,
			     64,
			     error ) != 1 )
			{
				fprintf(
				 stderr,
				 "Unable to retrieve password.\n" );

				goto on_error;
			}
			password_length = system_string_length(
			                   password );

			if( password_length > 0 )
			{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
				if( libfvde_logical_volume_set_utf16_password(
				     logical_volume,
				     (uint16_t *) password,
				     password_length,
				     error ) != 1 )
#else
				if( libfvde_logical_volume_set_utf8_password(
				     logical_volume,
				     (uint8_t *) password,
				     password_length,
				     error ) != 1 )
#endif
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set password.",
					 function );

					goto on_error;
				}
				memory_set(
				 password,
				 0,
				 64 );
			}
			fprintf(
			 stdout,
			 "\n\n" );

			result = libfvde_logical_volume_unlock(
			          logical_volume,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to unlock logical volume.",
				 function );

				goto on_error;
			}
			else if( result == 0 )
			{
				fprintf(
				 stdout,
				 "Unable to unlock volume.\n\n" );
			}
		}
		if( libcdata_array_append_entry(
		     info_handle->logical_volumes_array,
		     &entry_index,
		     (intptr_t *) logical_volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append logical volume: %d to array.",
			 function,
			 logical_volume_index );

			goto on_error;
		}
		logical_volume = NULL;
	}
	return( 1 );

on_error:
	if( logical_volume != NULL )
	{
		libfvde_logical_volume_free(
		 &logical_volume,
		 NULL );
	}
	if( info_handle->volume_group != NULL )
	{
		libfvde_volume_group_free(
		 &( info_handle->volume_group ),
		 NULL );
	}
	if( info_handle->volume != NULL )
	{
		libfvde_volume_free(
		 &( info_handle->volume ),
		 NULL );
	}
	/* The file IO pool must be freed after the volume
	 */
	if( info_handle->physical_volume_file_io_pool != NULL )
	{
		libbfio_pool_free(
		 &( info_handle->physical_volume_file_io_pool ),
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	memory_set(
	 password,
	 0,
	 64 );

	return( -1 );
}

/* Closes the info handle
 * Returns the 0 if succesful or -1 on error
 */
int info_handle_close(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_close";
	int result            = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( info_handle->physical_volume_file_io_pool == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid info handle - missing physical volume file IO pool.",
		 function );

		return( -1 );
	}
	if( info_handle->volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid info handle - missing volume.",
		 function );

		return( -1 );
	}
	if( libcdata_array_empty(
	     info_handle->logical_volumes_array,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfvde_logical_volume_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to empty logical volumes array.",
		 function );

		result = -1;
	}
	if( info_handle->volume_group != NULL )
	{
		if( libfvde_volume_group_free(
		     &( info_handle->volume_group ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free volume group.",
			 function );

			result = -1;
		}
	}
	if( libfvde_volume_close(
	     info_handle->volume,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close volume.",
		 function );

		result = -1;
	}
	if( libfvde_volume_free(
	     &( info_handle->volume ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free volume.",
		 function );

		result = -1;
	}
	if( libbfio_pool_close_all(
	     info_handle->physical_volume_file_io_pool,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close physical volume file IO pool.",
		 function );

		result = -1;
	}
	if( libbfio_pool_free(
	     &( info_handle->physical_volume_file_io_pool ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free physical volume file IO pool.",
		 function );

		result = -1;
	}
	return( result );
}

/* Prints an UUID value
 * Returns 1 if successful or -1 on error
 */
int info_handle_uuid_value_fprint(
     info_handle_t *info_handle,
     const char *value_name,
     const uint8_t *uuid_data,
     libcerror_error_t **error )
{
	system_character_t uuid_string[ 48 ];

	libfguid_identifier_t *uuid = NULL;
	static char *function       = "info_handle_uuid_value_fprint";
	int result                  = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libfguid_identifier_initialize(
	     &uuid,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create UUID.",
		 function );

		goto on_error;
	}
	if( libfguid_identifier_copy_from_byte_stream(
	     uuid,
	     uuid_data,
	     16,
	     LIBFGUID_ENDIAN_BIG,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy byte stream to UUID.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfguid_identifier_copy_to_utf16_string(
		  uuid,
		  (uint16_t *) uuid_string,
		  48,
		  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
		  error );
#else
	result = libfguid_identifier_copy_to_utf8_string(
		  uuid,
		  (uint8_t *) uuid_string,
		  48,
		  LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy UUID to string.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "%s: %" PRIs_SYSTEM "\n",
	 value_name,
	 uuid_string );

	if( libfguid_identifier_free(
	     &uuid,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free UUID.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( uuid != NULL )
	{
		libfguid_identifier_free(
		 &uuid,
		 NULL );
	}
	return( -1 );
}

/* Prints physcial volume information
 * Returns 1 if successful or -1 on error
 */
int info_handle_physical_volume_fprint(
     info_handle_t *info_handle,
     int physical_volume_index,
     libfvde_physical_volume_t *physical_volume,
     libcerror_error_t **error )
{
        system_character_t byte_size_string[ 16 ];
	uint8_t uuid_data[ 16 ];

	static char *function      = "info_handle_physical_volume_fprint";
	size64_t volume_size       = 0;
	uint32_t encryption_method = 0;
	int result                 = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "Physical volume: %d\n",
	 physical_volume_index + 1 );

	if( libfvde_physical_volume_get_identifier(
	     physical_volume,
	     uuid_data,
	     16,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve physical volume identifier.",
		 function );

		return( -1 );
	}
	if( info_handle_uuid_value_fprint(
	     info_handle,
	     "\tIdentifier\t\t\t",
	     uuid_data,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print UUID value.",
		 function );

		return( -1 );
	}
	if( libfvde_physical_volume_get_size(
	     physical_volume,
	     &volume_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tSize\t\t\t\t: " );

	result = byte_size_string_create(
	          byte_size_string,
	          16,
	          volume_size,
	          BYTE_SIZE_STRING_UNIT_MEBIBYTE,
	          NULL );

	if( result == 1 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "%" PRIs_SYSTEM " (%" PRIu64 " bytes)",
		 byte_size_string,
		 volume_size );
	}
	else
	{
		fprintf(
		 info_handle->notify_stream,
		 "%" PRIu64 " bytes",
		 volume_size );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	if( libfvde_physical_volume_get_encryption_method(
	     physical_volume,
	     &encryption_method,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve encryption method.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tEncryption method\t\t: " );

	if( encryption_method == LIBFVDE_ENCRYPTION_METHOD_AES_128_XTS )
	{
		fprintf(
		 info_handle->notify_stream,
		 "AES-XTS 128-bit" );
	}
	else
	{
		fprintf(
		 info_handle->notify_stream,
		 "Unknown" );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	return( 1 );
}

/* Prints logical volume information
 * Returns 1 if successful or -1 on error
 */
int info_handle_logical_volume_fprint(
     info_handle_t *info_handle,
     int logical_volume_index,
     libfvde_logical_volume_t *logical_volume,
     libcerror_error_t **error )
{
        system_character_t byte_size_string[ 16 ];
	uint8_t uuid_data[ 16 ];

	system_character_t *value_string = NULL;
	static char *function            = "info_handle_logical_volume_fprint";
	size64_t volume_size             = 0;
	size_t value_string_size         = 0;
	int result                       = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "Logical volume: %d\n",
	 logical_volume_index + 1 );

	if( libfvde_logical_volume_get_identifier(
	     logical_volume,
	     uuid_data,
	     16,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve logical volume identifier.",
		 function );

		goto on_error;
	}
	if( info_handle_uuid_value_fprint(
	     info_handle,
	     "\tIdentifier\t\t\t",
	     uuid_data,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print UUID value.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tName\t\t\t\t:" );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfvde_logical_volume_get_utf16_name_size(
	          logical_volume,
	          &value_string_size,
	          error );
#else
	result = libfvde_logical_volume_get_utf8_name_size(
	          logical_volume,
	          &value_string_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve logical volume name string size.",
		 function );

		goto on_error;
	}
	else if( ( result != 0 )
	      && ( value_string_size > 0 ) )
	{
		value_string = system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create logical volume name string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfvde_logical_volume_get_utf16_name(
		          logical_volume,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libfvde_logical_volume_get_utf8_name(
		          logical_volume,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve logical volume name string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 " %" PRIs_SYSTEM "",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	if( libfvde_logical_volume_get_size(
	     logical_volume,
	     &volume_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve logical volume size.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tSize\t\t\t\t: " );

	result = byte_size_string_create(
	          byte_size_string,
	          16,
	          volume_size,
	          BYTE_SIZE_STRING_UNIT_MEBIBYTE,
	          NULL );

	if( result == 1 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "%" PRIs_SYSTEM " (%" PRIu64 " bytes)",
		 byte_size_string,
		 volume_size );
	}
	else
	{
		fprintf(
		 info_handle->notify_stream,
		 "%" PRIu64 " bytes",
		 volume_size );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	result = libfvde_logical_volume_is_locked(
	          logical_volume,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if the logical volume is locked.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "\tIs locked\n" );
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	return( 1 );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	return( -1 );
}

/* Prints the volume information to a stream
 * Returns 1 if successful or -1 on error
 */
int info_handle_volume_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	uint8_t uuid_data[ 16 ];

	libfvde_logical_volume_t *logical_volume   = NULL;
	libfvde_physical_volume_t *physical_volume = NULL;
	system_character_t *value_string           = NULL;
	static char *function                      = "info_handle_volume_fprint";
	size_t value_string_size                   = 0;
	int number_of_logical_volumes              = 0;
	int number_of_physical_volumes             = 0;
	int result                                 = 0;
	int volume_index                           = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "Core Storage information:\n" );

	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	fprintf(
	 info_handle->notify_stream,
	 "Logical volume group:\n" );

	if( libfvde_volume_group_get_identifier(
	     info_handle->volume_group,
	     uuid_data,
	     16,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve logical volume group identifier.",
		 function );

		goto on_error;
	}
	if( info_handle_uuid_value_fprint(
	     info_handle,
	     "\tIdentifier\t\t\t",
	     uuid_data,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print UUID value.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tName\t\t\t\t:" );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libfvde_volume_group_get_utf16_name_size(
	          info_handle->volume_group,
	          &value_string_size,
	          error );
#else
	result = libfvde_volume_group_get_utf8_name_size(
	          info_handle->volume_group,
	          &value_string_size,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve volume group name string size.",
		 function );

		goto on_error;
	}
	else if( ( result != 0 )
	      && ( value_string_size > 0 ) )
	{
		value_string = system_string_allocate(
		                value_string_size );

		if( value_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create volume group name string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfvde_volume_group_get_utf16_name(
		          info_handle->volume_group,
		          (uint16_t *) value_string,
		          value_string_size,
		          error );
#else
		result = libfvde_volume_group_get_utf8_name(
		          info_handle->volume_group,
		          (uint8_t *) value_string,
		          value_string_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve volume group name string.",
			 function );

			goto on_error;
		}
		fprintf(
		 info_handle->notify_stream,
		 " %" PRIs_SYSTEM "",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	if( libfvde_volume_group_get_number_of_physical_volumes(
	     info_handle->volume_group,
	     &number_of_physical_volumes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of physical volumes.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tNumber of physical volumes\t: %d\n",
	 number_of_physical_volumes );

	if( libcdata_array_get_number_of_entries(
	     info_handle->logical_volumes_array,
	     &number_of_logical_volumes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of logical volumes from array.",
		 function );

		goto on_error;
	}
	fprintf(
	 info_handle->notify_stream,
	 "\tNumber of logical volumes\t: %d\n",
	 number_of_logical_volumes );

	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	for( volume_index = 0;
	     volume_index < number_of_physical_volumes;
	     volume_index++ )
	{
		if( libfvde_volume_group_get_physical_volume_by_index(
		     info_handle->volume_group,
		     volume_index,
		     &physical_volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve physical volume: %d.",
			 function,
			 volume_index );

			goto on_error;
		}
		if( info_handle_physical_volume_fprint(
		     info_handle,
		     volume_index,
		     physical_volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print physical volume: %d information.",
			 function,
			 volume_index );

			goto on_error;
		}
		if( libfvde_physical_volume_free(
		     &physical_volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free physical volume: %d.",
			 function,
			 volume_index );

			goto on_error;
		}
	}
	for( volume_index = 0;
	     volume_index < number_of_logical_volumes;
	     volume_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     info_handle->logical_volumes_array,
		     volume_index,
		     (intptr_t **) &logical_volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve logical volume: %d from array.",
			 function,
			 volume_index );

			goto on_error;
		}
		result = libfvde_logical_volume_is_locked(
		          logical_volume,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if logical volume is locked.",
			 function );

			goto on_error;
		}
		if( info_handle_logical_volume_fprint(
		     info_handle,
		     volume_index,
		     logical_volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print logical volume: %d information.",
			 function,
			 volume_index );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( physical_volume != NULL )
	{
		libfvde_physical_volume_free(
		 &physical_volume,
		 NULL );
	}
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	return( -1 );
}

