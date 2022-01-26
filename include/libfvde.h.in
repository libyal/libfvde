/*
 * Library to access the FileVault Drive Encryption (FVDE) format
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

#if !defined( _LIBFVDE_H )
#define _LIBFVDE_H

#include <libfvde/codepage.h>
#include <libfvde/definitions.h>
#include <libfvde/error.h>
#include <libfvde/extern.h>
#include <libfvde/features.h>
#include <libfvde/types.h>

#include <stdio.h>

#if defined( LIBFVDE_HAVE_BFIO )
#include <libbfio.h>
#endif

#if defined( __cplusplus )
extern "C" {
#endif

/* -------------------------------------------------------------------------
 * Support functions
 * ------------------------------------------------------------------------- */

/* Returns the library version
 */
LIBFVDE_EXTERN \
const char *libfvde_get_version(
             void );

/* Returns the access flags for reading
 */
LIBFVDE_EXTERN \
int libfvde_get_access_flags_read(
     void );

/* Retrieves the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_get_codepage(
     int *codepage,
     libfvde_error_t **error );

/* Sets the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_set_codepage(
     int codepage,
     libfvde_error_t **error );

/* Determines if a file contains a FVDE volume signature
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_check_volume_signature(
     const char *filename,
     libfvde_error_t **error );

#if defined( LIBFVDE_HAVE_WIDE_CHARACTER_TYPE )

/* Determines if a file contains a FVDE volume signature
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_check_volume_signature_wide(
     const wchar_t *filename,
     libfvde_error_t **error );

#endif /* defined( LIBFVDE_HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBFVDE_HAVE_BFIO )

/* Determines if a file contains a FVDE volume signature using a Basic File IO (bfio) handle
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_check_volume_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libfvde_error_t **error );

#endif /* defined( LIBFVDE_HAVE_BFIO ) */

/* -------------------------------------------------------------------------
 * Notify functions
 * ------------------------------------------------------------------------- */

/* Sets the verbose notification
 */
LIBFVDE_EXTERN \
void libfvde_notify_set_verbose(
      int verbose );

/* Sets the notification stream
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_notify_set_stream(
     FILE *stream,
     libfvde_error_t **error );

/* Opens the notification stream using a filename
 * The stream is opened in append mode
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_notify_stream_open(
     const char *filename,
     libfvde_error_t **error );

/* Closes the notification stream if opened using a filename
 * Returns 0 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_notify_stream_close(
     libfvde_error_t **error );

/* -------------------------------------------------------------------------
 * Error functions
 * ------------------------------------------------------------------------- */

/* Frees an error
 */
LIBFVDE_EXTERN \
void libfvde_error_free(
      libfvde_error_t **error );

/* Prints a descriptive string of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_error_fprint(
     libfvde_error_t *error,
     FILE *stream );

/* Prints a descriptive string of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_error_sprint(
     libfvde_error_t *error,
     char *string,
     size_t size );

/* Prints a backtrace of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_error_backtrace_fprint(
     libfvde_error_t *error,
     FILE *stream );

/* Prints a backtrace of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_error_backtrace_sprint(
     libfvde_error_t *error,
     char *string,
     size_t size );

/* -------------------------------------------------------------------------
 * Volume functions
 * ------------------------------------------------------------------------- */

/* Creates a volume
 * Make sure the value volume is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_volume_initialize(
     libfvde_volume_t **volume,
     libfvde_error_t **error );

/* Frees a volume
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_volume_free(
     libfvde_volume_t **volume,
     libfvde_error_t **error );

/* Signals the volume to abort its current activity
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_volume_signal_abort(
     libfvde_volume_t *volume,
     libfvde_error_t **error );

/* Opens a volume
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_volume_open(
     libfvde_volume_t *volume,
     const char *filename,
     int access_flags,
     libfvde_error_t **error );

#if defined( LIBFVDE_HAVE_WIDE_CHARACTER_TYPE )

/* Opens a volume
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_volume_open_wide(
     libfvde_volume_t *volume,
     const wchar_t *filename,
     int access_flags,
     libfvde_error_t **error );

#endif /* defined( LIBFVDE_HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBFVDE_HAVE_BFIO )

/* Opens a volume using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_volume_open_file_io_handle(
     libfvde_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libfvde_error_t **error );

#endif /* defined( LIBFVDE_HAVE_BFIO ) */

