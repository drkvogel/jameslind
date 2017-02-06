#ifndef	XTIME_C_INCLUDED
#define	XTIME_C_INCLUDED	1
/*===========================================================================*/
/*			XTIME HANDLING UTILITIES				*/
/*	DATES ARE TIMES MEASURED ONLY IN YEARS+MONTHS+DAYS		*/
/*===========================================================================*/
#include	<time.h>
#include	"xtime.h"
#include	"xtext.c"
/*---------------------------------------------------------------------------*/
static	char	*month_name[13]
	= {	"january",	"february",	"march",	"april",
		"may",		"june",		"july",		"august",
		"september",	"october",	"november",	"december",
		"" };
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
static	long	days_norm[13]
	= { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 0 };
static	long	dcum_norm[13]
	= {  0, 31, 59, 90,120,151,181,212,243,273,304,334,365 };
static	long	days_leap[13]
	= { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 0 };
static	long	dcum_leap[13]
	= {  0, 31, 60, 91,121,152,182,213,244,274,305,335,366 };
static	long	*days_year, *dcum_year;
/*---------------------------------------------------------------------------*/
/*	RETURN PIVOT YEAR SUCH THAT				*/
/*	2-DIGIT YEAR XX <= PIVOT  ->  4-DIGIT YEAR IN 20XX	*/
/*	2-DIGIT YEAR XX >  PIVOT  ->  4-DIGIT YEAR IN 19XX	*/
/*								*/
int default_century_pivot( void )
{	int	piv;
	time_t	today;
	struct	tm	*td;
	time( &today );
	td = localtime( &today );
	piv = ( td->tm_year + 5 ) % 100;
	return( piv );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* ADD DEFAULT CENTURY TO XDATE IF REQUIRED */
void default_century_apply( XDATE *dat )
{	static	int	pivot = -1;
	if ( dat->year > 99 )		/* ALREADY INCLUDES A CENTURY */
		{return;
		}
	if ( dat->year < 0 )
		{bug( "default_century_apply" );
		}
	if ( pivot < 0 )			/* ONCE-ONLY INITIALIZATION */
		{pivot = default_century_pivot();
		}
	dat->year += ( dat->year > pivot ) ? 1900 : 2000;
}
/*---------------------------------------------------------------------------*/
						/* DAY PORTION OF XDATE */
long date_day( XDATE *d )
{	return( d->day );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* MONTH PORTION OF XDATE */
long date_month( XDATE *d )
{	return( d->month );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* ACTUAL NAME OF MONTH */
char *date_month_name( XDATE *d )
{	return( month_name[ date_month( d ) - 1 ] );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* YEAR PORTION OF XDATE */
long date_year( XDATE *d )
{	return( d->year );
}
/*---------------------------------------------------------------------------*/
						/* IS YEAR A LEAP YEAR ? */
long leap_year_numeric( long nyear )
{	if ( 0 != nyear % 4 )
		{return( 0 );
		}
	return( ( 0 == nyear % 100 && 0 != nyear % 400 ) ? 0 : 1 );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* IS XDATE A LEAP YEAR ? */
long leap_year( XDATE *dat )
{	return( leap_year_numeric( dat->year ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* SET GLOBAL POINTERS FOR CURRENT YEAR */
void days_year_set( XDATE *dat )
{	if ( leap_year( dat ) )
		{days_year = days_leap;
		dcum_year  = dcum_leap;
		}
	else
		{days_year = days_norm;
		dcum_year  = dcum_norm;
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* DECIDE IF XDATE IS VALID */
long valid_date( XDATE *dat )
{	if ( dat->day < 1 || dat->month < 1 || dat->month > 12 || dat->year < 0
			|| ( dat->year > 99 && dat->year < 1800 ) )
		{return( 0 );
		}
	default_century_apply( dat );
	days_year_set( dat );
	return ( ( dat->day > days_year[ dat->month - 1 ] ) ? 0 : 1 );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* CONVERT MONTH+DAYS TO DAYS ONLY */
long date_daymon_to_daysonly( XDATE *d )
{	long	*dcum;
	dcum = ( leap_year( d ) ) ? dcum_norm : dcum_leap;
	return( d->day + dcum[ d->month - 1 ] );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* CONVERT DAYS ONLY TO MONTH+DAYS */
long date_daysonly_to_daymon( XDATE *d )
{	long	*dcum;
	dcum = ( leap_year( d ) ) ? dcum_norm : dcum_leap;
	return( d->day + dcum[ d->month - 1 ] );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* PRINT XDATE AS A STRING */
void date_to_string( char *s, XDATE *dt, int fcode )
{	char	*fmt = "ERROR";
	switch( fcode )
		{case 0:
			fmt = "%ld/%ld/%ld";
			break;
		case 1:
			fmt = "%2.2ld%2.2ld%ld";
			break;
		default:
			bug( "date_to_string: bad format code = %d", fcode );
			break;
		}
	sprintf( s, fmt, (long) date_day( dt ), (long) date_month( dt ),
		(long) date_year( dt ) );	
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
		/* CONVERT STRING A#B#C TO XDATE, A,B,C INTS, # NON-INT */
void string_to_date( char *s, XDATE *dt, char **msg, int allow_all_digit )
{       long	j;
	long	i = 0;
	long	val[3];
	char	all_digit = NO;
	char	*tmp;
	char	buf[100];
	tmp = skip_space( s );
	while ( EOS != tmp[i] )
		{if ( isalpha( tmp[i] ) )
			{*msg = "letters A-Z are not allowed in dates";
			return;
			}
		i++;
		}
	while ( --i > 0 && isspace( tmp[i] ) )
		{tmp[i] = EOS;
		}
	if ( YES == allow_all_digit && 6 == strlen( tmp ) )
		{all_digit = YES;
		for ( i = 0; i < 6; i++ )
			{if ( ! isdigit( tmp[i] ) )
				{all_digit = NO;
				}
			}
		}
	if ( YES != all_digit )
		{for ( i = 0; i < 3; i++ )
			{j = 0;
				while ( ! isdigit( *s ) && EOS != *s )
				{s++;
				}
			while ( isdigit( *s ) )
				{buf[j++] = *s++;
				}
			buf[j] = EOS;
			if ( 0 == j )
				{*msg = "missing value in date";
				return;
				}
			val[i] = atoi( buf );
			}
		while ( EOS != *s )
			{if ( ! isspace( *s++ ) )
				{*msg = "extra item at end of date";
				return;
				}
			}
		}
	else
		{strcpy( buf, tmp );		/* FORMAT IS 6-DIGIT DDMMYY */
		for ( i = 2; i >= 0; i-- )
			{buf[ 2 * ( i + 1 ) ] = EOS;
			val[i] = atoi( buf + ( 2 * i ) );
			}
		}
	dt->day = val[0];
	dt->month = val[1];
	dt->year = val[2];
	*msg = ( valid_date( dt ) ) ? NULL : "date is invalid";
}
/*---------------------------------------------------------------------------*/
		/* COMPARE DATES: DA < DB => -1, DA = DB => 0, DA > DB = 1 */
long date_cmp( XDATE *da, XDATE *db )
{	if ( da->year < db->year )	/* SEQUENTIALLY COMPARE BY UNITS */
		{return( -1 );
		}
	if ( da->year > db->year )
		{return( 1 );
		}
	if ( da->month < db->month )
		{return( -1 );
		}
	if ( da->month > db->month )
		{return( 1 );
		}
	if ( da->day < db->day )
		{return( -1 );
		}
	if ( da->day > db->day )
		{return( 1 );
		}
	return( 0 );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* COPY ONE XDATE INTO ANOTHER */
void date_copy( XDATE *to, XDATE *from )
{	to->day   = from->day;
	to->month = from->month;
	to->year  = from->year;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

void date_shift( XDATE *d )
{
	if ( 2 == d->month && 29 == d->day && ! leap_year( d ) )
		{d->day = 28;
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* SUBTRACT RAW DAYS+YEARS XTIME FROM XDATE */
void date_subtract_raw( XDATE *dt, long dy, long yr )
{	if ( dy < 0 || yr < 0 || ! valid_date( dt ) )
		{bug( "\ndate_subtract_raw, invalid input parameters" );
		exit( EXIT_SUCCESS );
		}
	days_year_set( dt );
	while ( dy >= dt->day )
		{dy -= dt->day;
		if ( dt->month > 1 )
			{dt->month--;
			}
		else
			{dt->year--;
			dt->month = 12;
			days_year_set( dt );
			}
		dt->day = days_year[ dt->month - 1 ];
		}
	dt->day -= dy;
	dt->year -= yr;
	date_shift( dt );
	if ( ! valid_date( dt ) )
		{bug( "\ndate_subtract_raw, invalid result" );
		exit( EXIT_SUCCESS );
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* ADD RAW XTIME TO XDATE */
void date_add_raw( XDATE *dt, long dy, long yr )
{	int	again = YES;
	if ( dy < 0 || yr < 0 || ! valid_date( dt ) )
		{bug( "\ndate_add_raw, invalid input parameters" );
		exit( EXIT_SUCCESS );
		}
	dt->year += yr;
	dt->day  += dy;
	do
		{while ( dt->month > 12 )
			{dt->year++;
			dt->month -= 12;
			}
		days_year_set( dt );
		if ( dt->day > days_year[ dt->month - 1 ] )
			{dt->day -= days_year[ dt->month - 1 ];
			dt->month++;
			}
		else
			{again = NO;
			}
		}
		while ( YES == again );
	if ( ! valid_date( dt ) )
		{bug( "\ndate_add_raw, invalid result" );
		exit( EXIT_SUCCESS );
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* GO BACK AN INTERVAL BEFORE BASELINE */
void date_before( XDATE *base, XDATE *out, int nday, int nyear )
{	date_copy( out, base );
	date_subtract_raw( out, nday, nyear );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* GO FORWARD AN INTERVAL AFTER BASELINE */
void date_after( XDATE *base, XDATE *out, int nday, int nyear )
{	date_copy( out, base );
	date_add_raw( out, nday, nyear );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* FIND NUMBER OF DAYS BETWEEN TWO DATES */
int date_days_diff( XDATE *d_to, XDATE *d_from )
{	int	count = 0;
	int	cmp, ydif, sign;
	XDATE	dtmp;
	XDATE	*first, *last;
	if ( ! valid_date( d_from ) || ! valid_date( d_to ) )
		{bug( "date_days_diff, invalid input parameters" );
		exit( EXIT_SUCCESS );
		}
	cmp = date_cmp( d_from, d_to );
	if ( 0 == cmp )
		{return( 0 );
		}
	else if ( cmp < 0 )
		{sign = 1;
		first = d_from;
		last  = d_to;
		}
	else			/* NEGATE BECAUSE BEGINNING IS AFTER END */
		{sign = -1;
		first = d_to;
		last  = d_from;
		}
	ydif = date_year( last ) - date_year( first );
	if ( ydif > 20 )
		{bug( "date_days_diff, years apart = %d", ydif );
		}
	date_copy( &dtmp, first );
	do
		{count++;
		date_add_raw( &dtmp, 1, 0 );
		}
		while ( 0 != date_cmp( last, &dtmp ) && count < 8000 );
	if ( count >= 8000 )
		{bug( "date_days_diff, runaway count" );
		exit( EXIT_SUCCESS );
		}
	return( count * sign );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
		/* RETURN WHOLE-NUMBER OF YEARS BETWEEN VALUES, ROUNDED DOWN */
int date_years_diff( XDATE *start, XDATE *final )
{	int	count;
	count = date_year( final ) - date_year( start );
	if ( date_month( start ) > date_month( final ) )
		{count--;
		}
	else if ( date_month( start ) == date_month( final )
			&& date_day( start ) > date_day( final ) )
		{count--;
		}
	return( count );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
							/* SET A XDATE */
void date_set( XDATE *dat, long d, long m, long y )
{	dat->day = d;
	dat->month = m;
	dat->year = y;
	if ( ! valid_date( dat ) )
		{bug( "illegal date %ld/%ld/%ld", d, m, y );
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* SET A XDATE TO TODAY */
void date_set_today( XDATE *dat )
{	char	tbuf[52];
	time_t	today;
	time( &today );
	strftime( tbuf, 50, "%d %m %Y", localtime( &today ) );
	sscanf( tbuf, "%ld %ld %ld", &(dat->day), &(dat->month), &(dat->year) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* SET A XDATE TO TOMORROW */
void date_set_tomorrow( XDATE *dtom )
{	date_set_today( dtom );
	date_add_raw( dtom, 1, 0 );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* SET A XDATE TO YESTERDAY */
void date_set_yesterday( XDATE *dyes )
{	date_set_today( dyes );
	date_subtract_raw( dyes, 1, 0 );
}
/*---------------------------------------------------------------------------*/
						/* HOUR PORTION OF XTIME */
int clock_hour( XCLOCK *t )
{	return( t->hour );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* MINUTE PORTION OF XTIME */
int clock_minute( XCLOCK *t )
{	return( t->minute );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* SECOND PORTION OF XTIME */
int clock_second( XCLOCK *t )
{	return( t->second );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* DECIDE IF XTIME IS VALID */
int valid_clock( XCLOCK *tm )
{	return( ( tm->hour < 0 || tm->hour > 23
			|| tm->minute < 0 || tm->minute > 59
			|| tm->second < 0 || tm->second > 59 )
		? 0 : 1 );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* PRINT XDATE AS A STRING */
void clock_to_string( char *s, XCLOCK *tm, int fcode )
{	char	*fmt = "ERROR";
	switch( fcode )
		{case 0:
			fmt = "%d:%2.2d";
			break;
		case 1:
			fmt = "%2.2d:%2.2d:%2.2d";
			break;
		default:
			bug( "clock_to_string: bad format code = %d", fcode );
			break;
		}
	sprintf( s, fmt, clock_hour( tm ), clock_minute( tm ), clock_second( tm ));
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
		/* CONVERT STRING A#B#C TO XCLOCK, A,B,C INTS, # NON-INT */
void string_to_clock( char *s, XCLOCK *tm, char **msg, int get_secs )
{       long	j;
	long	i = 0;
	long	val[3];
	char	*tmp;
	char	buf[100];
	tmp = skip_space( s );
	while ( EOS != tmp[i] )
		{if ( isalpha( tmp[i] ) )
			{*msg = "letters A-Z are not allowed in clocks";
			return;
			}
		i++;
		}
	while ( --i > 0 && isspace( tmp[i] ) )
		{tmp[i] = EOS;
		}
	for ( i = 0; i < 3; i++ )
		{j = 0;
			while ( ! isdigit( *s ) && EOS != *s )
			{s++;
			}
		while ( isdigit( *s ) )
			{buf[j++] = *s++;
			}
		buf[j] = EOS;
		if ( 0 == j )
			{if ( 2 == i && NO == get_secs )  /* NO SECONDS */
				{val[2] = 0;
				break;
				}
			*msg = "missing value in clock";
			return;
			}
		val[i] = atoi( buf );
		}
	while ( EOS != *s )
		{if ( ! isspace( *s++ ) )
			{*msg = "extra item at end of clock";
			return;
			}
		}
	tm->hour   = val[0];
	tm->minute = val[1];
	tm->second = val[2];
	*msg = ( valid_clock( tm ) ) ? NULL : "clock is invalid";
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
		/* COMPARE CLOCKS: CA < CB => -1, CA = CB => 0, CA > CB = 1 */
int clock_cmp( XCLOCK *ca, XCLOCK *cb )
{	if ( ca->hour < cb->hour )	/* SEQUENTIALLY COMPARE BY UNITS */
		{return( -1 );
		}
	if ( ca->hour > cb->hour )
		{return( 1 );
		}
	if ( ca->minute < cb->minute )
		{return( -1 );
		}
	if ( ca->minute > cb->minute )
		{return( 1 );
		}
	if ( ca->second < cb->second )
		{return( -1 );
		}
	if ( ca->second > cb->second )
		{return( 1 );
		}
	return( 0 );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* COPY ONE XCLOCK XTIME INTO ANOTHER */
void clock_copy( XCLOCK *to, XCLOCK *from )
{	to->hour   = from->hour;
	to->minute = from->minute;
	to->second = from->second;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* SUBTRACT RAW XTIME FROM XCLOCK */
void clock_subtract_raw( XCLOCK *c, int hr, int mins, int sec, int *day )
{	if ( hr < 0 || mins < 0 || sec < 0 || ! valid_clock( c ) )
		{bug( "\nclock_subtract_raw, invalid input parameters" );
		exit( EXIT_SUCCESS );
		}
	if ( c->second < sec )
		{c->second += 60;
		mins++;
		}
	c->second -= sec;
	if ( c->minute < mins )
		{c->minute += 60;
		hr++;
		}
	c->minute -= mins;
	if ( c->hour < hr )
		{c->hour += 24 - hr;
		*day = 1;
		}
	else
		{c->hour -= hr;
		*day = 0;
		}
	if ( ! valid_clock( c ) )
		{bug( "\nclock_subtract_raw, invalid result" );
		exit( EXIT_SUCCESS );
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
	/* RETURN DIFFERENCE OF TWO TIMES, FIRST IS TAKEN AS BASE-POINT */
void clock_subtract( XCLOCK *cs, XCLOCK *sub, int *day_underflow )
{	clock_subtract_raw( cs, clock_hour( sub ), clock_minute( sub ),
		clock_second( sub ), day_underflow );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* ADD RAW PERIOD TO XCLOCK */
void clock_add_raw( XCLOCK *ct, int hr, int min, int sec, int *day )
{	if ( hr < 0 || min < 0 || sec < 0 || ! valid_clock( ct ) )
		{bug( "\nclock_add_raw, invalid input parameters" );
		exit( EXIT_SUCCESS );
		}
	*day = 0;
	ct->second += sec;
	while ( ct->second > 59 )
		{ct->second -= 60;
		min++;
		}
	ct->minute += min;
	while ( ct->minute > 59 )
		{ct->minute -= 60;
		hr++;
		}
	ct->hour += hr;
	while ( ct->hour > 23 )
		{ct->hour -= 24;
		(*day)++;
		}
	if ( ! valid_clock( ct ) )
		{bug( "\nclock_add_raw, invalid result" );
		exit( EXIT_SUCCESS );
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
	/* ADD TWO TIMES, FIRST IS TAKEN AS BASE-POINT, SECOND AS INCREMENT */
void clock_add( XCLOCK *cl, XCLOCK *add, int *day_overflow )
{	clock_add_raw( cl, clock_hour( add ), clock_minute( add ),
		clock_second( add ), day_overflow );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
							/* SET A XTIME */
void clock_set( XCLOCK *ctm, int hr, int min, int sec )
{	ctm->hour   = hr;
	ctm->minute = min;
	ctm->second = sec;
	if ( ! valid_clock( ctm ) )
		{bug( "illegal clock value %d:%d:%d", hr, min, sec );
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* SET A XTIME TO PRESENT */
void clock_set_now( XCLOCK *cl )
{	char	tbuf[52];
	time_t	today;
	time( &today );
	strftime( tbuf, 50, "%H %M %S", localtime( &today ) );
	sscanf( tbuf, "%d %d %d", &(cl->hour), &(cl->minute), &(cl->second) );
}
/*---------------------------------------------------------------------------*/
						/* HOUR PORTION OF XTIME */
int time_year( XTIME *t )
{	return( date_year( &(t->date) ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* MINUTE PORTION OF XTIME */
int time_month( XTIME *t )
{	return( date_month( &(t->date) ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* SECOND PORTION OF XTIME */
int time_day( XTIME *t )
{	return( date_day( &(t->date) ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* HOUR PORTION OF XTIME */
int time_hour( XTIME *t )
{	return( clock_hour( &(t->clock) ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* MINUTE PORTION OF XTIME */
int time_minute( XTIME *t )
{	return( clock_minute( &(t->clock) ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* SECOND PORTION OF XTIME */
int time_second( XTIME *t )
{	return( clock_second( &(t->clock) ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* DECIDE IF XTIME IS VALID */
int valid_time( XTIME *tm )
{	return( valid_date( &(tm->date) ) && valid_clock( &(tm->clock) ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* PRINT XTIME AS A STRING */
void time_to_string( char *s, XTIME *tm, int fcode_date, char *separator,
		int fcode_clock )
{	date_to_string( s, &(tm->date), fcode_date );
	strcat( s, ( NULL != separator ) ? separator : " " );
	clock_to_string( s + strlen(s), &(tm->clock), fcode_clock );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
		/* CONVERT day/month/year hour:minute:second TO XTIME */
void string_to_time( char *s, XTIME *t, char **msg, int allow_all_digit,
		int get_secs )
{
	int	i = 0;
	char	sbuf[20];
	t->contents = 0;
	*msg = NULL;
	while ( isspace( *s ) && *s != EOS ) 
		{s++;
		}
	while ( ! isspace( *s ) && *s != EOS && i < 12 )
		{
		sbuf[ i++ ] = *s++;
		}
	sbuf[i] = EOS;
	if ( i > 10 )
		{*msg =
"date part of string_to_date must be d/m/y without spaces, dmy all ints";
		return;
		}
	t->contents = XTIME_DATE_SET;
	string_to_date( sbuf, &(t->date), msg, allow_all_digit );
	if ( *msg != NULL )
		{return;
		}
	string_to_clock( s, &(t->clock), msg, get_secs );
	t->contents |= XTIME_CLOCK_SET;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
		/* COMPARE TIMES: TA < TB => -1, TA = TB => 0, TA > TB = 1 */
int time_cmp( XTIME *ta, XTIME *tb )
{       int	val;
	if ( ( val = date_cmp( &(ta->date), &(tb->date) ) ) != 0 )
		{return( val );
		}
	return( clock_cmp( &(ta->clock), &(tb->clock) ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* COPY ONE XTIME INTO ANOTHER */
void time_copy( XTIME *to, XTIME *from )
{	to->contents = from->contents;
	date_copy( &(to->date), &(from->date) );
	clock_copy( &(to->clock), &(from->clock) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
							/* SET A XTIME */
void time_set( XTIME *tm, long d, long m, long y, int hr, int min, int sec )
{
	tm->contents = XTIME_ALL_SET;
	date_set( &(tm->date), d, m, y );
	clock_set( &(tm->clock), hr, min, sec );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* SET A XTIME TO PRESENT */
void time_set_now( XTIME *t )
{
	t->contents = XTIME_ALL_SET;
	date_set_today( &(t->date) );
	clock_set_now(  &(t->clock) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* ADD RAW PERIOD TO XTIME */
void time_add_raw( XTIME *t, int day, int year, int hour, int min, int sec )
{	int	extra;
	clock_add_raw( &(t->clock), hour, min, sec, &extra );
	date_add_raw( &(t->date), day + extra, year );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* SUBTRACT RAW PERIOD FROM XTIME */
void time_subtract_raw( XTIME *t, int day, int year, int hour, int min, int sec )
{	int	extra;
	clock_subtract_raw( &(t->clock), hour, min, sec, &extra );
	date_subtract_raw( &(t->date), day + extra, year );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* CONVERT GMT TO SYSTEM-DEFINED LOCAL XTIME */
int time_gmt_to_local( XTIME *t )
{
	int	year;
	time_t	whole, tmp;
	static	struct	tm parts, *p;
	if ( t->contents != XTIME_ALL_SET )
		{return( 0 );	/* CAN ONLY CONVERT IF XDATE+XTIME PRESENT */
		}
	year = time_year( t );

		/* N.B. INGRES CONVERTS 1960-2037, C WORKS ONLY AFTER 1970 */

	if ( year < 1970 || year > 2037 )
		{return( 0 );
		}
	parts.tm_year = year - 1900;
	parts.tm_mon  = time_month( t ) - 1;
	parts.tm_mday = time_day( t );
	parts.tm_hour = time_hour( t );
	parts.tm_min  = time_minute( t );
	parts.tm_sec  = time_second( t );
	parts.tm_isdst = 0;		/* DAYLIGHT SAVING=NO FOR GMT */
	putenv( "TZ=GMT" );
	whole = mktime( &parts );
#ifndef __BORLANDC__
	unsetenv( "TZ" );
#endif
	if ( whole == 0 )
		{return( 0 );
		}
	p = localtime( &whole );
	if ( p->tm_hour != parts.tm_hour || p->tm_min != parts.tm_min )
		{
		time_set( t, p->tm_mday, p->tm_mon + 1, p->tm_year + 1900,
			p->tm_hour, p->tm_min, p->tm_sec );
		return( 1 );	/* INDICATE CONVERSION HAS CHANGED XTIME */
		}
	return( 0 );				/* NO CONVERSION NEEDED */
}
/*===========================================================================*/
#endif								/* XTIME.C */
