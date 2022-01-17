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

#include "fvdetools_libbfio.h"
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

/* TODO implement

extern \
int libfvde_volume_open_physical_volume_files_file_io_pool(
     libfvde_volume_t *volume,
     libbfio_pool_t *file_io_pool,
     libcerror_error_t **error );

*/

#endif /* !defined( LIBFVDE_HAVE_BFIO ) */

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
		if( ( *mount_handle )->basename != NULL )
		{
			memory_free(
			 ( *mount_handle )->basename );
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

/* Sets the basename
 * Returns 1 if successful or -1 on error
 */
int mount_handle_set_basename(
     mount_handle_t *mount_handle,
     const system_character_t *basename,
     size_t basename_size,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_set_basename";

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
	if( mount_handle->basename != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid mount handle - basename value already set.",
		 function );

		return( -1 );
	}
	if( basename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid basename.",
		 function );

		return( -1 );
	}
	if( basename_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing basename.",
		 function );

		goto on_error;
	}
	if( basename_size > (size_t) ( SSIZE_MAX / sizeof( system_character_t ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid basename size value exceeds maximum.",
		 function );

		goto on_error;
	}
	mount_handle->basename = system_string_allocate(
	                          basename_size );

	if( mount_handle->basename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create basename string.",
		 function );

		goto on_error;
	}
	if( system_string_copy(
	     mount_handle->basename,
	     basename,
	     basename_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy basename.",
		 function );

		goto on_error;
	}
	mount_handle->basename[ basename_size - 1 ] = 0;

	mount_handle->basename_size = basename_size;

	return( 1 );

on_error:
	if( mount_handle->basename != NULL )
	{
		memory_free(
		 mount_handle->basename );

		mount_handle->basename = NULL;
	}
	mount_handle->basename_size = 0;

	return( -1 );
}

/* Sets the encrypted root plist file path
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

/* Sets the keys
 * Returns 1 if successful or -1 on error
 */
