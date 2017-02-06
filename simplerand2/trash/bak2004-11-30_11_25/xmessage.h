#ifndef XMESSAGE_H_INCLUDED
#define XMESSAGE_H_INCLUDED	1
/*===========================================================================*/
/*		HEADER FOR MESSAGE HANDLING UTILITIES			*/
/*===========================================================================*/
							/* SEND EMAIL */
int xemail( char *target, char *title, char *body );
						/* SEND MESSAGE BY EMAIL */
int xmessage_send_email( char *text );
				/* GET SINGLE-KEY OPTIONS FROM USER */
char ask_user( char *option );
				/* ASK USER WHETHER OR NOT TO CONTINUE */
void ask_continue( void );

void msg_byebye( int bstatus, char *errmess, ... );

void msg_blooper( char *bstatus, char *mess );

void aside( char *txt, ...  );

void inform( char *txt, ...  );
						/* GIVE WARNING MESSAGE */
void warning( char *txt, ...  );
						/* GIVE ERROR MESSAGE */
void error( char *txt, ...  );
					/* GIVE TERMINAL FAILURE MESSAGE */
void failure( char *txt, ...  );
					/* GIVE TERMINAL FAILURE MESSAGE */
void bug( char *txt, ...  );
					/* SET MESSAGE SCREEN-WRITE STATUS */
void xmessage_control_screen( int to_screen );
						/* SAY IF LOGFILE OPEN */
int xmessage_logfile_active( void );
					/* SET ADDRESS FOR MESSAGE LOGGING */
int xmessage_control_logfile( char *log_file, char *mode );
					/* SET ADDRESS FOR MESSAGE EMAIL */
void xmessage_close_logfile( void );

void xmessage_control_email( char *email_address );
					/* SET ADDRESS FOR MESSAGE EMAIL */
void xmessage_control_subject( char *subject );
					/* SET DEFAULTS FOR MESSAGE HANDLING */
void xmessage_control( int to_screen, char *log_file, char *email_address,
	char *subject );
/*===========================================================================*/
#endif								/* XMESSAGE.H */
