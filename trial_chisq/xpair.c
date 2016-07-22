#ifndef XPAIR_C_INCLUDED
#define	XPAIR_C_INCLUDED
/*===========================================================================*/
/*			GENERAL NAME=VALUE CONTAINER			*/
/*===========================================================================*/
#include "xinclude.h"
#include "xtime.c"
#include "xstring.c"
#include "xmd5.c"
/*---------------------------------------------------------------------------*/
#define	XPAIR_TYPE_NONE		0
#define	XPAIR_TYPE_BOOL		'b'
#define	XPAIR_TYPE_INT		'i'
#define	XPAIR_TYPE_REAL		'r'
#define	XPAIR_TYPE_TIME		't'
#define	XPAIR_TYPE_DATE		'd'
#define	XPAIR_TYPE_STRING	's'
/*---------------------------------------------------------------------------*/
#define	XPAIR_ORDERING_NONE	0
#define	XPAIR_ORDERING_BYNAME	1
/*---------------------------------------------------------------------------*/
typedef	struct
	{
	int	i;
	double	r;
	char	*s;
	XTIME	t;
	int	slen;
	}
	XPAIR_CONTENTS;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
typedef	struct
	{
	int	type;
	char	*name;
	int	sql_len;
	XPAIR_CONTENTS	val;
	}
	XPAIR_FIELD;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
typedef	struct
	{
	int	nfields;
	XPAIR_FIELD	*field;
	int	ordering;
	}
	XPAIR;
