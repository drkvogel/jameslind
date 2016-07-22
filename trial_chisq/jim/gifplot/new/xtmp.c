#ifndef XREQUEST_C_INCLUDED
#define XREQUEST_C_INCLUDED     1
/*===========================================================================*/
/*      C REQUEST ROUTINES SUBSITUTIING FOR GRANI INTERFACE             */
/*===========================================================================*/
#include "xdefs.h"
#include "xinclude.h"
#include "xalloc.c"
#include "xtext.c"
#include "xsingkey.c"
#include "xcurses.c"
#include "xtime.c"
#include "xrequest.h"
/*---------------------------------------------------------------------------*/
#define QUESTION_MARK           '?'
#define QUESTION_MARK_ALT       126     /* SOME KEYBOARDS RETURN THIS FOR '?' */
/*---------------------------------------------------------------------------*/
static  int     debug_request = 0;
static  char    request_err_msg[202];           /* ERROR MESSAGE BUFFER */
static  char    response[202];                  /* INPUT BUFFER         */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
static  int     req_global_el_nset;
static  char    **req_global_el_set;
static  int     req_global_menu_nkey;
static  REQUEST_MENU_ITEM       *req_global_menu_key;
/*---------------------------------------------------------------------------*/
static  int             qplusa_width = 78;
static  int             q_width, a_width;
static  long            q_on_screen = 5;        /* OLD QUESTIONS ON SCREEN */
static  int             max_exp_lines = 3;
static  int             max_aux_lines;
static  int             space_is_control_char = NO;
static  int             use_side_cursors = NO;
static  int             draw_separators = NO;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
static	int		req_string_allow_empty;
static	char		*req_string_check_allow;
static	char		*req_string_check_exclude;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
static	char		req_yesnox_unknown;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
static	int		req_clock_require_seconds;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
static	int		req_limits_enforce = YES; /* ARE LIMITS ENFORCED */
static	int		req_limits_within = YES; /* WERE VALUES WITHIN LIMITS */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
static	long		ndefault	= 0;
static  long            nstore		= 0;
static  long            nstack		= 0;
static  long            nstack_max	= 0;
static  long            aux_buf_nalloc	= 0;
static  long            aux_buf_nlines	= 0;
static  char            **aux_buf	= NULL;
static	REQUEST_DEFAULT		*adefault = NULL;
static  REQUEST_QUESTION        *qstore   = NULL;
static  REQUEST_QUESTION        *qcurrent = NULL;
static  long                    *qsindex  = NULL;
static  WINDOW          *w_title;               /* TITLE                */
static  WINDOW          *w_quest;               /* CURRENT QUESTION     */
static  WINDOW          *w_exp;                 /* EXPLANATION          */
static  WINDOW          *w_aux;                 /* AUXILLARY TEXT       */
static  WINDOW          **w_stack;              /* PREVIOUS QUESTIONS   */
static  WINDOW          *w_sep[3];              /* SEPARATOR LINES      */
/*---------------------------------------------------------------------------*/
						/* SCROLL AUXILLARY WINDOW */
