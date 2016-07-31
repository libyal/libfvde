%{
/*
 * XML plist scanner functions
 *
 * Copyright (C) 2011-2016, Omar Choudary <choudary.omar@gmail.com>
 *                          Joachim Metz <joachim.metz@gmail.com>
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
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "libfvde_libcerror.h"
#include "libfvde_libcnotify.h"
#include "libfvde_types.h"
#include "libfvde_xml_plist.h"
#include "libfvde_xml_plist_tag.h"

#define YYMALLOC	xml_plist_scanner_alloc
#define YYREALLOC	xml_plist_scanner_realloc
#define YYFREE		xml_plist_scanner_free

#define YYLEX_PARAM	NULL
#define YYPARSE_PARAM	parser_state

#if defined( HAVE_DEBUG_OUTPUT )
#define xml_plist_parser_rule_print( string ) \
	if( libcnotify_verbose != 0 ) libcnotify_printf( "xml_plist_parser: rule: %s\n", string )
#else
#define xml_plist_parser_rule_print( string )
#endif

%}

/* %name-prefix="xml_plist_scanner_" replaced by -p xml_plist_scanner_ */
/* %no-lines replaced by -l */

%lex-param { (void *) NULL }
%parse-param { void *parser_state }

%start xml_plist_main

%union
{
        /* The numeric value
         */
        uint32_t numeric_value;

        /* The string value
         */
	struct xml_plist_string_value
	{
		/* The string data
		 */
	        const char *data;

		/* The string length
		 */
		size_t length;

	} string_value;
}

%{

typedef struct xml_plist_parser_state xml_plist_parser_state_t;

struct xml_plist_parser_state
{
	/* The XML plist
	 */
	libfvde_xml_plist_t *xml_plist;

	/* The error
	 */
	libcerror_error_t **error;

	/* The root tag
	 */
	libfvde_xml_plist_tag_t *root_tag;

	/* The current tag
	 */
	libfvde_xml_plist_tag_t *current_tag;

	/* The parent tag
	 */
	libfvde_xml_plist_tag_t *parent_tag;
};

typedef size_t yy_size_t;
typedef struct yy_buffer_state* YY_BUFFER_STATE;

extern int xml_plist_scanner_suppress_error;

extern int xml_plist_scanner_lex_destroy(
            void );

extern void *xml_plist_scanner_alloc(
              yy_size_t size );

extern void *xml_plist_scanner_realloc(
              void *buffer,
              yy_size_t size );

extern void *xml_plist_scanner_free(
              void *buffer );

extern int xml_plist_scanner_lex(
            void *user_data );

extern void xml_plist_scanner_error(
             void *parser_state,
             const char *error_string );

extern YY_BUFFER_STATE xml_plist_scanner__scan_buffer(
                        char *buffer,
                        yy_size_t buffer_size );

extern void xml_plist_scanner__delete_buffer(
             YY_BUFFER_STATE buffer_state );

extern size_t xml_plist_scanner_buffer_offset;

static char *xml_plist_parser_function = "xml_plist_parser";

int xml_plist_parser_parse_buffer(
     libfvde_xml_plist_t *xml_plist,
     const uint8_t *buffer,
     size_t buffer_size,
     libcerror_error_t **error );

%}

/* Associativity rules
 */

/* Token definitions
 */
%token XML_ATTRIBUTE_ASSIGN
%token XML_COMMENT
%token XML_DOCTYPE
%token XML_PROLOGUE
%token XML_TAG_END
%token XML_TAG_END_SINGLE

%token <string_value> XML_ATTRIBUTE_NAME
%token <string_value> XML_ATTRIBUTE_VALUE
%token <string_value> XML_TAG_CLOSE
%token <string_value> XML_TAG_CONTENT
%token <string_value> XML_TAG_OPEN_START

%%

/* Parser rules
 */

xml_plist_main
	: xml_tag_open xml_tags xml_tag_close
	{
		xml_plist_parser_rule_print(
		 "xml_plist_main" );
	}
	;

xml_tags
	: /* empty */
	| xml_tag xml_tags
	{
		xml_plist_parser_rule_print(
		 "xml_tags" );
	}
	;

xml_tag
	: xml_tag_open xml_tags xml_tag_close
	{
		xml_plist_parser_rule_print(
		 "xml_tag" );
	}
	| xml_tag_open xml_tag_content xml_tag_close
	{
		xml_plist_parser_rule_print(
		 "xml_tag" );
	}
	| xml_tag_single
	{
		xml_plist_parser_rule_print(
		 "xml_tag" );
	}
	;

xml_tag_open_start
	: XML_TAG_OPEN_START
	{
		xml_plist_parser_rule_print(
		 "xml_tag_open_start" );

		if( $1.data == NULL )
		{
			libcerror_error_set(
			 ( (xml_plist_parser_state_t *) parser_state )->error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid tag name.",
			 xml_plist_parser_function );

			YYABORT;
		}
		( (xml_plist_parser_state_t *) parser_state )->current_tag = NULL;

		if( libfvde_xml_plist_tag_initialize(
		     &( ( (xml_plist_parser_state_t *) parser_state )->current_tag ),
		     (uint8_t *) $1.data,
		     $1.length,
		     ( (xml_plist_parser_state_t *) parser_state )->error ) != 1 )
		{
			libcerror_error_set(
			 ( (xml_plist_parser_state_t *) parser_state )->error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create XML plist tag.",
			 xml_plist_parser_function );

			YYABORT;
		}
		if( ( (xml_plist_parser_state_t *) parser_state )->root_tag == NULL )
		{
			( (xml_plist_parser_state_t *) parser_state )->root_tag = ( (xml_plist_parser_state_t *) parser_state )->current_tag;
		}
		else
		{
			if( libfvde_xml_plist_tag_append_element(
			     ( (xml_plist_parser_state_t *) parser_state )->parent_tag,
			     ( (xml_plist_parser_state_t *) parser_state )->current_tag,
			     ( (xml_plist_parser_state_t *) parser_state )->error ) != 1 )
			{
				libcerror_error_set(
				 ( (xml_plist_parser_state_t *) parser_state )->error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append attribute.",
				 xml_plist_parser_function );

				YYABORT;
			}
		}
		( (xml_plist_parser_state_t *) parser_state )->parent_tag = ( (xml_plist_parser_state_t *) parser_state )->current_tag;
	}
	;

xml_tag_open
	: xml_tag_open_start xml_attributes XML_TAG_END
	{
		xml_plist_parser_rule_print(
		 "xml_tag_open" );
	}
	;

xml_tag_single
	: xml_tag_open_start xml_attributes XML_TAG_END_SINGLE
	{
		xml_plist_parser_rule_print(
		 "xml_tag_single" );

		if( ( (xml_plist_parser_state_t *) parser_state )->current_tag == NULL )
		{
			libcerror_error_set(
			 ( (xml_plist_parser_state_t *) parser_state )->error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid current tag.",
			 xml_plist_parser_function );

			YYABORT;
		}
		( (xml_plist_parser_state_t *) parser_state )->parent_tag  = ( (xml_plist_parser_state_t *) parser_state )->current_tag->parent_tag;
		( (xml_plist_parser_state_t *) parser_state )->current_tag = ( (xml_plist_parser_state_t *) parser_state )->parent_tag;
	}
	;

xml_tag_close
	: XML_TAG_CLOSE
	{
		xml_plist_parser_rule_print(
		 "xml_tag_close" );

		if( $1.data == NULL )
		{
			libcerror_error_set(
			 ( (xml_plist_parser_state_t *) parser_state )->error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid tag name.",
			 xml_plist_parser_function );

			YYABORT;
		}
		if( ( (xml_plist_parser_state_t *) parser_state )->current_tag == NULL )
		{
			libcerror_error_set(
			 ( (xml_plist_parser_state_t *) parser_state )->error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid current tag.",
			 xml_plist_parser_function );

			YYABORT;
		}
		if( ( ( (xml_plist_parser_state_t *) parser_state )->current_tag->name_size != ( $1.length + 1 ) )
		 || ( libcstring_narrow_string_compare(
		       ( (xml_plist_parser_state_t *) parser_state )->current_tag->name,
		       $1.data,
		       $1.length ) != 0 ) )
		{
			libcerror_error_set(
			 ( (xml_plist_parser_state_t *) parser_state )->error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: mismatch in tag name.",
			 xml_plist_parser_function );

			YYABORT;
		}
		( (xml_plist_parser_state_t *) parser_state )->parent_tag  = ( (xml_plist_parser_state_t *) parser_state )->current_tag->parent_tag;
		( (xml_plist_parser_state_t *) parser_state )->current_tag = ( (xml_plist_parser_state_t *) parser_state )->parent_tag;
	}
	;

xml_tag_content
	: XML_TAG_CONTENT
	{
		xml_plist_parser_rule_print(
		 "xml_tag_content" );

		if( $1.data == NULL )
		{
			libcerror_error_set(
			 ( (xml_plist_parser_state_t *) parser_state )->error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid attribute value.",
			 xml_plist_parser_function );

			YYABORT;
		}
		if( libfvde_xml_plist_tag_set_value(
		     ( (xml_plist_parser_state_t *) parser_state )->current_tag,
		     (uint8_t *) $1.data,
		     $1.length,
		     ( (xml_plist_parser_state_t *) parser_state )->error ) != 1 )
		{
			libcerror_error_set(
			 ( (xml_plist_parser_state_t *) parser_state )->error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set value.",
			 xml_plist_parser_function );

			YYABORT;
		}
	}
	;

xml_attributes
	: /* empty */
	| xml_attribute xml_attributes
	{
		xml_plist_parser_rule_print(
		 "xml_attributes" );
	}
	;

xml_attribute
	: XML_ATTRIBUTE_NAME XML_ATTRIBUTE_ASSIGN XML_ATTRIBUTE_VALUE
	{
		xml_plist_parser_rule_print(
		 "xml_attribute" );

		if( $1.data == NULL )
		{
			libcerror_error_set(
			 ( (xml_plist_parser_state_t *) parser_state )->error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid attribute name.",
			 xml_plist_parser_function );

			YYABORT;
		}
		if( $3.data == NULL )
		{
			libcerror_error_set(
			 ( (xml_plist_parser_state_t *) parser_state )->error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid attribute value.",
			 xml_plist_parser_function );

			YYABORT;
		}
		if( libfvde_xml_plist_tag_append_attribute(
		     ( (xml_plist_parser_state_t *) parser_state )->current_tag,
		     (uint8_t *) $1.data,
		     $1.length,
		     (uint8_t *) $3.data,
		     $3.length,
		     ( (xml_plist_parser_state_t *) parser_state )->error ) != 1 )
		{
			libcerror_error_set(
			 ( (xml_plist_parser_state_t *) parser_state )->error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append attribute.",
			 xml_plist_parser_function );

			YYABORT;
		}
	}
	;

%%

int xml_plist_parser_parse_buffer(
     libfvde_xml_plist_t *xml_plist,
     const uint8_t *buffer,
     size_t buffer_size,
     libcerror_error_t **error )
{
	xml_plist_parser_state_t parser_state;
	
	YY_BUFFER_STATE buffer_state = NULL;
	size_t buffer_offset         = 0;
	int result                   = -1;

	buffer_state = xml_plist_scanner__scan_buffer(
	                (char *) &( buffer[ buffer_offset ] ),
	                buffer_size - buffer_offset );

	xml_plist_scanner_buffer_offset = (size_t) buffer_offset;

	if( buffer_state != NULL )
	{
		parser_state.xml_plist   = xml_plist;
		parser_state.error       = error;
		parser_state.root_tag    = NULL;
		parser_state.current_tag = NULL;
		parser_state.parent_tag  = NULL;

		if( xml_plist_scanner_parse(
		     &parser_state ) == 0 )
		{
			xml_plist->root_tag = parser_state.root_tag;

			result = 1;
		}
		xml_plist_scanner__delete_buffer(
		 buffer_state );
	}
	xml_plist_scanner_lex_destroy();

	return( result );
}

