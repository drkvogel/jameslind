#ifndef	XALLOT_C_INCLUDED
#define	XALLOT_C_INCLUDED	1
/*===========================================================================*/
/*			TREATMENT ALLOTMENT UTILITIES			*/
/*===========================================================================*/
#include "xdefs.h"
#include "xinclude.h"
#include "xalloc.c"
#include "xmessage.c"
/*---------------------------------------------------------------------------*/
typedef	struct
	{
	int	index;			/* INDEX OF TREATMENT		*/
	long	metric;			/* `GOODNESS' OF TREATMENT	*/
	double	rmetric;		/* RANDOMIZED METRIC		*/
	long	weight;			/* WEIGHT FOR TREATMENT		*/
	}
	ALLOT_STRATEGY_CHOICE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
typedef	struct
	{
	char	*label;			/* IDENTIFIER FOR DATA		*/
	int	ntreat;			/* NUMBER OF POSSIBLE TREATMENTS */
	int	nvalues;	/* NUM VALUES OF EPIDEMIOLOGICAL INTEREST */
	int	*nstrata;		/* NUM STRATA IN EACH VALUE	*/
	char	**name;			/* NAME OF EACH VALUE		*/
	int	ntypes;			/* CONTINUED PRODUCT OF NSTRATA[] */
	int	**past;			/* PREVIOUS ALLOCATIONS		*/
	int	okay;			/* VALID ORDERING IS STORED	*/
	ALLOT_STRATEGY_CHOICE	*choice;
	}
	ALLOT_STRATEGY;
/*---------------------------------------------------------------------------*/
static	int	debug_allot = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* SET INTERNAL DEBUGGING PARAMETER */
void allot_set_debug( int td )
{	debug_allot = td;
}
/*---------------------------------------------------------------------------*/
				/* DEBUG FUNCTION SHOWING INTERNAL CONTENTS */
void allot_show_internal( ALLOT_STRATEGY *ti )
{	int	i;
	printf( "\nData for ALLOT_STRAT \"%s\"", ti->label );
	printf( "\nntreat = %d\t\tnvalues = %d\t\tntypes = %d\n", ti->ntreat,
		ti->nvalues, ti->ntypes );
	for ( i = 0; i < ti->nvalues; i++ )
		{printf( "v[%d]=%d   ", i, ti->nstrata[i] );
		}
}
/*---------------------------------------------------------------------------*/
		/* CONVERT COMPOUND ARRAY-INDEX CODE TO ORIGINAL VALUES */
