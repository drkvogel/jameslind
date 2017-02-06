#ifndef XCGI_C_INCLUDED
#define XCGI_C_INCLUDED       1
/*===========================================================================*/
/*		UTILITIES FOR CONSTRUCTING CGI SCRIPTS			*/
/*===========================================================================*/
#include "xdefs.h"
#include "xinclude.h"
#include "xalloc.c"
#include "xmessage.c"
#include "xtext.c"
#include "xtime.c"
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define	XCGI_METHOD_GET			'm'
#define	XCGI_METHOD_POST		'p'
#define	XCGI_METHOD_INTERACTIVE		'I'
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define	XCGI_MAX_PARAM_NUM		1000	/* CRASH PREVENTION LIMITS */
#define	XCGI_MAX_PARAM_LEN		3000
#define	XCGI_MAX_PARAM_TOTLEN		50000
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
typedef	struct				/* HOLDS INPUT "&NAME=VALUE" DATA */
	{
	char	*name;
	char	*value;
	}
	XCGI_PARAM;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
static	int	xcgi_nparam = 0;
static	XCGI_PARAM	*xcgi_param = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
static  int    	debug_xcgi = 1;
static	int   	xcgi_method = 0;
static	char	*xcgi_logname = "xcgi.log";
static	FILE	*f_xcgi = NULL;
/*---------------------------------------------------------------------------*/
					/* WRITE LOG OF INTERNAL MESSAGES */
