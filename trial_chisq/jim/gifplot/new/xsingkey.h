#ifndef	XSINGKEY_H_INCLUDED
#define	XSINGKEY_H_INCLUDED	1
/*===========================================================================*/
/*		HEADER FOR UTILITIES FOR SINGLE-KEY INPUT		*/
/*===========================================================================*/
#include "xinclude.h"
/*---------------------------------------------------------------------------*/
						/* RE-MAP SPECIFIC KEYS */
static int singlekey_remap( int c );
					/* SET UP FOR SINGLE KEY INPUT */
void singlekey_init( void );
					/* TERMINATE SINGLE KEY INPUT */
void singlekey_end( void );
					/* GET SINGLEKEY INPUT */
int singlekey( void );
			/* RETURN ONE OF A LIST OF SINGLE-CHARACTER OPTIONS */
char singlekey_option( char *s );
/*---------------------------------------------------------------------------*/
#ifdef __BORLANDC__
/*---------------------------------------------------------------------------*/
#define SK_CHAR_RETURN          13
#define SK_CHAR_DELETE          8
#define	SK_CHAR_UPCUR 		-72
#define SK_CHAR_DOWNCUR         -80
#define SK_CHAR_LEFTCUR         -75
#define SK_CHAR_RIGHTCUR        -77
#define	SK_CHAR_PAGEUP		-73
#define	SK_CHAR_PAGEDOWN	-81
#define	SK_CHAR_HOME		-71
#define	SK_CHAR_END		-79
#define	SK_CHAR_ESC		27
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*---------------------------------------------------------------------------*/
#elif defined __unix__
/*---------------------------------------------------------------------------*/
#define SK_CHAR_RETURN          13
#define SK_CHAR_DELETE          8
#define	SK_CHAR_UPCUR 		-72
#define SK_CHAR_DOWNCUR         -80
#define SK_CHAR_LEFTCUR         -75
#define SK_CHAR_RIGHTCUR        -77
#define	SK_CHAR_PAGEUP		-73
#define	SK_CHAR_PAGEDOWN	-81
#define	SK_CHAR_HOME		-71
#define	SK_CHAR_END		-79
#define	SK_CHAR_ESC		27
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*---------------------------------------------------------------------------*/
#elif defined __DECC
/*---------------------------------------------------------------------------*/
#include <ssdef.h>				/* SYSTEM SERVICE DEFINITIONS */
#include <smgdef.h>			    	/* NAMES FOR EXOTIC KEYS */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define	SK_CHAR_UPCUR		274
#define	SK_CHAR_DOWNCUR		275
#define	SK_CHAR_LEFTCUR		276
#define	SK_CHAR_RIGHTCUR	277
#define	SK_CHAR_RETURN		13
#define	SK_CHAR_DELETE		127
#define	SK_CHAR_PAGEUP		315
#define	SK_CHAR_PAGEDOWN	316
#define	SK_CHAR_HOME		55
#define	SK_CHAR_END		58
#define	SK_CHAR_ESC		-1	/* REQUIRES TWO HITS TO RETURN VALUE */
/*---------------------------------------------------------------------------*/
#else							/* OTHER SYSTEMS ? */
/*---------------------------------------------------------------------------*/
#define	SK_CHAR_UPCUR		18
#define	SK_CHAR_DOWNCUR		19
#define	SK_CHAR_LEFTCUR		20
#define	SK_CHAR_RIGHTCUR	21
#define	SK_CHAR_RETURN		13
#define	SK_CHAR_DELETE		127
#define	SK_CHAR_PAGEUP		59
#define	SK_CHAR_PAGEDOWN	60
#define	SK_CHAR_HOME		55
#define	SK_CHAR_END		58
#define	SK_CHAR_ESC		-1	/* REQUIRES TWO HITS TO RETURN VALUE */
/*---------------------------------------------------------------------------*/
#endif
/*===========================================================================*/
#endif							/* XSINGKEY.H */