/* Opens the physical volume files
 * This function assumes the physical volume files are in same order as defined by the metadata
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_volume_open_physical_volume_files(
     libfvde_volume_t *volume,
     char * const filenames[],
     int number_of_filenames,
     libfvde_error_t **error );

#if defined( LIBFVDE_HAVE_WIDE_CHARACTER_TYPE )

/* Opens the physical volume files
 * This function assumes the physical volume files are in same order as defined by the metadata
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_volume_open_physical_volume_files_wide(
     libfvde_volume_t *volume,
     wchar_t * const filenames[],
     int number_of_filenames,
     libfvde_error_t **error );

#endif /* defined( LIBFVDE_HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBFVDE_HAVE_BFIO )

/* Opens the physical volume files
 * This function assumes the physical volume files are in same order as defined by the metadata
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_volume_open_physical_volume_files_file_io_pool(
     libfvde_volume_t *volume,
     libbfio_pool_t *file_io_pool,
     libfvde_error_t **error );

#endif /* defined( LIBFVDE_HAVE_BFIO ) */

/* Closes a volume
 * Returns 0 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_volume_close(
     libfvde_volume_t *volume,
     libfvde_error_t **error );

/* Reads the EncryptedRoot.plist file
 * This function needs to be used before one of the open or unlock functions
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_volume_read_encrypted_root_plist(
     libfvde_volume_t *volume,
     const char *filename,
     libfvde_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Reads the EncryptedRoot.plist file
 * This function needs to be used before one of the open or unlock functions
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_volume_read_encrypted_root_plist_wide(
     libfvde_volume_t *volume,
     const wchar_t *filename,
     libfvde_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBFVDE_HAVE_BFIO )

/* Reads the EncryptedRoot.plist file using a Basic File IO (bfio) handle
 * This function needs to be used before one of the open or unlock functions
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_volume_read_encrypted_root_plist_file_io_handle(
     libfvde_volume_t *volume,
     libbfio_handle_t *file_io_handle,
     libfvde_error_t **error );

#endif /* defined( LIBFVDE_HAVE_BFIO ) */

/* Retrieves the volume group
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_volume_get_volume_group(
     libfvde_volume_t *volume,
     libfvde_volume_group_t **volume_group,
     libfvde_error_t **error );

/* -------------------------------------------------------------------------
 * Volume functions - deprecated
 * ------------------------------------------------------------------------- */

/* Unlocks the volume
 * Returns 1 if the volume is unlocked, 0 if not or -1 on error
 *
 * This function is deprecated use libfvde_logical_volume_unlock instead
 */
LIBFVDE_DEPRECATED \
LIBFVDE_EXTERN \
int libfvde_volume_unlock(
     libfvde_volume_t *volume,
     libfvde_error_t **error );

/* Reads data at the current offset into a buffer
 * Returns the number of bytes read or -1 on error
 *
 * This function is deprecated use libfvde_logical_volume_read_buffer instead
 */
LIBFVDE_DEPRECATED \
LIBFVDE_EXTERN \
ssize_t libfvde_volume_read_buffer(
         libfvde_volume_t *volume,
         void *buffer,
         size_t buffer_size,
         libfvde_error_t **error );

/* Reads data at a specific offset
 * Returns the number of bytes read or -1 on error
 *
 * This function is deprecated use libfvde_logical_volume_read_buffer_at_offset instead
 */
LIBFVDE_DEPRECATED \
LIBFVDE_EXTERN \
ssize_t libfvde_volume_read_buffer_at_offset(
         libfvde_volume_t *volume,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libfvde_error_t **error );

/* Seeks a certain offset of the data
 * Returns the offset if seek is successful or -1 on error
 *
 * This function is deprecated use libfvde_logical_volume_seek_offset instead
 */
LIBFVDE_DEPRECATED \
LIBFVDE_EXTERN \
off64_t libfvde_volume_seek_offset(
         libfvde_volume_t *volume,
         off64_t offset,
         int whence,
         libfvde_error_t **error );

/* Retrieves the the current offset of the data
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libfvde_logical_volume_get_offset instead
 */
LIBFVDE_DEPRECATED \
LIBFVDE_EXTERN \
int libfvde_volume_get_offset(
     libfvde_volume_t *volume,
     off64_t *offset,
     libfvde_error_t **error );

/* Determines if the volume is locked
 * Returns 1 if locked, 0 if not or -1 on error
 *
 * This function is deprecated use libfvde_logical_volume_is_locked instead
 */
LIBFVDE_DEPRECATED \
LIBFVDE_EXTERN \
int libfvde_volume_is_locked(
     libfvde_volume_t *volume,
     libfvde_error_t **error );

/* Retrieves the size
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libfvde_logical_volume_get_size instead
 */
