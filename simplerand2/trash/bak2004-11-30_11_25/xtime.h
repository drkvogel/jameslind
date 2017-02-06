#ifndef	XTIME_H_INCLUDED
#define	XTIME_H_INCLUDED	1
/*===========================================================================*/
/*		HEADER FOR XTIME HANDLING UTILITIES			*/
/*	DATES ARE TIMES MEASURED ONLY IN YEARS+MONTHS+DAYS		*/
/*===========================================================================*/
#include	<time.h>
/*---------------------------------------------------------------------------*/
#define	XTIME_DATE_SET		4	/* BIT-FIELD-FLAGS FOR XTIME.CONTENTS */
#define	XTIME_CLOCK_SET		32
#define	XTIME_ALL_SET		(XTIME_DATE_SET|XTIME_CLOCK_SET)
/*---------------------------------------------------------------------------*/
typedef	struct					/* WHOLE DAYS		*/
	{
	long	day;
	long	month;
	long	year;
	}
	XDATE;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
typedef	struct					/* FRACTIONS OF DAYS	*/
	{
	int	hour;
	int	minute;
	int	second;
	}
	XCLOCK;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
typedef	struct
	{
	int	contents;	/* INDICATE IF XDATE AND/OR XCLOCK ARE SET */
	XDATE	date;
	XCLOCK	clock;
	}
	XTIME;
/*---------------------------------------------------------------------------*/
				/* DEFINE DEFAULT CENTURY FOR TWO-DIGIT YEARS */
void default_century( long cen_lo, long cen_hi, long cen_cutoff );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* ADD DEFAULT CENTURY TO XDATE IF REQUIRED */
void default_century_apply( XDATE *dat );
/*---------------------------------------------------------------------------*/
						/* DAY PORTION OF XDATE */
long date_day( XDATE *d );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* MONTH PORTION OF XDATE */
long date_month( XDATE *d );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* ACTUAL NAME OF MONTH */
char *date_month_name( XDATE *d );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* YEAR PORTION OF XDATE */
long date_year( XDATE *d );
/*---------------------------------------------------------------------------*/
						/* IS YEAR A LEAP YEAR ? */
long leap_year_numeric( long nyear );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* IS XDATE A LEAP YEAR ? */
long leap_year( XDATE *dat );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* SET GLOBAL POINTERS FOR CURRENT YEAR */
void days_year_set( XDATE *dat );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* DECIDE IF XDATE IS VALID */
long valid_date( XDATE *dat );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* CONVERT MONTH+DAYS TO DAYS ONLY */
long date_daymon_to_daysonly( XDATE *d );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* CONVERT DAYS ONLY TO MONTH+DAYS */
long date_daysonly_to_daymon( XDATE *d );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* PRINT XDATE AS A STRING */
void date_to_string( char *s, XDATE *dt, int fcode );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
		/* CONVERT STRING A#B#C TO XDATE, A,B,C INTS, # NON-INT */
void string_to_date( char *s, XDATE *dt, char **msg, int allow_all_digit );
/*---------------------------------------------------------------------------*/
		/* COMPARE DATES: DA < DB => -1, DA = DB => 0, DA > DB = 1 */
long date_cmp( XDATE *da, XDATE *db );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* COPY ONE XDATE INTO ANOTHER */
void date_copy( XDATE *to, XDATE *from );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* SUBTRACT RAW XTIME FROM XDATE */
void date_subtract_raw( XDATE *dt, long dy, long yr );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* ADD RAW XTIME TO XDATE */
void date_add_raw( XDATE *dt, long dy, long yr );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* GO BACK AN INTERVAL BEFORE BASELINE */
void date_before( XDATE *base, XDATE *out, int nd, int ny );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* GO FORWARD AN INTERVAL AFTER BASELINE */
void date_after( XDATE *base, XDATE *out, int nd, int ny );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
							/* SET A XDATE */
