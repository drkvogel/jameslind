#ifndef XRANDOM_C_INCLUDED
#define XRANDOM_C_INCLUDED       1
/*===========================================================================*/
/*      SELECTION OF RANDOM NUMBER GENERATORS, RANGE [0.0,0.1)  [NR]    */
/*===========================================================================*/
#include "xdefs.h"
#include "xinclude.h"
#include "xalloc.c"
#include "xmessage.c"
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
static  long    random_seed = 1;
static  long    random_algorithm = 0;
/*---------------------------------------------------------------------------*/
#define	RAN0_IA	16807
#define	RAN0_IM	2147483647
#define	RAN0_IQ	127773
#define	RAN0_IR	2836
#define	RAN0_MASK	123459876
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* RANDOM ALGORITHM #0  */
static double nr_ran0( long *idum )
{	long	k;
	double	ans;
	*idum ^= RAN0_MASK;
	k = (*idum) / RAN0_IQ;
	*idum = RAN0_IA * ( (*idum) - ( k * RAN0_IQ ) ) - ( RAN0_IR * k );
	if ( *idum < 0 )
		{*idum += RAN0_IM;
		}
	ans = (double) (*idum) / (double) RAN0_IM;
	*idum ^= RAN0_MASK;
	return( ans );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#undef	RAN0_IA
#undef	RAN0_IM
#undef	RAN0_IQ
#undef	RAN0_IR
#undef	RAN0_MASK
/*---------------------------------------------------------------------------*/
#define RAN1_IA         16807
#define RAN1_IM         2147483647
#define RAN1_AM         (1.0/RAN1_IM)
#define RAN1_IQ         127773
#define RAN1_IR         2836
#define RAN1_NTAB       32
#define RAN1_NDIV       (1+(RAN1_IM-1)/RAN1_NTAB)
#define RAN1_EPS        1.2e-7
#define RAN1_RNMX       (1.0-RAN1_EPS)
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* RANDOM ALGORITHM #1  */
static double nr_ran1( long *idum )
{       long    j, k;
	static  long    iy = 0;
	static  long    iv[RAN1_NTAB];
	double  temp;
	if ( *idum <= 0 || !iy )
		{*idum = ( *idum > -1 ) ? 1 : -(*idum);
		for ( j = RAN1_NTAB + 7; j >= 0; j-- )
			{k = (*idum) / RAN1_IQ;
			*idum = RAN1_IA * ( *idum - k * RAN1_IQ ) - RAN1_IR * k;
			if ( *idum < 0 )
				{*idum += RAN1_IM;
				}
			if ( j < RAN1_NTAB )
				{iv[j] = *idum;
				}
			}
		iy = iv[0];
		}
	k = (*idum) / RAN1_IQ;
	*idum = RAN1_IA * ( *idum - k * RAN1_IQ ) - RAN1_IR * k;
	if (*idum < 0 )
		{*idum += RAN1_IM;
		}
	j = iy / RAN1_NDIV;
	iy = iv[j];
	iv[j] = *idum;
	return( ( ( temp = RAN1_AM * iy ) > RAN1_RNMX ) ? RAN1_RNMX : temp );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#undef  RAN1_IA
#undef  RAN1_IM
#undef  RAN1_AM
#undef  RAN1_IQ
#undef  RAN1_IR
#undef  RAN1_NTAB
#undef  RAN1_NDIV
#undef  RAN1_EPS
#undef  RAN1_RNMX
/*---------------------------------------------------------------------------*/
#define RAN2_IM1        2147483563
#define RAN2_IM2        2147483399
#define RAN2_AM         ( 1.0 / RAN2_IM1 )
#define RAN2_IMM1       ( RAN2_IM1 - 1 )
#define RAN2_IA1        40014
#define RAN2_IA2        40692
#define RAN2_IQ1        53668
#define RAN2_IQ2        52774
#define RAN2_IR1        12211
#define RAN2_IR2        3791
#define RAN2_NTAB       32
#define RAN2_NDIV       ( 1 + RAN2_IMM1 / RAN2_NTAB )
#define RAN2_EPS        1.2e-7
#define RAN2_RNMX       ( 1.0 - RAN2_EPS )
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* RANDOM ALGORITHM #2  */
static double nr_ran2( long *idum )
{       long    j, k;
	static  long    iy = 0;
	static  long    idum2 = 123456789;
	static  long    iv[RAN2_NTAB];
	double  temp;
	if ( *idum <= 0 )
		{*idum = ( *idum > -1 ) ? 1 : -(*idum);
		idum2 = *idum;
		for ( j = RAN2_NTAB + 7; j >= 0; j-- )
			{k = (*idum ) / RAN2_IQ1;
			*idum = RAN2_IA1 * ( *idum - k * RAN2_IQ1 )
				- k * RAN2_IR1;
			if ( *idum < 0 )
				{*idum += RAN2_IM1;
				}
			if ( j < RAN2_NTAB )
				{iv[j] = *idum;
				}
			}
		iy = iv[0];
		}
	k = (*idum) / RAN2_IQ1;
	*idum = RAN2_IA1 * ( *idum - k * RAN2_IQ1 ) - k * RAN2_IR1;
	if ( *idum < 0 )
		{*idum += RAN2_IM1;
		}
	k = idum2 / RAN2_IQ2;
	idum2 = RAN2_IA2 * ( idum2 - k * RAN2_IQ2 ) - k * RAN2_IR2;
	if ( idum2 < 0 )
		{idum2 += RAN2_IM2;
		}
	j = iy / RAN2_NDIV;
	iy = iv[j] - idum2;
	iv[j] = *idum;
	if ( iy < 1 )
		{iy += RAN2_IMM1;
		}
	return( ( ( temp = RAN2_AM * iy ) > RAN2_RNMX ) ? RAN2_RNMX : temp );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#undef  RAN2_IM1
#undef  RAN2_IM2
#undef  RAN2_AM
#undef  RAN2_IMM1
#undef  RAN2_IA1
#undef  RAN2_IA2
#undef  RAN2_IQ1
#undef  RAN2_IQ2
#undef  RAN2_IR1
#undef  RAN2_IR2
#undef  RAN2_NTAB
#undef  RAN2_NDIV
#undef  RAN2_EPS
#undef  RAN2_RNMX
/*---------------------------------------------------------------------------*/
#define RAN3_MBIG       1000000000
#define RAN3_MSEED      161803398
#define RAN3_MZ         0
#define RAN3_FAC        ( 1.0 / RAN3_MBIG )
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* RANDOM ALGORITHM #3  */
static double nr_ran3( long *idum )
{       long    i, k, mj, mk, n;
	static  long    inext,inextp;
	static  long    iff = 0;
	static  long    ma[56];
	if ( *idum < 0 || 0 == iff )
		{iff = 1;
		mj = RAN3_MSEED - ( ( *idum < 0 ) ? -(*idum) : *idum );
		mj %= RAN3_MBIG;
		ma[55] = mj;
		mk = 1;
		for ( i = 1; i <= 54; i++ )
			{n = ( 21 * i ) % 55;
			ma[n] = mk;
			mk = mj - mk;
			if ( mk < RAN3_MZ )
				{mk += RAN3_MBIG;
				}
			mj = ma[n];
			}
		for ( k = 1; k <= 4; k++ )
			{for ( i = 1; i <= 55; i++ )
				{ma[i] -= ma[ 1 + ( i + 30 ) % 55 ];
				if ( ma[i] < RAN3_MZ )
					{ma[i] += RAN3_MBIG;
					}
				}
			}
		inext = 0;
		inextp = 31;
		*idum = 1;
		}
	if ( ++inext == 56 )
		{inext = 1;
		}
	if (++inextp == 56)
		{inextp = 1;
		}
	mj = ma[inext] - ma[inextp];
	if ( mj < RAN3_MZ )
		{mj += RAN3_MBIG;
		}
	ma[inext] = mj;
	return( mj * RAN3_FAC );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#undef  RAN3_MBIG
#undef  RAN3_MSEED
#undef  RAN3_MZ
#undef  RAN3_FAC
/*---------------------------------------------------------------------------*/
		/* RETURN RANDOM NUMBER IN RANGE 0.0 TO 1.0-    */
double random01( void )
{       double  r;
	switch( random_algorithm )
		{case 0:
			r = nr_ran0( &random_seed );
			break;
		case 1:
			r = nr_ran1( &random_seed );
			break;
		case 2:
			r = nr_ran2( &random_seed );
			break;
		case 3:
			r = nr_ran3( &random_seed );
			break;
		default:
			bug( "random_algorithm = %ld", random_algorithm );
			break;
		}
	return( r );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* RETURN RANDOM NUMBER IN RANGE 0-(N-1) */
long random0n( long n )
{	double	rn;
	if ( n < 1 )
		{bug( "random0n, arg %ld", n );
		}
	rn = (double) n * random01();
	return( ( (long) rn ) % n );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* FILL ARRAY ORDER[N] WITH JUGGLED INTEGERS 0-(N-1) */
void random0n_permutation( long n, long *order )
{	long	i, rn, count;
	for ( i = 0; i < n; i++ )
		{order[i] = -1;				/* MARK AS EMPTY */
		}
	count = n * n;
	for ( i = 0; i < n; i++ )
		{rn = random0n( n );
		while ( order[rn] >= 0 )  /* REPEAT TILL EMPTY ELEMENT FOUND */
			{if ( rn < n-1 )
				{rn++;
				}
			else
				{rn = 0;
				}
			if ( --count < 0 )	/* PREVENT WEIRD OVER-FILLS */
				{bug( "random_order0n" );
				exit( EXIT_FAILURE );
				}
			}
		order[rn] = i;
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
							/* SELECT SEED */
void random_set_seed( long seed )
{       random_seed = seed;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
							/* EXTRACT SEED */
int random_get_seed( void )
{	return( random_seed );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* SELECT CHOICE OF ALGORITHM */
void random_set_algorithm( long alg )
{       random_algorithm = alg;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* EXTRACT CHOICE OF ALGORITHM */
int random_get_algorithm( void )
{	return( random_algorithm );
}
/*===========================================================================*/
#endif                                                  /* XRANDOM.C */
