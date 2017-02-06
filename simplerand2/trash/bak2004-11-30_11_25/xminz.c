#ifndef	XMINZ_C_INCLUDED
#define	XMINZ_C_INCLUDED	1
/*===========================================================================*/
/*		TREATMENT MINIMIZATION UTILITIES			*/
/*===========================================================================*/
#include "xdefs.h"
#include "xinclude.h"
#include "xalloc.c"
#include "xmessage.c"
/*---------------------------------------------------------------------------*/
typedef	struct
	{
	char	*name;			/* NAME OF COLUMN VARIABLE	*/
	int	nvals;			/* NUMBER OF VALUES TAKEN IN COL */
	int	*past;
	}
	XMINZ_COL;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
typedef	struct
	{
	int	count;			/* RAW COUNT OF MATCHES		*/
	int	index;			/* INDEX OF TREATMENT		*/
	double	metric;			/* `GOODNESS' OF TREATMENT	*/
	int	weight;			/* WEIGHT FOR VALUES		*/
	}
	XMINZ_TREAT;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
/*
 FOR SIMPLE ALLOCATION, E.G. X STRATA => NCOLS = X+1, (EXTRA IS TREATMENT)

	PAST[A][B]	HOLDS VALUE FOR COL A, PERSON B
*/
typedef	struct
	{
	char	*label;			/* IDENTIFIER FOR DATA		*/
	int	ncols;			/* NUM COLS INTEREST		*/
	int	npast;		   	/* NUMBER OF PREVIOUS ALLOCATIONS */
	int	treat_nvals;		/* NUMBER OF TREATMENT VALUES	*/
	XMINZ_COL	*col;
	XMINZ_TREAT	*trt;
	}
	XMINZ_STRATEGY;
/*---------------------------------------------------------------------------*/
static	int	xminz_debug = 0;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* SET INTERNAL DEBUGGING PARAMETER */
void xminz_set_debug( int td )
{	xminz_debug = td;
}
/*---------------------------------------------------------------------------*/
			/* ORDER TWO COUNTS ACCORDING TO THEIR METRICS */
