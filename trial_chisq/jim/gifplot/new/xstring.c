#ifndef XSTRING_C_INCLUDED
#define	XSTRING_C_INCLUDED
/*===========================================================================*/
/*			GENERAL STRING HANDLER				*/
/*===========================================================================*/
#include "xinclude.h"
#include "xtime.c"
/*---------------------------------------------------------------------------*/
typedef struct
	{
	int	len_content;
	int	size_buffer;
	char	*buf;
	}
	XSTRING;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
XSTRING	*xstring_new( void )
{
	XSTRING	*snew;
	snew = (XSTRING *) malloc( sizeof(XSTRING) );
	snew->len_content = 0;
	snew->size_buffer = 0;
	snew->buf = NULL;
	return( snew );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
void xstring_delete( XSTRING *xs )
{
	if ( NULL == xs )
		{return;
		}
	xs->len_content = 0;
	xs->size_buffer = 0;
	if ( xs->buf != NULL )
		{free( xs->buf );
		xs->buf = NULL;
		}
	free( xs );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
char *xstring_value( XSTRING *xs )
{	return( ( NULL != xs->buf ) ? xs->buf : "" );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
void xstring_extend( XSTRING *xs, int size_required )
{
	if ( size_required <= xs->size_buffer )
		{return;
		}
	xs->size_buffer += size_required + 100;
	if ( NULL == xs->buf )
		{xs->buf = (char *) malloc( xs->size_buffer + 2 );
		xs->buf[0] = '\0';
		}
	else
		{xs->buf = (char *) realloc( xs->buf, xs->size_buffer + 2 );
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* OPTIMIZED ROUTINE FOR INSERTING SINGLE CHARACTERS */
XSTRING *xstring_add_char( XSTRING *xs, char c )
{
	int	size_total;
	size_total = xs->len_content + 1;
	xstring_extend( xs, size_total );
	xs->buf[ xs->len_content ] = c;
	xs->buf[ ++xs->len_content ] = '\0';
	return( xs );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* GENERAL ROUTINE FOR INSERTING CHAR ARRAYS */
XSTRING *xstring_add_string( XSTRING *xs, char *s )
{
	int	slen, size_total;
	if ( NULL == s || ( slen = strlen( s ) ) < 1 )
		{return( xs );
		}
	size_total = xs->len_content + slen;
	xstring_extend( xs, size_total );
	strcpy( xs->buf + xs->len_content, s );
	xs->len_content = size_total;
	return( xs );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* ADD (AT MOST) N CHARACTERS FROM STRING */
XSTRING *xstring_add_stringn( XSTRING *xs, char *s, int n )
{
	int	slen;
	char	*tmp;
	if ( n < 1 )
		{return( xs );
		}
	slen = strlen( s );
	if ( n >= slen )
		{return( xstring_add_string( xs, s ) );
		}
	tmp = (char *) malloc( slen + 2 );
	strcpy( tmp, s );
	tmp[n] = '\0';
	xstring_add_string( xs, tmp );
	free( tmp );
	return( xs );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
XSTRING *xstring_add_xstring( XSTRING *xs, XSTRING *value )
{
	return( xstring_add_string( xs, xstring_value( value ) ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
XSTRING *xstring_add_int( XSTRING *xs, int value )
{
	char	vbuf[30];
	sprintf( vbuf, "%d", value );
	return( xstring_add_string( xs, vbuf ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
XSTRING *xstring_add_date( XSTRING *xs, XDATE *value )
{
	char	vbuf[30];
	date_to_string( vbuf, value, 0 );
	return( xstring_add_string( xs, vbuf ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
XSTRING *xstring_add_time( XSTRING *xs, XTIME *value )
{
	char	vbuf[30];
	time_to_string( vbuf, value, 0, NULL, 1 );
	return( xstring_add_string( xs, vbuf ) );
}
/*===========================================================================*/
#endif
