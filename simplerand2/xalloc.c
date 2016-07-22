#ifndef XALLOC_C_INCLUDED
#define XALLOC_C_INCLUDED	1
/*===========================================================================*/
/*			MEMORY ALLOCATION UTILITIES			*/
/*===========================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#include "xalloc.h"
#include "xmessage.c"
/*---------------------------------------------------------------------------*/
				/* STORAGE FOR INDIVIDUAL ALLOC-ED VARIABLE */
typedef	struct	{
	void	*address;
	size_t	size;
	}	MEMORY_STORE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* STORAGE FOR ALLOC-ED SPACE */
typedef	struct	{
	long	full;
	long	available;
	MEMORY_STORE	*alloc;
	}	MEMORY;
/*---------------------------------------------------------------------------*/
static	int	xalloc_memory_analysis = 0;
static	long	aa_xalloc_mem_ops = 0;
static	MEMORY	*memory = NULL;
#define	MEM_HASH_NUM	13
/*---------------------------------------------------------------------------*/
static void aa_xalloc_addnew( void *ip, size_t space );
static void aa_xalloc_remove( void *ip );
/*---------------------------------------------------------------------------*/
							/* SHOW MEMORY USAGE */
static void aa_xalloc_show_memory( void )
{	long	ad, i, j, sz;
	long	overhead = 0;
	long	full = 0, mem_tot = 0, fcheck = 0;
	long	range_min = LONG_MIN, range_max = LONG_MAX;
	char	num[20], stmp[200];
	if ( xalloc_memory_analysis < 1 )
		{inform( "memory analysis not active" );
		return;
		}
	sprintf( stmp, "\n\nHashing:" );
	for ( i = 0; i < MEM_HASH_NUM; i++ )
		{sprintf( num, " %4ld", memory[i].full );
		strcat( stmp, num );
		full += memory[i].full;
		overhead += memory[i].available * (long) sizeof(MEMORY_STORE);
		for ( j = 0; j < memory[i].available; j++ )
			{if ( NULL != memory[i].alloc[j].address )
				{ad = (long) memory[i].alloc[j].address;
				sz = (long) memory[i].alloc[j].size;
				range_min = ( range_min < ad ) ? range_min : ad;
				range_max = ( range_max > ad + sz )
					? range_max : ad + sz;
				mem_tot += sz;
				fcheck++;
				}
			}
		}
	inform( stmp );
	inform( "\n\nTotal memory = %ld\t\t%ld regions,\t\t%.1lf%% overhead",
		mem_tot, full, 100.0 * (double) overhead / (double) mem_tot );
	inform( "\n\nAllocation limits  %ld <-> %ld", range_min, range_max );
	if ( fcheck != full )
		{bug("show_memory: %ld = fcheck != full = %ld", fcheck, full);
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* GIVE FAILURE MESSAGE FOR MEMORY */
static void aa_xalloc_failure( void *p, size_t siz )
{	printf( "\n\navailable RAM exceeded" );
	if ( xalloc_memory_analysis > 0 )
		{inform( "Memory analysis:" );
		aa_xalloc_show_memory();
		}
	inform( "out of memory\n\nold pointer = %p  (%ld)", p, (long) p );
	inform( "unsatisfied allocation = %ld words", (long) siz );
	failure( "insufficient memory to continue" );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* HASH-FUNCTION FOR MEMORY ALLOCATION */
static long aa_xalloc_hash( void *ptr )
{	long	ip;
	ip = (long) ptr;
	return( ( ( ip >= 0 ) ? ip : - ip ) % MEM_HASH_NUM );
}
/*---------------------------------------------------------------------------*/
						/* INITIALISE MEMORY COUNTERS */
void xalloc_init( int xma )
{	long	i, j;
	static	long	ncalls = 0;
	size_t	sz;
	inform( "xalloc_init called at level %d", xma );
	if ( ncalls++ > 0 )
		{failure( "xalloc_init called twice" );
		}
	if ( xma < 1 || xma > 2 )
		{failure( "xalloc_init, bad arg = %d", xma );
		}
	xalloc_memory_analysis = xma;
	memory = (MEMORY *) malloc( MEM_HASH_NUM * sizeof(MEMORY) );
	sz = (size_t) ( 52 * sizeof(MEMORY_STORE) );
	for ( i = 0; i < MEM_HASH_NUM; i++ )
		{memory[i].full = 0;
		memory[i].available = 52;
		memory[i].alloc = (MEMORY_STORE *) malloc( sz );
		for ( j = 0; j < 52; j++ )
			{memory[i].alloc[j].address = NULL;
			}
		}
	for ( i = 0; i < MEM_HASH_NUM; i++ )	/* STORE OWN VALUES */
		{j = aa_xalloc_hash( memory[i].alloc );
		memory[j].alloc[ memory[j].full ].address = memory[i].alloc;
		memory[j].alloc[ memory[j].full++ ].size = sz;
		}
}
/*---------------------------------------------------------------------------*/
								/* MALLOC */
void *aa_xmalloc( size_t request )
{	void *ptr;
	if ( (long) request < 0 )
		{bug( "aa_xmalloc: attempted -ve alloc of %ld", (long) request);
		}
	if ( 0 == (long) request )
		{request = 1;		/* DEFAULT TO AVOID NULL RETURN */
		}
	if ( NULL == ( ptr = malloc( request ) ) )
		{aa_xalloc_failure( NULL, request );
		}
	if ( xalloc_memory_analysis > 0 )
		{aa_xalloc_addnew( ptr, request );
		}
	aa_xalloc_mem_ops++;
	return( ptr );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* ADD NEW RECORD TO LIST OF STORAGE */
static void aa_xalloc_addnew( void *ip, size_t space )
{	long	i = 0;
	MEMORY	*m;
	m = memory + aa_xalloc_hash( ip );		/* USE HASH FUNCTION */
	while ( i < m->available && NULL != m->alloc[i].address )
		{i++;					/* FIND EMPTY STORE */
		}
	if ( i >= m->available )
		{bug( "aa_xalloc_addnew: full address array");
		}
	m->alloc[i].address = ip;			/* RECORD ALLOCATION */
	m->alloc[i].size = space;
	m->full++;
	if ( m->full + 3 > m->available )		/* INCREASE SPACE */
		{m->alloc = (MEMORY_STORE *) realloc( m->alloc,
			(size_t)( (m->available+51 ) * sizeof(MEMORY_STORE)) );
		if ( NULL == m->alloc )
			{printf( "\naa_alloc_addnew: out of memory" );
			aa_xalloc_failure( NULL,
			(size_t)((m->available+51) * sizeof(MEMORY_STORE)) );
			}
		for ( i = m->available; i < m->available + 51; i++ )
			{m->alloc[i].address = NULL;	/* MARK AS EMPTY */
			}
		m->available += 50;
		}
}
/*---------------------------------------------------------------------------*/
								/* REALLOC */
void *aa_xrealloc( void *pold, size_t request )
{	void *ptr;
	if ( NULL == pold )
		{bug( "aa_xrealloc: old pointer is null" );
		}
	if ( (long) request < 0 )
		{bug( "aa_xrealloc: attempted -ve alloc of %ld", (long)request);
		}
	if ( 0 == (long) request )
		{request = 1;		/* DEFAULT TO AVOID NULL RETURN */
		}
	if ( xalloc_memory_analysis > 0 )
		{if ( NULL == ( ptr = malloc( request ) ) )
			{aa_xalloc_failure( pold, request );
			}
		memcpy( ptr, pold, request );	/* FORCE MOVE TO UNCOVER BUGS */
		aa_xalloc_remove( pold );
		aa_xalloc_addnew( ptr, request );
		free( pold );
		}
	else
		{if ( NULL == ( ptr = realloc( pold, request ) ) )
			{aa_xalloc_failure( pold, request );
			}
		}
	aa_xalloc_mem_ops++;
	return( ptr );
}
/*---------------------------------------------------------------------------*/
							/* FREE POINTER */
void aa_xfree( void *p )
{	if ( NULL == p )
		{bug( "aa_free: old pointer is null" );
		}
	if ( xalloc_memory_analysis > 0 )
		{aa_xalloc_remove( p );
		}
	free( p );
	aa_xalloc_mem_ops++;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* REMOVE RECORD FROM LIST OF STORAGE */
static void aa_xalloc_remove( void *ip )
{       long	i, old;
	long	found = 0;
	MEMORY	*m;
	m = memory + aa_xalloc_hash( ip );	/* USE HASHING FUNCTION */
	for ( i = 0; i < m->available; i++ )
		{if ( ip == m->alloc[i].address )
			{old = i;		/* FIND ENTRY FOR POINTER */
			found++;
			}
		}
#if defined __BORLANDC__
	if ( found < 1 )/* SEGMENTED ARCHITECTURE ALLOWS APPARENT DUPLICATION */
		{bug( "aa_xalloc_remove: address %p not found", ip );
		}
#else
	if ( found != 1 )
		{bug( "aa_alloc_remove: address %p found %ld times", ip,
			found );
		}				/* TRASH DATA IN FREED SPACE */
	memset( m->alloc[old].address, 127, m->alloc[old].size );
#endif
	m->alloc[old].address = NULL;			/* MARK AS EMPTY */
	m->full--;
}
/*---------------------------------------------------------------------------*/
#undef	MEM_HASH_NUM
/*===========================================================================*/
#endif								/* XALLOC.C */