void allot_code_to_val( ALLOT_STRATEGY *ts, int code, int *val )
{	int	i;
	if ( code < 0 || code >= ts->ntypes )
		{bug( "allot_code_to_val %d", code );
		}
	for ( i = ts->nvalues-1; i > 0; i-- )
		{val[i] = code % ts->nstrata[i];
		code /= ts->nstrata[i];
		}
	val[ 0 ] = code;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
	/* EXTRACT SINGLE ORIGINAL VALUE INDEX FROM COMPOUND ARRAY-INDEX */
int allot_code_to_val_single( ALLOT_STRATEGY *ts, int code, int position )
{       static	int	nval_max = -1;
	static	int	*value = NULL;
	if ( position < 0 || position >= ts->nvalues )
		{bug( "allot_code_to_val_single %d", position );
		}
	if ( ts->nvalues > nval_max )
		{nval_max = ts->nvalues;
		value = ( NULL == value )
			? (int *) xmalloc( nval_max * sizeof(long) )
			: (int *) xrealloc( value, nval_max * sizeof(long) );
		}
	allot_code_to_val( ts, code, value );
	return( value[ position ] );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* CONVERT VALUES TO COMPOUND ARRAY-INDEX CODE */
int allot_val_to_code( ALLOT_STRATEGY *ts, int *val )
{	int	i;
	int	code = 0;
	for ( i = 0; i < ts->nvalues; i++ )
		{if ( val[i] < 0 || val[i] >= ts->nstrata[i] )
			{bug( "allot_val_to_code val[%d]=%d  (0,%d)", i,
				val[i], ts->nstrata[i] );
			}
		code *= ts->nstrata[i];
		code += val[i];
		}
	return( code );
}
/*---------------------------------------------------------------------------*/
				/* PERFORM PARTIAL COUNT FOR MINIMIZATION */
long allot_minimization_count( ALLOT_STRATEGY *tc, int *previous,
		int *current )
{	int	i, j;
	long	sum = 0;
	static	int	indx_max = -1;
	static	int	*indx = NULL;
	if ( debug_allot > 1 )
		{for ( i = 0; i < tc->nvalues; i++ )
			{aside( "current[%d] = %d", i, current[i] );
			}
		for ( i = 0; i < tc->ntypes; i++ )
			{aside( "previous[%d] = %d", i, previous[i] );
			}
		}
	if ( tc->nvalues > indx_max )
		{if ( NULL == indx )
			{indx = (int *) xmalloc( ( tc->nvalues + 4 )
				* sizeof(long) );
			}
		else
			{indx = (int *) xrealloc( indx, ( tc->nvalues + 4 )
				* sizeof(long) );
			}
		indx_max = tc->nvalues;
		}
	for ( i = 0; i <= tc->nvalues; i++ )	/* INITIALIZE COUNTERS */
		{indx[i] = 0;
		}
	for ( i = 0; i < tc->ntypes; i++ )	/* LOOP OVER ALL COMBINATIONS */
		{allot_code_to_val( tc, i, indx );
		for ( j = 0; j < tc->nvalues; j++ )
			{if ( current[j] == indx[j] )	/* INCREMENT SUM */
				{sum += previous[i];
				}
			}
		}
	if ( debug_allot > 0 )
		{aside( "\tsum = %ld", sum );
		}
	return( sum );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* ORDER TWO CHOICES ACCORDING TO THEIR METRICS */
int sort_cmp_ASCHOICE( const void *a, const void *b )
{	double	rdif;
	ALLOT_STRATEGY_CHOICE	*sca, *scb;
	sca = (ALLOT_STRATEGY_CHOICE *) a;
	scb = (ALLOT_STRATEGY_CHOICE *) b;
	rdif = sca->rmetric - scb->rmetric;
	if ( rdif < 0.0 )
		{return( -1 );
		}
	if ( rdif > 0.0 )
		{return( 1 );
		}
	return( 0 );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* ALLOCATE TREATMENT USING MINIMIZATION */
void allot_minimization( ALLOT_STRATEGY *tm, int *current_val )
{	int	i;
	int	first = 0, last = 1;
	for ( i = 0; i < tm->ntreat; i++ )		/* FIND LOWEST COUNT */
		{tm->choice[i].index = i;
		tm->choice[i].metric = allot_minimization_count( tm,
			tm->past[i], current_val );
		tm->choice[i].rmetric = (double) tm->choice[i].metric
			+ 0.1 * random01();	/* RANDOMLY ASSORT TIES */
		tm->choice[i].rmetric /= (double) tm->choice[i].weight;
		}
	qsort( tm->choice, (size_t) tm->ntreat, sizeof(ALLOT_STRATEGY_CHOICE),
		sort_cmp_ASCHOICE );
	if ( debug_allot > 0 )
		{for ( i = 0; i < tm->ntreat; i++ )
			{aside( "choice[%d]\tmetric=%d\trmetric=%lg\tindex=%d",
				i, tm->choice[i].metric, tm->choice[i].rmetric,
				tm->choice[i].index );
			}
		}
	tm->okay = YES;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
		/* RETURN CHOICE OF TREATMENT BY RANK OF GOODNESS, 0 IS BEST */
int allot_minimization_choice( ALLOT_STRATEGY *tc, int rank )
{       int	choice = -1;
	if ( rank < 0 || rank >= tc->ntreat )
		{bug( "allot_minimization_choice, rank %d", rank );
		}
	else if ( YES == tc->okay )
		{choice = tc->choice[ rank ].index;
		}
	return( choice );
}
/*---------------------------------------------------------------------------*/
		/* COUNT NUMBER OF PEOPLE WITH A PARTICULAR TREATMENT TR */
long allot_count_treatment(  ALLOT_STRATEGY *ast, int tr )
{	long	i;
	long	sum = 0;
	if ( tr < 0 || tr >= ast->ntreat )
		{bug( "allot_count_treatment, tr=%ld", tr );
		}
	for ( i = 0; i < ast->ntypes; i++ )
		{sum += ast->past[tr][i];
		}
	return( sum );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
		/* COUNT PEOPLE WITH TREATMENT TR IN STRATA ST OF VARIABLE VA */
long allot_count_group( ALLOT_STRATEGY *ast, int tr, int va, int st )
{       int	i;
	int	*value;
	long	sum = 0;
	if ( tr < 0 || tr >= ast->ntreat || va < 0 || va >= ast->nvalues
			|| st < 0 || st >= ast->nstrata[va] )
		{bug( "allot_count_group, tr=%ld va=%ld st=%ld", tr, va, st );
		}
	value = (int *) xmalloc( ( ast->nvalues + 1 ) * sizeof(int) );
	for ( i = 0; i < ast->ntypes; i++ )
		{allot_code_to_val( ast, i, value );
		if ( value[ va ] == st )
			{sum += ast->past[tr][i];
			}
		}
	xfree( value );
	return( sum );
}
/*---------------------------------------------------------------------------*/
					/* FREE TREATMENT STRATEGY STRUCTURE */
void allot_free( ALLOT_STRATEGY **ts )
{	int	i;
	for ( i = 0; i < (*ts)->ntreat; i++ )
		{xfree( (*ts)->past[i] );
		}
	for ( i = 0; i < (*ts)->nvalues; i++ )
		{xfree( (*ts)->name[i] );
		}
	xfree( (*ts)->name );
	xfree( (*ts)->past );
	xfree( (*ts)->nstrata );
	xfree( (*ts)->label );
	xfree( (*ts)->choice );
	xfree( *ts );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* ADD TREATMENT ENTRY TO COUNTERS */
int allot_add( ALLOT_STRATEGY *ta, int trt, int ind )
{	if ( trt < 0 || trt >= ta->ntreat || ind < 0 || ind >= ta->ntypes )
		{aside( "ERROR allot_add, invalid  trt=%d  or  ind=%d",
			trt, ind );
		return( NO );
		}
	ta->past[ trt ][ ind ]++;
	return( YES );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* SET OPTIONAL WEIGHTS FOR MINIMIZATION */
void allot_setweight( ALLOT_STRATEGY *tw, int *wts )
{	int	i;
	for ( i = 0; i < tw->ntreat; i++ )
		{if ( ( tw->choice[i].weight = wts[i] ) < 1
				|| tw->choice[i].weight > 1000 )
			{bug( "allot_setweight, invalid value %d",
				wts[i] );
			}
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* SET COUNTERS TO ZERO */
void allot_zeroise( ALLOT_STRATEGY *tz )
{	long	i, j;
	for ( i = 0; i < tz->ntreat; i++ )
		{for ( j = 0; j < tz->ntypes; j++ )
			{tz->past[i][j] = 0;
			}
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
		/* ALLOCATE AND INITIALIZE TREATMENT STRATEGY STRUCTURE */
ALLOT_STRATEGY *allot_init( char *title, int nt, int nv, int *ns, char **nam )
{       int	i;
	ALLOT_STRATEGY	*ts;
	ts = xmalloc( sizeof(ALLOT_STRATEGY) );
	ts->label = (char *) xmalloc( strlen( title ) + 2 );
	strcpy( ts->label, title );
	if ( ( ts->ntreat = nt ) < 2 || nt > 100 )
		{bug( "Strategy %s, ntreat = %d", title, nt );
		}
	if ( ( ts->nvalues = nv ) < 1 || nv > 100 )
		{bug( "Strategy %s, nvalues = %d", title, nv );
		}
	ts->okay = NO;
	ts->ntypes = 1;
	ts->nstrata = (int *) xmalloc( ( ts->nvalues + 2 ) * sizeof(long) );
	for ( i = 0; i < nv; i++ )
		{if ( ns[i] < 2 || ns[i] > 100 )
			{bug( "Strategy %s, value[%d] = %d", title, i, ns[i] );
			}
		ts->ntypes *= ( ts->nstrata[i] = ns[i] );
		}
	ts->past = (int **) xmalloc( ts->ntreat * sizeof(int *) );
	ts->choice = (ALLOT_STRATEGY_CHOICE *) xmalloc( ts->ntreat
		* sizeof(ALLOT_STRATEGY_CHOICE) );
	for ( i = 0; i < ts->ntreat; i++ )
		{ts->past[i] = (int *) xmalloc( ts->ntypes * sizeof(long) );
		ts->choice[i].weight = 1;
		}
	ts->name = (char **) xmalloc( ts->nvalues * sizeof(char *) );
	for ( i = 0; i < ts->nvalues; i++ )		/* STORE VALUE NAMES */
		{ts->name[i] = (char *) xmalloc( strlen( nam[i] ) + 1 );
		strcpy( ts->name[i], nam[i] );
		}
	allot_zeroise( ts );
	return( ts );
}
/*===========================================================================*/
#endif							/* XALLOT.C */

