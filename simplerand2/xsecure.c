#ifndef XSECURE_C_INCLUDED
#define XSECURE_C_INCLUDED	1
/*===========================================================================*/
/*			SECURITY UTILITIES				*/
/*===========================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#include "xalloc.h"
/*---------------------------------------------------------------------------*/
	/* VERHOEFF CHECK-DIGIT, BASED ON NUM.REC.IN C, 2ND ED., P 902 */
static int verhoeff_decchk( char *string, char *ch )
{	char	c;
	int	j, n;
	int	k = 0, m = 0;
	static	int	ip[10][8] =
		{
			{ 0,1,5,8,9,4,2,7 },
			{ 1,5,8,9,4,2,7,0 },
			{ 2,7,0,1,5,8,9,4 },
			{ 3,6,3,6,3,6,3,6 },
			{ 4,2,7,0,1,5,8,9 },
			{ 5,8,9,4,2,7,0,1 },
			{ 6,3,6,3,6,3,6,3 },
			{ 7,0,1,5,8,9,4,2 },
			{ 8,9,4,2,7,0,1,5 },
			{ 9,4,2,7,0,1,5,8 }
		};
	static	int	ij[10][10] =
		{
			{ 0,1,2,3,4,5,6,7,8,9 },
			{ 1,2,3,4,0,6,7,8,9,5 },
			{ 2,3,4,0,1,7,8,9,5,6 },
			{ 3,4,0,1,2,8,9,5,6,7 },
			{ 4,0,1,2,3,9,5,6,7,8 },
			{ 5,9,8,7,6,0,4,3,2,1 },
			{ 6,5,9,8,7,1,0,4,3,2 },
			{ 7,6,5,9,8,2,1,0,4,3 },
			{ 8,7,6,5,9,3,2,1,0,4 },
			{ 9,8,7,6,5,4,3,2,1,0 }
		};
	n = strlen( string );
	for ( j = 0; j < n; j++ )
		{c = string[j];
		if ( isdigit( c ) )
			{k = ij[ k ][ ip[ ( c + 2 ) % 10 ][ 7 & m++ ] ];
			}
		}
	for ( j = 0; j < 10; j++ )
		{if ( ! ij[ k ][ ip[ j ][ m & 7 ] ] )
			{break;
			}
		}
	*ch = (char) j + '0';
	return( k == 0 );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* SET CHECK-DIGIT FOR CHECK DECIMALS IN STRING */
char verhoeff_check_string_set( char *snum )
{       char	chk;
	verhoeff_decchk( snum, &chk );
	return( chk );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* SET CHECK-DIGIT FOR CHECK DECIMALS IN STRING */
int verhoeff_check_number_set( int number )
{       char	chk;
	char	snum[50];
	sprintf( snum, "%d", number );
	verhoeff_decchk( snum, &chk );
	return( (int) chk - (int) '0' );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* CHECK DECIMALS IN STRING HAVE OKAY CHECK-DIGIT */
int verhoeff_check_string_okay( char *snum )
{       char	chk;
	return( verhoeff_decchk( snum, &chk ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* CHECK DECIMALS IN STRING HAVE OKAY CHECK-DIGIT */
int verhoeff_check_number_okay( int number )
{       char	chk;
	char	snum[50];
	sprintf( snum, "%d", number );
	return( verhoeff_decchk( snum, &chk ) );
}
/*===========================================================================*/
#endif								/* XSECURE.C */
