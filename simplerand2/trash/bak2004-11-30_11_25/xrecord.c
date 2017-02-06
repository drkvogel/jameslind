#ifndef	XRECORD_C_INCLUDED
#define XRECORD_C_INCLUDED	1
/*===========================================================================*/
/*		COPY TRIAL RECORD TO DAILY-SUMMARY AND PRINT FILES	*/
/*===========================================================================*/
#include "xinclude.h"
#include "xmessage.c"
/*---------------------------------------------------------------------------*/
static	char	xrecord_printer_filename[200] = "";
static	FILE	*fr_lpr = NULL;		/* PRINT FILE		*/
static	FILE	*fr_sum = NULL;		/* DAILY SUMMARY LOG	*/
/*---------------------------------------------------------------------------*/
						/* PRINT TO FILES */
void xrecord_printf( char *txt, ... )
{       va_list	args;
	va_start( args, txt );
	if ( NULL != fr_lpr )
		{vfprintf( fr_lpr, txt, args );
		}
	if ( NULL != fr_sum )
		{vfprintf( fr_sum, txt, args );
		}
	va_end( args );                         /* TIDY ARGUMENT LIST */
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
							/* CLOSE FILES */
void xrecord_close( char *dispose )
{	long	sys_status;
	char	*dispose_command;
	time_t	today;
	time( &today );
	if ( NULL != fr_lpr )
		{fprintf( fr_lpr, "\nprint file closed at %s", ctime(&today) );
		fclose( fr_lpr );
		if ( NULL != dispose )
			{dispose_command = (char *) xmalloc( strlen( dispose )
				+ strlen( xrecord_printer_filename ) + 50 );
			sprintf( dispose_command, "%s %s", dispose,
				xrecord_printer_filename );
			aside( "system call \"%s\"", dispose_command );
			sys_status = system( dispose_command );
			aside( "status returned = %ld", sys_status );
			xfree( dispose_command );
			}
		fr_lpr = NULL;
		}
	if ( NULL != fr_sum )
		{fprintf( fr_sum, "\nFile closed at %s", ctime( &today ) );
		fclose( fr_sum );
		fr_sum = NULL;
		}
	xrecord_printer_filename[0] = EOS;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* RETURN NAME OF CREATED FILE TO CALLING PROGRAM */
void xrecord_getname( char *file_name )
{	strcpy( file_name, xrecord_printer_filename );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
							/* OPEN FILES */
void xrecord_open( char *name )
{	char	tim[30], sum_name[200];
	time_t	today;
	time( &today );
	strftime( tim,  20, "%d%b%y_%H%M", localtime( &today ) );
	sprintf( xrecord_printer_filename, "%s_%s.PF", name, tim );
	if ( NULL != ( fr_lpr = fopen( xrecord_printer_filename, "w" ) ) )
		{aside( "printer file \"%s\" opened", xrecord_printer_filename);
		fprintf( fr_lpr, "\nFile %s opened at %s",
			xrecord_printer_filename, ctime( &today ) );
		}
	else
		{aside( "unable to open printer file \"%s\"",
			xrecord_printer_filename );
		}
	strftime( sum_name, 20, "PRF%d%b.LIS", localtime( &today ) );
	if ( NULL != ( fr_sum = fopen( sum_name, "a" ) ) )
		{aside( "summary file \"%s\" opened", sum_name );
		fprintf( fr_sum, "\nFile %s opened at %s", sum_name,
			ctime( &today ) );
		}
	else
		{aside( "unable to open summary file \"%s\"", sum_name );
		}
}
/*===========================================================================*/
#endif							/* XRECORD.C */
