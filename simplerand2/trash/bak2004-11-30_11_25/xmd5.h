#ifndef MD5_INCLUDED_H
#define MD5_INCLUDED_H
/*===========================================================================*/
#define	MD5_KEYLENGTH	33	/* (2*16)+1	*/
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
typedef unsigned char md5_byte_t; /* 8-bit byte */
typedef unsigned int  md5_word_t; /* 32-bit word */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
typedef	struct
	{
	md5_byte_t 	digest[16];
	char		hex[MD5_KEYLENGTH];
	}
	XMD5_DIGEST;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
int 	xmd5_calc( XMD5_DIGEST *m, char *buffer, int length );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
int 	xmd5_calc_sz( XMD5_DIGEST *m, char *txt );
/*===========================================================================*/
#endif 								/* MD5.H */