int mount_handle_set_keys(
     mount_handle_t *mount_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function   = "mount_handle_set_keys";
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
	mount_handle->key_size = 16;

	return( 1 );

on_error:
	memory_set(
	 mount_handle->key_data,
	 0,
	 16 );

	mount_handle->key_size = 0;

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

	mount_handle->password        = string;
	mount_handle->password_length = string_length;

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
     const system_character_t *filename,
     libcerror_error_t **error )
{
	libbfio_handle_t *volume_file_io_handle          = NULL;
	libbfio_handle_t *physical_volume_file_io_handle = NULL;
	libbfio_pool_t *physical_volume_file_io_pool     = NULL;
	libfvde_logical_volume_t *logical_volume         = NULL;
	libfvde_volume_t *volume                         = NULL;
	system_character_t *basename_end                 = NULL;
	static char *function                            = "mount_handle_open";
	size_t basename_length                           = 0;
	size_t filename_length                           = 0;
	int entry_index                                  = 0;
	int logical_volume_index                         = 0;
	int number_of_logical_volumes                    = 0;

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
	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	filename_length = system_string_length(
	                   filename );

	basename_end = system_string_search_character_reverse(
	                filename,
	                (system_character_t) LIBCPATH_SEPARATOR,
	                filename_length + 1 );

	if( basename_end != NULL )
	{
		basename_length = (size_t) ( basename_end - filename ) + 1;
	}
	if( basename_length > 0 )
	{
		if( mount_handle_set_basename(
		     mount_handle,
		     filename,
		     basename_length,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set basename.",
			 function );

			goto on_error;
		}
	}
	if( libbfio_file_range_initialize(
	     &volume_file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize volume file IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libbfio_file_range_set_name_wide(
	     volume_file_io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
#else
	if( libbfio_file_range_set_name(
	     volume_file_io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to set name.",
		 function );

		goto on_error;
	}
	if( libbfio_file_range_set(
	     volume_file_io_handle,
	     mount_handle->volume_offset,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to set volume offset.",
		 function );

		goto on_error;
	}
	if( libfvde_volume_initialize(
	     &volume,
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
	if( libfvde_volume_open_file_io_handle(
	     volume,
	     volume_file_io_handle,
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
/* TODO determine if the first file is a metadata only file and change filenames accordingly
 */

#ifdef TODO
/* TODO control maximum number of handles */
	if( libbfio_pool_initialize(
	     &physical_volume_file_io_pool,
	     0,
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
	if( libbfio_file_range_initialize(
	     &physical_volume_file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize physical volume file IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libbfio_file_range_set_name_wide(
	     physical_volume_file_io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
#else
	if( libbfio_file_range_set_name(
	     physical_volume_file_io_handle,
	     filename,
	     filename_length,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to set name.",
		 function );

		goto on_error;
	}
	if( libbfio_file_range_set(
	     physical_volume_file_io_handle,
	     mount_handle->volume_offset,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to set volume offset.",
		 function );

		goto on_error;
	}
	if( libbfio_pool_append_handle(
	     physical_volume_file_io_pool,
	     &entry_index,
	     physical_volume_file_io_handle,
	     LIBBFIO_OPEN_READ,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to append file IO handle to physical volume file IO pool.",
		 function );

		goto on_error;
	}
	/* The pool takes over management of the file IO handle
	 */
	physical_volume_file_io_handle = NULL;

	if( libfvde_volume_open_physical_volume_files_file_io_pool(
	     volume,
	     physical_volume_file_io_pool,
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
#endif /* TODO */

	if( libfvde_volume_get_volume_group(
	     volume,
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
		if( mount_file_system_append_logical_volume(
		     mount_handle->file_system,
		     logical_volume,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append logical volume: %d to file system.",
			 function,
			 logical_volume_index );

			goto on_error;
		}
		logical_volume = NULL;
	}
	mount_handle->file_io_handle               = volume_file_io_handle;
	mount_handle->volume                       = volume;
	mount_handle->physical_volume_file_io_pool = physical_volume_file_io_pool;

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
	if( physical_volume_file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &physical_volume_file_io_handle,
		 NULL );
	}
	if( physical_volume_file_io_pool != NULL )
	{
		libbfio_pool_free(
		 &physical_volume_file_io_pool,
		 NULL );
	}
	if( volume != NULL )
	{
		libfvde_volume_free(
		 &volume,
		 NULL );
	}
	if( volume_file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &volume_file_io_handle,
		 NULL );
	}
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

			return( -1 );
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

			return( -1 );
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

		return( -1 );
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

		return( -1 );
	}
	if( libfvde_volume_free(
	     &( mount_handle->volume ),
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free volume.",
		 function );

		return( -1 );
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

		return( -1 );
	}
	if( libbfio_handle_free(
	     &( mount_handle->file_io_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file IO handle.",
		 function );

		return( -1 );
	}
	return( 0 );
}

/* Unlocks an encrypted volume
 * Returns 1 if the volume is unlocked, 0 if not or -1 on error
 */
int mount_handle_input_unlock(
     mount_handle_t *mount_handle,
     libcerror_error_t **error )
{
	libfvde_volume_t *fvde_volume = NULL;
	static char *function         = "mount_handle_input_unlock";
	int result                    = 0;

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
/* TODO refactor
	if( mount_file_system_get_volume_by_index(
	     mount_handle->file_system,
	     0,
	     &fvde_volume,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve volume: 0.",
		 function );

		return( -1 );
	}
*/
	fvde_volume = mount_handle->volume;

	if( mount_handle->password != NULL )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libfvde_volume_set_utf16_password(
		     fvde_volume,
		     (uint16_t *) mount_handle->password,
		     mount_handle->password_length,
		     error ) != 1 )
#else
		if( libfvde_volume_set_utf8_password(
		     fvde_volume,
		     (uint8_t *) mount_handle->password,
		     mount_handle->password_length,
		     error ) != 1 )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set password.",
			 function );

			return( -1 );
		}
	}
	if( mount_handle->recovery_password != NULL )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libfvde_volume_set_utf16_recovery_password(
		     fvde_volume,
		     (uint16_t *) mount_handle->recovery_password,
		     mount_handle->recovery_password_length,
		     error ) != 1 )
#else
		if( libfvde_volume_set_utf8_recovery_password(
		     fvde_volume,
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

			return( -1 );
		}
	}
	result = libfvde_volume_unlock(
	          fvde_volume,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to unlock volume.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		mount_handle->is_locked = 0;
	}
	return( result );
}

/* Determine if the mount handle is locked
 * Returns 1 if locked, 0 if not or -1 on error
 */
int mount_handle_is_locked(
     mount_handle_t *mount_handle,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_is_locked";

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
	return( mount_handle->is_locked );
}

/* Retrieves a specific logical volume from the volume group
 * Returns 1 if successful or -1 on error
 */
int mount_handle_get_logical_volume_by_index(
     mount_handle_t *mount_handle,
     libfvde_volume_group_t *fvde_volume_group,
     int logical_volume_index,
     libfvde_logical_volume_t **fvde_logical_volume,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_get_logical_volume_by_index";
	int result            = 0;

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
	if( fvde_logical_volume == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid logical volume.",
		 function );

		return( -1 );
	}
	if( *fvde_logical_volume != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid logical volume value already set.",
		 function );

		return( -1 );
	}
	if( libfvde_volume_group_get_logical_volume_by_index(
	     fvde_volume_group,
	     logical_volume_index,
	     fvde_logical_volume,
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
	if( mount_handle->password != NULL )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( libfvde_logical_volume_set_utf16_password(
		     *fvde_logical_volume,
		     (uint16_t *) mount_handle->password,
		     mount_handle->password_length,
		     error ) != 1 )
#else
		if( libfvde_logical_volume_set_utf8_password(
		     *fvde_logical_volume,
		     (uint8_t *) mount_handle->password,
		     mount_handle->password_length,
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
		     *fvde_logical_volume,
		     (uint16_t *) mount_handle->recovery_password,
		     mount_handle->recovery_password_length,
		     error ) != 1 )
#else
		if( libfvde_logical_volume_set_utf8_recovery_password(
		     *fvde_logical_volume,
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
	result = libfvde_logical_volume_is_locked(
	          *fvde_logical_volume,
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
	else if( result != 0 )
	{
		if( libfvde_logical_volume_unlock(
		     *fvde_logical_volume,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to unlock logical volume.",
			 function );

			goto on_error;
		}
		mount_handle->is_locked = result;
	}
	return( 1 );

on_error:
	if( *fvde_logical_volume != NULL )
	{
		libfvde_logical_volume_free(
		 fvde_logical_volume,
		 NULL );
	}
	return( -1 );
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