int xminz_sort_cmp_TREAT( const void *a, const void *b )
{	double	rdif;
	XMINZ_TREAT	*sca, *scb;
	sca = (XMINZ_TREAT *) a;
	scb = (XMINZ_TREAT *) b;
	rdif = sca->metric - scb->metric;
	if ( rdif < 0.0 )
		{return( -1 );
		}
	if ( rdif > 0.0 )
		{return( 1 );
		}
	return( 0 );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
		/* COUNT TREATMENTS FOR ENTRIES MATCHING CURRENT PERSON */
void xminz_count_match( int nrows, int *past_val, int *past_trt, int match,
		XMINZ_TREAT *tcount )
{	int	i;
	for ( i = 0; i < nrows; i++ )
		{if ( past_val[i] == match )
			{tcount[ past_trt[i] ].count++;
			}
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* MARK AS NOT EVALUATED */
void xminz_clear_results( XMINZ_STRATEGY *tm )
{	if ( NULL != tm->trt )
		{xfree( tm->trt );
		tm->trt = NULL;
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* DUMP INTERNALS FOR DEBUG */
void xminz_dump( XMINZ_STRATEGY *xs, XMINZ_COL *ctreat )
{
	int	i, j;
	char	buf[500], tmp[50];
	for ( i = 0; i < ctreat->nvals; i++ )
		{aside( "rank[%d]\tindx %d\tcount=%d\tmetric=%lg",
			i, xs->trt[i].index,
			xs->trt[i].count, xs->trt[i].metric );
		}
	if ( xminz_debug & 2 )		/* SHOW ALL PAST STORAGE */
		{
		for ( i = 0; i < xs->npast; i++ )
			{sprintf( buf, "PAST %d", i );
			for ( j = 0; j < xs->ncols; j++ )
				{sprintf( tmp, " %d", xs->col[j].past[i] );
				}
			}
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* ALLOCATE TREATMENT USING MINIMIZATION */
			/* ANALYSE ONLY FOR COLUMNS WITH +VE WEIGHT */
int xminz_eval( XMINZ_STRATEGY *tm, int *current, int *use, int treat_col,
		int *weight )
{	int	i, cur;
	XMINZ_COL	*cval, *ctreat;
	xminz_clear_results( tm );
	if ( treat_col < 0 || treat_col >= tm->ncols )
		{bug( "xminz_eval: treat_col, index out of range" );
		return( NO );
		}
	ctreat = tm->col + treat_col;
	tm->treat_nvals = ctreat->nvals;
	if ( use[treat_col] != 0 )
		{bug( "xminz_eval: use[treat_col] must be 0" );
		return( NO );
		}
	tm->trt = (XMINZ_TREAT *) xmalloc( ctreat->nvals
		* sizeof(XMINZ_TREAT) );
	for ( i = 0; i < ctreat->nvals; i++ )
		{
		tm->trt[i].index = i;
		tm->trt[i].count = 0;
		}
	for ( i = 0; i < tm->ncols; i++ )
		{
		if ( use[i] != 0 )
			{
			cval = tm->col + i;
			cur = current[i];
			if ( cur < 0 || cur >= cval->nvals )
				{bug( "xminz_eval: illegal current[%d]=%d",
					i, cur );
				xminz_clear_results( tm );
				return( NO );
				}
			xminz_count_match( tm->npast, cval->past, ctreat->past,
				cur, tm->trt );
			}
		}				/* FIND LOWEST METRIC */
	for ( i = 0; i < ctreat->nvals; i++ )
		{tm->trt[i].metric = (double) tm->trt[i].count;
		if ( NULL != weight )
			{if ( weight[i] < 1 )
				{bug(
				"xminz_eval: weight[%d]must be positive" );
				xminz_clear_results( tm );
				return( NO );
				}
			tm->trt[i].metric *= (double) weight[i];
			}
		tm->trt[i].metric += 0.001 * random01();  /* RANDOMIZE TIES */
		}
	qsort( tm->trt, (size_t) ctreat->nvals, sizeof(XMINZ_TREAT),
		xminz_sort_cmp_TREAT );
	if ( xminz_debug > 0 )
		{xminz_dump( tm, ctreat);
		}
	return( YES );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
		/* SELECT CHOICE OF TREATMENT BY RANK OF GOODNESS, 0 IS BEST */
int xminz_select( XMINZ_STRATEGY *tc, int rank )
{
	if ( NULL == tc->trt )
		{bug( "xminz_select: xminz_eval not called successfully" );
		return( -1 );
		}
	if ( rank < 0 || rank >= tc->treat_nvals )
		{bug( "xminz_select, out of range rank %d", rank );
		return( -1 );
		}
	return( tc->trt[ rank ].index );
}
/*---------------------------------------------------------------------------*/
		/* COUNT NUMBER OF PEOPLE WITH A PARTICULAR VAL IN A COL */
int xminz_count_val( XMINZ_STRATEGY *ast, int column, int val )
{	int	i;
	int	sum = 0;
	if ( column < 0 || column >= ast->ncols )
		{bug( "xminz_count_val, invalid col=%d", column );
		return( -1 );
		}
	for ( i = 0; i < ast->npast; i++ )
		{if ( val == ast->col[column].past[i] )
			{sum++;
			}
		}
	return( sum );
}
/*---------------------------------------------------------------------------*/
					/* FREE TREATMENT STRATEGY STRUCTURE */
void xminz_free( XMINZ_STRATEGY **ts )
{	int	i;
	XMINZ_COL	*c;
	c = (*ts)->col;
	for ( i = 0; i < (*ts)->ncols; i++ )
		{
		xfree( c[i].name );
		xfree( c[i].past );
		}
	xfree( (*ts)->label );
	xfree( (*ts)->col );
	xfree( *ts );
	*ts = NULL;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* ADD NEW DATA TO COUNTERS */
int xminz_add( XMINZ_STRATEGY *ta, int *val )
{
	int	i, v;
	int	*p;
	XMINZ_COL	*c;
	xminz_clear_results( ta );		/* MARK AS NOT EVALUATED */
	c = ta->col;
	for ( i = 0; i < ta->ncols; i++ )
		{
		v = c[i].past[ta->npast] = val[i];
		if ( v < 0 || v >= c[i].nvals )
			{aside( "ERROR xminz_add, invalid val[%d]=%d", i, v );
			return( NO );
			}
		}
	if ( ++(ta->npast) % 100 == 0 )
		{for ( i = 0; i < ta->ncols; i++ )	/* EXTEND SPACES */
			{c[i].past = (int *) xrealloc( c[i].past,
				( ta->npast + 105 ) * sizeof(int) );
			}
		}
	return( YES );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
		/* ALLOCATE AND INITIALIZE TREATMENT STRATEGY STRUCTURE */
XMINZ_STRATEGY *xminz_init( char *title, int ncols, int *max_indx, char **nam )
{       int	i;
	XMINZ_STRATEGY	*ts;
	XMINZ_COL	*cl;
	ts = (XMINZ_STRATEGY *) xmalloc( sizeof(XMINZ_STRATEGY) );
	ts->label = (char *) xmalloc( strlen( title ) + 2 );
	strcpy( ts->label, title );
	if ( ncols < 2 || ncols > 100 )
		{bug( "xminz_init %s: ncols = %d", title, ncols );
		return( NULL );
		}
	ts->ncols = ncols;
	ts->trt = NULL;			/* MARK AS NOT EVALUATED	*/
	ts->npast = 0;			/* NUMBER OF `PAST' ALLOCATIONS	*/
	ts->col = (XMINZ_COL *) xmalloc( ts->ncols * sizeof(XMINZ_COL) );
	for ( i = 0; i < ts->ncols; i++ )	/* STORE COLUMN NAMES */
		{
		cl = ts->col + i;
		cl->name = (char *) xmalloc( strlen( nam[i] ) + 1 );
		strcpy( cl->name, nam[i] );
		if ( max_indx[i] < 2 || max_indx[i] > 100 )
			{bug( "xminz_init: %s, max_indx[%d] = %d", title,
				max_indx[i] );
			}
		cl->nvals = max_indx[i];
		cl->past = (int *) xmalloc( 105 * sizeof(int) );
		}
	return( ts );
}
/*===========================================================================*/
#endif							/* XMINZ.C */
