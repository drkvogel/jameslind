#ifndef	XREQUEST_H_INCLUDED
#define	XREQUEST_H_INCLUDED	1
/*===========================================================================*/
/*	HEADER FOR REQUEST ROUTINES SUBSITUTIING FOR GRANI INTERFACE	*/
/*===========================================================================*/
#define	SCREEN_WIDTH		80
#define	REQ_MENU_MAX_FLAGS	10
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define	REQ_STATE_OK		0	/* NORMAL RETURN, ALL IS WELL!	*/
#define	REQ_STATE_QUIT		10	/* INSTRUCTION TO QUIT		*/
#define	REQ_STATE_QUIT_LO	12	/* FORCE QUIT - ANSWER TOO LOW	*/
#define	REQ_STATE_QUIT_HI	14	/* FORCE QUIT - ANSWER TOO HIGH	*/
#define	REQ_STATE_START		20	/* GO TO FIRST QUESTION		*/
#define	REQ_STATE_1UP		30	/* RETURN TO PREVIOUS QUESTION	*/
#define	REQ_STATE_BLANK		40	/* ANSWER WAS AN ALLOWED BLANK	*/
#define	REQ_STATE_INVALID	50	/* INVALID ANSWER, LIMITS UN-ENFORCED */
#define	REQ_STATE_ABCMENU	60	/* ABORT MENU ACTIVATED, BUT DISABLED */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* MACRO TO IMPLEMENT ABANDON AND GO-BACK OPTIONS */
#define	REQUEST_CHECK( p, q, r )	if ( REQ_STATE_QUIT == state	\
					|| REQ_STATE_QUIT_LO == state	\
					|| REQ_STATE_QUIT_HI == state )	\
					{p;				\
					}				\
				else if ( REQ_STATE_START == state )	\
					{q;				\
					}  				\
				else if ( REQ_STATE_1UP == state )	\
					{r;				\
					}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define	REQUEST_OK( p )			( ( REQ_STATE_OK == p ) ? 1 : 0 )
#define	REQUEST_BLANK( p )		( ( REQ_STATE_BLANK == p ) ? 1 : 0 )
#define	REQUEST_FATAL_LO( p )		( ( REQ_STATE_QUIT_LO == p ) ? 1 : 0 )
#define	REQUEST_FATAL_HI( p )		( ( REQ_STATE_QUIT_HI == p ) ? 1 : 0 )
#define	REQUEST_FATAL( q )	( REQUEST_FATAL_LO(q) || REQUEST_FATAL_HI(q) )
#define REQUEST_INVALID( q )		( ( REQ_STATE_INVALID == q ) ? 1 : 0 )
#define	REQUEST_ABCMENU( p )		( REQ_STATE_ABCMENU == p	    \
						|| REQ_STATE_START == p	\
						|| REQ_STATE_QUIT == p	\
						|| REQ_STATE_1UP == p )
/*---------------------------------------------------------------------------*/
typedef	char	*(*REQUEST_CHECK_FUNC)( char * );
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
typedef	struct				/* STORE OPTIONAL DEFAULT ANSWERS */
	{
	long	index;				/* INDEX FOR QUESTION	*/
	char	*ans;				/* ANSWER		*/
	}
	REQUEST_DEFAULT;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
typedef	struct				/* STORES MAIN MENU INFORMATION */
	{
	long	index;				/* INDEX FOR QUESTION	*/
	char	*ques;				/* QUESTION TEXT	*/
	char	*ans;				/* ANSWER		*/
	char	*etc;				/* SUPPLEMETARY ANSWER	*/
	REQUEST_CHECK_FUNC	chkfn;
	}
	REQUEST_QUESTION;
/*---------------------------------------------------------------------------*/
typedef	struct
	{
	char	*key;
	char	*txt;
	long	iflag[REQ_MENU_MAX_FLAGS];	/* INTEGER FLAGS FOR USER */
	}
	REQUEST_MENU_ITEM;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
typedef	struct
	{
	char	*title;
	int	nitems;
	int	auto_number_key;
	REQUEST_MENU_ITEM	*item;
	}
	REQUEST_MENU;
/*===========================================================================*/
#endif								/* XREQUEST.H */
