#ifndef	XPACK_DB_C_INCLUDED
#define	XPACK_DB_C_INCLUDED	1
/*===========================================================================*/
/*		PACK MANAGEMENT SYSTEM - DATABASE OPERATIONS		*/
/*===========================================================================*/
#include "xdefs.h"
#include "xinclude.h"
#include "xalloc.h"
#include "xalloc.c"
#include "xmessage.c"
#include "xtext.c"
#include "xcurses.c"
#include "xsingkey.c"
/*---------------------------------------------------------------------------*/
#include "xsql.c"
#include "zpkcode.c"
/*---------------------------------------------------------------------------*/
/*	N.B.  ASSUME ALL CODES (KEYS) ARE STORED IN UPPER CASE		*/
#define	XPACK_STATUS_OK		1
#define	XPACK_STATUS_ANY	(-2001)
#define	XPACK_MAX_CODE_LEN	8
#define XPACK_INVALID_CODE					\
	"invalid code, must be characters A-Z,0-9 only, of length 1-8"
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define	SEL_COUNTRY "SELECT gcode,gname,status,udate FROM country "
#define SEL_CENTRE  "SELECT gcode,ccode,cname,status,restock,udate FROM centre"
#define SEL_SET	    "SELECT ccode,scode,status,upri,udate,adate FROM sets "
#define	SEL_UNIT    "SELECT scode,ucode,status,content,udate FROM unit "
#define	SEL_STAFF   "SELECT ccode,tcode,tname,status,udate,type,aux FROM staff "
/*---------------------------------------------------------------------------*/
int	set_is_active( SET *sa );
int	unit_is_allocable( UNIT *ua );
void	xpack_set_select( long *nst, SET **st, char *cent_code, int order );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
static	int	xpack_session_id = -1;
static	char	xpack_db_dbname[100] = "";
static	XSQL_DYN *d_cntry, *d_cen, *d_stf, *d_set, *d_unit;
static	char	sql_search_string[100+TAGLEN];		/* SEARCH STRING */
static	char	sql_command[500];			/* COMMAND BUFFER */
/*---------------------------------------------------------------------------*/
						/* OPEN A TRANSACTION */
