#ifndef	XSQL_H_INCLUDED
#define	XSQL_H_INCLUDED	1
/*===========================================================================*/
/*		ACCESS ROUTINES HEADER FOR EMBEDDED OPEN-SQL			*/
/*===========================================================================*/
#include "xdefs.h"
#include "xinclude.h"
#include "xalloc.h"
#include "xalloc.c"
#include "xmessage.c"
#include "xtext.c"
#include "xtime.c"
/*---------------------------------------------------------------------------*/
#define	DYNA_TYPE_REAL		'r'
#define DYNA_TYPE_INT		'i'
#define	DYNA_TYPE_STRING	's'
#define	DYNA_TYPE_FIXCHAR	'x'	/* ALSO LOGICAL KEYS */
#define DYNA_TYPE_DATE		'd'
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
typedef	struct
	{
	long	i;
	double	r;
	char	*s;
	TIME	t;	/* INGRES DATE STORES BOTH DATE AND TIME-OF-DAY */
	}
	XSQL_DYN_FIELD_VAR;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
typedef struct
	{
	int	ntype;			/* CODE DESCRIPTION OF C VARIABLE */
	char	*typ;			/* TEXT DESCRIPTION OF INGRES VAR */
	char	*name;			/* NAME OF FIELD		*/
	int	length;			/* LENGTH OF DATA FIELD		*/
	short	nullable;		/* CAN RAW SQL FIELD BE NULL	*/
	short	indicator;		/* 0 IF NOT-NULL,  -1 IF NULL	*/
	XSQL_DYN_FIELD_VAR	val;	/* VALUE OF FIELD		*/
	XSQL_DYN_FIELD_VAR	def;	/* DEFAULT VALUE FOR NULLS	*/
	}
	XSQL_DYN_FIELD;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

typedef	struct
	{
	int	nfield;			/* NUMBER OF FIELDS RETRIVED	*/
	char	*dyn_id;		/* NAME OF PREPARED QUERY	*/
	char	*sel_cmd;		/* ACTUAL QUERY STRING		*/
	char	cur_id[20];		/* UNIQUE ID FOR CURSOR		*/
	int	cur_ok;
	XSQL_DYN_FIELD	*field;		/* FIELD DESCRIPTIONS AND CONTENTS */
	IISQLDA	idesc;			/* INGRES STRUCTURE DESCRIPTION	*/
	}
	XSQL_DYN;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define	XQUERY	XSQL_DYN
/*---------------------------------------------------------------------------*/
				/* ERROR GENERATED WHILE DATABASE IS OPEN */
static void xsql_error( char *err_msg );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* REPORT ERROR FLAG SET BY SQL */
static void xsql_error_sql( void );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* CHECK ACCESS IS OKAY */
static void xsql_check_sqlcode( char *text, char *item );
/*---------------------------------------------------------------------------*/
					/* INITIALIZE DATABASE OPERATIONS */
int xsql_init_main( char *db_name, char *access_options );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* INITIALIZE DATABASE OPERATIONS */
int xsql_init( char *db_name );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* INITIALIZE DATABASE OPERATIONS */
int xsql_init_exclusive( char *db_name );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* TERMINATE DATABASE OPERATIONS */
void xsql_end( char *db_name );
/*---------------------------------------------------------------------------*/
					/* DECLARE AND OPEN A DYNAMIC CURSOR */
void xsql_dyn_cursor_init( XSQL_DYN *xd, char *access_mode );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* CLOSE A DYNAMIC CURSOR */
void xsql_dyn_cursor_end( XSQL_DYN *xd );
void xsql_query_close( XQUERY *xd );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* PREPARE QUERY AND TRANSLATION STRUCTURES */
XSQL_DYN *xsql_dyn_init( char *dyna_id, char *dyna_query_string );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* FREE STORAGE USED BY DYNAMIC STRUCTURE */
void xsql_dyn_end( XSQL_DYN *xd );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* FETCH DATA INTO CURSOR */
int xsql_dyn_fetch( XSQL_DYN *xd );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* RETURN NUMBER OF FIELDS SELECTED */
int xsql_dyn_nfields( XSQL_DYN *xd );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* RETURN NAME OF FIELD */
char *xsql_dyn_field_name( XSQL_DYN *xd, int indx );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* RETURN LENGTH OF FIELD */
int xsql_dyn_field_length( XSQL_DYN *xd, int indx );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* RETURN INTEGER CODE FOR FIELD TYPE */
int xsql_dyn_field_type( XSQL_DYN *xd, int indx );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* RETURN TEXT DESCRIPTION OF FIELD */
char *xsql_dyn_field_description( XSQL_DYN *xd, int indx );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
		/* RETURN POINTER TO APPROPRIATE VARIABLE HOLDING VALUE */
void *xsql_dyn_field_get( XSQL_DYN *xd, int indx );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* FILL STRING WITH VALUE OF ANY TYPE */
void xsql_get_string( XSQL_DYN *xd, int indx, char *out );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
		/* INSERT VALUE INTO SQL STRUCTURE PRIOR TO MODIFYING DB  */
void *xsql_dyn_field_put( XSQL_DYN *xd, int indx, void *val );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
	/* CONVERT STRING INTO APPROPRIATE TYPE AND INSERT INTO SQL STRUCTURE */
void xsql_dyn_field_put_string( XSQL_DYN *xd, int indx, char *in );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
		/* CONVERT OLD LONG NAMES TO MORE INTUITIVE SHORT NAMES */
#define	xsql_dyn_init_fetch		xsql_query_open
#define	xsql_dyn_cursor_fetch		xsql_query_fetch
#define	xsql_dyn_end_fetch		xsql_query_close
#define xsql_dyn_field_get_string       xsql_get_string
#define xsql_dyn_field_get_date         xsql_get_date
#define xsql_dyn_field_get_time         xsql_get_time
#define xsql_dyn_field_get_int          xsql_get_int
#define xsql_dyn_field_get_real         xsql_get_real
#define	xsql_execute_immediate		xsql_exec
#define	xsql_execute_procedure		xsql_exproc
/*===========================================================================*/
#endif								/* XSQL.H */