void request_scroll_aux( int direction )
{       int     i, max_page, offset;
	static  int     page = 0;
	max_page = ( aux_buf_nlines / ( max_aux_lines - 2 ) );
	if ( max_page * ( max_aux_lines - 2 ) == aux_buf_nlines )
		{max_page--;
		}
	switch( direction )
		{case -2:                               /* GO TO TOP */
			page = 0;
			break;
		case -1:                                /* PAGE UP */
			if ( --page < 0 )
				{page = max_page;
				}
			break;
		case 0:                                 /* SHOW CURRENT */
			break;
		case 1:                                 /* PAGE DOWN */
			if ( ++page > max_page )
				{page = 0;
				}
			break;
		case 2:                                 /* GO TO BOTTOM */
			page = max_page;
			break;
		default:
			bug( "request_scroll_aux %d", direction );
			break;
		}
	offset = page * ( max_aux_lines - 2 );
	wclear( w_aux );
	for ( i = 0; i < max_aux_lines - 2; i++ )
		{if ( i + offset < aux_buf_nlines )
			{wmove( w_aux, i, 0 );
			wprintw( w_aux, aux_buf[ i + offset ] );
			}
		}
	if ( max_page > 0 )
		{wmove( w_aux, max_aux_lines - 1, 0 );
		wprintw( w_aux, "This is page %2d/%d.   For more use  %s  %s",
			page + 1 , max_page + 1,
			( page > 0 ) ? "<PageUp>" : "        ",
			( page < max_page ) ? "  <PageDown>" : " " );
		}
	wrefresh( w_aux );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* DISPLAY AUXILLARY MESSAGE OF SINGLE SIMPLE STRING */
void request_show_aux( char *msg )
{       int     i;
	char    *copy;
	if ( NULL == msg )
		{request_scroll_aux( 0 );	/* REFRESH LAST-SEEN SCREEN */
		return;
		}
	for ( i = 0; i < aux_buf_nlines; i++ )
		{xfree( aux_buf[i] );		/* DISCARD OLD SPACE */
		}
	aux_buf_nlines = 0;
	while ( EOS != *msg )                   /* MAKE ARRAY FOR SCROLLING */
		{i = 0;
		copy = aux_buf[aux_buf_nlines] = (char *) xmalloc(
			SCREEN_WIDTH + 2 );
		while ( i < SCREEN_WIDTH && *msg != EOS && *msg != EOL )
			{copy[ i++ ] = *msg++;
			}
		copy[i] = EOS;
		if ( EOL == *msg )
			{msg++;
			}
		if ( ++aux_buf_nlines >= aux_buf_nalloc )
			{aux_buf_nalloc += 10;
			aux_buf = (char **) xrealloc( aux_buf,
				( aux_buf_nalloc + 2 ) * sizeof(char *) );
			}
		}
	request_scroll_aux( -2 );	/* SHOW MESSAGE, STARTING AT TOP */
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* DISPLAY AUXILLARY MESSAGE */
void request_vshow_aux( char *msg, ... )
{       char    *buf;
	va_list args;                           /* VARIABLE ARGUMENT LIST */
	buf = (char *) xmalloc( ( SCREEN_WIDTH * max_aux_lines )
		+ strlen( msg ) );
	va_start( args, msg );
	vsprintf( buf, msg, args );
	va_end( args );                         /* TIDY ARGUMENT LIST */
	request_show_aux( buf );
	xfree( buf );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* DISPLAY TEMPORARY AUXILLARY MESSAGE */
void request_vshow_aux_tmp( char *msg, ... )
{       char    *buf;
	va_list args;                           /* VARIABLE ARGUMENT LIST */
	wclear( w_aux );
	if ( NULL != msg )
		{buf = (char *) xmalloc( ( SCREEN_WIDTH * max_aux_lines )
			+ strlen( msg ) );
		va_start( args, msg );
		vsprintf( buf, msg, args );
		va_end( args );                 /* TIDY ARGUMENT LIST */
		wprintw( w_aux, buf );
		xfree( buf );
		}
	wrefresh( w_aux );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* DISPLAY AN EXPLANATORY MESSAGE BELOW QUESTION */
void request_show_exp( char *msg )
{       wclear( w_exp );
	wprintw( w_exp, msg );
	wrefresh( w_exp );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* DISPLAY AN EXPLANATORY MESSAGE BELOW QUESTION */
void request_vshow_exp( char *msg, ... )
{       char    *buf;
	va_list args;                           /* VARIABLE ARGUMENT LIST */
	buf = (char *) xmalloc( (SCREEN_WIDTH * max_exp_lines) + strlen(msg) );
	va_start( args, msg );
	vsprintf( buf, msg, args );
	va_end( args );
	request_show_exp( buf );
	xfree( buf );
}
/*---------------------------------------------------------------------------*/
					/* SHOW MESSAGE FOR `GET OUT' CLAUSE */
static void continue_or_quit_msg( void )
{       request_vshow_aux_tmp( "\nSelect option from:\n"
		"\nA  Abandon entry completely (or Q to Quit)%s"
		"\npress <return> to continue randomization",
		( nstore < 2 ) ? " " :
		"\nB  go Back to Beginning to alter answers"
		"\nC  Change last answer (or UpCursor)" );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* OFFER `GET OUT' CLAUSES */
static int request_continue_or_quit( void )
{       int     yn, ynlow;
	int     code = -1;
	long    c;
	aside( "CONTINUE_OR_QUIT screen activated" );
	continue_or_quit_msg();
	do
		{c = singlekey();
		switch( c )
			{case 'a':
			case 'A':
			case 'q':
			case 'Q':
				request_vshow_aux_tmp(
"Are you sure you want to Quit and Abandon this entry ?    Answer Y or N" );
				do
					{yn = singlekey();
					ynlow = tolower( yn );
					}
					while ( 'y' != ynlow && 'n' != ynlow );
				if ( 'y' == ynlow )
					{code = REQ_STATE_QUIT;
					}
				else
					{continue_or_quit_msg();
					}
				break;
			case 'b':
			case 'B':
				if ( nstore > 1 )
					{code = REQ_STATE_START;
					request_show_aux( " " );
					}
				break;
			case 'c':
			case 'C':
			case SK_CHAR_UPCUR:
				if ( nstore > 1 )
					{code = REQ_STATE_1UP;
					request_show_aux( " " );
					}
				break;
			case SK_CHAR_RETURN:
				code = 0;
				request_show_aux( NULL );  /* RESTORE SCREEN */
				break;
			default:
				break;
			}
		}
		while ( code < 0 );
	aside( "CONTINUE_OR_QUIT code=%ld", code );
	return( code );
}
/*---------------------------------------------------------------------------*/
				/* SHOULD ABSOLUTE LIMITS BE ENFORCED */
void request_limits_enforce( int enforce )
{	if ( 0 != enforce )
		{aside( "request: LIMITS, being enforced" );
		req_limits_enforce = YES;
		}
	else
		{aside( "request: LIMITS, disabled - NOT enforced" );
		req_limits_enforce = NO;
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* HAVE LIMITS BEEN OBEYED (WHEN NOT ENFORCED) */
int request_limits_within( void )
{	return( req_limits_within );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
	/* MODIFY RETURN STATUS ACCORDING TO LIMIT ENFORCEMENT STRATEGY */
int request_limits_invalid( int state )
{	int	out_state;
	if ( req_limits_enforce )
		{aside( "LIMITS violated, enforcing" );
		out_state = state;
		}
	else			/* ABSOLUTE LIMITS NOT BEING ENFORCED */
		{aside( "LIMITS violated, NOT enforcing" );
		out_state = REQ_STATE_INVALID;
		}
	return( out_state );
}
/*---------------------------------------------------------------------------*/
							/* CREATE A NEW MENU */
REQUEST_MENU *request_menu_init( char *titl )
{       REQUEST_MENU    *r;
	r = (REQUEST_MENU *) xmalloc( sizeof(REQUEST_MENU) );
	r->title = (char *) xmalloc( strlen( titl ) + 2 );
	strcpy( r->title, titl );                       /* STORE TITLE */
	r->nitems = 0;
	r->item = (REQUEST_MENU_ITEM *) xmalloc( sizeof(REQUEST_MENU_ITEM) );
	return( r );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
							/* ADD ITEM TO MENU */
void request_menu_add_item( REQUEST_MENU *r, char *mkey, char *mtxt )
{       if ( NULL == r->item )
		{bug( "request_menu_add_item" );
		}
	if ( 0 == r->nitems % 20 )
		{r->item = (REQUEST_MENU_ITEM *) xrealloc( r->item,
			( r->nitems+22 ) * sizeof(REQUEST_MENU_ITEM) );
		}
	if ( 0 == r->nitems )
		{r->auto_number_key = ( NULL == mkey );
		}
	if ( ( NULL == mkey && ! r->auto_number_key )
			|| ( NULL != mkey && r->auto_number_key ) )
		{bug( "inconsistent key (some auto, some not) in menu %s",
			r->title );
		}
	if ( r->auto_number_key )
		{r->item[ r->nitems ].key = (char *) xmalloc( 10 );
		sprintf( r->item[ r->nitems ].key, "%ld", r->nitems + 1 );
		}
	else
		{r->item[ r->nitems ].key = duplicate_string( skip_space(mkey));
		}
	r->item[ r->nitems ].txt = duplicate_string( skip_space( mtxt ) );
	r->nitems++;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* EXTRACT KEY BY INDEX FROM MENU */
void request_menu_copy_key( char *out, REQUEST_MENU *r, int n )
{       if ( NULL == out )
		{bug( "request_menu_copy_key %s, NULL out string", r->title );
		}
	if ( n < 0 || n >= r->nitems )
		{bug( "request_menu_copy_key %s, index %ld out of array [%ld]",
			r->title, n, r->nitems );
		}
	strcpy( out, r->item[ n ].key );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* EXTRACT KEY BY INDEX FROM MENU */
void request_menu_copy_txt( char *out, REQUEST_MENU *r, int n )
{       if ( NULL == out )
		{bug( "request_menu_copy_txt %s, NULL out string", r->title );
		}
	if ( n < 0 || n >= r->nitems )
		{bug( "request_menu_copy_txt %s, index %d out of array [%ld]",
			r->title, n, r->nitems );
		}
	strcpy( out, r->item[ n ].txt );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* RETURN VALUE OF A MENU-ITEM FLAG */
long request_menu_get_iflag( REQUEST_MENU *r, int ni, int nf )
{	if ( ni < 0 || ni >= r->nitems || nf < 0 || nf >= REQ_MENU_MAX_FLAGS )
		{bug( "request_menu_get_iflag %s %d %ld", r->title, ni,
			r->nitems );
		return( -1 );
		}
	return( r->item[ ni ].iflag[ nf ] );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* SET VALUE OF A MENU-ITEM FLAG */
void request_menu_set_iflag( REQUEST_MENU *r, int ni, int nf, long ival )
{	if ( -1 == ni )
		{ni = r->nitems - 1;	/* SHORT-HAND FOR HIGHEST ENTRY INDEX */
		}
	if ( ni < 0 || ni >= r->nitems || nf < 0 || nf >= REQ_MENU_MAX_FLAGS )
		{bug( "request_menu_set_iflag %s %d %ld", r->title, ni,
			r->nitems );
		r->item[ ni ].iflag[ nf ] = -1;
		}
	r->item[ ni ].iflag[ nf ] = ival;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* CONSTRUCT A MENU FROM A FILE */
REQUEST_MENU *request_menu_load_file( char *titl, char *fname, char *filter,
		int auto_key )
{       int     len;
	char    *pbuf;
	char    *kbuf = NULL, *tbuf = NULL;
	char    buf[202];
	FILE    *fm;
	REQUEST_MENU    *rm;
	if ( NULL == ( fm = fopen( fname, "r" ) ) )
		{return( NULL );
		}
	rm = request_menu_init( titl );
	while ( EOF != read_line( fm, buf, 200 ) )
		{if ( NULL != filter && EOS != filter[0] )
			{if ( NULL == ( pbuf = filter_line( buf, filter ) ) )
				{continue;              /* SKIP THIS ENTRY */
				}
			}
		else
			{pbuf = buf;
			}
		len = 0;
		if ( YES != auto_key )                  /* READ KEY FROM FILE */
			{kbuf = skip_space( pbuf );
			if ( EOS == kbuf )
				{warning(
				"request_menu_load_file: missing key" );
				continue;                       /* CARRY ON */
				}
			while ( ! isspace( pbuf[len] ) && EOS != pbuf[len] )
				{len++;
				}
			pbuf[ len++ ] = EOS;
			}
		tbuf = skip_space( pbuf + len );
		if ( EOS == tbuf )
			{warning(
		"request_menu_load_file: premature end of txt after key %s",
				kbuf );
			continue;                       /* CARRY ON */
			}
		request_menu_add_item( rm, kbuf, tbuf );
		}
	return( rm );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* FREE SPACE USED BY MENU */
void request_menu_free( REQUEST_MENU *rm )
{       int     i;
	for ( i = 0; i < rm->nitems; i++ )
		{xfree( rm->item[ i ].key );
		xfree(  rm->item[ i ].txt );
		}
	xfree( rm->item );
	xfree( rm->title );
	xfree( rm );
}
/*---------------------------------------------------------------------------*/
			/* STORE TEXT IN ETC. FIELD OF CURRENT QUESTION */
void request_store_etc_current( char *etc_txt )
{	if ( NULL == etc_txt )
		{return;
		}
	qcurrent->etc = (char *) xrealloc( qcurrent->etc, strlen(etc_txt) + 2 );
	strcpy( qcurrent->etc, etc_txt );
	string_squeeze( qcurrent->etc );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* CLEAR ANSWER BUFFER FOR CURRENT QUESTION */
void request_clear_answer( int id )
{       int     nr = 0;
	while ( nr < nstore && id != qstore[nr].index ) /* SEE IF STORED */
		{nr++;
		}
	if ( nr >= nstore )
		{bug( "attempt to clear answer %d before asking question", id );
		}
	qstore[nr].ans[0] = qstore[nr].etc[0] = EOS;
	wmove( w_quest, 0, q_width + 1 );
	wclrtoeol( w_quest );
	wrefresh( w_quest );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* CLEAR ANSWER BUFFER FOR CURRENT QUESTION */
void request_clear_answer_current( void )
{       qcurrent->ans[0] = qcurrent->etc[0] = EOS;
	wmove( w_quest, 0, q_width + 1 );
	wclrtoeol( w_quest );
	wrefresh( w_quest );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* GET ANSWER TO QUESTION */
void request_get_answer( int *state, char *answer, char *single,
		int accept_blank )
{       int     i, sp, pos, len;
	long    c;
	long    ntries = 0;
	char    *err_msg;
	char    *ans;
	ans = qcurrent->ans;
	do
		{len = pos = strlen( ans );
		if ( 0 == len )                         /* MARK LOCATION */
			{wmove( w_quest, 0, q_width + 1 );
			wprintw( w_quest, "*" );
			wrefresh( w_quest );
			}
		do
			{c = singlekey();               /* GET CHARACTERS */
			if ( TAB == c )
				{c = SPACE;     /* STANDARDIZE WHITE-SPACE */
				}
			if ( YES == space_is_control_char && SPACE == c )
				{request_scroll_aux( 1 );
				}
			else if ( SK_CHAR_ESC == c || QUESTION_MARK == c
					|| QUESTION_MARK_ALT == c )
				{if ( 0 != ( *state
						= request_continue_or_quit() ) )
					{return;        /* GRANI ? MENU */
					}
				}
			else if ( c > 0 && isprint( c ) )
				{if ( NULL != single )
					{c = tolower( c );
					for ( i = 0; i < strlen( single ); i++ )
						{if ( tolower( single[i] ) == c)
							{len = 1;
							ans[0] = single[i];
							ans[1] = EOS;
							wmove( w_quest, 0,
								q_width + 1 );
							wprintw( w_quest, "%s",
								ans );
							wrefresh( w_quest );
							goto REQUEST_GOT_ANSWER;
							}
						}
					request_vshow_aux(
						"enter one of characters: %s",
						single );
					}
				else
					{if ( len < a_width )
						{for ( i = len; i >= pos; i-- )
							{ans[i+1] = ans[i];
							}
						ans[ pos ] = (char) c;
						len++;
						pos++;
						}
					else
						{request_vshow_aux(
			"answers cannot be more than %d characters long",
							a_width );
						}
					}
				}
			else
				{switch( c )
					{case SK_CHAR_DELETE:
						if ( pos > 0 )
							{for ( i = pos-1;
								i < len; i++ )
								{ans[i]
								= ans[i+1];
								}
							len--;
							pos--;
							}
						break;
					case SK_CHAR_RIGHTCUR:
						if ( YES == use_side_cursors
								&& pos < len )
							{pos++;
							}
						break;
					case SK_CHAR_LEFTCUR:
						if ( YES == use_side_cursors
								&& pos > 0 )
							{pos--;
							}
						break;
					case SK_CHAR_UPCUR:
						request_show_aux( " " );
						*state = REQ_STATE_1UP;
						return;         /*NOTREACHED*/
						break;
					case SK_CHAR_HOME:
						request_scroll_aux( -2 );
						break;
					case SK_CHAR_PAGEUP:
						request_scroll_aux( -1 );
						break;
					case SK_CHAR_PAGEDOWN:
						request_scroll_aux( 1 );
						break;
					case SK_CHAR_END:
						request_scroll_aux( 2 );
						break;
					default:
						break;
					}
				}
			sp = 0;                 /* REMOVE ANY LEADING SPACES */
			while ( SPACE == ans[sp] )
				{sp++;
				}
			if ( sp > 0 )                   /* SHUFFLE UP */
				{for ( i = sp; i <= len; i++ )
					{ans[ i-sp ] = ans[i];
					}
				len -= sp;
				pos = ( pos >= sp ) ? pos - sp : 0;
				}
			wmove( w_quest, 0, q_width + 1 );
			wprintw( w_quest, "%s ", ans );
			wrefresh( w_quest );
			wmove( w_quest, 0, q_width + pos - 2 );
			}
			while ( c != SK_CHAR_RETURN && c != SK_CHAR_DOWNCUR );
REQUEST_GOT_ANSWER:
		while ( len > 0 && isspace( ans[len-1] ) )
			{len--;		/* REMOVE ANY TRAILING SPACES */
			}
		ans[len] = EOS;
		if ( ( len > 0 || accept_blank != YES ) &&
				NULL != qcurrent->chkfn )
			{err_msg = qcurrent->chkfn( ans );
			}
		else
			{err_msg = NULL;
			}
		ntries++;
		if ( NULL != err_msg )
			{if ( (char *) 1 != err_msg )
				{request_show_aux( err_msg );
				aside( "\tREJECT %d \"%s\" as \"%s\"", ntries,
					ans, err_msg );
				}
			else
				{aside( "\tREJECT %d \"%s\"", ntries, ans );
				}
			}
		len = strlen( ans );    /* ALLOW FOR CHANGES BY CHECK FUNC */
		if ( pos > len )
			{pos = len;
			}
		}
		while ( NULL != err_msg );
	strcpy( answer, qcurrent->ans );
	aside( "\tACCEPT %d \"%s\"", ntries, answer );
	request_show_aux( " " );
	*state = ( EOS != answer[0] ) ? 0 : REQ_STATE_BLANK;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
		/* WRITE INTERNAL QUESTION STORAGE TO FILE FOR DEBUGGING */
void request_debug_questions( void )
{	long	i;
	static	long	ncalls = 0;
	aside( "\n---------- DEBUG QUESTIONS %ld ----------\n\n STORE %ld",
		ncalls++, nstore );
	for ( i = 0; i < nstore; i++ )
		{aside( "[%ld]\t\t%4ld\t%-30.30s\t%-10.10s", i, qstore[i].index,
			qstore[i].ques, qstore[i].ans );
		}
	aside( "STACK", nstack );
	for ( i = 0; i < nstack; i++ )
		{aside( "[%ld]\t%4ld\t%4ld\t%-30.30s", i, qsindex[i],
			qstore[ qsindex[i] ].index, qstore[ qsindex[i] ].ques );
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
		/* ALLOW USER PROGRAMS TO RETRIEVE QUESTION STACK HISTORY */
void request_gethistory_question( int nback, char *qout )
{	if ( nback < 0 )
		{sprintf( qout, "ERROR, request_history_questions, nback = %d",
			nback );
		}
	else if ( 0 == nstack )
		{sprintf( qout, "[Q-%d]\tno questions stored", nback );
		}
	else if ( nback >= nstack )
		{sprintf( qout, "[Q-%d]\tnot asked", nback );
		}
	else
		{sprintf( qout, "[Q-%d]\t%-60.60s\nANSWER\t%-30.30s", nback,
			qstore[ qsindex[ nstack-nback-1 ] ].ques,
			qstore[ qsindex[ nstack-nback-1 ] ].ans );
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* DISPLAY QUESTION STACK */
void request_show_qstack( void )
{       int     i, offset, margin, nq;
	WINDOW	*wsi;
	REQUEST_QUESTION	*q;
	offset = ( nstack > q_on_screen ) ? nstack - q_on_screen - 1: 0;
	for ( i = 0; i < q_on_screen; i++ )
		{nq = offset + i;
		wsi = w_stack[i];
		wclear( wsi );
		if ( nq < nstack - 1 )
			{q = qstore + qsindex[nq];
			wprintw( wsi, "%-*.*s %s", q_width, q_width,
				q->ques, q->ans );
			if ( EOS != q->etc[0] )
				{margin = SCREEN_WIDTH - ( q_width
					+ strlen( q->ans ) + 6 );
				if ( strlen( q->etc ) > margin )
					{wprintw( wsi, "   (%*.*s..",
						margin - 1, margin - 1,
						q->etc );
					}
				else
					{wprintw( wsi, "   (%s)", q->etc );
					}
				}
			}
		wrefresh( wsi );
		}
	wclear( w_quest );
	wprintw( w_quest, "%-*.*s %s", q_width, q_width, qcurrent->ques,
		qcurrent->ans );
	wrefresh( w_quest );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* HAS A DEFAULT ANSWER BEEN DEFINED ? */
int request_ask_question_default( int idnum )
{	int	i;
	for ( i = 0; i < ndefault; i++ )
		{if ( idnum == adefault[i].index )
			{return( i );
			}
		}
	return( -1 );					/* NOT FOUND */
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* HAS A QUESTION ALREADY BEEN STORED ? */
int request_ask_question_instore( int idnum )
{	int	i;
	for ( i = 0; i < nstore; i++ )
		{if ( idnum == qstore[i].index )
			{return( i );
			}
		}
	return( -1 );					/* NOT FOUND */
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* IS A QUESTION ALREADY IN THE DISPLAY STACK ? */
int request_ask_question_instack( int idnum )
{	int	i;
	for ( i = 0; i < nstack; i++ )
		{if ( idnum == qstore[ qsindex[i] ].index )
			{return( i );
			}
		}
	return( -1 );					/* NOT FOUND */
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* MAIN QUESTION ASKING ROUTINE */
int request_ask_question( int id, char *qu, REQUEST_CHECK_FUNC fn )
{       int     nk, nr, deflt;
	if ( ( nr = request_ask_question_instore( id ) ) < 0 )
		{if ( 0 == nstore % 10 )
			{qstore = (REQUEST_QUESTION *) xrealloc( qstore,
				( nstore + 12 ) * sizeof(REQUEST_QUESTION) );
			}
		qcurrent = qstore + nstore;	  /* FIRST TIME */
		qsindex[ nstack ] = nstore;
		qcurrent->index = id;
		qcurrent->ques = (char *) xmalloc( strlen( qu ) + 2 );
		strcpy( qcurrent->ques, qu );
		qcurrent->ans = (char *) xmalloc( a_width + 22 );
		qcurrent->etc = (char *) xmalloc( 2 );
		qcurrent->ans[0] = qcurrent->etc[0] = EOS;
		qcurrent->chkfn = fn;
		nstack++;
		nstore++;
		deflt = request_ask_question_default( id );
		if ( deflt >= 0 )
			{strcpy( qcurrent->ans, adefault[deflt].ans );
			}
		}
	else					  /* STORED PREVIOUSLY */
		{qcurrent = qstore + nr;
		if ( ( nk = request_ask_question_instack( id ) ) < 0 )
			{qsindex[ nstack  ] = nr;	/* EXTEND STACK */
			nstack++;
			}
		else					/* TRUNCATE STACK */
			{nstack = nk + 1;
			}
		}
	if ( nstack >= nstack_max )
		{nstack_max += 10;
		qsindex = (long *) xrealloc( qsindex, ( nstack_max + 2 )
			* sizeof(long) );
		}
	aside( "ASK %d \"%s\"", qcurrent->index, qcurrent->ques );
	request_show_qstack();          /* DISPLAY QUESTION LIST */
	if ( 2 & debug_request )
		{request_debug_questions();
		}
	return( 0 );
}
/*---------------------------------------------------------------------------*/
					/* VERIFY ANSWER TO PREVIOUS QUESTION */
void request_verify( int *v_status, int *v_answer )
{       int     c;
	*v_status = 0;
	*v_answer = -1;
	while ( 0 == *v_status && -1 == *v_answer )
		{c = tolower( singlekey() );
		switch( c )
			{case 'y':
			case 'Y':
				*v_answer = YES;
				break;
			case 'n':
			case 'N':
				*v_answer = NO;
				break;
			case SK_CHAR_ESC:
			case '?':
				*v_status = request_continue_or_quit();
				break;
			default:
				break;
			}
		}
	request_show_aux( NULL );
}
/*---------------------------------------------------------------------------*/
					/* PRE-LOAD A DEFAULT FOR A QUESTION */
void request_default( int nq, char *default_answer )
{	int	pos, len;
	REQUEST_DEFAULT	*rd;
	if ( NULL == default_answer )
		{aside( "ERROR, request_default has NULL argument" );
		return;
		}
	pos = request_ask_question_default( nq );
	len = strlen( default_answer );
	if ( len > a_width )
		{aside( "request_default, truncating from %d to %d chars",
			nq, (int) a_width );
		len = a_width;
		}
	if ( pos < 0 )					/* NEW ENTRY */
		{if ( NULL == adefault )
			{adefault = (REQUEST_DEFAULT *) xmalloc(
				12 * sizeof(REQUEST_DEFAULT) );
			}
		else if ( 0 == ndefault % 10 )
			{adefault = (REQUEST_DEFAULT *) xrealloc( adefault,
				( ndefault + 12 ) * sizeof(REQUEST_DEFAULT) );
			}
		rd = adefault + ndefault;
		rd->index = nq;
		rd->ans = (char *) xmalloc( len + 2 );
		ndefault++;
		}
	else				/* OVER-WRITE PREVIOUS ENTRY */
		{if ( pos >= ndefault || adefault[pos].index != nq )
			{bug( "request_default, bad pos" );
			}
		rd = adefault + pos;
		rd->ans = (char *) xrealloc( rd->ans, len + 2 );
		}
	strncpy( rd->ans, default_answer, len );
	rd->ans[len] = EOS;
}
/*---------------------------------------------------------------------------*/
				/* CHECK THAT ANSWER HAS VALID FORMAT */
char *request_yn_check( char *w )
{       int     len;
	len = strlen( w );
	string_tolower( w );
	if ( len > 0 && 0 == strncmp( "yes", w, len ) )
		{strcpy( w, "yes" );
		}
	else if ( len > 0 && 0 == strncmp( "no", w, len ) )
		{strcpy( w, "no" );
		}
	else
		{return( "answer must be either Y or N" );
		}
	return( NULL );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
							/* GET YES/NO ANSWER */
void request_yesno( int nq, int *yn_status, char *question, int *yn_answer,
		char *explain_msg, int allow_blank )
{       char    answer[30];
	request_show_exp( ( NULL != explain_msg ) ? explain_msg
		: "enter Y for Yes or N for No" );
	request_ask_question( nq, question, request_yn_check );
	request_get_answer( yn_status, answer, "yn", allow_blank );
	switch( answer[0] )
		{case 'n':
			*yn_answer = 0;
			break;
		case 'y':
			*yn_answer = 1;
			break;
		default:
			*yn_answer = -1;
			if ( 0 == *yn_status )
				{bug( "request_yesno \"%s\"", answer );
				}
			break;
		}
}
/*---------------------------------------------------------------------------*/
				/* CHECK THAT ANSWER HAS VALID FORMAT */
char *request_ynx_check( char *w )
{       int     len;
	static	char	bad[50];
	len = strlen( w );
	string_tolower( w );
	if ( len > 0 && 0 == strncmp( "yes", w, len ) )
		{strcpy( w, "yes" );
		}
	else if ( len > 0 && 0 == strncmp( "no", w, len ) )
		{strcpy( w, "no" );
		}
	else if ( len != 1 || w[0] != req_yesnox_unknown )
		{sprintf( bad, "answer must be either Y or N or %c",
			toupper( req_yesnox_unknown ) );
		return( bad );
		}
	return( NULL );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* GET YES~NO~UNKNOWN ANSWER */
void request_yesnox( int nq, int *ynx_status, char *question, int *ynx_answer,
		char x_char, char *explain_msg )
{	char	ans_char;
	char    accept[5], answer[30];
	req_yesnox_unknown = ( 0 != x_char ) ? tolower( x_char ) : 'x';
	sprintf( accept, "yn%c", req_yesnox_unknown );
	request_show_exp( ( NULL != explain_msg ) ? explain_msg
		: ( ( 0 != x_char )
			? accept
			: "enter Y for Yes, N for No, or X for Unknown" ) );
	request_ask_question( nq, question, request_ynx_check );
	request_get_answer( ynx_status, answer, accept, NO );
	ans_char = answer[0];
	if ( req_yesnox_unknown == ans_char )
		{*ynx_answer = -1;
		}
	else if ( 'n' == ans_char )
		{*ynx_answer = 0;
		}
	else if ( 'y' == ans_char )
		{*ynx_answer = 1;
		}
	else if ( 0 == *ynx_status )
		{bug( "request_ynx, \"%s\"", answer );
		}
}
/*---------------------------------------------------------------------------*/
				/* CHECK THAT ANSWER HAS VALID FORMAT */
char *request_sex_check( char *w )
{       int     len;
	len = strlen( w );
	string_tolower( w );
	if ( len > 0 && 0 == strncmp( "male", w, len ) )
		{strcpy( w, "male" );
		}
	else if ( len > 0 && 0 == strncmp( "female", w, len ) )
		{strcpy( w, "female" );
		}
	else
		{return( "answer must be either M or F" );
		}
	return( NULL );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* GET MALE=0, FEMALE=1 ANSWER */
void request_sex( int nq, int *sx_status, char *question, int *sx_answer,
		char *explain_msg, int allow_blank )
{       char    answer[SCREEN_WIDTH];
	request_show_exp( ( NULL != explain_msg ) ? explain_msg
		: "enter M for male or F for female" );
	request_ask_question( nq, question, request_sex_check );
	request_get_answer( sx_status, answer, "mf", allow_blank );
	switch( answer[0] )
		{case 'm':
			*sx_answer = 0;
			break;
		case 'f':
			*sx_answer = 1;
			break;
		default:
			*sx_answer = -1;
			if ( 0 == *sx_status )
				{bug( "request_sex, \"%s\"", answer );
				}
			break;
		}
}
/*---------------------------------------------------------------------------*/
				/* CHECK THAT ANSWER HAS VALID FORMAT */
char *request_string_check( char *w )
{       int	i = 0;
	if ( NULL == w )
		{bug( "request_string_check" );
		}
	if ( YES != req_string_allow_empty && EOS == w[0] )
		{return( "answer cannot be empty" );
		}
	while ( EOS != w[i] )		/* ACCEPT ONLY GENERIC CHARS IN FMT */
		{if ( ! allow_character( w[i], req_string_check_allow )
				|| ( NULL != req_string_check_exclude
				&& allow_character( w[i],
				req_string_check_exclude ) ) )
			{sprintf( request_err_msg,
				"illegal character \"%c\" in answer \"%s\"",
				w[i], w );
			return( request_err_msg );
			}
		i++;
		}
	return( NULL );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* GET SINGLE WORD ANSWER */
void request_string( int nq, int *status, char *question, char *answer,
		char *explain_msg, int allow_empty, char *allow, char *exclude )
{       char	*default_explan;
	req_string_allow_empty   = allow_empty;
	req_string_check_allow   = allow;
	req_string_check_exclude = exclude;
	default_explan = "enter text";
	if ( NULL != allow && 1 == strlen( allow ) )
		{if ( isdigit( allow[0] ) )
			{default_explan = "enter digits 0-9 only";
			}
		else if ( isalpha( allow[1] ) )
			{default_explan = "enter letters A-Z only";
			}
		}
	request_show_exp( ( NULL != explain_msg ) ? explain_msg
		: default_explan );
	request_ask_question( nq, question, request_string_check );
	request_get_answer( status, answer, NULL, NO );
}
/*---------------------------------------------------------------------------*/
				/* CHECK THAT ANSWER HAS VALID FORMAT */
char *request_int_check( char *w )
{       long    n;
	char    *tmp;
	tmp = w;
	if ( '-' == tmp[0] )
		{tmp++;
		}
	if ( EOS == *tmp )
		{return( "answer cannot be empty" );
		}
	while ( EOS != *tmp )
		{if ( ! isdigit( *tmp ) )
			{sprintf( request_err_msg,
			"illegal character \"%c\" in numeric answer \"%s\","
		"\n\nonly characters `-' and 0-9 are allowed in whole numbers",
				*tmp, w );
			request_clear_answer_current();
			return( request_err_msg );
			}
		tmp++;
		}
	n = atol( w );
	sprintf( w, "%ld", n );         /* ENSURE BUFFERS ALL AGREE */
	return( NULL );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* GET INTEGER ANSWER */
void request_int( int nq, int *status, char *question, int *answer,
		char *explain_msg,
		int low_abs, char *la_msg,
		int low_opt, char *lo_msg,
		int upp_opt, char *uo_msg,
		int upp_abs, char *ua_msg,
		int allow_blank )
{       int     verify_answer;
	if ( low_abs > low_opt || low_opt > upp_opt || upp_opt > upp_abs )
		{bug( "request_int: bad bounding parameters %d %d %d %d",
			low_abs, low_opt, upp_opt, upp_abs );
		}
	request_show_exp( ( NULL != explain_msg ) ? explain_msg
		: "question requires a `whole number' answer" );
	request_ask_question( nq, question, request_int_check );
	while ( 1 )
		{request_get_answer( status, response, NULL, allow_blank );
		if ( *status != 0 )
			{return;
			}
		*answer = atol( response );
		if ( *answer < low_abs )                /* STRONG CHECK */
			{request_vshow_aux_tmp( ( NULL == la_msg )
				? "Answer %d is invalid"
		"\n\nPatients cannot be entered with values less than %d"
				"\n\nIs this value correct ?    Y or N"
				: la_msg,
				*answer, low_abs );
			request_verify( status, &verify_answer );
			if ( REQ_STATE_QUIT == *status
					|| REQ_STATE_START == *status )
				{return;
				}
			if ( 1 == verify_answer )	/* BAD ANSWER IS TRUE */
				{*status = request_limits_invalid(
					REQ_STATE_QUIT_LO );
				return;
				}
			request_clear_answer_current();
			}
		else if ( *answer > upp_abs )
			{request_vshow_aux_tmp( ( NULL == ua_msg )
			? "Answer %d is invalid"
		"\n\nPatients cannot be entered with values more than %d"
				"\n\nIs this value correct ?    Y or N"
				: ua_msg,
				*answer, upp_abs );
			request_verify( status, &verify_answer );
			if ( REQ_STATE_QUIT == *status
					|| REQ_STATE_START == *status )
				{return;
				}
			if ( 1 == verify_answer )	/* BAD ANSWER IS TRUE */
				{*status = request_limits_invalid(
					REQ_STATE_QUIT_HI );
				return;
				}
			request_clear_answer_current();
			}
		else if ( *answer < low_opt )           /* WEAK CHECK */
			{request_vshow_aux_tmp( ( NULL == lo_msg )
	? "A value of %d is surprisingly low - is this correct ?  Y or N"
				: lo_msg,
				*answer );
			request_verify( status, &verify_answer );
			if ( *status != 0 || 1 == verify_answer )
				{return;
				}
			request_clear_answer_current();
			}
		else if ( *answer > upp_opt )
			{request_vshow_aux_tmp( ( NULL == uo_msg )
	? "A value of %d is surprisingly high - is this correct ?  Y or N"
				: uo_msg,
				*answer );
			request_verify( status, &verify_answer );
			if ( *status != 0 || 1 == verify_answer )
				{return;
				}
			request_clear_answer_current();
			}
		else
			{return;
			}
		}
}
/*---------------------------------------------------------------------------*/
				/* CHECK THAT ANSWER HAS VALID FORMAT */
char *request_double_check( char *w )
{       int     n_dots = 0;
	double  d;
	char    *tmp;
	tmp = w;
	if ( '-' == tmp[0] )
		{tmp++;
		}
	if ( EOS == *tmp )
		{return( "answer cannot be empty" );
		}
	while ( EOS != *tmp )
		{if ( DOT == *tmp )
			{if ( ++n_dots > 1 )
				{sprintf( request_err_msg,
			"more than one `.' found in numeric answer \"%s\"",
					w );
				return( request_err_msg );
				}
			}
		else if ( ! isdigit( *tmp ) )
			{sprintf( request_err_msg,
			"illegal character \"%c\" in numeric answer \"%s\","
	"\n\nonly characters `-', `.' and 0-9 are allowed in numbers",
				*tmp, w );
			request_clear_answer_current();
			return( request_err_msg );
			}
		tmp++;
		}
	d = atof( w );
	sprintf( w, "%lg", d );         /* ENSURE BUFFERS ALL AGREE */
	return( NULL );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* GET DOUBLE ANSWER */
void request_double( int nq, int *status, char *question, double *answer,
		char *explain_msg,
		double low_abs, char *la_msg,
		double low_opt, char *lo_msg,
		double upp_opt, char *uo_msg,
		double upp_abs, char *ua_msg,
		int allow_blank )
{       int     verify_answer;
	if ( low_abs > low_opt || low_opt > upp_opt || upp_opt > upp_abs )
		{bug( "request_double: bad bounding parameters %lg %lg %lg %lg",
			low_abs, low_opt, upp_opt, upp_abs );
		}
	request_show_exp( ( NULL != explain_msg ) ? explain_msg
		: "question requires a numeric answer" );
	request_ask_question( nq, question, request_double_check );
	while ( 1 )
		{request_get_answer( status, response, NULL, allow_blank );
		if ( *status != 0 )
			{return;
			}
		*answer = atof( response );
		if ( *answer < low_abs )                /* STRONG CHECK */
			{request_vshow_aux( ( NULL == la_msg )
				? "Answer %lg is invalid"
		"\n\nPatients cannot be entered with values less than %lg"
				"\n\nIs this value correct ?    Y or N"
				: la_msg,
				*answer, low_abs );
			request_verify( status, &verify_answer );
			if ( REQ_STATE_QUIT == *status
					|| REQ_STATE_START == *status )
				{return;
				}
			if ( 1 == verify_answer )	/* BAD ANSWER IS TRUE */
				{*status = request_limits_invalid(
					REQ_STATE_QUIT_LO );
				return;
				}
			request_clear_answer_current();
			}
		else if ( *answer > upp_abs )
			{request_vshow_aux( ( NULL == ua_msg )
				? "Answer %lg is invalid"
		"\n\nPatients cannot be entered with values more than %lg"
				"\n\nIs this value correct ?    Y or N"
				: ua_msg,
				*answer, upp_abs );
			request_verify( status, &verify_answer );
			if ( REQ_STATE_QUIT == *status
					|| REQ_STATE_START == *status )
				{return;
				}
			if ( 1 == verify_answer )	/* BAD ANSWER IS TRUE */
				{*status = request_limits_invalid(
					REQ_STATE_QUIT_HI );
				return;
				}
			request_clear_answer_current();
			}
		else if ( *answer < low_opt )           /* WEAK CHECK */
			{request_vshow_aux_tmp( ( NULL == lo_msg )
	? "A value of %lg is surprisingly low - is this correct ?  Y or N"
				: lo_msg,
				*answer );
			request_verify( status, &verify_answer );
			if ( *status != 0 || 1 == verify_answer )
				{return;
				}
			request_clear_answer_current();
			}
		else if ( *answer > upp_opt )
			{request_vshow_aux_tmp( ( NULL == uo_msg )
	? "A value of %lg is surprisingly high - is this correct ?  Y or N"
				: uo_msg,
				*answer );
			request_verify( status, &verify_answer );
			if ( *status != 0 || 1 == verify_answer )
				{return;
				}
			request_clear_answer_current();
			}
		else
			{return;
			}
		}
}
/*---------------------------------------------------------------------------*/
							/* DISPLAY A DATE */
void request_date_show( FILE *fd, DATE *t )
{       fprintf( fd, "%ld/%ld/%ld", date_day( t ), date_month( t ),
		date_year( t ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* CHECK THAT ANSWER HAS VALID FORMAT */
char *request_date_check( char *w )
{       char    *msg;
	DATE    dt;
	string_to_date( w, &dt, &msg, YES );
	if ( NULL == msg )			/* STANDARDIZE DISPLAY FORMAT */
		{date_to_string( w, &dt, 0 );
		}
	return( msg );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
							/* GET A DATE */
void request_date( int nq, int *status, char *question, DATE *answer,
		char *explain_msg,
		DATE *low_abs, char *la_msg,
		DATE *low_opt, char *lo_msg,
		DATE *upp_opt, char *uo_msg,
		DATE *upp_abs, char *ua_msg,
		int allow_blank )
{       int     verify_answer;
	char    response[SCREEN_WIDTH];
	char    *msg;
	char	dbuf[20];
	request_show_exp( ( NULL != explain_msg ) ? explain_msg
	: "question requires a date entered as day/month/year, e.g. 23/4/56" );
	request_ask_question( nq, question, request_date_check );
	while ( 1 )
		{request_get_answer( status, response, NULL, allow_blank );
		if ( *status != 0 )
			{return;
			}
		string_to_date( response, answer, &msg, YES );
		if ( msg != NULL )
			{bug( "request_date" );
			}
		if ( date_cmp( answer, low_abs ) < 0 )  /* STRONG CHECK */
			{date_to_string( dbuf, low_abs, 0 );
			request_vshow_aux_tmp( ( NULL == la_msg )
			? "Date %s is not allowed"
			  "\n\nPatients cannot be entered with a date before %s"
			  "\n\nIs the date correct ?    Y or N"
				: la_msg, response, dbuf );
			request_verify( status, &verify_answer );
			if ( REQ_STATE_QUIT == *status
					|| REQ_STATE_START == *status )
				{return;
				}
			if ( 1 == verify_answer )	/* BAD ANSWER IS TRUE */
				{*status = request_limits_invalid(
					REQ_STATE_QUIT_LO );
				return;
				}
			request_clear_answer_current();
			}
		else if ( NULL != upp_abs && date_cmp( answer, upp_abs ) > 0 )
			{date_to_string( dbuf, upp_abs, 0 );
			request_vshow_aux_tmp( ( NULL == ua_msg )
			? "Date %s is not allowed"
			  "\n\nPatients cannot be entered with a date after %s"
			  "\n\nIs the date correct ?    Y or N"
				: ua_msg, response, dbuf );
			request_verify( status, &verify_answer );
			if ( REQ_STATE_QUIT == *status
					|| REQ_STATE_START == *status )
				{return;
				}
			if ( 1 == verify_answer )
				{*status = request_limits_invalid(
					REQ_STATE_QUIT_HI );
				return;
				}
			request_clear_answer_current();
			}
		else if ( date_cmp( answer, low_opt ) < 0 )     /* WEAK CHECK */
			{date_to_string( dbuf, low_opt, 0 );
			request_vshow_aux_tmp( ( NULL == lo_msg )
		? "Date %s is surprisingly early - is this correct ?  Y or N"
				: lo_msg, response, dbuf );
			request_verify( status, &verify_answer );
			if ( *status != 0 || 1 == verify_answer )
				{return;
				}
			request_clear_answer_current();
			}
		else if ( date_cmp( answer, upp_opt ) > 0 )
			{date_to_string( dbuf, upp_opt, 0 );
			request_vshow_aux_tmp( ( NULL == uo_msg )
		? "Date %s is surprisingly late - is this correct ?  Y or N"
				: uo_msg, response, dbuf );
			request_verify( status, &verify_answer );
			if ( *status != 0 || 1 == verify_answer )
				{return;
				}
			request_clear_answer_current();
			}
		else
			{return;
			}
		}
}
/*---------------------------------------------------------------------------*/
						/* DISPLAY A CLOCK TIME */
void request_clock_show( FILE *fd, CLOCK *t )
{       fprintf( fd, "%2d:%2.2d", clock_hour( t ), clock_minute( t ) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* CHECK THAT ANSWER HAS VALID FORMAT */
char *request_clock_check( char *w )
{       char    *msg;
	CLOCK    tm;
	string_to_clock( w, &tm, &msg, req_clock_require_seconds );
	if ( NULL == msg )			/* STANDARDIZE DISPLAY FORMAT */
		{clock_to_string( w, &tm, 0 );
		}
	return( msg );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
							/* GET A CLOCK TIME */
void request_clock( int nq, int *status, char *question, CLOCK *answer,
		char *explain_msg, 
		CLOCK *low_abs, char *la_msg,
		CLOCK *low_opt, char *lo_msg,
		CLOCK *upp_opt, char *uo_msg,
		CLOCK *upp_abs, char *ua_msg,
		int allow_blank,
		int require_seconds )
{	int	verify_answer;
	char    response[SCREEN_WIDTH];
	char    *msg;
	char	cbuf[20];
	req_clock_require_seconds = require_seconds;
	request_show_exp( ( NULL != explain_msg ) ? explain_msg
		: ( ( YES != req_clock_require_seconds )
? "question requires a 24-hour time entered as hour:minute, e.g. 14:52"
: "question requires a 24-hour time entered as hour:min:sec, e.g. 14:52:30" ) );
	request_ask_question( nq, question, request_clock_check );
	while ( 1 )
		{request_get_answer( status, response, NULL, allow_blank );
		if ( *status != 0 )
			{return;
			}
		string_to_clock( response, answer, &msg,
			req_clock_require_seconds );
		if ( msg != NULL )
			{bug( "request_string" );
			}
		if ( clock_cmp( answer, low_abs ) < 0 )  /* STRONG CHECK */
			{clock_to_string( cbuf, low_abs, 0 );
			request_vshow_aux_tmp( ( NULL == la_msg )
			? "Time %s is not allowed"
			  "\n\nPatients cannot be entered with a time before %s"
			  "\n\nIs the time correct ?    Y or N"
				: la_msg, response, cbuf );
			request_verify( status, &verify_answer );
			if ( REQ_STATE_QUIT == *status
					|| REQ_STATE_START == *status )
				{return;
				}
			if ( 1 == verify_answer )	/* BAD ANSWER IS TRUE */
				{*status = request_limits_invalid(
					REQ_STATE_QUIT_LO );
				return;
				}
			request_clear_answer_current();
			}
		else if ( NULL != upp_abs && clock_cmp( answer, upp_abs ) > 0 )
			{clock_to_string( cbuf, upp_abs, 0 );
			request_vshow_aux_tmp( ( NULL == ua_msg )
			? "Time %s is not allowed"
			  "\n\nPatients cannot be entered with a time after %s"
			  "\n\nIs the time correct ?    Y or N"
				: ua_msg, response, cbuf );
			request_verify( status, &verify_answer );
			if ( REQ_STATE_QUIT == *status
					|| REQ_STATE_START == *status )
				{return;
				}
			if ( 1 == verify_answer )
				{*status = request_limits_invalid(
					REQ_STATE_QUIT_HI );
				return;
				}
			request_clear_answer_current();
			}
		else if ( clock_cmp( answer, low_opt ) < 0 )     /* WEAK CHECK */
			{clock_to_string( cbuf, low_opt, 0 );
			request_vshow_aux_tmp( ( NULL == lo_msg )
		? "Tine %s is surprisingly early - is this correct ?  Y or N"
				: lo_msg, response, cbuf );
			request_verify( status, &verify_answer );
			if ( *status != 0 || 1 == verify_answer )
				{return;
				}
			request_clear_answer_current();
			}
		else if ( clock_cmp( answer, upp_opt ) > 0 )
			{clock_to_string( cbuf, upp_opt, 0 );
			request_vshow_aux_tmp( ( NULL == uo_msg )
		? "Time %s is surprisingly late - is this correct ?  Y or N"
				: uo_msg, response, cbuf );
			request_verify( status, &verify_answer );
			if ( *status != 0 || 1 == verify_answer )
				{return;
				}
			request_clear_answer_current();
			}
		else
			{return;
			}
		}
}
/*---------------------------------------------------------------------------*/
					/* FIND INDEX OF ELEMENT IN SET */
int request_element_index( char *el, int nmem, char **set )
{       int     i;
	if ( NULL == el || EOS == el[0] )
		{return( -1 );
		}
	for ( i = 0; i < nmem; i++ )
		{if ( 0 == strcmp_nocase( el, set[i] ) )
			{return( i );
			}
		}
	return( -1 );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* VALIDATE ELEMENT OF SET */
char *request_element_check( char *element )
{       int     index;
	if ( ( index = request_element_index( element, req_global_el_nset,
			req_global_el_set ) ) < 0 )
		{return( (char *) 1 );
		}
	strcpy( element, req_global_el_set[index] );    /* MAKE IDENTICAL */
	return( NULL );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* GET ELEMENT FROM SET OF OPTIONS */
void request_element( int nq, int *status, char *question, int *answer,
		char *explain_msg, char **set )
{       int     i, cols, len, nset, width;
	int     len_max = 1;
	char    *buf, *tmp;
	char    el_answer[SCREEN_WIDTH];
	nset = lines_of_text( set );
	if ( 1 & debug_request )
		{display_multi_line( "set", nset, set );
		}
	if ( nset < 2 )
		{bug( "request_element nset = %d", nset );
		}
	request_show_exp( ( NULL != explain_msg ) ? explain_msg
		: "Select one choice from the set listed below" );
	for ( i = 0; i < nset; i++ )
		{if ( ( len = strlen( set[i] ) ) > len_max )
			{len_max = len;
			}
		}
	width = len_max + 4;
	cols = ( SCREEN_WIDTH - 5 ) / width;
	tmp = (char *) xmalloc( width + 4 );
	buf = (char *) xmalloc( ( width + 1 ) * nset );
	buf[0] = EOS;
	for ( i = 0; i < nset; i++ )
		{if ( i > 0 && 0 == i % cols )
			{strcat( buf, "\n" );
			}
		sprintf( tmp, "   %-*.*s", len_max, len_max, set[i] );
		strcat( buf, tmp );
		}
	request_show_aux( buf );
	req_global_el_nset = nset;                      /* GLOBALIZE SET */
	req_global_el_set = set;
	space_is_control_char = YES;
	request_ask_question( nq, question, request_element_check );
	request_get_answer( status, el_answer, NULL, NO );
	space_is_control_char = NO;
	if ( 0 == *status )
		{if ( ( *answer = request_element_index( el_answer, nset,
				set ) ) < 0 )
			{bug( "request_element \"%s\"", el_answer );
			}
		}
	xfree( buf );
	xfree( tmp );
}
/*---------------------------------------------------------------------------*/
						/* FIND INDEX OF KEY IN MENU */
int request_menu_index( char *ky, int nk, REQUEST_MENU_ITEM *it )
{       int     i;
	if ( NULL == it )
		{return( -1 );
		}
	for ( i = 0; i < nk; i++ )
		{if ( 0 == strcmp_nocase( ky, it[i].key ) )
			{return( i );
			}
		}
	return( -1 );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* VALIDATE KEY IN MENU */
char *request_menu_check( char *key )
{       int     index;
	if ( ( index = request_menu_index( key, req_global_menu_nkey,
			req_global_menu_key ) ) < 0 )
		{return( (char *) 1 );
		}
	strcpy( key, req_global_menu_key[index].key );  /* MAKE IDENTICAL */
	return( NULL );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* GET ANSWER FROM A MENU */
void request_menu( int nq, int *status, char *question, int *answer,
		char *explain_msg, REQUEST_MENU *rm )
{       int     i, ncols, kl, tl, max_len;
	int     klen = 3, tlen = 5;
	char    *buf, *tmp;
	char    ky_answer[SCREEN_WIDTH];
	for ( i = 0; i < rm->nitems; i++ )
		{kl = string_squeeze( rm->item[i].key );
		tl  = string_squeeze( rm->item[i].txt );
		klen = ( kl > klen ) ? kl : klen;
		tlen = ( tl > tlen ) ? tl : tlen;
		}
	max_len = klen + tlen + 10;
	ncols = ( SCREEN_WIDTH - 2 ) / max_len;
	request_show_exp( ( NULL != explain_msg ) ? explain_msg
		: "Select one choice from the menu listed below" );
	tmp = (char *) xmalloc( max_len + 3 );
	buf = (char *) xmalloc( max_len * rm->nitems );
	buf[0] = EOS;
	for ( i = 0; i < rm->nitems; i++ )
		{sprintf( tmp, "%c%-*.*s  %-*.*s    ",
			( ncols < 1 || 0 == i % ncols ) ? EOL : SPACE,
			klen, klen, rm->item[i].key,
			tlen, tlen, rm->item[i].txt );
		strcat( buf, tmp );
		}
	request_show_aux( buf );
	req_global_menu_nkey = rm->nitems;              /* GLOBALIZE KEY */
	req_global_menu_key = rm->item;
	space_is_control_char = YES;
	request_ask_question( nq, question, request_menu_check );
	request_get_answer( status, ky_answer, NULL, NO );
	space_is_control_char = NO;
	if ( 0 == *status )
		{if ( ( *answer = request_menu_index( ky_answer, rm->nitems,
				rm->item ) ) < 0 )
			{bug( "request_menu \"%s\"", ky_answer );
			}
		request_store_etc_current( rm->item[*answer].txt );
		}
	xfree( buf );
	xfree( tmp );
}
/*---------------------------------------------------------------------------*/
					/* DISPLAY A MESSAGE TO BE READ OUT */
void request_display( int *status, char *explain_msg, char *main_msg )
{	int	c;
	char    *buf;
	request_show_exp( ( NULL != explain_msg ) ? explain_msg : " " );
	if ( NULL == main_msg )
		{bug( "request_display, NULL main_msg" );
		}
	buf = (char *) xmalloc( strlen( main_msg ) + 80 );
	sprintf( buf,
		" (Read out message below, press <SPACE> when finished)\n%s",
		main_msg );
	request_show_aux( buf );
	do
		{c = singlekey();               	/* GET CHARACTER */
		if ( SK_CHAR_ESC == c || QUESTION_MARK == c
				|| QUESTION_MARK_ALT == c )
			{*status = request_continue_or_quit();
			if ( 0 != *status )
				{break;
				}
			}
		}
		while ( SPACE != c );
	request_show_exp( " " );
	request_show_aux( " " );
	xfree( buf );
}
/*---------------------------------------------------------------------------*/
						/* DRAW SEPARATING LINES */
void request_separators( void )
{       int     i;
	int     yval[3] = { 1, 5, 13 };
	for ( i = 0; i < 3; i++ )
		{w_sep[i] = newwin( 1, 80, yval[i], 0 );
		}
	wprintw( w_sep[0], "---------------------------------------"
			"-----------------------------------------" );
	wprintw( w_sep[1], "======================================="
			"=========================================" );
	wprintw( w_sep[2], "---------------------------------------"
			"-----------------------------------------" );
	for ( i = 0; i < 3; i++ )
		{wrefresh( w_sep[i] );
		}
	draw_separators = YES;
}
/*---------------------------------------------------------------------------*/
						/* SET TITLE FOR SCREEN */
void request_set_title( char *titl, ... )
{       char    *buf;
	va_list args;                           /* VARIABLE ARGUMENT LIST */
	buf = (char *) xmalloc( SCREEN_WIDTH + strlen(titl) + 2 );
	va_start( args, titl );
	vsprintf( buf, titl, args );
	va_end( args );
	wclear( w_title );
	wprintw( w_title, " %s", buf );
	wrefresh( w_title );
	xfree( buf );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* INITIALIZE REQUEST SYSTEM */
void request_init( char *title, int qw )
{       int     i, aux_start;
	aside( "REQUEST_INIT \"%s\"", title );
	if ( nstack != 0 || nstore != 0 )
		{bug( "request_init: request_end not called previously" );
		}
	q_width = qw;
	a_width = qplusa_width - q_width;
	if ( a_width < 5 || q_width < 20 )
		{bug( "request_init: bad width q=%d a=%d", q_width, a_width );
		}	
	singlekey_init();
	initscr();
#if defined __unix__
	cbreak();
	noecho();
	nonl();
	intrflush( stdscr, FALSE );
	keypad( stdscr, TRUE );
#elif defined __BORLANDC__
	pcurses_colour_background( BLACK );
	pcurses_colour_text( LIGHTCYAN );
	pcurses_colour_bold( YELLOW );
	pcurses_colour_reverse( RED );
	pcurses_colour_underline( LIGHTGREEN );
#endif
	w_title = newwin( 1, 80, 0, 0 );
	request_set_title( title );
	qstore = (REQUEST_QUESTION *) xmalloc( 12 * sizeof(REQUEST_QUESTION) );
	if ( NULL == qsindex )
		{qsindex = (long *) xmalloc( 12 * sizeof(long) );
		nstack_max = 10;
		}

	w_exp = newwin( max_exp_lines, 80, 2, 0 );

	w_stack = (WINDOW **) xmalloc( q_on_screen * sizeof(WINDOW *) );
	for ( i = 0; i < q_on_screen; i++ )
		{w_stack[i] = newwin( 1, 80, i + max_exp_lines + 3, 0 );
		}
	w_quest = newwin( 1, 80, q_on_screen + max_exp_lines + 4, 0 );
	wsetattr( w_quest, _BOLD );
	aux_start = q_on_screen + max_exp_lines + 6;
	max_aux_lines = 24 - aux_start;
	w_aux = newwin( max_aux_lines, 80, aux_start, 0 );
	aux_buf_nalloc = 10;
	aux_buf = (char **) xmalloc( ( aux_buf_nalloc + 2 ) * sizeof(char *) );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* CLEAR HISTORY FROM STACK */
void request_stack_wipe( void )
{       long    i;
	for ( i = 0; i < q_on_screen; i++ )
		{wclear( w_stack[i] );
		wrefresh( w_stack[i] );
		}
	wclear( w_quest );
	wclear( w_exp );
	wclear( w_aux );
	wrefresh( w_quest );
	wrefresh( w_exp );
	wrefresh( w_aux );
	for ( i = 0; i < nstore; i++ )
		{xfree( qstore[i].ans );
		xfree( qstore[i].ques );
		}
	nstore = 0;
	nstack = 0;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
					/* FREE SPACE AND RESET GLOBALS */
void request_end( void )
{       int     i;
	for ( i = 0; i < q_on_screen; i++ )
		{delwin( w_stack[i] );
		}
	delwin( w_title );
	delwin( w_quest );
	delwin( w_exp );
	delwin( w_aux );
	if ( YES == draw_separators )
		{for ( i = 0; i < 3; i++ )
			{delwin( w_sep[i] );
			}
		}
	endwin();
	xfree( w_stack );
	for ( i = 0; i < nstore; i++ )
		{xfree( qstore[i].ans );
		xfree( qstore[i].ques );
		}
	xfree( qstore );
	xfree( qsindex );
	nstore = 0;
	nstack = 0;
	for ( i = 0; i < aux_buf_nlines; i++ )
		{xfree( aux_buf[i] );
		}
	xfree( aux_buf );
	aside( "REQUEST_END" );
	singlekey_end();
}
/*===========================================================================*/
#endif                                                          /* XREQUEST.C */
