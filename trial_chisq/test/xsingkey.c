#ifndef	XSINGKEY_C_INCLUDED
#define	XSINGKEY_C_INCLUDED	1
/*===========================================================================*/
/*			UTILITIES FOR SINGLE-KEY INPUT			*/
/*===========================================================================*/
#include "xinclude.h"
#include "xsingkey.h"
/*---------------------------------------------------------------------------*/
typedef	struct		/* KEYBOARD-SPECIFIC TRANSLATIONS FOR SOME KEYS */
	{
	int	from;
	int	to;
	}
	SINGLEKEY_MAPPING;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
static	SINGLEKEY_MAPPING	*singlekey_current_map = NULL;
/*---------------------------------------------------------------------------*/
						/* RE-MAP SPECIFIC KEYS */
static int singlekey_remap( int c )
{	SINGLEKEY_MAPPING	*skm;
	if ( NULL != ( skm = singlekey_current_map ) )
		{while ( 0 != skm->from )
			{if ( c == skm->from )
				{return( skm->to );	/* TRANSLATE KEY */
				}
			skm++;
			}
		}
	return( c );				/* ORIGINAL UN-MAPPED KEY */
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* DEFINE A NEW DEFAULT MAPPING */
void singlekey_set_map( SINGLEKEY_MAPPING *new_map )
{	int	i, fr, to;
	singlekey_current_map = new_map;
	if ( NULL == new_map )
		{return;				/* SWITCH OFF MAPPING */
		}
	for ( i = 0; i < 256; i++ )
		{fr = singlekey_current_map[i].from;
		to = singlekey_current_map[i].to;
		if ( fr < -127 || fr > 512 || to < -127 || to > 127
				|| ( 0 == fr && 0 != to ) )
			{bug( "singlekey_set_map, from = %d  ->  to = %d",
				fr, to );	/* INVALID CHARACTER RANGE */
			}
		if ( 0 == fr )
			{return;		/* VERIFY LAST ENTRY IS 0,0 */
			}
		}
	bug( "singlekey_set_map, apparently unterminated argument" );
}
/*---------------------------------------------------------------------------*/
#ifdef __BORLANDC__
/*---------------------------------------------------------------------------*/
					/* SET UP FOR SINGLE KEY INPUT */
void singlekey_init( void )
{	static	SINGLEKEY_MAPPING	sm[] =
		{
			{	126,	'?'	},
			{	0,	0	}
		};
	singlekey_set_map( sm );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* TERMINATE SINGLE KEY INPUT */
void singlekey_end( void )
{	singlekey_set_map( NULL );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

int singlekey( void )
{	int	c;
	c = getch();
	if ( 0 == c )
		{c = - getch();
		}
	return( singlekey_remap( c ) );
}
/*---------------------------------------------------------------------------*/
#elif defined __unix__
/*---------------------------------------------------------------------------*/
					/* SET UP FOR SINGLE KEY INPUT */
void singlekey_init( void )
{	static	SINGLEKEY_MAPPING	sm[] =
		{
			{	126,	'?'	},
			{	0,	0	}
		};
	singlekey_set_map( sm );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* TERMINATE SINGLE KEY INPUT */
void singlekey_end( void )
{	singlekey_set_map( NULL );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

int singlekey( void )
{	int	c;
	c = getch();
	if ( 0 == c )
		{c = - getch();
		}
	return( singlekey_remap( c ) );
}
/*---------------------------------------------------------------------------*/
#elif defined __DECC
/*---------------------------------------------------------------------------*/
#include <ssdef.h>				/* SYSTEM SERVICE DEFINITIONS */
#include <smgdef.h>			    	/* NAMES FOR EXOTIC KEYS */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
static	long	singlekey_ret_status;		/* ?? LOCALIZE ?? */
static	long	singlekey_keyboard;			/* GLOBAL VARIABLES */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
long	SMG$CREATE_VIRTUAL_KEYBOARD( long * );
long	SMG$READ_KEYSTROKE( long *, short *, void *, long * );
long	SMG$DELETE_VIRTUAL_KEYBOARD( long * );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* SET UP FOR SINGLE KEY INPUT */
void singlekey_init( void )
{	long	ret_status;
	static	SINGLEKEY_MAPPING	sm[] =
		{
			{	SMG$K_TRM_ENTER,	SK_CHAR_RETURN	},
			{	SMG$K_TRM_KP0,		'0'     },
			{	SMG$K_TRM_KP1,		'1'     },
			{	SMG$K_TRM_KP2,		'2'     },
			{	SMG$K_TRM_KP3,		'3'     },
			{	SMG$K_TRM_KP4,		'4'     },
			{	SMG$K_TRM_KP5,		'5'     },
			{	SMG$K_TRM_KP6,		'6'     },
			{	SMG$K_TRM_KP7,		'7'     },
			{	SMG$K_TRM_KP8,		'8'     },
			{	SMG$K_TRM_KP9,		'9'     },
			{	SMG$K_TRM_MINUS,	'-'	},
			{	SMG$K_TRM_COMMA,	','	},
			{	SMG$K_TRM_PERIOD,	'.'	},
			{	0,			0	}
		};
	singlekey_set_map( sm );
	ret_status = SMG$CREATE_VIRTUAL_KEYBOARD( &singlekey_keyboard );
/*	return( (int) ret_status );	*/
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* GET UN-TERMINATED SINGLE KEYSTROKE */
int singlekey( void )
{	long	ret_status;
	long	timeout = 15000;
	short	key;
	ret_status = SMG$READ_KEYSTROKE( &singlekey_keyboard, &key, 0,
		&timeout );

	/* kc = key & 0x7f;  */

	return( singlekey_remap( (int) key ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* TERMINATE SINGLE KEY INPUT */
void singlekey_end( void )
{       long	ret_status;
	singlekey_set_map( NULL );
	ret_status = SMG$DELETE_VIRTUAL_KEYBOARD( &singlekey_keyboard );
/*	return( (int) ret_status );	*/
}
/*---------------------------------------------------------------------------*/
#else							/* OTHER SYSTEMS ? */
/*---------------------------------------------------------------------------*/
					/* SET UP FOR SINGLE KEY INPUT */
void singlekey_init( void )
{	singlekey_set_map( NULL );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* GET UN-TERMINATED SINGLE KEYSTROKE */
int singlekey( void )
{	int	c;
	c = fgetc( stdin );
	if ( 0 == c )
		{c = - fgetc( stdin );
		}
	return( singlekey_remap( c ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* TERMINATE SINGLE KEY INPUT */
void singlekey_end( void )
{	singlekey_set_map( NULL );
}
/*---------------------------------------------------------------------------*/
#endif
/*---------------------------------------------------------------------------*/
			/* RETURN ONE OF A LIST OF SINGLE-CHARACTER OPTIONS */
char singlekey_option( char *s )
{	int	ic;
	char	c;
	char	*opt;
	while ( NULL != s && EOS != s[0] )
		{ic = singlekey();
		c = tolower( ic );
		opt = s;
		while ( *opt != EOS )
			{if ( c == tolower( *opt ) )
				{return( c );
				}
			opt++;
			}
		}
	return( EOS );				/* BAD-INPUT CLAUSE */
}
/*===========================================================================*/
#endif							/* XSINGKEY.C */
