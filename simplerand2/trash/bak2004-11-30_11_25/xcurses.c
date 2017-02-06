#ifndef	XCURSES_C_INCLUDED
#define	XCURSES_C_INCLUDED	1
/*===========================================================================*/
/*		EMULATION OF CURSES SCREEN HANDLING LIBRARY		*/
/*===========================================================================*/
#if defined __BORLANDC__
#define USE_DEC_CURSES	0
#else
#define	USE_DEC_CURSES	1
#endif
/*---------------------------------------------------------------------------*/
#if USE_DEC_CURSES
#include <curses.h>
#ifndef _BOLD
#define _BOLD 0
void wsetattr( WINDOW *w, int ignore )
{			/* UNIX IMPLEMENTATON DOESN'T INCLUDE THIS FUNCTION */
}
#endif
#else
/*---------------------------------------------------------------------------*/
#if defined __DECC

#define	cprintf	printf

#elif defined __BORLANDC__

#include <conio.h>

#endif
/*---------------------------------------------------------------------------*/
#include "xinclude.h"
#include "xdefs.h"
#include "xalloc.c"
#include "xmessage.c"
/*---------------------------------------------------------------------------*/
#define	_BLINK		1			/* ATTRIBUTES FOR PRINTING */
#define	_BOLD		2
#define	_REVERSE	4
#define	_UNDERLINE	8
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define NORMAL_BACK_DEFAULT	BLACK
#define	NORMAL_TEXT_DEFAULT	GREEN
#define	BOLD_TEXT_DEFAULT	YELLOW
#define REVERSE_TEXT_DEFAULT	RED
#define	UNDERLINE_TEXT_DEFAULT	LIGHTBLUE
/*---------------------------------------------------------------------------*/
typedef	struct
	{
	int	origin;
	int	size;
	int	max;
	int	cursor;
	}
	WINDOW_LOCATION;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
typedef	struct
	{
	int	index;
	WINDOW_LOCATION	x;
	WINDOW_LOCATION	y;
	char	**c;
	int	attribute;
	}
	WINDOW;
