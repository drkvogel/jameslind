#ifndef XB64_H_DEFINED
#define XB64_H_DEFINED	1
/*===========================================================================*/
/*			BASE-64 ENCODE AND DECODE			*/
/*===========================================================================*/
void xb64_chunking_set( int do_chunking );
int xb64_encode( unsigned char *dat, int len_dat, unsigned char **b64,
	int *len_b64 );
int xb64_decode( unsigned char *b64, int len_b64, unsigned char **dat,
	int *len_dat );
int xb64_encode_txt( char *txt, unsigned char **b64, int *len_b64 );
int xb64_decode_txt( unsigned char *b64, int len_b64, char **txt, int *len_txt);
/*===========================================================================*/
#endif