LIBFVDE_DEPRECATED \
LIBFVDE_EXTERN \
int libfvde_volume_get_size(
     libfvde_volume_t *volume,
     size64_t *size,
     libfvde_error_t **error );

/* Retrieves the encryption method
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libfvde_physical_volume_get_encryption_method instead
 */
LIBFVDE_DEPRECATED \
LIBFVDE_EXTERN \
int libfvde_volume_get_encryption_method(
     libfvde_volume_t *volume,
     uint32_t *encryption_method,
     libfvde_error_t **error );

/* Retrieves the logical volume group identifier
 * The identifier is a UUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libfvde_volume_group_get_identifier instead
 */
LIBFVDE_DEPRECATED \
LIBFVDE_EXTERN \
int libfvde_volume_get_logical_volume_group_identifier(
     libfvde_volume_t *volume,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libfvde_error_t **error );

/* Retrieves the physical volume size
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libfvde_physical_volume_get_size instead
 */
LIBFVDE_DEPRECATED \
LIBFVDE_EXTERN \
int libfvde_volume_get_physical_volume_size(
     libfvde_volume_t *volume,
     size64_t *size,
     libfvde_error_t **error );

/* Retrieves the encryption method of the physical volume
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libfvde_physical_volume_get_encryption_method instead
 */
LIBFVDE_DEPRECATED \
LIBFVDE_EXTERN \
int libfvde_volume_get_physical_volume_encryption_method(
     libfvde_volume_t *volume,
     uint32_t *encryption_method,
     libfvde_error_t **error );

/* Retrieves the physical volume identifier
 * The identifier is a UUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libfvde_physical_volume_get_identifier instead
 */
LIBFVDE_DEPRECATED \
LIBFVDE_EXTERN \
int libfvde_volume_get_physical_volume_identifier(
     libfvde_volume_t *volume,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libfvde_error_t **error );

/* Retrieves the logical volume size
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libfvde_logica_volume_get_size instead
 */
LIBFVDE_DEPRECATED \
LIBFVDE_EXTERN \
int libfvde_volume_get_logical_volume_size(
     libfvde_volume_t *volume,
     size64_t *size,
     libfvde_error_t **error );

/* Retrieves the encryption method of the logical volume
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated
 */
LIBFVDE_DEPRECATED \
LIBFVDE_EXTERN \
int libfvde_volume_get_logical_volume_encryption_method(
     libfvde_volume_t *volume,
     uint32_t *encryption_method,
     libfvde_error_t **error );

/* Retrieves the logical volume identifier
 * The identifier is a UUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libfvde_logical_volume_get_identifier instead
 */
LIBFVDE_DEPRECATED \
LIBFVDE_EXTERN \
int libfvde_volume_get_logical_volume_identifier(
     libfvde_volume_t *volume,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libfvde_error_t **error );

/* Sets the keys
 * This function needs to be used before one of the open or unlock functions
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use libfvde_logical_volume_set_key instead
 */
LIBFVDE_DEPRECATED \
LIBFVDE_EXTERN \
int libfvde_volume_set_keys(
     libfvde_volume_t *volume,
     const uint8_t *volume_master_key,
     size_t volume_master_key_size,
     libfvde_error_t **error );

/* Sets an UTF-8 formatted password
 * This function needs to be used before one of the open or unlock functions
 * Returns 1 if successful, 0 if password is invalid or -1 on error
 *
 * This function is deprecated use libfvde_logical_volume_set_utf8_password instead
 */
LIBFVDE_DEPRECATED \
LIBFVDE_EXTERN \
int libfvde_volume_set_utf8_password(
     libfvde_volume_t *volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfvde_error_t **error );

/* Sets an UTF-16 formatted password
 * This function needs to be used before one of the open or unlock functions
 * Returns 1 if successful, 0 if password is invalid or -1 on error
 *
 * This function is deprecated use libfvde_logical_volume_set_utf16_password instead
 */
LIBFVDE_DEPRECATED \
LIBFVDE_EXTERN \
int libfvde_volume_set_utf16_password(
     libfvde_volume_t *volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfvde_error_t **error );

/* Sets an UTF-8 formatted recovery password
 * This function needs to be used before one of the open or unlock functions
 * Returns 1 if successful, 0 if recovery password is invalid or -1 on error
 *
 * This function is deprecated use libfvde_logical_volume_set_utf8_recovery_password instead
 */
LIBFVDE_DEPRECATED \
LIBFVDE_EXTERN \
int libfvde_volume_set_utf8_recovery_password(
     libfvde_volume_t *volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfvde_error_t **error );

