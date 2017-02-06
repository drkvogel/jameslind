#ifndef XALLOC_H_INCLUDED
#define XALLOC_H_INCLUDED	1
/*===========================================================================*/
/*			MEMORY ALLOCATION UTILITIES			*/
/*===========================================================================*/
void xalloc_init( int xma );
void *aa_xmalloc( size_t request );
void *aa_xrealloc( void *pold, size_t request );
void aa_xfree( void *p );
/*---------------------------------------------------------------------------*/
#define	xmalloc		aa_xmalloc
#define xrealloc 	aa_xrealloc
#define xfree( a )	{ aa_xfree( ( a ) ); a = NULL; }
/*===========================================================================*/
#endif								/* XALLOC.H */