void date_set( XDATE *dat, long d, long m, long y );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* SET A XDATE TO TODAY */
void date_set_today( XDATE *dat );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* SET A XDATE TO TOMORROW */
void date_set_tomorrow( XDATE *dtom );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* SET A XDATE TO YESTERDAY */
void date_set_yesterday( XDATE *dyes );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* FIND NUMBER OF DAYS BETWEEN TWO DATES */
int date_days_diff( XDATE *d_to, XDATE *d_from );
/*---------------------------------------------------------------------------*/
						/* HOUR PORTION OF XTIME */
int clock_hour( XCLOCK *t );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* MINUTE PORTION OF XTIME */
int clock_minute( XCLOCK *t );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* SECOND PORTION OF XTIME */
int clock_second( XCLOCK *t );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* DECIDE IF XTIME IS VALID */
int valid_clock( XCLOCK *tm );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* PRINT XDATE AS A STRING */
void clock_to_string( char *s, XCLOCK *tm, int fcode );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
		/* CONVERT STRING A#B#C TO XCLOCK, A,B,C INTS, # NON-INT */
void string_to_clock( char *s, XCLOCK *tm, char **msg, int get_secs );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
		/* COMPARE CLOCKS: CA < CB => -1, CA = CB => 0, CA > CB = 1 */
int clock_cmp( XCLOCK *ca, XCLOCK *cb );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* COPY ONE XCLOCK XTIME INTO ANOTHER */
void clock_copy( XCLOCK *to, XCLOCK *from );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* SUBTRACT RAW XTIME FROM XCLOCK */
void clock_subtract_raw( XCLOCK *c, int hr, int min, int sec, int *day );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
	/* RETURN DIFFERENCE OF TWO TIMES, FIRST IS TAKEN AS BASE-POINT */
void clock_subtract( XCLOCK *cs, XCLOCK *sub, int *day_underflow );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* ADD RAW PERIOD TO XCLOCK */
void clock_add_raw( XCLOCK *ct, int hr, int min, int sec, int *day );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
	/* ADD TWO TIMES, FIRST IS TAKEN AS BASE-POINT, SECOND AS INCREMENT */
void clock_add( XCLOCK *cl, XCLOCK *add, int *day_overflow );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
							/* SET A XTIME */
void clock_set( XCLOCK *ctm, int hr, int min, int sec );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* SET A XTIME TO PRESENT */
void clock_set_now( XCLOCK *cl );
/*---------------------------------------------------------------------------*/
						/* HOUR PORTION OF XTIME */
int time_year( XTIME *t );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* MINUTE PORTION OF XTIME */
int time_month( XTIME *t );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* SECOND PORTION OF XTIME */
int time_day( XTIME *t );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* HOUR PORTION OF XTIME */
int time_hour( XTIME *t );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* MINUTE PORTION OF XTIME */
int time_minute( XTIME *t );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* SECOND PORTION OF XTIME */
int time_second( XTIME *t );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* DECIDE IF XTIME IS VALID */
int valid_time( XTIME *tm );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* PRINT XTIME AS A STRING */
void time_to_string( char *s, XTIME *tm, int fcode_date, char *separator,
		int fcode_clock );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
		/* COMPARE TIMES: TA < TB => -1, TA = TB => 0, TA > TB = 1 */
int time_cmp( XTIME *ta, XTIME *tb );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* COPY ONE XTIME INTO ANOTHER */
void time_copy( XTIME *to, XTIME *from );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
							/* SET A XTIME */
void time_set( XTIME *tm, long d, long m, long y, int hr, int min, int sec );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* SET A XTIME TO PRESENT */
void time_set_now( XTIME *t );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* ADD RAW PERIOD TO XTIME */
void time_add_raw( XTIME *t, int day, int year, int hour, int min, int sec );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* SUBTRACT RAW PERIOD FROM XTIME */
void time_subtract_raw( XTIME *t, int day, int year, int hour, int min,int sec);
/*===========================================================================*/
#ifndef __BORLANDC__	/* DATE,CLOCK,TIME CONFLICT WITH BC INTERNALS */
#define	DATE	XDATE
#define	CLOCK	XCLOCK
#define	TIME	XTIME
#endif
/*===========================================================================*/
#endif								/* XTIME.H */