/* Sets an UTF-16 formatted recovery password
 * This function needs to be used before one of the open or unlock functions
 * Returns 1 if successful, 0 if recovery password is invalid or -1 on error
 *
 * This function is deprecated use libfvde_logical_volume_set_utf16_recovery_password instead
 */
LIBFVDE_DEPRECATED \
LIBFVDE_EXTERN \
int libfvde_volume_set_utf16_recovery_password(
     libfvde_volume_t *volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfvde_error_t **error );

/* -------------------------------------------------------------------------
 * Volume group functions
 * ------------------------------------------------------------------------- */

/* Frees a volume group
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_volume_group_free(
     libfvde_volume_group_t **volume_group,
     libfvde_error_t **error );

/* Retrieves the logical volume group identifier
 * The identifier is a UUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_volume_group_get_identifier(
     libfvde_volume_group_t *volume_group,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libfvde_error_t **error );

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_volume_group_get_utf8_name_size(
     libfvde_volume_group_t *volume_group,
     size_t *utf8_string_size,
     libfvde_error_t **error );

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_volume_group_get_utf8_name(
     libfvde_volume_group_t *volume_group,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libfvde_error_t **error );

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_volume_group_get_utf16_name_size(
     libfvde_volume_group_t *volume_group,
     size_t *utf16_string_size,
     libfvde_error_t **error );

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_volume_group_get_utf16_name(
     libfvde_volume_group_t *volume_group,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libfvde_error_t **error );

/* Retrieves the number of physical volumes
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_volume_group_get_number_of_physical_volumes(
     libfvde_volume_group_t *volume_group,
     int *number_of_physical_volumes,
     libfvde_error_t **error );

/* Retrieves a specific physical volume
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_volume_group_get_physical_volume_by_index(
     libfvde_volume_group_t *volume_group,
     int volume_index,
     libfvde_physical_volume_t **physical_volume,
     libfvde_error_t **error );

/* Retrieves the number of logical volumes
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_volume_group_get_number_of_logical_volumes(
     libfvde_volume_group_t *volume_group,
     int *number_of_logical_volumes,
     libfvde_error_t **error );

/* Retrieves a specific logical volume
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_volume_group_get_logical_volume_by_index(
     libfvde_volume_group_t *volume_group,
     int volume_index,
     libfvde_logical_volume_t **logical_volume,
     libfvde_error_t **error );

/* -------------------------------------------------------------------------
 * Physical volume functions
 * ------------------------------------------------------------------------- */

/* Frees a physical volume
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_physical_volume_free(
     libfvde_physical_volume_t **physical_volume,
     libfvde_error_t **error );

/* Retrieves the physical volume identifier
 * The identifier is a UUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_physical_volume_get_identifier(
     libfvde_physical_volume_t *physical_volume,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libfvde_error_t **error );

/* Retrieves the encryption method
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_physical_volume_get_encryption_method(
     libfvde_physical_volume_t *physical_volume,
     uint32_t *encryption_method,
     libfvde_error_t **error );

/* Retrieves the size
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_physical_volume_get_size(
     libfvde_physical_volume_t *physical_volume,
     size64_t *size,
     libfvde_error_t **error );

/* -------------------------------------------------------------------------
 * Logical volume functions
 * ------------------------------------------------------------------------- */

/* Frees a logical volume
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_logical_volume_free(
     libfvde_logical_volume_t **logical_volume,
     libfvde_error_t **error );

/* Unlocks the logical volume
 * Returns 1 if the volume is unlocked, 0 if not or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_logical_volume_unlock(
     libfvde_logical_volume_t *logical_volume,
     libfvde_error_t **error );

/* Reads data at the current offset into a buffer
 * Returns the number of bytes read or -1 on error
 */
LIBFVDE_EXTERN \
ssize_t libfvde_logical_volume_read_buffer(
         libfvde_logical_volume_t *logical_volume,
         void *buffer,
         size_t buffer_size,
         libfvde_error_t **error );

/* Reads data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
LIBFVDE_EXTERN \
ssize_t libfvde_logical_volume_read_buffer_at_offset(
         libfvde_logical_volume_t *logical_volume,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libfvde_error_t **error );

/* Seeks a certain offset of the data
 * Returns the offset if seek is successful or -1 on error
 */
LIBFVDE_EXTERN \
off64_t libfvde_logical_volume_seek_offset(
         libfvde_logical_volume_t *logical_volume,
         off64_t offset,
         int whence,
         libfvde_error_t **error );

