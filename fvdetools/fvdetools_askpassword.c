/*
 * Function to read password from stdin
 *
 * Copyright (C) 2017, Rob Wu <rob@robwu.nl>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <memory.h>
#include <stdio.h>
#include <types.h>

#if defined( _WIN32 )
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#include "fvdetools_askpassword.h"

#if defined( _WIN32 ) && defined( HAVE_WIDE_SYSTEM_CHARACTER )
#undef getch // conio.h defines getch as _getch, replace with _getwch
#define getch _getwch
#endif

#if !defined( _WIN32 )
// The code in fvdetools_askpassword ensures that getchar behaves like _getch.
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
#define getch getwchar
#else
#define getch getchar
#endif
#endif

system_character_t *reallocate_password_buffer(
                     system_character_t * password_buffer,
                     size_t size ) {
	system_character_t *newbuffer = memory_reallocate(
	                                 password_buffer,
	                                 size * sizeof( system_character_t ) );
	if( newbuffer == NULL )
	{
		fvdetools_free_password(
		 password_buffer );
	}

	return newbuffer;
}

// returns 1 on success, 0 on failure.
// asked_password becomes a non-NULL pointer (and the return value is 1)
// iff the call succeeded.
int fvdetools_ask_password(
     system_character_t ** asked_password,
     const system_character_t * prompt )
{
	size_t asked_password_length = 0;
	size_t input_buffer_size = 0;
	system_character_t ch;
	system_character_t *password_buffer = NULL;
	int changedmode = 0;
	char *error = NULL;

	// First try to force the terminal to not echo the password to stdout.

	// Note: errors in attempts to change the terminal mode are deliberately
	// ignored, in case stdio is a pipe instead of a console.
#if defined( _WIN32 )
	HANDLE hStdin = GetStdHandle(
	                 STD_INPUT_HANDLE );
	DWORD oldmode, newmode;
    if( GetConsoleMode(
	     hStdin,
	     &oldmode ) )
	{
		newmode = oldmode;
		newmode &= ~ENABLE_ECHO_INPUT;
		if( SetConsoleMode(
		     hStdin,
		     newmode ) )
		{
			changedmode = 1;
		}
	}
#else
	struct termios old_termios, new_termios;
	if( tcgetattr(
	     STDIN_FILENO,
	     &old_termios ) == 0 )
	{
		new_termios = old_termios;
		new_termios.c_lflag &= ~(ECHO | ICANON);
		if( tcsetattr(
		     STDIN_FILENO,
		     TCSADRAIN,
		     &new_termios ) == 0 )
		{
			changedmode = 1;
		}
	}
#endif

	// After disabling echoing of input, prompt for the password.
	printf(
	 "%s ",
	 prompt );
	fflush( stdout );

	for( ;; )
	{
		ch = getch();
		if( ( ch == (system_character_t) '\n' )
		 || ( ch == (system_character_t) '\r' )
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		 || ( ch == WEOF )
#else
		 || ( ch == EOF )
#endif
		 || ( ch == (system_character_t) '\0') )
		{
			break;
		}

		// Backspace and DEL.
		if( (ch == (system_character_t) '\b' )
		 || (ch == (system_character_t) 127 ) )
		{
			if( asked_password_length > 0 )
			{
				--asked_password_length;
			}
			continue;
		}

		if( input_buffer_size <= asked_password_length )
		{
			// Allocate more than necessary to avoid constant reallocation.
			password_buffer = reallocate_password_buffer(
			                   password_buffer,
			                   asked_password_length * 2 + 1 );
			if( password_buffer == NULL )
			{
				error = "Failed to allocate memory for password";
				break;
			}
		}
		password_buffer[ asked_password_length++ ] = ch;
	}

	password_buffer = reallocate_password_buffer(
	                   password_buffer,
	                   asked_password_length + 1 );
	if( password_buffer == NULL )
	{
		error = "Failed to allocate memory for password terminator";
	}
	else
	{
		password_buffer[ asked_password_length ] = (system_character_t) '\0';
	}

	// Restore the console / terminal state.
	if( changedmode )
	{
#if defined( _WIN32 )
		SetConsoleMode(
		 hStdin,
		 oldmode );
#else
		tcsetattr(
		 STDIN_FILENO,
		 TCSADRAIN,
		 &old_termios );
#endif
	}

	printf( "\n" );

	if( error )
	{
		fvdetools_free_password(
		 password_buffer );
		fprintf(
		 stderr,
		 "Failed to get password: %s\n",
		 error );
		*asked_password = NULL;
		return 0;
	}

	*asked_password = password_buffer;
	return 1;
}

void fvdetools_free_password(
      system_character_t* asked_password )
{
	memory_free(
	 asked_password );
}
