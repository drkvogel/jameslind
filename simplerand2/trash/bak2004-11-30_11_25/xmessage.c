#ifndef XMESSAGE_C_INCLUDED
#define XMESSAGE_C_INCLUDED	1
/*===========================================================================*/
/*			MESSAGE HANDLING UTILITIES			*/
/*===========================================================================*/
#include "xinclude.h"
#include "xalloc.h"
void filext( char *, char * );
/*---------------------------------------------------------------------------*/
static	int	msg_write_to_screen = 1;		/* WRITE TO SCREEN  */
static	char	msg_log_file_name[102] = "message.log";	/* DEFAULT LOG-FILE */
static	char	msg_log_file_name2[102] = "";		/* EXTRA LOG-FILE   */
static	char	msg_email_address[102] = "";		/* DON'T SEND EMAIL */
static	char	msg_email_subject[102] = "";		/* DEFAULT SUBJECT  */
static	FILE	*msg_flogf = NULL;
static	FILE	*msg_flogf2 = NULL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
static	char	xmessage_sbuf[2000];
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
static	int	msg_verbose = 1;
static	int	msg_death_or_glory = 0;
static	int	msg_max_warns = -1;
static	int	msg_num_warns = 0;
static	int	msg_max_errors = -1;
static	int	msg_num_errors = 0;
static	int	msg_debug = 0;
/*---------------------------------------------------------------------------*/
						/* CHANGE DEBUG-INFO LEVEL */
void xmessage_set_debug( int dbg )
{	msg_debug = dbg;
}
/*---------------------------------------------------------------------------*/
							/* SEND EMAIL */
