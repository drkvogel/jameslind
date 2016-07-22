#ifndef XDIR_C_INCLUDED
#define XDIR_C_INCLUDED 1
/*===========================================================================*/
/*                      DIRECTORY UTILITIES				*/
/*===========================================================================*/
#include "xdefs.h"
#include "xinclude.h"
#include "xtime.c"
/*---------------------------------------------------------------------------*/
#ifndef __vms__
#include <dirent.h>	/* SHOULD BE LIBRARY CALLS FOR UNIX AND BORLANDC */
#else
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
typedef	struct dirent				/* ENTRY FOR A SINGLE FILE */
	{
	char	*d_name;
	}
	DIRENT;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
typedef	struct          		/* DESCRIPTION OF A WHOLE DIR */
	{
	long	nfiles;
	long	nret;
	char	dname;
	DIRENT	*d;
	}
	DIR;
/*---------------------------------------------------------------------------*/
int	opendir_generic = YES;
/*---------------------------------------------------------------------------*/
		/* SPECIFY IF HIGHEST-VERSIONS-ONLY ARE TO BE RETURNED */
void opendir_generic_set( int hos )
{	opendir_generic = ( 0 != hos ) ? YES : NO;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* RETURN A UNIQUE FILE NAME */
char *opendir_unique_fname( void )
{
	return( "sys$login:dir.tmp" );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* CHECK DIRECTORY FILE EXISTS */
int opendir_exists( char *dir_nam )
{	int	n;
	int	c = 0;
	char	file_name[210];
	FILE	*ftmp;
	n = strlen( dir_nam );
	if ( EOS == dir_nam[0] || COLON == dir_nam[n-1] )
		{return( YES );	/* UNCHECKABLE LOGICAL NAME, CROSS-FINGERS! */
		}
	else if ( ']' != dir_nam[n-1] )
		{bug( "directory name \"%s\" not terminated by ']'", dir_nam );
		exit( 0 );
		}
	strncpy( file_name, dir_nam, 200 );
	n--;
	strcpy( file_name + n, ".dir" );
	while ( --n >= 0 && '[' != ( c = file_name[n] ) && '.' != c )
		{;
		}
	if ( '.' == c )
		{file_name[n] = ']';
		}
	else if ( '[' == c )			/* SHUFFLE UP */
		{while ( EOS != file_name[n] )
			{file_name[n] = file_name[n+1];
			n++;
			}
		}
	else
		{bug( "apparently invalid directory name \"%s\"",
			dir_nam );
		exit( 0 );
		}
	if ( NULL == ( ftmp = fopen( file_name, "r" ) ) )
		{return( NO );
		}
	fclose( ftmp );
	return( YES );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* LOAD CONTENTS OF DIRECTORY */
DIR *opendir_filter( char *dir_name, char *filter )
{	long	i, bp, bc;
	long	nalloc = 100;
	char	*list_name;
	char	command[200], file_name[200];
	FILE	*fd;
	DIR	*pd = NULL;
	if ( YES != opendir_exists( dir_name ) )
		{aside( "Unable to find directory file %s", dir_name );
		return( NULL );
		}
	list_name = opendir_unique_fname();
	sprintf( command, "dir/nohead/notrail/out=%s %s%s%s", list_name,
		dir_name, filter, ( YES == opendir_generic ) ? ";" : " " );
	aside( "COM=%s", command );
	system( command );
	if ( NULL == ( fd = fopen( list_name, "r" ) ) )
		{aside( "Unable to get listing for directory \"%s\"" );
		return( NULL );
		}
	pd = (DIR *) xmalloc( sizeof(DIR) );
	pd->nfiles = pd->nret = 0;
	pd->d = (DIRENT *) xmalloc( ( nalloc + 2 ) * sizeof(DIRENT) );
	while ( EOF != read_line( fd, file_name, 200 ) )
		{bp = 0;
		while ( file_name[bp] != ']' && file_name[bp] != EOS )
			{bp++;
			}
		if ( EOS == file_name[bp] || EOS == file_name[bp+1])
			{aside( "bad file name \"%s\"", file_name );
			return( NULL );	/* VMS SYNTAX VIOLATED */
			}
		if ( YES == opendir_generic )	/* REMOVE VERSION NUMBER */
			{bc = bp + 1;
			while ( SEMICOLON != file_name[bc]
					&& EOS != file_name[bc] )
				{bc++;
				}
			file_name[bc] = EOS;
			}
		pd->d[ pd->nfiles ].d_name = duplicate_string( file_name+bp+1 );
		if ( ++(pd->nfiles) >= nalloc )
			{nalloc += 100;
			pd->d = (DIRENT *) xrealloc( pd->d, ( nalloc + 2 )
				* sizeof(DIRENT) );
			}
		}
	fclose( fd );
	return( pd );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* LOAD CONTENTS OF DIRECTORY */
DIR *opendir( char *dir_name )
{	return( opendir_filter( dir_name, "*.*" ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* RETURN NEXT FILE IN LIST */
DIRENT *readdir( DIR *pd )
{       DIRENT	*rd;
	if ( NULL != pd && NULL != pd->d && pd->nret < pd->nfiles )
		{rd = pd->d + pd->nret;
		pd->nret++;
		}
	else				/* NO MORE FILES TO RETURN */
		{rd = NULL;
		}
	return( rd );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* CLOSE DIRECTORY */
void closedir( DIR *pd )
{	long	i;
	for ( i = 0; i < pd->nfiles; i++ )
		{xfree( pd->d[i].d_name );
		}
	xfree( pd->d );
	xfree( pd );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#endif
/*---------------------------------------------------------------------------*/
					/* REPEATEDLY ATTEMPT TO OPEN FILE */
FILE *xfopen( char *fname, char *access, int max_attempts )
{	int	attempts = 1;
	char	tstring[100];
	TIME	tlast, tnow;
	FILE	*f;
	if ( NULL != ( f = fopen( fname, access ) ) )
		{return( f );	/* IMMEDIATE RETURN IF NO PROBLEMS */
		}
	time_set_now( &tlast );
	time_to_string( tstring, &tlast, 0, NULL, 1 );
	aside( "xfopen: \"%s\"\n PROBLEM/CONFLICT, access \"%s\" at %s",
		fname, access, tstring );
	while ( attempts < max_attempts )
		{attempts++;
		do			/* WAIT TILL SECOND HAS ALTERED */
			{time_set_now( &tnow );
			}
			while ( 0 == time_cmp( &tlast, &tnow ) );
		if ( NULL != ( f = fopen( fname, access ) ) )
			{time_to_string( tstring, &tnow, 0, NULL, 1 );
			aside(
			"xfopen: file \"%s\"\n OK on attempt %d at %s",
					fname, attempts, tstring );
			return( f );
			}
		time_copy( &tlast, &tnow );
		}
	time_to_string( tstring, &tnow, 0, NULL, 1 );
	aside( "xfopen: file \"%s\"\n ABANDONDED after attempt %d at %s",
		fname, attempts, tstring );
	return( NULL );
}
/*===========================================================================*/
#endif                                                          /* XDIR.C */