/*---------------------------------------------------------------------------*/
const	int	border = YES;
static	int	debug_pcurses = 0;
static	int	nruns = 0;
#ifdef __BORLANDC__
static	int	normal_text_colour;
static	int	normal_back_colour;
static	int	bold_text_colour;
static	int	reverse_text_colour;
static	int	underline_text_colour;
struct	text_info	screen_defaults;
#endif
/*---------------------------------------------------------------------------*/
#ifdef __BORLANDC__
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* SHOW RANGE OF PC COLOURS AVAILABLE */
void pcurses_colour_demo( void )
{       int	i;
	char	*name[] = {
		"BLACK",	"BLUE",		"GREEN",	"CYAN",
		"RED",		"MAGENTA",	"BROWN",	"LIGHTGRAY",
		"DARKGRAY",	"LIGHTBLUE",	"LIGHTGREEN",	"LIGHTCYAN",
		"LIGHTRED",	"LIGHTMAGENTA",	"YELLOW",	"WHITE" };
	for ( i = 0; i < 16; i++ )
		{textcolor( i );
		printf( "\n" );
		cprintf( "%3d %-20s", i, name[i] );
		textcolor( i + BLINK );
		printf( "\t" );
		cprintf( "%3d %-20s", i, name[i] );
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* SELECT TEXT ENHANCEMENT COLOR FOR PC SCREENS */
static int pcurses_select_text_color( int at )
{	int	tc;
	tc = normal_text_colour;
	if ( at & _BOLD )
		{tc = bold_text_colour;
		}
	if ( at & _REVERSE )
		{tc = reverse_text_colour;
		}
	if ( at & _UNDERLINE )
		{tc = BROWN;
		}
	return( tc + ( ( at & _BLINK ) ? BLINK : 0 ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#endif
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* PUT CONTENTS OF WINDOW ONTO SCREEN */
void wrefresh( WINDOW *w )
{	int	i, j, blank;
	if ( 0 == debug_pcurses )
		{
#ifdef __BORLANDC__
		textcolor( pcurses_select_text_color( w->attribute ) );
#endif
		for ( i = 0; i < w->y.size; i++ )
			{gotoxy( w->x.origin + 1, w->y.origin + i + 1 );
			cprintf( w->c[i] );
			}
#ifdef __BORLANDC__
		if ( 0 != w->attribute )	/* RESTORE TO DEFAULTS */
			{textcolor( normal_text_colour );
			}
#endif
		}
	else
		{blank = YES;
		for ( i = 0; i < w->y.size; i++ )
			{j = 0;
			while ( EOS != w->c[i][j] )
				{if ( SPACE != w->c[i][j] )
					{blank = NO;
					}
				j++;
				}
			}
		if ( YES == blank )
			{inform( "wrefresh %d is BLANK", w->index );
			return;
			}
		inform( "wrefresh %d", w->index );
		if ( YES == border )
			{printf( "\n" );
			for ( i = 0; i < w->x.size; i++ )
				{printf( "#" );
				}
			}
		for ( i = 0; i < w->y.size; i++ )
			{printf( "\n%s", w->c[i] );
			}
		if ( YES == border )
			{printf( "\n" );
			for ( i = 0; i < w->x.size; i++ )
				{printf( "#" );
				}
			}
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* CREATE TEXT DESCRIPTION OF WINDOW ATTRIBUTES */
void list_attributes( char *list, int at )
{	if ( 0 == at )
		{strcat( list, " *NONE*" );
		}
	else
		{if ( _BLINK & at )
			{strcat( list, " BLINK" );
			}
		if ( _BOLD & at )
			{strcat( list, " BOLD" );
			}
		if ( _REVERSE & at )
			{strcat( list, " REVERSE" );
			}
		if ( _UNDERLINE & at )
			{strcat( list, " UNDERLINE" );
			}
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* CLEAR ATTRIBUTES ON A WINDOW */
void wclrattr( WINDOW *w, int att )
{       char	tmp[100];
	if ( w->attribute > _BLINK + _BOLD + _REVERSE + _UNDERLINE
			|| w->attribute < 0 )
		{failure( "illegal attribute %d", w->attribute );
		}
	if ( w->attribute & att )
		{w->attribute -= att;
		}
	if ( 0 != debug_pcurses )
		{sprintf( tmp, "wclrattr %d ", w->index );
		list_attributes( tmp, w->attribute );
		inform( tmp );
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* SET ATTRIBUTES ON A WINDOW */
void wsetattr( WINDOW *w, int att )
{	char	tmp[100];
	if ( w->attribute > _BLINK + _BOLD + _REVERSE + _UNDERLINE
			|| w->attribute < 0 )
		{failure( "illegal attribute %d", w->attribute );
		}
	w->attribute |= att;
	if ( 0 != debug_pcurses )
		{sprintf( tmp, "wsetattr %d ", w->index );
		list_attributes( tmp, w->attribute );
		inform( tmp );
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* BLANK CONTENTS OF WINDOW */
void wclear( WINDOW *w )
{	int	i, j;
	if ( 0 != debug_pcurses )
		{inform( "wclear %d", w->index );
		}
	for ( i = 0; i < w->y.size; i++ )
		{for ( j = 0; j < w->x.size; j++ )
			{w->c[i][j] = SPACE;
			}
		w->c[i][j] = EOS;
		}
	w->x.cursor = w->y.cursor = 0;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* BLANK CONTENTS OF WINDOW TO END OF CURRENT LINE */
void wclrtoeol( WINDOW *w )
{	int	i;
	if ( 0 != debug_pcurses )
		{inform( "wclrtoeol %d", w->index );
		}
	for ( i = w->x.cursor; i < w->x.size; i++ )
		{w->c[ w->y.cursor ][i] = SPACE;
		}
	w->c[ w->y.cursor ][ w->x.size ] = EOS;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* MOVE CURSOR WITHIN WINDOW */
void wmove( WINDOW *w, int yc, int xc )
{	if ( 0 != debug_pcurses )
		{inform( "wmove %d %d %d", w->index, yc, xc );
		}
	if ( xc < 0 || yc < 0 || xc >= w->x.size || yc >= w->y.size )
		{failure( "wmove %d, illegal y=%d, x=%d", w->index,
			yc, xc );
		}
	w->x.cursor = xc;
	w->y.cursor = yc;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* WRITE TEXT TO WINDOW */
void wprintw( WINDOW *w, char *fmt, ... )
{	int	i = 0;
	char	*tmp;
	va_list	args;				/* VARIABLE ARGUMENT LIST */
	if ( 0 != debug_pcurses )
		{inform( "wprintw %d \"%s\"...", w->index, fmt );
		}
	tmp = (char *) xmalloc( 100 + strlen( fmt ) + ( w->x.size*w->y.size ) );
	va_start( args, fmt );
	vsprintf( tmp, fmt, args );
	va_end( args );				/* TIDY ARGUMENT LIST */
	while ( tmp[i] != EOS && w->y.cursor < w->y.size )
		{while ( w->x.cursor < w->x.size && tmp[i] != EOS
				&& tmp[i] != EOL )
			{w->c[ w->y.cursor ][ w->x.cursor++ ] = tmp[ i++ ];
			}
		if ( tmp[i] != EOS )
			{if ( EOL == tmp[i] )
				{i++;
				}
			w->x.cursor = 0;
			w->y.cursor++;
			}
		}
	xfree( tmp );
}
/*---------------------------------------------------------------------------*/

WINDOW *newwin( int ys, int xs, int yo, int xo )
{	int	i, j;
	static	int	count = 0;
	WINDOW	*w;
	w = (WINDOW *) xmalloc( sizeof(WINDOW) );
	w->index = count++;
	if ( 0 != debug_pcurses )
		{inform( "newwin %d, %d %d %d %d", w->index, ys, xs, yo, xo );
		}
	if ( ( w->x.origin = xo ) < 0 || ( w->y.origin = yo ) < 0
			|| ( w->x.size = xs ) < 1 || ( w->y.size = ys ) < 1
			|| ( w->x.max = w->x.origin + w->x.size ) > 80
			|| ( w->y.max = w->y.origin + w->y.size ) > 24 )
		{failure( "newwin : invalid dimensions %d %d %d %d [ %d %d ]",
			w->y.size, w->x.size, w->y.origin, w->x.origin,
			w->y.max, w->x.max );
		}
	w->x.cursor = w->y.cursor = 0;
	w->attribute = 0;
	w->c = (char **) xmalloc( w->y.size * sizeof(char *) );
	for ( i = 0; i < w->y.size; i++ )
		{w->c[i] = (char *) xmalloc( w->x.size + 2 );
		for ( j = 0; j < w->x.size; j++ )
			{w->c[i][j] = SPACE;
			}
		w->c[i][j] = EOS;
		}
	return( w );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

void delwin( WINDOW *w )
{	int	i;
	if ( 0 != debug_pcurses )
		{inform( "\ndelwin %d", w->index );
		}
	for ( i = 0; i < w->y.size; i++ )
		{xfree( w->c[i] );
		}
	xfree( w->c );
	xfree( w );
}
/*---------------------------------------------------------------------------*/
#ifdef __BORLANDC__
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* SELECT BOLD TEXT COLOUR */
void pcurses_colour_bold( int col )
{	bold_text_colour = col;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* SELECT REVERSE TEXT COLOUR */
void pcurses_colour_reverse( int col )
{	reverse_text_colour = col;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* SELECT UNDERLINE TEXT COLOUR */
void pcurses_colour_underline( int col )
{	underline_text_colour = col;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* SELECT DEFAULT TEXT COLOUR */
void pcurses_colour_text( int tc )
{	textcolor( normal_text_colour = tc );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* SELECT BACKGROUND COLOUR */
void pcurses_colour_background( int bc )
{	textbackground( normal_back_colour = bc );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#endif
/*---------------------------------------------------------------------------*/
						/* START SCREEN HANDLING */
void initscr( void )
{	nruns++;
	if ( 0 == debug_pcurses )
		{
#ifdef __BORLANDC__
		normal_text_colour = NORMAL_TEXT_DEFAULT;
		normal_back_colour = NORMAL_BACK_DEFAULT;
		bold_text_colour = BOLD_TEXT_DEFAULT;
		reverse_text_colour = REVERSE_TEXT_DEFAULT;
		underline_text_colour = UNDERLINE_TEXT_DEFAULT;
		gettextinfo( &screen_defaults );
		textmode( C80 );
		textcolor( normal_text_colour );
		textbackground( normal_back_colour );
		clrscr();
#endif
		}
	else
		{inform( "initscr" );
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* TERMINATE SCREEN HANDLING */
void endwin( void )
{	if ( nruns < 1 )
		{failure( "endwin called before initscr" );
		}
	if ( 0 == debug_pcurses )
		{
#ifdef __BORLANDC__
		textmode( screen_defaults.currmode );	/* RESTORE INITIAL */
		textattr( screen_defaults.normattr );
		clrscr();
#endif
		}
	else
		{inform( "endwin" );
		}
}
/*---------------------------------------------------------------------------*/
#endif				/* END OF ALTERNATIVE TO USE_DEC_CURSES */
/*===========================================================================*/
#endif							/* XCURSES.C */
