#ifndef XUSER_C_INCLUDED
#define XUSER_C_INCLUDED 1
/*===========================================================================*/
/*		UTILITIES CONTROLLED BY MAINFRAME USERNAME		*/
/*===========================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#if ! ( defined __BORLANDC__ || defined __unix__ )
#include <unixlib.h>
#endif
/*---------------------------------------------------------------------------*/
static	int	pretend_live = 0;		/* PRETEND TO BE LIVE ? */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define	N_LIVE_USERS	12
				/* BOOLEAN INDICATING LIVE OR TEST STATUS */
int user_is_live( void )
{
#ifdef __BORLANDC__
	return( pretend_live );
#else
	int	i; 
	char	*user;
	char	live_nodes[N_LIVE_USERS][7] =
		{
			"RAND0",
			"RAND1",
			"RAND2",
			"RAND3",
			"RAND4",
			"RAND5",
			"RAND6",
			"RAND7",
			"RAND8",
			"RAND9",
			"RABASE",
			"NIGHT"
		};
	if ( pretend_live )		/* OVER-RIDE OPTION FOR TESTING */
		{return( 1 );
		}
	user = getenv( "USER" );
	for ( i = 0; i < N_LIVE_USERS; i++ )
		{if ( 0 == strcmp( user, live_nodes[i] ) )
			{return( 1 );       
			}
		}
	return( 0 );
#endif
}
#undef N_LIVE_USERS
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* RETURN STRING ACCORDING TO LIVE/TEST STATUS */
void get_user( char **live_or_test )
{	*live_or_test = ( user_is_live() ) ? "LIVE" : "TEST";  
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* IS SUB-STRING `RABASE' IN STRING */
int rabase_in_string( char *s )
{	return( ( locate_substring_nocase( "rabase", s ) < 0 ) ? NO : YES );
}
/*===========================================================================*/
#endif								/* XUSER.C */
