/*
 * Mount handle
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

#include <common.h>
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "fvdetools_input.h"
#include "fvdetools_libbfio.h"
#include "fvdetools_libcdata.h"
#include "fvdetools_libcerror.h"
#include "fvdetools_libcpath.h"
#include "fvdetools_libfvde.h"
#include "fvdetools_libuna.h"
#include "mount_file_entry.h"
#include "mount_file_system.h"
#include "mount_handle.h"

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

#define MOUNT_HANDLE_NOTIFY_STREAM		stdout

/* Copies a string of a decimal value to a 64-bit value
 * Returns 1 if successful or -1 on error
 */
int mount_handle_system_string_copy_from_64_bit_in_decimal(
     const system_character_t *string,
     size_t string_size,
     uint64_t *value_64bit,
     libcerror_error_t **error )
{
	static char *function              = "mount_handle_system_string_copy_from_64_bit_in_decimal";
	system_character_t character_value = 0;
	size_t string_index                = 0;
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

/* Creates a mount handle
 * Make sure the value mount_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int mount_handle_initialize(
     mount_handle_t **mount_handle,
     int unattended_mode,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_initialize";

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( *mount_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid mount handle value already set.",
		 function );

		return( -1 );
	}
	*mount_handle = memory_allocate_structure(
	                 mount_handle_t );

	if( *mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create mount handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *mount_handle,
	     0,
	     sizeof( mount_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear mount handle.",
		 function );

		goto on_error;
	}
	if( mount_file_system_initialize(
	     &( ( *mount_handle )->file_system ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize file system.",
		 function );

		goto on_error;
	}
	( *mount_handle )->notify_stream   = MOUNT_HANDLE_NOTIFY_STREAM;
	( *mount_handle )->unattended_mode = unattended_mode;

	return( 1 );

on_error:
	if( *mount_handle != NULL )
	{
		memory_free(
		 *mount_handle );

		*mount_handle = NULL;
	}
	return( -1 );
}

/* Frees a mount handle
 * Returns 1 if successful or -1 on error
 */
int mount_handle_free(
     mount_handle_t **mount_handle,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_free";
	int result            = 1;

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( *mount_handle != NULL )
	{
		if( ( *mount_handle )->physical_volume_file_io_pool != NULL )
		{
			if( mount_handle_close(
			     *mount_handle,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close mount handle.",
				 function );

				result = -1;
			}
		}
		if( mount_file_system_free(
		     &( ( *mount_handle )->file_system ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file system.",
			 function );

			result = -1;
		}
		if( memory_set(
		     ( *mount_handle )->key_data,
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
		 *mount_handle );

		*mount_handle = NULL;
	}
	return( result );
}

/* Signals the mount handle to abort
 * Returns 1 if successful or -1 on error
 */
int mount_handle_signal_abort(
     mount_handle_t *mount_handle,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_signal_abort";

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( mount_handle->volume != NULL )
	{
		if( libfvde_volume_signal_abort(
		     mount_handle->volume,
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
	return( 1 );
}

/* Sets the path of the EncryptedRoot.plist.wipekey file
 * Returns 1 if successful or -1 on error
 */
int mount_handle_set_encrypted_root_plist(
     mount_handle_t *mount_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_set_encrypted_root_plist";

	if( mount_handle == NULL )
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
	mount_handle->encrypted_root_plist_path = string;

	return( 1 );
}

/* Sets the key
 * Returns 1 if successful or -1 on error
 */
int mount_handle_set_key(
     mount_handle_t *mount_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function   = "mount_handle_set_key";
	size_t string_length    = 0;
	uint32_t base16_variant = 0;

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	if( memory_set(
	     mount_handle->key_data,
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
	     mount_handle->key_data,
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
	mount_handle->key_data_size = 16;

	return( 1 );

on_error:
	memory_set(
	 mount_handle->key_data,
	 0,
	 16 );

	mount_handle->key_data_size = 0;

	return( -1 );
}

/* Sets the volume offset
 * Returns 1 if successful or -1 on error
 */
int mount_handle_set_offset(
     mount_handle_t *mount_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_set_offset";
	size_t string_length  = 0;
	uint64_t value_64bit  = 0;

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	string_length = system_string_length(
	                 string );

	if( mount_handle_system_string_copy_from_64_bit_in_decimal(
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
	mount_handle->volume_offset = (off64_t) value_64bit;

	return( 1 );
}

/* Sets the password
 * Returns 1 if successful or -1 on error
 */
int mount_handle_set_password(
     mount_handle_t *mount_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_set_password";
	size_t string_length  = 0;

	if( mount_handle == NULL )
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

	mount_handle->user_password        = string;
	mount_handle->user_password_length = string_length;

	return( 1 );
}

/* Sets the recovery password
 * Returns 1 if successful or -1 on error
 */
int mount_handle_set_recovery_password(
     mount_handle_t *mount_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_set_recovery_password";
	size_t string_length  = 0;

	if( mount_handle == NULL )
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

	mount_handle->recovery_password        = string;
	mount_handle->recovery_password_length = string_length;

	return( 1 );
}

/* Sets the path prefix
 * Returns 1 if successful or -1 on error
 */
int mount_handle_set_path_prefix(
     mount_handle_t *mount_handle,
     const system_character_t *path_prefix,
     size_t path_prefix_size,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_set_path_prefix";

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( mount_file_system_set_path_prefix(
	     mount_handle->file_system,
	     path_prefix,
	     path_prefix_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set path prefix.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Opens the mount handle
 * Returns 1 if successful or -1 on error
 */
int mount_handle_open(
     mount_handle_t *mount_handle,
     system_character_t * const * filenames,
     int number_of_filenames,
     libcerror_error_t **error )
{
	system_character_t password[ 64 ];

	libbfio_handle_t *file_io_handle         = NULL;
	libfvde_logical_volume_t *logical_volume = NULL;
	static char *function                    = "mount_handle_open";
	size_t filename_length                   = 0;
	size_t password_length                   = 0;
	int filename_index                       = 0;
	int logical_volume_index                 = 0;
	int number_of_logical_volumes            = 0;
	int result                               = 0;

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( mount_handle->physical_volume_file_io_pool != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid mount handle - physical volume file IO pool value already set.",
		 function );

		return( -1 );
	}
	if( mount_handle->volume != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid mount handle - volume value already set.",
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
	     mount_handle->volume_offset,
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
	     &( mount_handle->volume ),
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
	if( mount_handle->encrypted_root_plist_path != NULL )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libfvde_volume_read_encrypted_root_plist_wide(
		     mount_handle->volume,
		     mount_handle->encrypted_root_plist_path,
		     error ) != 1 )
#else
		if( libfvde_volume_read_encrypted_root_plist(
		     mount_handle->volume,
		     mount_handle->encrypted_root_plist_path,
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
	     mount_handle->volume,
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
	     &( mount_handle->physical_volume_file_io_pool ),
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
	     mount_handle->physical_volume_file_io_pool,
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
		     mount_handle->volume_offset,
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
		     mount_handle->physical_volume_file_io_pool,
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
	     mount_handle->volume,
	     mount_handle->physical_volume_file_io_pool,
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
	     mount_handle->volume,
	     &( mount_handle->volume_group ),
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
	     mount_handle->volume_group,
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
		     mount_handle->volume_group,
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
		if( mount_handle->key_data_size != 0 )
		{
			if( libfvde_logical_volume_set_key(
			     logical_volume,
			     mount_handle->key_data,
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
		if( mount_handle->user_password != NULL )
		{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			if( libfvde_logical_volume_set_utf16_password(
			     logical_volume,
			     (uint16_t *) mount_handle->user_password,
			     mount_handle->user_password_length,
			     error ) != 1 )
#else
			if( libfvde_logical_volume_set_utf8_password(
			     logical_volume,
			     (uint8_t *) mount_handle->user_password,
			     mount_handle->user_password_length,
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
		if( mount_handle->recovery_password != NULL )
		{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			if( libfvde_logical_volume_set_utf16_recovery_password(
			     logical_volume,
			     (uint16_t *) mount_handle->recovery_password,
			     mount_handle->recovery_password_length,
			     error ) != 1 )
#else
			if( libfvde_logical_volume_set_utf8_recovery_password(
			     logical_volume,
			     (uint8_t *) mount_handle->recovery_password,
			     mount_handle->recovery_password_length,
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
		      && ( mount_handle->unattended_mode == 0 ) )
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
		if( mount_file_system_append_logical_volume(
		     mount_handle->file_system,
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
	if( mount_handle->volume_group != NULL )
	{
		libfvde_volume_group_free(
		 &( mount_handle->volume_group ),
		 NULL );
	}
	if( mount_handle->volume != NULL )
	{
		libfvde_volume_free(
		 &( mount_handle->volume ),
		 NULL );
	}
	/* The file IO pool must be freed after the volume
	 */
	if( mount_handle->physical_volume_file_io_pool != NULL )
	{
		libbfio_pool_free(
		 &( mount_handle->physical_volume_file_io_pool ),
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

/* Closes the mount handle
 * Returns the 0 if succesful or -1 on error
 */
int mount_handle_close(
     mount_handle_t *mount_handle,
     libcerror_error_t **error )
{
	libfvde_logical_volume_t *logical_volume = NULL;
	static char *function                    = "mount_handle_close";
	int logical_volume_index                 = 0;
	int number_of_logical_volumes            = 0;
	int result                               = 0;

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( mount_file_system_get_number_of_logical_volumes(
	     mount_handle->file_system,
	     &number_of_logical_volumes,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of logical volumes.",
		 function );

		return( -1 );
	}
	for( logical_volume_index = number_of_logical_volumes - 1;
	     logical_volume_index > 0;
	     logical_volume_index-- )
	{
		if( mount_file_system_get_logical_volume_by_index(
		     mount_handle->file_system,
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

			result = -1;
		}
		if( libfvde_logical_volume_free(
		     &logical_volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free logical volume: %d.",
			 function,
			 logical_volume_index );

			result = -1;
		}
	}
	if( libfvde_volume_group_free(
	     &( mount_handle->volume_group ),
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
	if( libfvde_volume_close(
	     mount_handle->volume,
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
	     &( mount_handle->volume ),
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
	if( libbfio_pool_free(
	     &( mount_handle->physical_volume_file_io_pool ),
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

/* Retrieves a file entry for a specific path
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int mount_handle_get_file_entry_by_path(
     mount_handle_t *mount_handle,
     const system_character_t *path,
     mount_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libfvde_logical_volume_t *logical_volume = NULL;
	const system_character_t *filename       = NULL;
	static char *function                    = "mount_handle_get_file_entry_by_path";
	size_t path_length                       = 0;
	int logical_volume_index                 = 0;
	int result                               = 0;

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		return( -1 );
	}
	path_length = system_string_length(
	               path );

	if( path_length == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid path length value out of bounds.",
		 function );

		return( -1 );
	}
	result = mount_file_system_get_logical_volume_index_from_path(
	          mount_handle->file_system,
	          path,
	          path_length,
	          &logical_volume_index,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve logical volume index.",
		 function );

		return( -1 );
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	if( logical_volume_index != -1 )
	{
		if( mount_file_system_get_logical_volume_by_index(
		     mount_handle->file_system,
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

			return( -1 );
		}
		if( logical_volume == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing logical volume: %d.",
			 function,
			 logical_volume_index );

			return( -1 );
		}
		filename = &( path[ 0 ] );
	}
	if( mount_file_entry_initialize(
	     file_entry,
	     mount_handle->file_system,
	     logical_volume_index,
	     filename,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize file entry for logical volume: %d.",
		 function,
		 logical_volume_index );

		return( -1 );
	}
	return( 1 );
}

