#ifndef XB64_C_DEFINED
#define XB64_C_DEFINED	1
/*===========================================================================*/
/*			BASE-64 ENCODE AND DECODE			*/
/*===========================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xb64.h"
/*---------------------------------------------------------------------------*/
static int	xb64_chunking = 0; /* SET TO 1 FOR FULL RFC1421 COMPLIANCE */
static const char xb64_trans[65] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
/*---------------------------------------------------------------------------*/
		/* DETERMINE IF CHUNKING SHOULD BE APPLIED DURING ENCODE */
void xb64_chunking_set( int do_chunking )
{	xb64_chunking = ( do_chunking != 0 ) ? 1 : 0;
}
/*---------------------------------------------------------------------------*/
#define	XB64_MASK 	0x3F
#define	XB64_ADD(x)	(*b64)[(*len_b64)++] = x
#define	XB64_ENC(x)	XB64_ADD( xb64_trans[x] )
						/* ENCODE ARRAY AS B64 TEXT */
int xb64_encode( unsigned char *dat, int len_dat, unsigned char **b64,
	int *len_b64 )
{
	int	byte = 0, chunk = 0;
	unsigned long	quantum = 0;
	*len_b64 = 0;
	*b64 = NULL;
	if ( len_dat < 0 || NULL == dat )
		{return( 0 );
		}
	*b64 = (unsigned char *) malloc(
		( ( len_dat * 4 ) / 3 )
		+ ( xb64_chunking ? ( len_dat / 50 ) : 0 )
		+ 10
		);
	while ( len_dat-- > 0 )
		{
		quantum = ( quantum << 8 ) | (unsigned char) *dat++;
		if ( 3 == ++byte )
			{
			XB64_ENC( quantum >> 18 );
			XB64_ENC( (quantum >> 12) & XB64_MASK );
			XB64_ENC( (quantum >> 6) & XB64_MASK );
			XB64_ENC( quantum & XB64_MASK );
			quantum = 0;
			byte = 0;
			if ( 1 == xb64_chunking && ++chunk >= 19 )
				{/* RFC1421 SPLITS OUTPUT INTO LINES OF 76 */
				XB64_ADD( '\n' );
				chunk = 0;
				}
			}
		}
	if ( 1 == byte ) 	/* TAKE CARE OF EXTRA BITS */
		{
		XB64_ENC( quantum >> 6 );
		XB64_ENC( quantum & XB64_MASK );
		XB64_ADD( '=' );
		XB64_ADD( '=' );
		}
	else if ( 2 == byte )
		{
		XB64_ENC( quantum >> 12 );
		XB64_ENC( (quantum >> 6) & XB64_MASK );
		XB64_ENC( quantum & XB64_MASK );
		XB64_ADD( '=' );
		}
	(*b64)[*len_b64] = '\0';
	return( 1 );
}
#undef	XB64_MASK
#undef	XB64_ADD
#undef	XB64_ENC
/*---------------------------------------------------------------------------*/

int xb64_encode_txt( char *txt, unsigned char **b64, int *len_b64 )
{
	int	len_dat;
	unsigned char *dat;
	len_dat = strlen( txt );
	dat = (unsigned char *) txt;
	return( xb64_encode( dat, len_dat, b64, len_b64 ) );
}
/*---------------------------------------------------------------------------*/
#define	XB64_OUT(x)	(*dat)[(*len_dat)++] = x
						/* DECODE B64 INTO ARRAY */
int xb64_decode( unsigned char *b64, int len_b64, unsigned char **dat,
	int *len_dat )
{
	int	status = 1;
	int	byte = 0;
	unsigned char 	input;
	unsigned long	quantum = 0;
	*len_dat = 0;
	*dat = NULL;
	if ( len_b64 < 0 || NULL == b64 )
		{return( 0 );
		}
	*dat = (unsigned char *) malloc( ( ( 3 * len_b64 ) / 4 ) + 5 );
	while ( len_b64-- > 0 )
		{
		input = *b64++;
		if ( '\n' == input )
			{continue;	/* IGNORE END-OF-LINE SEPARATORS */
			}
		if( input == '=' )
			{		/* END OF DATA MARKER REACHED */
			if ( 1 == byte || 2 == byte )
				{
				XB64_OUT( quantum );
				}
			else if ( 3 == byte )
				{
				XB64_OUT( quantum >> 8 );
				XB64_OUT( quantum & 0xff );
				}
			quantum = 0;
			byte = 0;
			}
		else
			{
			if( input >= 'A' && input <= 'Z' )
				{input += (- 'A');
				}
			else if( input >= 'a' && input <= 'z' )
				{input += ( 26 - 'a' );
				}
			else if( input >= '0' && input <= '9' )
				{input += ( 52 - '0' );
				}
			else if( input == '+' )
				{input = 62;
				}
			else if( input == '/' )
				{input = 63;
				}
			else			/* NOTE FAILURE AND RETURN */
				{return( 0 );
				}
			quantum = ( quantum << 6 ) | input;
			if( 4 == ++byte )
				{
				XB64_OUT( quantum >> 16 );
				XB64_OUT( (quantum >> 8) & 0xff );
				XB64_OUT( quantum & 0xff );
				quantum = 0;
				byte = 0;
				}
			}
		}
	return( status );
}
#undef	XB64_OUT(x)
/*---------------------------------------------------------------------------*/
int xb64_decode_txt( unsigned char *b64, int len_b64, char **txt, int *len_txt )
{
	int	status;
	unsigned char	*dat;
	status = xb64_decode( b64, len_b64, &dat, len_txt );
	*txt = (char *) dat;
	return( status );
}
/*===========================================================================*/
#endif
