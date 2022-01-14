/*
 * Common input functions for the fvdetools
 *
 * Copyright (C) 2011-2022, Joachim Metz <joachim.metz@gmail.com>
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
#include <file_stream.h>
#include <memory.h>
#include <types.h>

#if defined( HAVE_TERMIOS_H )
#include <termios.h>
#endif

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "fvdetools_libcerror.h"
#include "fvdetools_input.h"

/* Prompts the user for a password
 * Returns 1 if successful or -1 on error
 */
int fvdetools_prompt_for_password(
     FILE *stream,
     const system_character_t *request_string,
     system_character_t *password,
     size_t password_size,
     libcerror_error_t **error )
{
#if !defined( WINAPI )
	struct termios echo_off_termios;
	struct termios original_termios;
#endif

	static char *function        = "fvdetools_prompt_for_password";
	system_character_t character = 0;
	size_t password_index        = 0;
	int result                   = 1;

#if defined( WINAPI )
	DWORD echo_off_mode          = 0;
	HANDLE input_handle          = INVALID_HANDLE_VALUE;
	DWORD original_mode          = 0;
#endif

	if( request_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid request string.",
		 function );

		return( -1 );
	}
	if( password == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid password.",
		 function );

		return( -1 );
	}
	if( ( password_size < 32 )
	 || ( password_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid password size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( WINAPI )
	input_handle = GetStdHandle(
	                STD_INPUT_HANDLE );

	if( input_handle == INVALID_HANDLE_VALUE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve input handle.",
		 function );

		return( -1 );
	}
	if( GetConsoleMode(
	     input_handle,
	     &original_mode ) == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve original console mode.",
		 function );

		return( -1 );
	}
#else
	if( tcgetattr(
	     STDIN_FILENO,
	     &original_termios ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve terminal IO settings.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     &echo_off_termios,
	     &original_termios,
	     sizeof( struct termios ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy terminal IO settings.",
		 function );

		return( -1 );
	}
#endif /* defined( WINAPI ) */

	fprintf(
	 stream,
	 "%" PRIs_SYSTEM ": ",
	 request_string );

#if defined( WINAPI )
	echo_off_mode = original_mode & ~( ENABLE_ECHO_INPUT );

	if( SetConsoleMode(
	     input_handle,
	     echo_off_mode ) == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set echo-off console mode.",
		 function );

		return( -1 );
	}
#else
	echo_off_termios.c_lflag &= ~( ECHO | ICANON );

	if( tcsetattr(
	     STDIN_FILENO,
	     TCSADRAIN,
	     &echo_off_termios ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set echo-off terminal IO settings.",
		 function );

		return( -1 );
	}
#endif /* defined( WINAPI ) */

	while( password_index < password_size )
	{
#if !defined( HAVE_WIDE_SYSTEM_CHARACTER )
		character = (system_character_t) getchar();
#elif defined( WINAPI )
		character = (system_character_t) _getwch();
#else
		character = (system_character_t) getwchar();
#endif
#if !defined( HAVE_WIDE_SYSTEM_CHARACTER )
		if( character == (system_character_t) EOF )
#else
		if( character == (system_character_t) WEOF )
#endif
		{
			break;
		}
		if( ( character == (system_character_t) '\n' )
		 || ( character == (system_character_t) '\r' )
		 || ( character == (system_character_t) 0 ) )
		{
			break;
		}
		/* Handle backspace
		 */
		if( character == (system_character_t) 0x08 )
		{
			if( password_index > 0 )
			{
				password_index--;
			}
		}
		else
		{
			password[ password_index++ ] = (system_character_t) character;
		}
	}
	if( password_index >= password_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid password size value too small.",
		 function );

		result = -1;
	}
	else
	{
		password[ password_index ] = (system_character_t) 0;
	}
#if defined( WINAPI )
	if( SetConsoleMode(
	     input_handle,
	     echo_off_mode ) == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set original console mode.",
		 function );

		return( -1 );
	}
#else
	if( tcsetattr(
	     STDIN_FILENO,
	     TCSADRAIN,
	     &original_termios ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set original terminal IO settings.",
		 function );

		return( -1 );
	}
#endif /* defined( WINAPI ) */

	return( result );
}

