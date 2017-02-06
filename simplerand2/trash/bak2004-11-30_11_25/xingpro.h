#ifndef INGRES_PROTO_H_INCLUDED
#define INGRES_PROTO_H_INCLUDED	1
/*===========================================================================*/
/*		(ASSUMED) INGRES EMBEDDED-C PROTOTYPES		*/
/*===========================================================================*/
/*
	These are used to prevent DCL compiler messages saying that the
	functions are implicitly declared (presumably as `int').  They
	may be incorrect, but seem to function okay.
*/
/*---------------------------------------------------------------------------*/
int	IIcsClose();
int	IIcsDaGet();
int	IIcsERplace();
int	IIcsERetrieve();
int	IIcsGetio();
int	IIcsOpen();
int	IIcsQuery();
int	IIcsRetrieve();
int	IIgetdomio();
char	*IILQdbl();
char	*IILQint();
int	IILQisInqSqlio();
int	IInextget();
int	IIputdomio();
int	IIretinit();
int	IIsqConnect();
int	IIsqDaIn();
int	IIsqDisconnect();
int 	IIsqExImmed();
int 	IIsqExStmt();
int	IIsqFlush();
int	IIsqInit();
int	IIsqMods();
int	IIsqPrepare();
int	IIsyncup();
int	IIwritio();
int	IIxact();
/*===========================================================================*/
#endif							/* INGRES_PROTO.H */