/* Retrieves the the current offset of the data
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_logical_volume_get_offset(
     libfvde_logical_volume_t *logical_volume,
     off64_t *offset,
     libfvde_error_t **error );

/* Retrieves the logical volume identifier
 * The identifier is a UUID and is 16 bytes of size
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_logical_volume_get_identifier(
     libfvde_logical_volume_t *logical_volume,
     uint8_t *uuid_data,
     size_t uuid_data_size,
     libfvde_error_t **error );

/* Retrieves the size of the UTF-8 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_logical_volume_get_utf8_name_size(
     libfvde_logical_volume_t *logical_volume,
     size_t *utf8_string_size,
     libfvde_error_t **error );

/* Retrieves the UTF-8 encoded name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_logical_volume_get_utf8_name(
     libfvde_logical_volume_t *logical_volume,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libfvde_error_t **error );

/* Retrieves the size of the UTF-16 encoded name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_logical_volume_get_utf16_name_size(
     libfvde_logical_volume_t *logical_volume,
     size_t *utf16_string_size,
     libfvde_error_t **error );

/* Retrieves the UTF-16 encoded name
 * The size should include the end of string character
 * Returns 1 if successful, 0 if not available or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_logical_volume_get_utf16_name(
     libfvde_logical_volume_t *logical_volume,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libfvde_error_t **error );

/* Retrieves the size
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_logical_volume_get_size(
     libfvde_logical_volume_t *logical_volume,
     size64_t *size,
     libfvde_error_t **error );

/* Determines if the logical volume is locked
 * Returns 1 if locked, 0 if not or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_logical_volume_is_locked(
     libfvde_logical_volume_t *logical_volume,
     libfvde_error_t **error );

/* Sets the key
 * This function needs to be used before the unlock function
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_logical_volume_set_key(
     libfvde_logical_volume_t *logical_volume,
     const uint8_t *volume_master_key,
     size_t volume_master_key_size,
     libfvde_error_t **error );

/* Sets an UTF-8 formatted password
 * This function needs to be used before the unlock function
 * Returns 1 if successful, 0 if password is invalid or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_logical_volume_set_utf8_password(
     libfvde_logical_volume_t *logical_volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfvde_error_t **error );

/* Sets an UTF-16 formatted password
 * This function needs to be used before the unlock function
 * Returns 1 if successful, 0 if password is invalid or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_logical_volume_set_utf16_password(
     libfvde_logical_volume_t *logical_volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfvde_error_t **error );

/* Sets an UTF-8 formatted recovery password
 * This function needs to be used before the unlock function
 * Returns 1 if successful, 0 if recovery password is invalid or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_logical_volume_set_utf8_recovery_password(
     libfvde_logical_volume_t *logical_volume,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libfvde_error_t **error );

/* Sets an UTF-16 formatted recovery password
 * This function needs to be used before the unlock function
 * Returns 1 if successful, 0 if recovery password is invalid or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_logical_volume_set_utf16_recovery_password(
     libfvde_logical_volume_t *logical_volume,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libfvde_error_t **error );

/* -------------------------------------------------------------------------
 * LVF encryption context and EncryptedRoot.plist file functions
 * ------------------------------------------------------------------------- */

/* Creates an encryption context plist
 * Make sure the value plist is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_encryption_context_plist_initialize(
     libfvde_encryption_context_plist_t **plist,
     libfvde_error_t **error );

/* Frees an encryption context plist
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_encryption_context_plist_free(
     libfvde_encryption_context_plist_t **plist,
     libfvde_error_t **error );

/* Retrieves the (un)encrypted data size of an encryption context plist
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_encryption_context_plist_get_data_size(
     libfvde_encryption_context_plist_t *plist,
     size64_t *data_size,
     libfvde_error_t **error );

/* Copies the unencrypted data of an encryption context plist
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_encryption_context_plist_copy_data(
     libfvde_encryption_context_plist_t *plist,
     uint8_t *data,
     size_t data_size,
     libfvde_error_t **error );

#if defined( LIBFVDE_HAVE_BFIO )

/* Reads an encryption context plist file (EncryptedRoot.plist) using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_encryption_context_plist_read_file_io_handle(
     libfvde_encryption_context_plist_t *plist,
     libbfio_handle_t *file_io_handle,
     libfvde_error_t **error );

#endif /* defined( LIBFVDE_HAVE_BFIO ) */

/* Decrypts the previously read encryption context plist file (EncryptedRoot.plist) with the given key.
 * Returns 1 if successful, 0 if not or -1 on error
 */
LIBFVDE_EXTERN \
int libfvde_encryption_context_plist_decrypt(
     libfvde_encryption_context_plist_t *plist,
     const uint8_t *key,
     size_t key_bit_size,
     libfvde_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBFVDE_H ) */

