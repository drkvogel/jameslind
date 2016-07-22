#ifndef XTEXT_H_INCLUDED
#define XTEXT_H_INCLUDED	1
/*===========================================================================*/
/*		HEADER FOR TEXT HANDLING UTILITIES			*/
/*===========================================================================*/
			/* MOVE STRING POINTER TO END OF WHITE-SPACE INTERVAL */
char *skip_space( char *line );
				/* CASE-INSENSITIVE STRING COMPARISON */
int strcmp_nocase( char *a, char *b );
	/* CASE-INSENSITIVE STRING COMPARISON UP TO FIRST `LEN' CHARACTERS */
int strncmp_nocase( char *a, char *b, int len );
			/* FIND LOCATION OF FIRST OCCURENCE OF SUB IN STRING */
int locate_substring( char *sub, char *strn );
			/* FIND LOCATION OF FIRST OCCURENCE OF SUB IN STRING */
int locate_substring_nocase( char *sub, char *strn );
				/* CONVERT ALL OF STRING TO LOWER CASE */
void string_tolower( char *s );
				/* CONVERT ALL OF STRING TO UPPER CASE */
void string_toupper( char *s );
			/* REMOVE END AND EXTRA SPACES, TURN TABS TO SPACES */
int string_squeeze( char *sq );
		/* COUNT TEXT LINES BEFORE FIRST EMPTY STRING */
int lines_of_text( char **sarray );
				/* SHOW CONTENTS OF MULTI-LINE STRING ARRAY */
void display_multi_line( char *title, int nlines, char **line );
		/* CONVERT STRING INTO SEPARATE SPACE-DELIMITED UNITS */
void split_string_into_words( char *input, int *nwords, char ***word );
		/* RETURN ONLY LINES BEGINNING WITH `KEY', CHOPPING OFF KEY */
char *filter_line( char *in, char *key );
			/* GET LINE FROM FILE, IGNORING COMMENTS (!...)	*/
int read_line( FILE *f, char *s, int maxc );
					/* MAKE A DUPLICATE COPY OF A STRING */
char *duplicate_string( char *in );
				/* SEE IF CHAR C IS PART OF ALLOWED SET */
int allow_character( char c, char *cset );
			/* CONDENSE A STRING INTO RELEVANT CHARACTERS ONLY */
void condense_string( char *s, char *retain, char *exclude );
			/* ADD EXTENSION TO FILENAME IF NOT ALREADY PRESENT */
void filext( char *fnam, char *ext );
						/* IS ALL OF STRING ALPHA */
int isalpha_all( char *s );
			/* GENERATE UNIQUE STRING BASED ON SYSTEM TIME */
void string_unique( char *output );
/*===========================================================================*/
#endif								/* XTEXT.H */