int xemail( char *target, char *title, char *body )
{	char	*command;
	char	tmp_file_name[ L_tmpnam + 20 ];
	FILE	*ft;
	command = (char *) xmalloc( 50 + strlen( target ) + strlen( title ) );
	strcpy( tmp_file_name, "xemail_XXXXXX" );
	mkstemp( tmp_file_name );
	filext( tmp_file_name, "tmp" );
	if ( NULL == ( ft = fopen( tmp_file_name, "w" ) ) )
		{return( 1 );
		}
	fprintf( ft, body );
	fclose( ft );
	sprintf( command, "mail/subj=\"%s\" %s %s", title, tmp_file_name,
		target );
	aside( "Xemail: %s", command );
	system( command );
	if ( msg_debug > 0 )
		{aside( "Xmessage body:"
			"\n- - - - - - - - - - - - - - - - - - - - - - - - -"
			"\n%s"
			"\n- - - - - - - - - - - - - - - - - - - - - - - - -",
			body );
		}
	xfree( command );
	return( 0 );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* SEND MESSAGE BY EMAIL */
int xmessage_send_email( char *text )
{	if ( EOS == msg_email_address[0] )		/* EMAIL SWITCHED OFF */
		{return( 2 );
		}
	return( xemail( msg_email_address, ( EOS != msg_email_subject )
		? msg_email_subject : "<automatic xmessage>", text ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
							/* EMAIL A FILE */
int xmessage_file_email( char *fname )
{	char	*command;
	FILE	*f;
	if ( EOS == msg_email_address[0] )		/* EMAIL SWITCHED OFF */
		{return( 2 );
		}
	command = (char *) xmalloc( 80 + strlen( msg_email_address )
		+ strlen( msg_email_subject ) + strlen( fname ) );
	if ( NULL == ( f = fopen( fname, "r" ) ) )
		{sprintf( command,
		"ERROR, unable to open file \"%s\" for emailing to \"%s\"",
			fname, msg_email_address );
		aside( command );
		return( xmessage_send_email( command ) );
		}
	fclose( f );
	sprintf( command, "mail/subj=\"%s\" %s %s",
		( EOS != msg_email_subject )
		? msg_email_subject : "<automatic xmessage file>",
		fname, msg_email_address );
	aside( "Xemail (file): %s", command );
	system( command );
	xfree( command );
	return( 0 );
}
/*---------------------------------------------------------------------------*/
void failure( char *txt, ...  );
void bug( char *txt, ...  );
void msg_byebye( int bstatus, char *errmess, ... );
/*---------------------------------------------------------------------------*/
				/* GET SINGLE-KEY OPTIONS FROM USER */
char ask_user( char *option )
{	int	i, j, m, n;
	int	ntries = 0;
	char	ans[102];
	char	low_option[102];
	if ( ( n = (int) strlen( option ) ) < 2 || n > 100 )
		{bug( "ask_user, option" );
		}
	for ( i = 0; i < n; i++ )
		{low_option[i] = tolower( option[i] );
		}
	while( ++ntries < 10 )
		{fgets( ans, 100, stdin );
		m = (int) strlen( ans );
		for ( i = 0; i < m; i++ )
			{ans[i] = tolower( ans[i] );
			for ( j = 0; j < n; j++ )
				{if ( ans[i] == low_option[j] )
					{return( low_option[j] );
					}
				}
			}
		printf( "\n\nReply with one of: %c", low_option[0] );
		for ( i = 1; i < n; i++ )
			{printf( ", %c", low_option[i] );
			}
		printf( " ?  " );
		}
	failure( "too many invalid answers" );
	return( 0 );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* ASK USER WHETHER OR NOT TO CONTINUE */
void ask_continue( void )
{	printf("\n\n\tContinue or Quit ? \t[c/q]\t");
	if ( 'q' == ask_user( "cq" ) )
		{msg_byebye( 0, "early exit at user request" );
		}
}
/*---------------------------------------------------------------------------*/

void msg_byebye( int bstatus, char *errmess, ... )
{	char	etext[200];
	va_list	args;				/* VARIABLE ARGUMENT LIST */
	va_start( args, errmess );
	vsprintf( etext, errmess, args );
	va_end( args );				/* TIDY ARGUMENT LIST */
	msg_death_or_glory = 3;
	if ( NULL != msg_flogf )
		{fprintf( msg_flogf, "\n\n* END OF LOGFILE *\n" );
		fclose( msg_flogf );
		printf( "\nLogfile \"%s\" closed", msg_log_file_name );
		}
	printf( "\n" );
	exit(0);
	if ( bstatus == 99 )
		{printf( "B" );
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

void msg_blooper( char *bstatus, char *mess )
{       if ( msg_verbose > 0 )
		{printf( "\n%s%s", bstatus, mess);
		}
	if ( NULL != msg_flogf )
		{fprintf( msg_flogf, "\n%s%s", bstatus, mess );
		fflush( msg_flogf );
		}
	if ( NULL != msg_flogf2 )
		{fprintf( msg_flogf2, "\n%s%s", bstatus, mess );
		fflush( msg_flogf2 );
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* WRITE ONLY TO LOGFILE */
								/*VARARGS*/
void aside( char *txt, ...  )
{	va_list	args;				/* VARIABLE ARGUMENT LIST */
	if ( NULL == msg_flogf && NULL == msg_flogf2 )
		{return;
		}
	va_start( args, txt );
	if ( NULL != msg_flogf )
		{fprintf( msg_flogf, "\n" );
		vfprintf( msg_flogf, txt, args );
		fflush( msg_flogf );
		}
	if ( NULL != msg_flogf2 )
		{fprintf( msg_flogf2, "\n" );
		vfprintf( msg_flogf2, txt, args );
		fflush( msg_flogf2 );
		}
	va_end( args );				/* TIDY ARGUMENT LIST */
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* GIVE INFORMATION MESSAGE */
								/*VARARGS*/
void inform( char *txt, ...  )
{	va_list	args;				/* VARIABLE ARGUMENT LIST */
	va_start( args, txt );
	vsprintf( xmessage_sbuf, txt, args );
	va_end( args );				/* TIDY ARGUMENT LIST */
	msg_blooper( " ", xmessage_sbuf );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* GIVE WARNING MESSAGE */
								/*VARARGS*/
void warning( char *txt, ...  )
{	va_list	args;				/* VARIABLE ARGUMENT LIST */
	va_start( args, txt );
	vsprintf( xmessage_sbuf, txt, args );
	va_end( args );				/* TIDY ARGUMENT LIST */
	msg_blooper( "WARNING ", xmessage_sbuf );
	if ( msg_max_warns >= 0 && ++msg_num_warns >= msg_max_warns )
		{if ( msg_death_or_glory < 1 )
			{ask_continue();	/* ASK WHETHER TO CONTINUE */
			}
		msg_max_warns *= 2;				/* RESET LIMITS	*/
		printf( "\nmax_warns reset to %ld", msg_max_warns );
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
							/* GIVE ERROR MESSAGE */
								/*VARARGS*/
void error( char *txt, ...  )
{	va_list	args;				/* VARIABLE ARGUMENT LIST */
	va_start( args, txt );
	vsprintf( xmessage_sbuf, txt, args );
	va_end( args );				/* TIDY ARGUMENT LIST */
	msg_blooper( "ERROR   ", xmessage_sbuf );
	if ( msg_max_errors >= 0 && ++msg_num_errors >= msg_max_errors )
		{if ( msg_death_or_glory < 2 )
			{ask_continue();	/* ASK WHETHER TO CONTINUE */
			}
		msg_max_errors *= 2;			/* RESET LIMITS */
		printf( "\nmax_errors reset to %ld", msg_max_errors );
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* GIVE TERMINAL FAILURE MESSAGE */
								/*VARARGS*/
void failure( char *txt, ...  )
{	va_list	args;
	va_start( args, txt );
	vsprintf( xmessage_sbuf, txt, args );
	va_end( args );
	msg_blooper( "FAILURE ", xmessage_sbuf );
	if ( msg_death_or_glory < 3 )
		{msg_byebye( 0, " " );			/* TERMINATE PROCESS */
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* GIVE TERMINAL FAILURE MESSAGE */
								/*VARARGS*/
void bug( char *txt, ...  )
{	va_list	args;
	va_start( args, txt );
	vsprintf( xmessage_sbuf, txt, args );
	va_end( args );
	msg_blooper( "BUG ", xmessage_sbuf );
	msg_byebye( 0, " " );			/* TERMINATE PROCESS */
}
/*---------------------------------------------------------------------------*/
					/* SET MESSAGE SCREEN-WRITE STATUS */
void xmessage_control_screen( int to_screen )
{	msg_write_to_screen = ( 0 != to_screen );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* SAY IF LOGFILE OPEN */
int xmessage_logfile_active( void )
{	return( ( NULL != msg_flogf ) ? YES : NO );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* SAY IF LOGFILE OPEN */
int xmessage_logfile2_active( void )
{	return( ( NULL != msg_flogf2 ) ? YES : NO );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
							/* CLOSE LOGFILE */
void xmessage_close_logfile( void )
{	if ( NULL == msg_flogf )
		{return;
		}
	fclose( msg_flogf );
	msg_flogf = NULL;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
							/* CLOSE LOGFILE */
void xmessage_close_logfile2( void )
{	if ( NULL == msg_flogf2 )
		{return;
		}
	fclose( msg_flogf2 );
	msg_flogf2 = NULL;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* SET ADDRESS FOR MESSAGE LOGGING */
int xmessage_control_xlogfile( FILE **xf, char *mlogfname, char *xlog_file,
		char *xmode )
{	if ( NULL == xlog_file )
		{return( NO );
		}
	if ( 0 != strcmp( xmode, "w" ) && 0 != strcmp( xmode, "a" ) )
		{error( "invalid mode \"%s\" for logfile \"%s\" - not opened",
			xlog_file );
		return( NO );
		}
	if ( 0 == strcmp( xlog_file, mlogfname ) && NULL != *xf )
		{return( YES );			/* FILE ALREADY OPEN */
		}
	if ( NULL != *xf )			/* CLOSE ANY PREVIOUS FILE */
		{fclose( *xf );
		}
	strncpy( mlogfname, xlog_file, 100 );
	return( ( NULL != ( *xf = fopen( mlogfname, xmode ) ) )
		? YES : NO );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* SET ADDRESS FOR MESSAGE LOGGING */
int xmessage_control_logfile( char *log_file, char *mode )
{
	return( xmessage_control_xlogfile( &msg_flogf, msg_log_file_name,
		log_file, mode ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* SET ADDRESS FOR MESSAGE LOGGING */
int xmessage_control_logfile2( char *log_file, char *mode )
{
	return( xmessage_control_xlogfile( &msg_flogf2, msg_log_file_name2,
		log_file, mode ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* SET ADDRESS FOR MESSAGE EMAIL */
void xmessage_control_email( char *email_address )
{	if ( NULL == email_address )
		{msg_email_address[0] = EOS;
		}
	else
		{strncpy( msg_email_address, email_address, 100 );
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* SET ADDRESS FOR MESSAGE EMAIL */
void xmessage_control_subject( char *subject )
{	if ( NULL == subject )
		{msg_email_subject[0] = EOS;
		}
	else
		{strncpy( msg_email_subject, subject, 100 );
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* SET DEFAULTS FOR MESSAGE HANDLING */
void xmessage_control( int to_screen, char *log_file, char *email_address,
		char *subject )
{	xmessage_control_screen( to_screen );
	(void) xmessage_control_logfile( log_file, "w" );
	xmessage_control_email( email_address );
	xmessage_control_subject( subject );
}
/*===========================================================================*/
#endif								/* XMESSAGE.C */