void xpack_open( void )
{
	if ( EOS == xpack_db_dbname[0] )
		{bug( "xpack_target has not been called correctly" );
		exit( EXIT_FAILURE );
		}
	xsql_persistence( 10, YES );
	xsql_init( xpack_db_dbname );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* TERMINATE A TRANSATION */
void xpack_close( void )
{
	xsql_end( xpack_db_dbname );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* EXECUTE A SINGLE COMMAND */
void xpack_exec( char *command )
{	xpack_open();
	xsql_execute_immediate( command );
	xpack_close();
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* RETURN USERNAME MAKING CONNECTION */
char *xpack_username( void )
{
	static	char	uname[20] = "?";
	char	tmp[22];
	if ( '?' == uname[0] )	/* DETERMINE USERNAME IF NOT FOUND ALREADY */
		{
		strncpy( tmp, getenv( "USER" ), 15 );
		uname[15] = EOS;
		xsql_squote( uname, tmp );
		}
	return( uname );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
	/* SET TARGET FOR DATABASE TRANSACTIONS - OPEN/CLOSE AS NEEDED */
void xpack_target( char *dbname )
{
	int	nullret_defined = xsql_dyn_null_handler_int;
	XSQL_DYN	*xd_sesh = NULL;
	strcpy( xpack_db_dbname, dbname );
	aside( "xpack_target: initialising \"%s\"", xpack_db_dbname );
	if ( YES != nullret_defined )	/* TEMPORARILY ALLOW NULLABLE FIELDS */
		{xsql_nullreturn_int( -1 );
		}
	xpack_open();
	xd_sesh = xsql_dyn_init_fetch( "sesh",
		"SELECT MAX(sesh_id) AS maxsid FROM db_sesh", "r" );
	if ( 0 == xsql_dyn_cursor_fetch( xd_sesh ) )
		{xpack_session_id = xsql_dyn_field_get_int( xd_sesh, 0 );
		}
	else
		{xpack_session_id = 0;
		}
	xsql_dyn_end_fetch( xd_sesh );
	xpack_close();
	if ( YES != nullret_defined )	/* RESTORE PREVIOUS BEHAVIOUR */
		{xsql_dyn_null_handler_int = nullret_defined;
		}
	xpack_session_id++;
	sprintf( sql_command,
		"INSERT INTO DB_SESH (sesh_id,sdate,uname)"
		" VALUES (%d,'NOW','%s')",
		xpack_session_id, xpack_username() );
	xpack_exec( sql_command );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* ADD ENTRY TO DATABASE LOG */
void xpack_loggit( int action_code, char *notes )
{
	int	log_id = -1;
	int	has_note = -1;
	has_note = ( EOS != notes[0] ) ? 1 : 0;
	sprintf( sql_command,
		"INSERT INTO db_log (log_id,sesh_id,ldate,action,notes)"
		" VALUES (%d,%d,'NOW',%d,%d)",
		log_id, xpack_session_id, action_code, has_note );
}
/*---------------------------------------------------------------------------*/
			/* DETERMINE IF STRING IS ALLOWABLE AS A CODE */
int xpack_valid_code( char *cd )
{	int	i, n;
	int	result = YES;
	n = strlen( cd );
	if ( n > XPACK_MAX_CODE_LEN )
		{aside( "invalid code, length=%d, excedes max of %d",
			XPACK_MAX_CODE_LEN );
		return( NO );
		}
	for ( i = 0; i < n; i++ )
		{if ( ! isalnum( cd[i] ) )
			{result = NO;
			}
		if ( toupper( cd[i] ) != cd[i] )
			{result = NO;
			}
		}
	if ( YES != result )
		{aside( "invalid code, \"%s\"", cd );
		}
	return( result );		/* CODE IS ALLOWED */
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

		/* COPY STRING TO UPPER CASE TO USE AS KEY FOR SEARCH */
void xpack_sql_search_string( char *out, char *in )
{	strcpy( out, in );
	string_toupper( out );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* COUNT INSTANCES OF OBJECT CODE */
int xpack_count( char *tab_name, char *code_name, char *value )
{	int	count = 0;
	static	char	sel[200];
	XSQL_DYN	*d_uc;
	sprintf( sel, "SELECT * FROM %s WHERE %s='%s'",
		tab_name, code_name, value );
	xpack_open();
	d_uc = xsql_dyn_init_fetch( "ucheck", sel, "r" );
	while ( 0 == xsql_dyn_cursor_fetch( d_uc ) )
		{count++;
		}
	xsql_dyn_end_fetch( d_uc );
	xpack_close();
	return( count );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* VERIFY THAT OBJECT EXISTS AND IS UNIQUE */
char *xpack_unique( char *tab_name, char *code_name, char *value )
{	int	cnt;
	static	char	msg[200];
	cnt = xpack_count( tab_name, code_name, value );
	if ( 1 == cnt )
		{return( NULL );			/* SUCCESS */
		}
	sprintf( msg, "object '%s' found %d times in table %s",
		value, cnt, tab_name );
	return( msg );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* VERIFY THAT OBJECT DOES NOT EXIST */
char *xpack_unused( char *tab_name, char *code_name, char *value )
{	int	cnt;
	static	char	msg[200];
	cnt = xpack_count( tab_name, code_name, value );
	if ( 0 == cnt )
		{return( NULL );			/* SUCCESS */
		}
	sprintf( msg, "object '%s' found %d times in table %s",
		value, cnt, tab_name );
	return( msg );
}
/*---------------------------------------------------------------------------*/
				/* UNIT: SET,CODE,STATUS,CONTENTS,UPDATE */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* CHANGE STATUS OF UNIT */
char *xpack_unit_status( UNIT *us )
{	char	*msg = NULL;
	char	new_date[20];
	aside( "changing status of (set \"%s\") unit \"%s\" to %d", us->root,
		us->code, us->status );
	date_set_today( &(us->udate) );
	date_to_string( new_date, &(us->udate), 0 );
	sprintf( sql_command,
"UPDATE unit SET status=%d, udate='%s' WHERE scode='%s' AND ucode='%s'",
		us->status, new_date, us->root, us->code );
	xpack_exec( sql_command );
	return( msg );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* ASSIGN AND VALIDATE UNIT STATUS CODE */
int xpack_unit_validate( UNIT *un )
{	if ( NULL == ( un->pcd = pack_code( un->status, unit_status ) )
			|| un->content < 0 || un->content >= pack_ntreatments )
		{aside( "ERROR (set %s) unit %s, content = %ld", un->root,
			un->code, un->content );
		return( NO );
		}
	return( YES );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* COPY SQL STRUCTURE INTO PROGRAM STRUCTURE */
int xpack_unit_convert( UNIT *ut )
{
	xsql_dyn_field_get_string( d_unit, 0, ut->root );
	xsql_dyn_field_get_string( d_unit, 1, ut->code );
	ut->status = xsql_dyn_field_get_int( d_unit, 2 );
	ut->content = xsql_dyn_field_get_int( d_unit, 3 );
	xsql_dyn_field_get_date( d_unit, 4, &(ut->udate) );
	return( xpack_unit_validate( ut ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* LOAD ALL UNITS WITHIN ONE SET FROM FILE */
void xpack_unit_select( long *nun, UNIT **un, char *set_code )
{	
	*nun = 0;
	*un = (UNIT *) xmalloc( ( pack_nunits_per_set + 2 ) * sizeof(UNIT) );
	xpack_sql_search_string( sql_search_string, set_code );
	sprintf( sql_command,
		SEL_UNIT " WHERE scode='%s'",
		sql_search_string );
	xpack_open();
	d_unit = xsql_dyn_init_fetch( "unitfet", sql_command, "r" );
	while ( 0 == xsql_dyn_cursor_fetch( d_unit ) )
		{
		if ( YES == xpack_unit_convert( *un + (*nun) ) )
			{(*nun)++;
			if ( 0 == ( *nun % pack_nunits_per_set ) )
				{*un = (UNIT *) xrealloc( *un, ( *nun + 2
					+ pack_nunits_per_set ) * sizeof(UNIT));
				}
			}
		}
	xsql_dyn_end_fetch( d_unit );
	xpack_close();
	if ( *nun > pack_nunits_per_set )
		{aside( "WARNING set %s has %ld units", set_code, *nun );
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* LOAD A SINGLE UNIT FROM FILE */
int xpack_unit_single( UNIT *un, char *s_code, char *u_code )
{       long	nu;
	int	i = -1, got = -1;
	UNIT	*unt;
	xpack_unit_select( &nu, &unt, s_code );
	while ( ++i < nu )
		{if ( 0 == strcmp_nocase( u_code, unt[i].code ) )
			{got = i;
			break;
			}
		}
	if ( got > -1 )
		{*un = unt[got];	/* COPY CONTENTS BEFORE FREEING */
		}
	xfree( unt );
	return( got );
}
/*---------------------------------------------------------------------------*/
			/* SET: CENTRE,CODE,STATUS,UPDATE,ARRIVAL-DATE */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* CHANGE STATUS OF SET */
char *xpack_set_status( SET *ss )
{	char	*msg = NULL;
	char	new_udate[20], new_adate[20];	
	aside( "changing status of set \"%s\" to %d %s", ss->code, ss->status,
		( SETSTATUS_OK != ss->status ) ? " " : "(=ok)" );
	date_set_today( &(ss->udate) );
	date_to_string( new_udate, &(ss->udate), 0 );
	if ( SETSTATUS_OK != ss->status )
		{
		sprintf( sql_command,
		"UPDATE sets SET status=%d, udate='%s' WHERE scode='%s'",
			ss->status, new_udate, ss->code );
		}
	else			/* CHANGE ARRIVAL DATE TO TODAY */
		{
		date_set_today( &(ss->adate) );
		date_to_string( new_adate, &(ss->adate), 0 );
		sprintf( sql_command,
	"UPDATE sets SET status=%d, udate='%s', adate='%s' WHERE scode='%s'",
			ss->status, new_udate, new_adate, ss->code );
		}
	xpack_exec( sql_command );
	return( msg );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* CHANGE USAGE-PRIORITY OF SET */
char *xpack_set_upriority( SET *ss )
{	char	*msg = NULL;
	char	new_udate[20], new_adate[20];
	aside( "changing upriority of set \"%s\" to %d", ss->code,
		ss->upriority );
	date_set_today( &(ss->udate) );
	date_to_string( new_udate, &(ss->udate), 0 );
	sprintf( sql_command,
		"UPDATE sets SET upri=%d, udate='%s' WHERE scode='%s'",
		ss->upriority, new_udate, ss->code );
	xpack_exec( sql_command );
	return( msg );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* CHANGE USAGE-PRIORITY OF SET */
char *xpack_set_upriority_extreme( SET *ss, int use_first )
{	int	i, up;
	int	maxp = 0, minp = 0;
	long	ns;
	SET	*slist;
	if ( 0 == strcmp( ss->root, "0" ) )
		{bug( "xpack_set_upriority_extreme, unassigned set" );
		exit( EXIT_SUCCESS );
		}
	xpack_set_select( &ns, &slist, ss->root, 0 );
	for ( i = 0; i < ns; i++ )
		{up = slist[i].upriority;
		if ( up < minp )
			{minp = up;
			}
		else if ( up > maxp )
			{maxp = up;
			}
		}
	if ( NULL != slist )
		{xfree( slist );
		}
	ss->upriority = ( NO == use_first ) ? ( maxp + 1 ) : ( minp - 1 );
	return( xpack_set_upriority( ss ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* ASSIGN SET TO A CENTRE */
char *xpack_set_assign( SET *st, char *centre_cod, int status )
{	char	*msg = NULL;
	char	new_date[20];	
	if ( EOS == st->code[0] )
		{return( "empty code" );
		}
	if ( EOS == centre_cod[0] )
		{bug( "empty centre" );
		}
	aside( "assigning set \"%s\" to centre \"%s\"", st->code,
		centre_cod );
	strcpy( st->root, centre_cod );
	date_set_today( &(st->udate) );
	date_to_string( new_date, &(st->udate), 0 );
	sprintf( sql_command,
	"UPDATE sets SET ccode='%s', status=%d, udate='%s' WHERE scode='%s'",
		centre_cod, status, new_date, st->code );
	xpack_exec( sql_command );
	return( msg );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* ADD NEW SET TO CENTRE */
char *xpack_set_add( SET *se, char *centre_code )
{
	int	new_status;
	if ( 0 != strcmp( se->root, "0" ) )
		{bug( "set %s has invalid location %s for add", se->root,
			se->code );
		}
	new_status = ( 0 != strcmp( "0", centre_code ) ) ? 10 : 0;
	return( xpack_set_assign( se, centre_code, new_status ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* CHANGE LOCATION OF ASSIGNED SET */
char *xpack_set_relocate( SET *se, char *centre_code )
{	if ( 0 == strcmp( se->root, "0" ) )
		{bug( "set %s not previously assigned before relocation",
			se->code );
		}
	return( xpack_set_assign( se, centre_code, se->status ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* ASSIGN AND VALIDATE SET STATUS CODE */
int xpack_set_validate( SET *st )
{	st->nunit = 0;
	st->unit = NULL;
	if ( NULL == ( st->pcd = pack_code( st->status, set_status ) ) )
		{aside( "ERROR set %s", st->code );
		return( NO );
		}
	return( YES );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* COPY SQL STRUCTURE INTO PROGRAM STRUCTURE */
int xpack_set_convert( SET *se )
{
	xsql_dyn_field_get_string( d_set, 0, se->root );
	xsql_dyn_field_get_string( d_set, 1, se->code );
	se->status = xsql_dyn_field_get_int( d_set, 2 );
	se->upriority = xsql_dyn_field_get_int( d_set, 3 );
	xsql_dyn_field_get_date( d_set, 4, &(se->udate) );
	xsql_dyn_field_get_date( d_set, 5, &(se->adate) );
	return( xpack_set_validate( se ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* FIND ENTRY FOR A SPECIFIC SET */
int xpack_set_search( char *set_code, SET *st )
{	int	count = 0;
	xpack_sql_search_string( sql_search_string, set_code );
	sprintf( sql_command,
		SEL_SET
		" WHERE scode='%s'",
		sql_search_string );
	xpack_open();
	d_set = xsql_dyn_init_fetch( "setfet", sql_command, "r" ); 
	while ( 0 == xsql_dyn_cursor_fetch( d_set ) )
		{
		if ( YES == xpack_set_convert( st ) )
			{count++;
			}
		}
	xsql_dyn_end_fetch( d_set );
	xpack_close();
	return( count );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* LOAD ALL SETS WITHIN ONE CENTRE FROM FILE */
void xpack_set_select( long *nst, SET **st, char *cent_code, int order )
{
	char	*oclause;
	*nst = 0;
	*st = (SET *) xmalloc( 12 * sizeof(SET) );
	xpack_sql_search_string( sql_search_string, cent_code );
	switch( order )
		{case 0:
			oclause = "ORDER BY upri";
			break;
		case 1:
			oclause = "ORDER BY scode";
			break;
		default:
			oclause = "";
			break;
		}
	sprintf( sql_command,
		SEL_SET " WHERE ccode='%s'  %s",
		sql_search_string, oclause );
	xpack_open();
	d_set = xsql_dyn_init_fetch( "setfet", sql_command, "r" );
	while ( 0 == xsql_dyn_cursor_fetch( d_set ) )
		{
		if ( YES == xpack_set_convert( *st + (*nst) ) )
			{(*nst)++;
			if ( *nst > 0 && 0 == *nst % 10 )
				{*st = (SET *) xrealloc( *st, ( *nst + 12 )
					* sizeof(SET) );
				}
			}
		}
	xsql_dyn_end_fetch( d_set );
	xpack_close();
}
/*---------------------------------------------------------------------------*/
		/* STAFF: CCODE,TCODE,TNAME,STATUS,UDATE,TYPE,AUX */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* CHANGE STATUS OF STAFF */
char *xpack_staff_status( STAFF *ss )
{	char	*msg = NULL;
	char	new_date[20];	
	aside( "changing status of staff \"%s\" to %d", ss->code,
		ss->status );
	if ( NULL != ( msg = xpack_unique( "staff", "tcode", ss->code ) ) )
		{return( msg );
		}
	date_set_today( &(ss->udate) );
	date_to_string( new_date, &(ss->udate), 0 );
	sprintf( sql_command, "UPDATE staff "
		" SET status=%d, UDATE='%s' "
		" WHERE tcode='%s'",
		ss->status, new_date, ss->code );
	xpack_exec( sql_command );
	return( msg );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* ADD STAFF TO A CENTRE */
char *xpack_staff_add( STAFF *st, char *centre_cod )
{	char	*msg = NULL;
	char	new_date[20], tname[100];
	if ( EOS == st->code[0] )
		{return( "empty code" );
		}
	if ( EOS == centre_cod[0] )
		{bug( "empty centre" );
		}
	aside( "creating staff \"%s\" at centre \"%s\"", st->code,
		centre_cod );
	if ( NULL != ( msg = xpack_unused( "staff", "tcode", st->code ) ) )
		{return( msg );
		}
	if ( YES != xpack_valid_code( st->code ) )
		{return( XPACK_INVALID_CODE );
		}
	date_set_today( &(st->udate) );
	date_to_string( new_date, &(st->udate), 0 );
	strcpy( st->root, centre_cod );
	st->status = 1;					/* INITIALLY ACTIVE */
	xsql_squote( tname, st->name ); 
	sprintf( sql_command, "INSERT INTO staff "
		"( ccode, tcode, tname, status, udate, type, aux ) "
		"VALUES "
		"( '%s', '%s', '%s', %d, '%s', %d, '%s' )",
		st->root, st->code, tname, st->status, new_date, st->type,
		st->aux );
	xpack_exec( sql_command );
	return( msg );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* ASSIGN AND VALIDATE SET STATUS CODE */
int xpack_staff_validate( STAFF *st )
{
	if ( NULL == ( st->pcd = pack_code( st->status, staff_status ) ) )
		{aside( "ERROR staff %s", st->code );
		return( NO );
		}
	return( YES );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* COPY SQL STRUCTURE INTO PROGRAM STRUCTURE */
int xpack_staff_convert( STAFF *sf, int status )
{
	sf->status = xsql_dyn_field_get_int( d_stf, 3 );
	if ( status != XPACK_STATUS_ANY && status != sf->status )
		{return( NO );
		}
	xsql_dyn_field_get_string( d_stf, 0, sf->root );
	xsql_dyn_field_get_string( d_stf, 1, sf->code );
	xsql_dyn_field_get_string( d_stf, 2, sf->name );
	xsql_dyn_field_get_date( d_stf, 4, &(sf->udate) );
	sf->type   = xsql_dyn_field_get_int( d_stf, 5 );
	xsql_dyn_field_get_string( d_stf, 6, sf->aux );
	return( xpack_staff_validate( sf ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* FIND ENTRY FOR A SPECIFIC STAFF */
int xpack_staff_search( char *s_txt, STAFF *st, int nfld )
{
	int	count = 0;
	if ( nfld != 1 && nfld != 2 )
		{bug( "xpack_staff_search %ld", nfld );
		}
	xpack_sql_search_string( sql_search_string, s_txt );
	sprintf( sql_command,
		SEL_STAFF
		" WHERE LOWERCASE(%s)=LOWERCASE('%s')",
		( 1 == nfld ) ? "tcode" : "tname", sql_search_string );
	xpack_open();
	d_stf = xsql_dyn_init_fetch( "stafet", sql_command, "r" ); 
	while ( 0 == xsql_dyn_cursor_fetch( d_stf ) )
		{
		if ( YES == xpack_staff_convert( st, XPACK_STATUS_ANY ) )
			{count++;
			}
		}
	xsql_dyn_end_fetch( d_stf );
	xpack_close();
	return( count );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* LOAD ALL STAFF WITHIN ONE CENTRE FROM FILE */
void xpack_staff_select_status( long *nst, STAFF **st, char *cent_code,
	int status )
{
	*nst = 0;
	*st = (STAFF *) xmalloc( 12 * sizeof(STAFF) );
	xpack_sql_search_string( sql_search_string, cent_code );
	sprintf( sql_command,
		SEL_STAFF " WHERE ccode='%s'",
		sql_search_string );
	xpack_open();
	d_stf = xsql_dyn_init_fetch( "stafet", sql_command, "r" );
	while ( 0 == xsql_dyn_cursor_fetch( d_stf ) )
		{
		if ( YES == xpack_staff_convert( *st + (*nst), status ) )
			{(*nst)++;
			if ( *nst > 0 && 0 == *nst % 10 )
				{*st = (STAFF *) xrealloc( *st, ( *nst + 12 )
					* sizeof(STAFF) );
				}
			}
		}
	xsql_dyn_end_fetch( d_stf );
	xpack_close();
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* LOAD ALL STAFF WITHIN ONE CENTRE */
void xpack_staff_select( long *nst, STAFF **st, char *cent_code )
{
	xpack_staff_select_status( nst, st, cent_code, XPACK_STATUS_ANY );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* LOAD ALL ACTIVE STAFF WITHIN ONE CENTRE */
void xpack_staff_select_active( long *nst, STAFF **st, char *cent_code )
{
	xpack_staff_select_status( nst, st, cent_code, XPACK_STATUS_OK );
}
/*---------------------------------------------------------------------------*/
				/* CENTRE: COUNTRY,CODE,NAME,STATUS,UPDATE */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* CHANGE STATUS OF CENTRE */
char *xpack_centre_status( CENTRE *cs )
{	char	*msg = NULL;
	char	new_date[20];	
	date_set_today( &(cs->udate) );
	date_to_string( new_date, &(cs->udate), 0 );
	sprintf( sql_command,
		"UPDATE centre SET status=%d, udate='%s' WHERE ccode='%s'",
		cs->status, new_date, cs->code );
	xpack_exec( sql_command );
	return( msg );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* CHANGE RESTOCK THRESHOLD AT CENTRE */
char *xpack_centre_restock( CENTRE *cs )
{	char	*msg = NULL;
	char	new_date[20];	
	date_set_today( &(cs->udate) );
	date_to_string( new_date, &(cs->udate), 0 );
	sprintf( sql_command,
		"UPDATE centre SET restock=%d, udate ='%s' WHERE ccode='%s'",
		cs->restock, new_date, cs->code );
	xpack_exec( sql_command );
	return( msg );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* ADD NEW CENTRE TO FILE */
char *xpack_centre_add( CENTRE *cn )
{	char	*msg = NULL;
	DATE	td;
	char	add_date[20];
	char	name[200];
	if ( EOS == cn->code[0] || EOS == cn->name[0] )
		{return( "empty code or name" );
		}
	if ( YES != xpack_valid_code( cn->code ) )
		{return( XPACK_INVALID_CODE );
		}
	if ( EOS == cn->root )
		{bug( "xpack_centre_add: empty cn->root" );
		}
	date_set_today( &td );
	date_to_string( add_date, &td, 0 );
	xsql_squote( name, cn->name );
	sprintf( sql_command,
		"INSERT INTO centre "
		"( gcode, ccode, cname, status, restock, udate ) "
		"VALUES "
		"( '%s', '%s', '%s', 0, %d, '%s' )",
		cn->root, cn->code, name, cn->restock, add_date );
	xpack_exec( sql_command );
	xsql_check_sqlcode( "failed to add centre \"%s\"", cn->code );
	return( msg );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* COUNT ALLOCABLE UNITS AT CENTRE */
void xpack_centre_unit_count( CENTRE *ce, int nmax, long *count )
{	int	i;
	UNIT	un;
	for ( i = 0; i < nmax; i++ )
		{count[i] = 0;
		}
	xpack_sql_search_string( sql_search_string, ce->code );
	sprintf( sql_command,
		SEL_UNIT " WHERE unit.status = 1 "
			" AND unit.scode "
			" in "
			" ( SELECT scode "
			" FROM sets "
			" WHERE ( sets.status = 10 OR sets.status = 11 ) "
			" AND "
			" ccode='%s' )",
		sql_search_string );
	xpack_open();
	d_unit = xsql_dyn_init_fetch( "unicount", sql_command, "r" );
	while ( 0 == xsql_dyn_cursor_fetch( d_unit ) )
		{
		if (YES == xpack_unit_convert( &un ) )
			{count[ un.content ]++;
			}
		}
	xsql_dyn_end_fetch( d_unit );
	xpack_close();
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* ASSIGN AND CHECK CENTRE STATUS CODE */
int xpack_centre_validate( CENTRE *cen )
{	if ( NULL == ( cen->pcd = pack_code( cen->status, centre_status ) ) )
		{aside( "ERROR centre %s", cen->code );
		return( NO );
		}
	return( YES );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* COPY SQL STRUCTURE INTO PROGRAM STRUCTURE */
int xpack_centre_convert( CENTRE *ce, int status )
{
	ce->status = xsql_dyn_field_get_int( d_cen, 3 );
	if ( status != XPACK_STATUS_ANY && status != ce->status )
		{return( NO );
		}
	xsql_dyn_field_get_string( d_cen, 0, ce->root );
	xsql_dyn_field_get_string( d_cen, 1, ce->code );
	xsql_dyn_field_get_string( d_cen, 2, ce->name );
	ce->restock = xsql_dyn_field_get_int( d_cen, 4 );
	xsql_dyn_field_get_date( d_cen, 5, &(ce->udate) );
	return( xpack_centre_validate( ce ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* FIND ENTRY FOR A SPECIFIC CENTRE */
int xpack_centre_search( char *c_txt, CENTRE *c, int nfld )
{	int	count = 0;
	if ( nfld != 1 && nfld != 2 )
		{bug( "xpack_centre_search %ld", nfld );
		}
	xpack_sql_search_string( sql_search_string, c_txt );
	sprintf( sql_command,
		SEL_CENTRE
		" WHERE %s='%s'",
		( 1 == nfld ) ? "ccode" : "cname", sql_search_string );
	xpack_open();
	d_cen = xsql_dyn_init_fetch( "cen", sql_command, "r" );
	while ( 0 == xsql_dyn_cursor_fetch( d_cen ) )
		{
		xsql_dyn_cursor_fetch( d_cen );
		if ( YES == xpack_centre_convert( c, XPACK_STATUS_ANY ) )
			{count++;
			}
		}
	xsql_dyn_end_fetch( d_cen );
	xpack_close();
	return( count );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
	/* LOAD ALL CENTRES WITHIN ONE COUNTRY HAVING MATCHING STATUS */
void xpack_centre_select_status( long *ncent, CENTRE **cent, char *cntry_code,
	int status )
{
	*ncent = 0;
	*cent = (CENTRE *) xmalloc( 12 * sizeof(CENTRE) );
	xpack_sql_search_string( sql_search_string, cntry_code );
	sprintf( sql_command,
		SEL_CENTRE
		" WHERE gcode='%s' ORDER BY ccode ASC",
		sql_search_string );
	xpack_open();
	d_cen = xsql_dyn_init_fetch( "cen", sql_command, "r" );
	while ( 0 == xsql_dyn_cursor_fetch( d_cen ) )
		{
		if ( YES == xpack_centre_convert( *cent + (*ncent), status ) )
			{(*ncent)++;
			if ( *ncent > 0 && 0 == *ncent % 10 )
				{*cent = (CENTRE *) xrealloc( *cent,
					( *ncent + 12 ) * sizeof(CENTRE) );
				}
			}
		}
	xsql_dyn_end_fetch( d_cen );
	xpack_close();
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* LOAD ALL CENTRES WITHIN ONE COUNTRY */
void xpack_centre_select( long *ncent, CENTRE **cent, char *cntry_code )
{
	xpack_centre_select_status( ncent, cent, cntry_code, XPACK_STATUS_ANY );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* LOAD ALL ACTIVE CENTRES WITHIN ONE COUNTRY */
void xpack_centre_select_active( long *ncent, CENTRE **cent, char *cntry_code )
{
	xpack_centre_select_status( ncent, cent, cntry_code, XPACK_STATUS_OK );
}
/*---------------------------------------------------------------------------*/
					/* COUNTRY: CODE,NAME,STATUS,UPDATE */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* CHANGE STATUS OF COUNTRY */
char *xpack_country_status( COUNTRY *cntry )
{	char	*msg = NULL;
	char	new_date[20];
	date_set_today( &(cntry->udate) );
	date_to_string( new_date, &(cntry->udate), 0 );
	sprintf( sql_command,
		"UPDATE country SET status=%d, udate='%s' WHERE gcode='%s'",
		cntry->status, new_date, cntry->code );
	xpack_exec( sql_command );
	return( msg );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* ADD NEW COUNTRY TO FILE */
char *xpack_country_add( char *cod, char *nam )
{	char	*msg = NULL;
	DATE	td;
	char	add_date[20];
	char	name[200];
	date_set_today( &td );
	if ( EOS == cod[0] || EOS == nam[0] )
		{return( "empty code or name" );
		}
	if ( YES != xpack_valid_code( cod ) )
		{return( XPACK_INVALID_CODE );
		}
	date_to_string( add_date, &td, 0 );
	xsql_squote( name, nam );
	sprintf( sql_command, "INSERT INTO country "
		"( gcode, gname, status, udate ) "
		"VALUES "
		"('%s', '%s', 0, '%s' )",
		cod, name, add_date );
	xpack_exec( sql_command );
	xsql_check_sqlcode( "failed to add country \"%s\"", cod );
	return( msg );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* ASSIGN AND CHECK COUNTRY STATUS CODE */
int xpack_country_validate( COUNTRY *cnt )
{	if ( NULL == ( cnt->pcd = pack_code( cnt->status, country_status ) ) )
		{aside( "ERROR country %s", cnt->code );
		return( NO );
		}
	return( YES );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* COPY SQL STRUCTURE INTO PROGRAM STRUCTURE */
int xpack_country_convert( COUNTRY *cn, int status )
{
	cn->status = xsql_dyn_field_get_int( d_cntry, 2 );
	if ( status != XPACK_STATUS_ANY && status != cn->status )
		{return( NO );
		}
	xsql_dyn_field_get_string( d_cntry, 0, cn->code );
	xsql_dyn_field_get_string( d_cntry, 1, cn->name );
	xsql_dyn_field_get_date( d_cntry, 3, &(cn->udate) );
	return( xpack_country_validate( cn ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* FIND ENTRY FOR A SPECIFIC COUNTRY */
int xpack_country_search( char *c_txt, COUNTRY *c, int nfld )
{	int	count = 0;
	if ( nfld != 0 && nfld != 1 )
		{bug( "xpack_country_search %ld", nfld );
		}
	xpack_sql_search_string( sql_search_string, c_txt );
	sprintf( sql_command,
		SEL_COUNTRY  " WHERE %s='%s'",
		( 0 == nfld ) ? "gcode" : "gname", sql_search_string );
	xpack_open();
	d_cntry = xsql_dyn_init_fetch( "csel",sql_command, "r" );
	while ( 0 == xsql_dyn_cursor_fetch( d_cntry ) )
		{
		if ( YES == xpack_country_convert( c, XPACK_STATUS_ANY ) )
			{count++;
			}
		}
	xsql_dyn_end_fetch( d_cntry );
	xpack_close();
	return( count );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* LOAD ALL COUNTRIES WITH SPECIFIED STATUS */
void xpack_country_select_status( long *ncnt, COUNTRY **cnt, int status )
{
	*ncnt = 0;
	*cnt = (COUNTRY *) xmalloc( 12 * sizeof(COUNTRY) );
	xpack_open();
	d_cntry = xsql_dyn_init_fetch( "csel",
		SEL_COUNTRY " ORDER BY gcode ASC", "r" );
	while ( 0 == xsql_dyn_cursor_fetch( d_cntry ) )
		{
		if ( YES == xpack_country_convert( *cnt + (*ncnt), status ) )
			{(*ncnt)++;
			if ( *ncnt > 0 && 0 == *ncnt % 10 )
				{*cnt = (COUNTRY *) xrealloc( *cnt,
					( *ncnt + 12 ) * sizeof(COUNTRY) );
				}
			}
		}
	xsql_dyn_end_fetch( d_cntry );
	xpack_close();
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* LOAD ALL COUNTRIES */
void xpack_country_select( long *ncnt, COUNTRY **cnt )
{
	xpack_country_select_status( ncnt, cnt, XPACK_STATUS_ANY );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* LOAD ALL ACTIVE COUNTRIES */
void xpack_country_select_active( long *ncnt, COUNTRY **cnt )
{
	xpack_country_select_status( ncnt, cnt, XPACK_STATUS_OK );
}
/*---------------------------------------------------------------------------*/
				/* COUNT TOTAL NUMBER OF ROWS IN TABLE */
static int xpack_table_count( char *table_name )
{	int	count = -1;
	char	query[100];
	XSQL_DYN	*xd_count;
	sprintf( query, "SELECT COUNT(*) FROM %s", table_name );
	xpack_open();
	xd_count = xsql_dyn_init_fetch( "row_counter", query, "r" );

	if ( 0 == xsql_dyn_cursor_fetch( xd_count ) )
		{count = xsql_dyn_field_get_int( xd_count, 0 );
		}

	xsql_dyn_end_fetch( xd_count );
	xpack_close();
	return( count );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
int xpack_staff_count( void )
{	return( xpack_table_count( "staff" ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
int xpack_unit_count( void )
{	return( xpack_table_count( "unit" ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
int xpack_set_count( void )
{	return( xpack_table_count( "sets" ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
int xpack_centre_count( void )
{	return( xpack_table_count( "centre" ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
int xpack_country_count( void )
{	return( xpack_table_count( "country" ) );
}
/*---------------------------------------------------------------------------*/
typedef	struct
	{
	char	*param;
	int	ivalue;
	char	*svalue;
	}
	XPACK_CONFIG_PARAM;
/*---------------------------------------------------------------------------*/
			/* GET CONFIGURATION PARAMETER(S) FROM DATABASE */
void xpack_config_param( char *name, int *ival, char *sval, int reload )
{
	int	i;
	int	count = 0;
	static	int	nparams = 0;
	char	tmp[100];
	XSQL_DYN	*xd_param;
	static	XPACK_CONFIG_PARAM	*xcp = NULL;
	if ( reload && NULL != xcp )	/* FORCE REFRESH OF EXISTING DATA */
		{
		for ( i = 0; i < nparams; i++ )
			{
			xfree( xcp[i].param  );
			xfree( xcp[i].svalue );
			}
		xfree( xcp );
		xcp = NULL;
		}		
	if ( NULL == xcp )			/* LOAD DATA AND CACHE IT */
		{
		nparams = 0;
		xcp = (XPACK_CONFIG_PARAM *) xmalloc(
				12 * sizeof(XPACK_CONFIG_PARAM) );
		xpack_open();
		xd_param = xsql_dyn_init_fetch( "cfg_param",
			"SELECT param,ivalue,svalue"
			" FROM xconfig ORDER BY param",
			"r" );
		while ( 0 == xsql_dyn_cursor_fetch( xd_param ) )
			{
			xsql_dyn_field_get_string( xd_param, 0, tmp );
			xcp[nparams].param = duplicate_string( tmp );
			xcp[nparams].ivalue
				= xsql_dyn_field_get_int( xd_param, 1 );
			xsql_dyn_field_get_string( xd_param, 2, tmp );
			xcp[nparams].svalue = duplicate_string( tmp );
			if ( 0 == ++nparams % 10 )
				{
				xcp = (XPACK_CONFIG_PARAM *) xrealloc( xcp,
					( nparams + 12 )
					* sizeof(XPACK_CONFIG_PARAM) );
				}
			}
		xsql_dyn_end_fetch( xd_param );
		xpack_close();
		aside( "xpack_config_param, %d loaded", nparams );
		}
	for ( i = 0; i < nparams; i++ )
		{
		if ( 0 == strcmp( name, xcp[i].param ) )
			{
			*ival = xcp[i].ivalue;
			strcpy( sval, xcp[i].svalue );
			count++;
			}
		}
	if ( count != 1 )
		{
		failure(
	"xpack_config_param: read %d copies of config parameter \"%s\"",
			count, name );
		exit( EXIT_SUCCESS );
		}
}
/*---------------------------------------------------------------------------*/
			/* GET AN INTEGER PARAMETER FROM XPARAM TABLE */
int xpack_xparam_get_int( char *field_name, int default_retval )
{
	int	retval;
	char	query[200];
	XSQL_DYN	*xd_xpget = NULL;
	sprintf( query, "SELECT ivalue FROM xparam WHERE param='%s'",
		field_name );
	xpack_open();
	xd_xpget = xsql_dyn_init_fetch( "cnt", query, "r" );
	if ( 0 == xsql_dyn_cursor_fetch( xd_xpget ) )
		{retval = xsql_dyn_field_get_int( xd_xpget, 0 );
		}
	else
		{retval = default_retval;
		}
	xsql_dyn_end_fetch( xd_xpget );
	xpack_close();
	return( retval );
}
/*---------------------------------------------------------------------------*/
			/* SET AN INTEGER PARAMETER IN XPARAM TABLE */
int xpack_xparam_set_int( char *field_name, int value )
{
	char	query[200];
	xpack_open();
	sprintf( query, "UPDATE xparam"
		" SET ivalue=%d"
		" WHERE param='%s'",
		value, field_name );
	xsql_execute_immediate( query );
	xpack_close();
	return( 1 );
}
/*---------------------------------------------------------------------------*/
			/* RETURN A COUNTER AND INCREMENT CURRENT VALUE */
int xpack_counter( char *field_name, int increment, int default_retval )
{
	int	val;
	char	query[200];
	val = xpack_xparam_get_int( field_name, default_retval );
	if ( val != default_retval )
		{sprintf( query, "UPDATE xparam"
			" SET ivalue = ivalue + %d"
			" WHERE param='%s'",
			increment, field_name );
		xpack_open();
		xsql_execute_immediate( query );
		xpack_close();
		}
	return( val );
}
/*---------------------------------------------------------------------------*/
					/* GET RANDOM SEED FROM XPARAM TABLE */
int xpack_ranseed_get( int default_retval )
{	return( xpack_xparam_get_int( "RANSEED", default_retval ) );
}
/*---------------------------------------------------------------------------*/
					/* SET RANDOM SEED IN XPARAM TABLE */
int xpack_ranseed_set( int val )
{	return( xpack_xparam_set_int( "RANSEED", val ) );
}
/*---------------------------------------------------------------------------*/
					/* GET NEXTID FROM XPARAM TABLE */
int xpack_nextid( int default_retval )
{	return( xpack_counter( "NEXTID", 1, default_retval ) );
}
/*===========================================================================*/
#endif							/* XPACK_DB.C */