/*---------------------------------------------------------------------------*/
XPAIR *xpair_new( void )
{
	XPAIR	*p = NULL;
	p = (XPAIR *) malloc( sizeof(XPAIR) );
	p->nfields = 0;
	p->field = (XPAIR_FIELD *) malloc( 12 * sizeof(XPAIR_FIELD) );
	p->ordering = XPAIR_ORDERING_NONE;
	return( p );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* EMPTY CONTENTS OF POINTER */
int xpair_clear( XPAIR *p )
{
	return( 0 );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
int xpair_delete( XPAIR *p )
{
	int	i;
	if ( NULL == p->field )
		{
		return( 1 );			/* ASSUME ALREADY FREED */
		}
	for ( i = 0; i < p->nfields; i++ )
		{
		if ( XPAIR_TYPE_STRING == p->field[i].type )
			{free( p->field[i].val.s );
			}
		free( p->field[i].name );
		}
	free( p->field );
	p->field = NULL;
	p->nfields = 0;
	free( p );
	return( 0 );
}
/*---------------------------------------------------------------------------*/
int xpair_qsort_cmp_byname( const void *a, const void *b )
{
	XPAIR_FIELD	*pa;
	XPAIR_FIELD	*pb;
	pa = (XPAIR_FIELD *) a;
	pb = (XPAIR_FIELD *) b;
	return( strcmp( pa->name, pb->name ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
void xpair_sort_byname( XPAIR *p )
{
	if ( p->nfields > 1 )
		{qsort( (void *) p->field, (size_t) p->nfields, 
			sizeof(XPAIR_FIELD), xpair_qsort_cmp_byname );
		}
	p->ordering = XPAIR_ORDERING_BYNAME;
}
/*---------------------------------------------------------------------------*/

XPAIR_FIELD *xpair_addfield( XPAIR *pa, char *field_name )
{
	XPAIR_FIELD	*fld;
	if ( 0 == ++(pa->nfields) % 10 )
		{pa->field = (XPAIR_FIELD *) realloc( pa->field,
			( pa->nfields + 12 ) * sizeof(XPAIR_FIELD) );
		}
	fld = pa->field + ( pa->nfields - 1 );
	fld->name = (char *) malloc( strlen(field_name) + 2 );
	strcpy( fld->name, field_name );
	fld->type = XPAIR_TYPE_NONE;
	fld->val.slen = 0;
	fld->val.s = NULL;
	pa->ordering = XPAIR_ORDERING_NONE;
	return( fld );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
int xpair_int( XPAIR *pp, char *name, int value )
{
	XPAIR_FIELD	*fld;
	fld = xpair_addfield( pp, name );
	fld->type = XPAIR_TYPE_INT;
	fld->val.i = value;
	return( 0 );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
int xpair_bool( XPAIR *pp, char *name, int value )
{
	XPAIR_FIELD	*fld;
	if ( value != 0 && value != 1 )
		{printf( "\nERROR, xpair_bool, field %s has illegal value %d",
			name, value );
		return( 1 );
		}
	fld = xpair_addfield( pp, name );
	fld->type = XPAIR_TYPE_BOOL;
	fld->val.i = value;
	return( 0 );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
int xpair_time( XPAIR *pp, char *name, XTIME *value )
{
	XPAIR_FIELD	*fld;
	fld = xpair_addfield( pp, name );
	fld->type = XPAIR_TYPE_TIME;
	time_copy( &(fld->val.t), value );
	return( 0 );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* CREATE XTIME FIELD HOLDING CURRENT TIME */
int xpair_time_now( XPAIR *pp, char *name )
{
	XTIME	t;
	time_set_now( &t );
	return( xpair_time( pp, name, &t ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
int xpair_date( XPAIR *pp, char *name, XDATE *value )
{
	XPAIR_FIELD	*fld;
	fld = xpair_addfield( pp, name );
	fld->type = XPAIR_TYPE_DATE;
	date_copy( &(fld->val.t.date), value );
	return( 0 );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* CREATE XDATE VALUE HOLDING TODAY'S DATE */
int xpair_date_today( XPAIR *pp, char *name )
{
	XDATE	d;
	date_set_today( &d );
	return( xpair_date( pp, name, &d ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
int xpair_string( XPAIR *pp, char *name, char *value )
{
	int	vlen;
	XPAIR_FIELD	*fld;
	fld = xpair_addfield( pp, name );
	fld->type = XPAIR_TYPE_STRING;
	fld->val.slen = strlen( value );
	fld->val.s = (char *) malloc( fld->val.slen + 2 );
	strcpy( fld->val.s, value );
	return( 0 );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
int xpair_char( XPAIR *pp, char *name, char value )
{
	char	cs[2];
	cs[0] = value;
	cs[1] = EOS;
	return( xpair_string( pp, name, cs ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
int xpair_real( XPAIR *pp, char *name, double value )
{
	XPAIR_FIELD	*fld;
	fld = xpair_addfield( pp, name );
	fld->type = XPAIR_TYPE_REAL;
	fld->val.r = value;
	return( 0 );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
int xpair_copy_field( XPAIR *pp, XPAIR_FIELD *cf )
{
	XPAIR_FIELD	*fld;
	fld  = xpair_addfield( pp, cf->name );
	fld->type = cf->type;
	fld->val.i = cf->val.i;
	fld->val.r = cf->val.r;
	fld->val.t = cf->val.t;
	if ( NULL != cf->val.s )
		{fld->val.slen = cf->val.slen;
		fld->val.s = (char *) malloc( fld->val.slen + 2 );
		strcpy( fld->val.s, cf->val.s );
		}
	return( 0 );
}
/*---------------------------------------------------------------------------*/

char *xpair_field_astext( XPAIR_FIELD *f )
{
	static	char	buf[100];
	if ( f->val.s != NULL )
		{return( f->val.s );
		}
	switch( f->type )
		{
		case XPAIR_TYPE_BOOL:
		case XPAIR_TYPE_INT:
			sprintf( buf, "%d", f->val.i );
			break;
		case XPAIR_TYPE_REAL:
			sprintf( buf, "%8.1le", f->val.r );
			break;
		case XPAIR_TYPE_TIME:
			time_to_string( buf, &(f->val.t), 0, NULL, 1 );
			break;
		case XPAIR_TYPE_DATE:
			date_to_string( buf, &(f->val.t.date), 0 );
			break;
		default:
			bug( "xpair_field_astext, invalid type" );
			break;
		}
	return( buf );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* CALCULATE MD5 DIGEST FOR PAIRS */
void xpair_md5( XPAIR *pp, char *md5_hex_digest )
{
	int	i;
	char	rbuf[30];
	XSTRING	*total;
	XPAIR_FIELD	*pf;
	md5_hex_digest[0] = '\0';
	total = xstring_new();
	for ( i = 0; i < pp->nfields; i++ )
		{
		pf = pp->field + i;
		xstring_add_string( total, ( i > 0 ) ? "&" : "[" );
		xstring_add_string( total, pf->name );		
		xstring_add_string( total, xpair_field_astext( pf ) );
		}
	xstring_add_string( total, "]" );
}
/*---------------------------------------------------------------------------*/
				/* DUMP CONTENTS OF XPAIR TO FILE FOR DEBUG */
int xpair_dump( XPAIR *pp, FILE *f, char *label )
{
	int	i;
	if ( NULL == f )
		{return( 1 );
		}
	fprintf( f, "\nXPAIR \"%s\" %d fields, ordering %d",
		label, pp->nfields, pp->ordering );
	for ( i = 0; i < pp->nfields; i++ )
		{fprintf( f, "\n[%d]\t%-15.15s\t\"%s\"",
			i, pp->field[i].name,
			xpair_field_astext( pp->field + i ) );
		}
}
/*---------------------------------------------------------------------------*/
					/* RETURN POINTER TO NAMED FIELD */
XPAIR_FIELD *xpair_get_field( char *emsg, XPAIR *p, char *field_name )
{	int	i;
	emsg[0] = '\0';
	for ( i = 0; i < p->nfields; i++ )
		{if ( 0 == strcmp( field_name, p->field[i].name ) )
			{return( p->field + i );
			}
		}
	sprintf( emsg, "xpair_get.., unknown name %-20.20s", field_name );
	return( NULL );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

int xpair_get_int( char *emsg, XPAIR *p, char *field_name )
{
	XPAIR_FIELD	*f;
	f = xpair_get_field( emsg, p, field_name );
	if ( NULL == f )
		{return( 0 );
		}
	if ( f->type != XPAIR_TYPE_INT )
		{sprintf( emsg, "xpair_get_int, non-int field %-20.20s",
			field_name );
		return( 0 );
		}
	return( f->val.i );
}
/*===========================================================================*/
#endif
