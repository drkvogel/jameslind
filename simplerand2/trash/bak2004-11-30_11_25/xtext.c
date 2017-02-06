#ifndef XTEXT_C_INCLUDED
#define XTEXT_C_INCLUDED	1
/*===========================================================================*/
/*				TEXT HANDLING UTILITIES			*/
/*===========================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#include "xmessage.c"
/*---------------------------------------------------------------------------*/
			/* MOVE STRING POINTER TO END OF WHITE-SPACE INTERVAL */
char *skip_space( char *line )
{	while ( isspace( *line ) )
		{line++;
		}
	return( line );
}
/*---------------------------------------------------------------------------*/
				/* CASE-INSENSITIVE STRING COMPARISON */
int strcmp_nocase( char *a, char *b )
{	char	ca, cb;
	if ( NULL == a )
		{return( ( NULL == b ) ? 0 : -1 );
		}
	if ( NULL == b )
		{return( 1 );
		}
	do
		{if ( ( ca = tolower( *a++ ) ) != ( cb = tolower( *b++ ) ) )
			{return( ca - cb );
			}
		}
		while ( EOS != ca );
	return( 0 );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
	/* CASE-INSENSITIVE STRING COMPARISON UP TO FIRST `LEN' CHARACTERS */
int strncmp_nocase( char *a, char *b, int len )
{	char	ca, cb;
	if ( NULL == a )
		{return( ( NULL == b ) ? 0 : -1 );
		}
	if ( NULL == b )
		{return( 1 );
		}
	do
		{if ( ( ca = tolower( *a++ ) ) != ( cb = tolower( *b++ ) ) )
			{return( ca - cb );
			}
		len--;
		}
		while ( EOS != ca && len > 0 );
	return( 0 );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* FIND LOCATION OF FIRST OCCURENCE OF SUB IN STRING */
int locate_substring( char *sub, char *strn )
{	int	len;
	int	pos = 0;
	char	c;
	c = *sub;
	if ( EOS == c )
		{return( -1 );
		}
	len = strlen( sub ) - 1;
	while ( EOS != strn[pos] )
		{if ( strn[pos] == c && ( 0 == len
				|| 0 == strncmp( sub+1, strn+pos+1, len ) ) )
			{return( pos );		/* INEFFICIENT ALGORITHM */
			}
		pos++;
		}
	return( -1 );					/* NOT FOUND */
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* FIND LOCATION OF FIRST OCCURENCE OF SUB IN STRING */
int locate_substring_nocase( char *sub, char *strn )
{	int	len;
	int	pos = 0;
	char	c;
	c = tolower( *sub );
	if ( EOS == c )
		{return( -1 );
		}
	len = strlen( sub ) - 1;
	while ( EOS != strn[pos] )
		{if ( tolower( strn[pos] ) == c && ( 0 == len
				|| 0 == strncmp_nocase( sub+1, strn+pos+1,
				len ) ) )
			{return( pos );		/* INEFFICIENT ALGORITHM */
			}
		pos++;
		}
	return( -1 );					/* NOT FOUND */
}
/*---------------------------------------------------------------------------*/
				/* CONVERT ALL OF STRING TO LOWER CASE */
void string_tolower( char *s )
{	if ( NULL == s )
		{return;
		}
	while ( EOS != *s )
		{*s = tolower( *s );
		s++;
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* CONVERT ALL OF STRING TO UPPER CASE */
void string_toupper( char *s )
{	if ( NULL == s )
		{return;
		}
	while ( EOS != *s )
		{*s = toupper( *s );
		s++;
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* CONVERT STRING TO CAPITALIZED FORMAT */
void string_capitalize( char *sc, int force_hi, int force_lo )
{	int	current_al;
	int	last_al = 0;
	while ( EOS != *sc )
		{current_al = isalnum( *sc );
		if ( current_al )
			{if ( last_al ) 	/* MAP: AB cd -> Ab cd */
				{if ( YES == force_lo )
					{*sc = tolower( *sc );
					}
				}
			else			/* MAP: AB cd -> AB Cd */
				{if ( YES == force_hi )
					{*sc = toupper( *sc );
					}
				}
			}
		last_al = current_al;
		sc++;
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* REMOVE END AND EXTRA SPACES, TURN TABS TO SPACES */
int string_squeeze( char *sq )
{	int	previous_space = YES, j = 0;
	char	*raw;
	raw = sq;
	while ( EOS != *raw )
		{if ( ! isspace( *raw ) )
			{sq[ j++ ] = *raw;
			previous_space = NO;
			}
		else if ( ! previous_space )	/* OMIT MULTIPLE SPACES */
			{sq[ j++ ] = SPACE;
			previous_space = YES;
			}
		raw++;
		}
	while ( --j >= 0 && isspace( sq[j] ) )
		{;						/* TRIM END */
		}
	sq[ j+1 ] = EOS;
	return( j+1 );				/* RETURN SQUEEZED LENGTH */
}
/*---------------------------------------------------------------------------*/
		/* COUNT TEXT LINES BEFORE FIRST EMPTY STRING */
int lines_of_text( char **sarray )
{	int	nl = 0;
	while ( sarray[nl++][0] != EOS )
		{;
		}
	return(  nl - 1 );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* SHOW CONTENTS OF MULTI-LINE STRING ARRAY */
void display_multi_line( char *title, int nlines, char **line )
{	int	i;
	inform( "\nMULTI LINE: %s", title );
	for ( i = 0; i < nlines; i++ )
		{inform( "[%d]\t\"%s\"", i, line[i] );
		}
}
/*---------------------------------------------------------------------------*/
		/* CONVERT STRING INTO SEPARATE SPACE-DELIMITED UNITS */
void split_string_into_words( char *input, int *nwords, char ***word )
{
	int	len;
	char	*wd;
	*nwords = 0;
	if ( NULL == input )
		{return;
		}
	*word = (char **) xmalloc( 12 * sizeof(char *) );
	input = skip_space( input );
	while ( EOS != *input )
		{len = 0;
		while ( EOS != ( input[len] ) && ! isspace( input[len] ) )
			{len++;
			}
		wd = (*word)[ *nwords ] = (char *) xmalloc( len + 2 );
		strncpy( wd, input, len );
		wd[len] = EOS;
		(*nwords)++;
		input = skip_space( input + len );
		if ( 0 == *nwords % 10 )
			{*word = (char **) xrealloc( *word,
				( ( *nwords) + 12 ) * sizeof(char *) );
			}
		}
}
/*---------------------------------------------------------------------------*/
		/* RETURN ONLY LINES BEGINNING WITH `KEY', CHOPPING OFF KEY */
char *filter_line( char *in, char *key )
{       int	result;
	int	len = 0;
	char	c;
	char	*tmp;
	tmp = skip_space( in );
	while ( ! isspace( tmp[len] ) && EOS != tmp[len] )
		{len++;
		}
	c = tmp[len];
	tmp[ len ] = EOS;
	result = strcmp_nocase( tmp, key );
	tmp[len] = c;
	if ( result != 0 )			/* KEY DOES NOT MATCH */
		{return( NULL );
		}
	tmp += len;
	while ( isspace( *tmp ) && *tmp != EOS )
		{tmp++;
		}
	return( tmp );			/* POINTER TO NEXT FIELD IN STRING */
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define	COMMENT		'!'
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* GET LINE FROM FILE, IGNORING COMMENTS (!...)	*/
int read_line( FILE *f, char *s, int maxc )
{	int	i, j;
	int	current_input_line = 0;
	int	in_quotes = NO;
	char	c;
	do
		{do
			{current_input_line++;
			if ( NULL == fgets( s, (int) maxc, f ) )
				{s[0] = EOS;
				return( EOF );	/* RETURN EOF ON END_OF_FILE */
				}
			i = 0;
			while ( isspace( s[i] ) )
				{i++;	/* GET FIRST NON-WHITESPACE CHARACTER */
				}
			}				/* SKIP EMPTY LINES */
			while ( EOS == s[i] || EOL == s[i] );
		j = 0;
		while ( EOL != ( c = s[i] ) && EOS != c && i++ < maxc
				&& ( COMMENT != c || YES == in_quotes ) )
			{s[j++] = c;			/* COPY CHARACTER */
			if ( QUOTE == c )	/* NOTE QUOTED STRINGS */
				{in_quotes = ( NO == in_quotes ) ? YES : NO;
				}
			}
		s[j] = EOS;
		if ( EOL != c && COMMENT != c && EOS != c )
			{warning( "input line truncated to: %s", s );
			}
		while ( --j >= 0 && isspace( s[j] ) )
			{s[j] = EOS;				/* TRIM TAIL */
			}
		}
		while ( j < 0 );	/* IF LINE BLANK, THEN READ NEXT */
	return( j );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#undef	COMMENT
/*---------------------------------------------------------------------------*/
					/* MAKE A DUPLICATE COPY OF A STRING */
char *duplicate_string( char *in )
{	char	*out;
	if ( NULL == in )
		{out = (char *) xmalloc( 2 );
		out[0] = EOS;
		}
	out = (char *) xmalloc( strlen( in ) + 2 );
	strcpy( out, in );
	return( out );
}
/*---------------------------------------------------------------------------*/
				/* SEE IF CHAR C IS PART OF ALLOWED SET */
int allow_character( char c, char *cset )
{	if ( NULL == cset || EOS == cset[0] )
		{return( YES );			/* ALLOW ALL CHARACTERS */
		}
	while ( EOS != *cset )
		{if ( ( isalpha( c ) && isalpha( *cset ) )
				|| ( isdigit( c ) && isdigit( *cset ) )
				|| c == *cset )
			{return( YES );
			}
		cset++;
		}
	return( NO );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* CONDENSE A STRING INTO RELEVANT CHARACTERS ONLY */
void condense_string( char *s, char *retain, char *exclude )
{	char	*buf;
	if ( NULL == s || EOS == s[0] )
		{return;
		}
	buf = s;
	if ( NULL != retain && EOS != retain[0] )   /* RETAIN MATCHING CHARS */
		{while ( EOS != *buf )
			{if ( allow_character( *buf, retain ) )
				{*s = *buf;
				s++;
				}
			buf++;
			}
		*s = EOS;
		}
	else if ( NULL != exclude && EOS != exclude[0] )  /* EXCLUDE MATCHES */
		{while ( EOS != *buf )
			{if ( ! allow_character( *buf, exclude ) )
				{*s = *buf;
				s++;
				}
			buf++;
			}
		*s = EOS;
		}
}
/*---------------------------------------------------------------------------*/
			/* ADD EXTENSION TO FILENAME IF NOT ALREADY PRESENT */
void filext( char *fnam, char *ext )
{	int	i = -1;
	while ( EOS != fnam[++i] && i < 200 )
		{if ( DOT == fnam[i] || SEMICOLON == fnam[i] )
			{return;
			}				/* GET LENGTH */
		}
	if ( 0 == i )
		{failure( "filext: illegal file name encountered" );
		}
	strcat( fnam, "." );				/* ADD EXTENSION */
	strcat( fnam, ext );
}
/*---------------------------------------------------------------------------*/
						/* IS ALL OF STRING ALPHA */
int isalpha_all( char *s )
{	while ( EOS != *s )
		{if ( ! isalpha( *s ) )
			{return( NO );
			}
		s++;
		}
	return( YES );
}
/*---------------------------------------------------------------------------*/
		/* GENERATE A UNIQUE (AT LEAST TO CURRENT PROCESS) STRING */
void string_unique( char *output )
{	int	i = -1;
	long	n;
	static	long	ncalls = 0;
	char	*base36char = "0123456789abcdefghijklmnopqrstuvwxyz_";
	TIME	nt;
	time_set_now( &nt );
	n = (++ncalls) + time_second( &nt )
		+ ( 60 * ( time_minute( &nt )
		+ ( 60 * ( time_hour( &nt )
		+ ( 24 * ( time_day( &nt )
		+ ( 32 * ( time_month( &nt )
		+ ( 12 * ( time_year( &nt ) - 1980 )
		) ) ) ) ) ) ) ) );
	while ( ++i < 10 && n > 0 )
		{output[ i ] = base36char[ n % 36 ];
		n /= 36;
		}
	output[i] = EOS;
}
/*===========================================================================*/
#endif								/* XTEXT.C */