void xcgi_log( char *txt, ... )
{	time_t	tnow;
	va_list	args;				/* VARIABLE ARGUMENT LIST */
	time( &tnow );
	if ( NULL == ( f_xcgi = fopen( xcgi_logname, "w" ) ) )
		{return;			/* CAN DO BUGGER ALL */
		}
	fprintf( f_xcgi, "\n\nLogtime %s\n", ctime( &tnow ) );
	va_start( args, txt );
	vfprintf( f_xcgi, txt, args );
	va_end( args );				/* TIDY ARGUMENT LIST */
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* SET INTERNAL DEBUG VALUE */
void xcgi_set_debug( int dbg )
{	debug_xcgi = dbg;
}
/*---------------------------------------------------------------------------*/
			/* READ FILE AND COPY VERBATIM TO OUTPUT STREAM */
void xcgi_verbatim( char *vfname )
{       int	c;
	FILE	*fi;
	fflush( stdout );
	if ( NULL == ( fi = fopen( vfname, "r" ) ) )
		{xcgi_log( "xcgi_verbatim, unable to find file \"%s\"",
			vfname );
		printf( "\n<p><hr><b>SERVER ERROR</b><br>" );
		printf( "<i>Unable to include document</i> " );
		printf( "\n<tt>%s</tt><hr><p>\n", vfname );
		return;
		}
	while ( EOF != ( c = fgetc( fi ) ) )
		{fputc( c, stdout );	/* READ THEN WRITE EACH CHARACTER */
		}
	fflush( stdout );
	fclose( fi );
}
/*---------------------------------------------------------------------------*/
							/* EXIT NICELY */
void xcgi_exit( void )
{       long	i;
	fflush( stdout );
	if ( NULL != f_xcgi )
		{fclose( f_xcgi );
		}
	if ( NULL != xcgi_param )
		{for ( i = 0; i < xcgi_nparam; i++ )
			{xfree( xcgi_param[i].name );
			xfree( xcgi_param[i].value );
			}
		xfree( xcgi_param );
		}
	exit( EXIT_SUCCESS );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* TERMINATE DODGY REQUEST AND EXIT */
void xcgi_exit_bad( char *err_txt )
{
	xcgi_log( err_txt );
	xcgi_exit();
}
/*---------------------------------------------------------------------------*/
		/* CONVERT 2-DIGIT HEXADECIMAL STRING TO NUMERIC VALUE */
int xcgi_unhex( char *hx )
{       int	i, c;
	int	val = 0;
	for ( i = 0; i < 2; i++ )
		{c = hx[i];
		if ( '0' <= c && c <= '9' )
			{c -= '0';
			}
		else if ( 'a' <= c && c <= 'f' )
			{c -= ( 'a' - 10 );
			}
		else if ( 'A' <= c && c <= 'F' )
			{c -= ( 'A' - 10 );
			}
		else
			{xcgi_log( "xcgi_unhex: bad value \"%s\"", hx );
			return( (int) '?' );
			}
		val += ( i < 1 ) ? ( 16 * c ) : c;
		}
	return( val );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* DECODE URL-ENCODED STRING IN PLACE */
void xcgi_decode_URL( char *txt )
{       int	code;
	int	pos = 0, newpos = 0;
	while ( txt[pos] != EOS )
		{if ( '+' == txt[pos] )		/* CONVERT PLUS TO SPACE */
			{txt[newpos] = ' ';
			}
		else if ( '%' == txt[pos] )	/* UN-ESCAPE HEX CODE */
			{code = xcgi_unhex( txt + pos + 1 );
			if ( code > 8 )
				{txt[newpos] = (char) code;
				}
			else
				{xcgi_log(
			"xcgi_decode_URL: suspicious escape code %d ignored",
					code );
				txt[newpos] = '?';
				}
			pos += 2;
			}
		else				/* COPY NORMAL CHARACTER */
			{txt[newpos] = txt[pos];
			}
		pos++;
		newpos++;
		}
	txt[newpos] = EOS;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* GET NEXT PARAMETER FROM STRING */
long xcgi_next_param( char *in, char terminator, char **out )
{	long	nc = 0;
	if ( NULL == ( *out = (char *) xmalloc( 52 ) ) )
		{xcgi_exit_bad( "memory error on server" );
		}
	while ( terminator != in[nc] && EOS != in[nc] )
		{(*out)[nc] = in[nc];
		if ( 0 == ++nc % 50 )
			{if ( NULL == ( *out = xrealloc( *out, nc + 52 ) ) )
				{xcgi_exit_bad( "server out of memory" );
				}
			if ( nc > XCGI_MAX_PARAM_LEN )
				{xcgi_log( "runway parameter ?   length > %ld",
					nc );
				xcgi_exit_bad(
					"overlong parameter in user script" );
				}
			}
		}
	(*out)[ nc ] = EOS;
	xcgi_decode_URL( *out );
	return( nc );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* CONVERT PARAMETER STRING INTO SEPARATE ITEMS */
void xcgi_split_params( char *param )
{	long	pos = 0;
	if ( debug_xcgi > 0 )
		{xcgi_log( "splitting\n\"%s\"", param );
		}
	if ( xcgi_nparam != 0 )
		{xcgi_log(
		"split_params, multiple call,  repeat call has arg \"%s\"",
			param );
		}
	xcgi_param = (XCGI_PARAM *) xmalloc( 22 * sizeof(XCGI_PARAM) );
	while ( EOS != param[pos] )
		{pos += xcgi_next_param( param + pos, '=',
			&xcgi_param[ xcgi_nparam ].name );
		if ( EOS != param[pos] )
			{pos++;
			}
		pos += xcgi_next_param( param + pos, '&',
			&xcgi_param[ xcgi_nparam ].value );
		if ( EOS != param[pos] )
			{pos++;
			}
		if ( 0 == ++xcgi_nparam % 20 )
			{if ( NULL == ( xcgi_param = (XCGI_PARAM *) xrealloc(
					xcgi_param, ( xcgi_nparam + 22 )
					* sizeof(XCGI_PARAM) ) )
					|| xcgi_nparam > XCGI_MAX_PARAM_NUM )
				{xcgi_log(
				"xcgi_split_params: at least %ld parameters",
					xcgi_nparam );
				xcgi_exit_bad(
				"user script has sent too many parameters" );
				}
			}
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* FIND DATA RECEIPT METHOD */
int xcgi_find_method( void )
{       char	*meth;
	if ( NULL == ( meth = getenv( "REQUEST_METHOD" ) ) )
		{xcgi_log( "xcgi_find_method, REQUEST_METHOD not found" );
		return( 0 );
		}
	if ( 0 == strcmp_nocase( meth, "GET" ) )
		{return( XCGI_METHOD_GET );
		}
	else if ( 0 == strcmp_nocase( meth, "POST" ) )
		{return( XCGI_METHOD_POST );
		}
	xcgi_log( "unrecognized method = \"%s\"", meth );
	return( 0 );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* WRITE INITIAL MIME HEADER TO OUTPUT */
void xcgi_header( char *type )
{       char	*mime_fmt = NULL;
	if ( 0 == strcmp_nocase( type, "html" ) )
		{mime_fmt = "text/html";
		printf( "content-type: text/html\n\n" );
		}
	else if ( 0 == strcmp_nocase( type, "plain" ) )
		{mime_fmt = "text/plain";
		printf( "content-type: text/plain\n\n" );
		}
	else
		{mime_fmt = "text/plain";
		printf( "content-type: text/plain\n\n" );
		xcgi_log( "unrecognized format type \"%s\"", type );
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* INITIALIZE SYSTEM AND GET PARAMETERS */
void xcgi_init( int ac, char **av )
{	int	i, content_length;
	char	*pbuf;
	FILE	*fi;
	setvbuf( stdout, NULL, _IONBF, 0 );		/* DISABLE BUFFERING */
	xcgi_method = xcgi_find_method();		/* METHOD */
	switch( xcgi_method )
		{case 0:
			if ( ac > 1 && 0 == strcmp_nocase( "-qf", av[1] )
					&& NULL != ( fi = fopen(
					"qstring.get", "r" ) ) )
				{pbuf = (char *) xmalloc( 1001 );
				read_line( fi, pbuf, 1000 );
				fclose( fi );
				}
			else if ( ac > 2 && 0 == strcmp( "-qs", av[1] ) )
				{pbuf = duplicate_string( av[2] );
				}
			else
				{printf( "\nUnsupported CGI method"
				"\nInteractive usage (2 types):"
			"\n\"-qf\"        reads line from file \"qstring.get\""
			"\n\"-qs param\"  uses `param' as QUERY_STRING" );
				xcgi_exit_bad(
		"Client not using a supported CGI method to request data." );
				exit( EXIT_SUCCESS );
				}
			xcgi_split_params( pbuf );
			xfree( pbuf );
			xcgi_method = XCGI_METHOD_INTERACTIVE;
			break;
		case XCGI_METHOD_GET:	 		/* METHOD="GET" */
			xcgi_split_params( getenv( "QUERY_STRING" ) );
			break;
		case XCGI_METHOD_POST:	 		/* METHOD="POST" */
			content_length = atoi( getenv( "CONTENT_LENGTH" ) );
			if ( content_length < 0 || content_length >
					XCGI_MAX_PARAM_TOTLEN
					|| ( NULL == ( pbuf = (char *)
					xmalloc( content_length + 2 ) ) ) )
				{xcgi_log(
				"total of %ld bytes recieved from user script",
					content_length );
				xcgi_exit_bad(
				"too much input transmitted by user script" );
				}
			for ( i = 0; i < content_length; i++ )
				{pbuf[i] = fgetc( stdin );
				}
			pbuf[ content_length ] = EOS;
			xcgi_split_params( pbuf );
			xfree( pbuf );
			break;
		default:
			xcgi_log( "internal error, xcgi_method = %ld",
				xcgi_method );
			xcgi_exit_bad( "server error" );
			break;
		}
	fflush( stdout );
}
/*---------------------------------------------------------------------------*/
					/* RETURN NUMBER OF PARAMETERS */
int xcgi_param_number( void )
{	return( xcgi_nparam );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* RETURN NAME OF INDEXED PARAMETER */
char *xcgi_param_name_number( int num )
{	static	char	empty[3] = "";
	if ( num >= 0 && num < xcgi_nparam )
		{return( xcgi_param[num].name );
		}
	return( empty );				/* NOT FOUND */
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* RETURN VALUE OF INDEXED PARAMETER */
char *xcgi_param_value_number( int num )
{	static	char	empty[3] = "";
	if ( num >= 0 && num < xcgi_nparam )
		{return( xcgi_param[num].value );
		}
	return( empty );				/* NOT FOUND */
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* DOES PARAMETER EXIST */
int xcgi_param_exists( char *nam )
{	int	i;
	i = xcgi_nparam;
	while ( --i >= 0 )
		{if ( 0 == strcmp( nam, xcgi_param[i].name ) )
			{return( 1 );
			}
		}
	return( 0 );					/* NOT FOUND */
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* RETURN VALUE OF NAMED PARAMETER */
char *xcgi_param_value_named( char *nam )
{	int	i;
	static	char	empty[3] = "";
	for ( i = 0; i < xcgi_nparam; i++ )
		{if ( 0 == strcmp( nam, xcgi_param[i].name ) )
			{return( xcgi_param[i].value );
			}
		}
	return( empty );				/* NOT FOUND */
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* SHOW ALL PARAMETERS AS COMMENTS IN OUTPUT */
void xcgi_param_showall( void )
{	int	i, np;
	np = xcgi_param_number();
	printf( "\n\n<!-- XCGI found %d parameters -->\n", np );
	for ( i = 0; i < np; i++ )
		{printf( "\n<!--  \"%s\"=\"%s\"  -->",
			xcgi_param_name_number( i ),
			xcgi_param_value_number( i ) );
		}
	printf( "\n" );
}
/*===========================================================================*/
#endif                                                  	/* XCGI.C */
