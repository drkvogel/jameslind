#include <unistd.h>
#include <signal.h>

#include "xcgi.c"

#define LOCK_FILE "trial_chisq.lock"
#define GIF_FILE  "p.gif"

#define USE_CGI 1

extern int errno, sys_nerr;
extern __const char *__const sys_errlist[];

extern int plot_();

char this_file[100];

int main(int argc, char **argv, char *envp[])
{
    FILE *fp;
    int i=0;
    int p1, p2;
    int status;
    pid_t pid;
    char *envstring, **eval;
    
	/*strncpy(this_file, __FILE__, (strlen(__FILE__) - 2));
	this_file[(strlen(__FILE__) - 2)] = '\0';
	strcat(this_file, ".cgi"); */
    
    strcpy(this_file, "trial_chisq.cgi");

    if (USE_CGI) {
        xcgi_init(argc, argv);
        xcgi_header("html");
    }
    
    p1 = atoi(xcgi_param_value_named("p1"));
    p2 = atoi(xcgi_param_value_named("p2"));
    
    printf( "<html>\n<head>\n<title>trial chisq</title>\n"
            "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">"
            "<meta http-equiv=\"expires\" content=\"now\">\n"
			"<meta http-equiv=\"pragma\" content=\"no-cache\">\n"
            "<style type=\"text/css\">\n"
            "<!--\n"
			" body, h1, h2, h3, h4, h5, h6 { font-family: Verdana, sans-serif;}\n"
            " body { font-size: 80%%; }\n"
			" tr.colheader { font-weight: bold; width: 0%% }\n"
			" td { font-size: 80%%; vertical-align: top}\n"
		//	" div.small { font-size: 80%%; }\n"
			" .nowrap {white-space: nowrap}\n"
			"-->\n"
			"</style>\n"
            "</head>\n");
    
    printf("<body bgcolor=\"#fffff0\">\n");
    
    printf("<h3>GIFPLOT!</h3>");

    printf( "<form name=\"plotform\" method=\"post\" action=\"%s\">\n", this_file);
    
    printf( "<table>\n"
            "<tr><td>p1</td><td width=\"100%\"><input name=\"p1\" type=\"text\" size=\"4\" maxlength=\"4\"></td></tr>\n"
            "<tr><td>p2</td><td width=\"100%\"><input name=\"p2\" type=\"text\" size=\"4\" maxlength=\"4\"></td></tr>\n"
            "<tr><td>&nbsp;</td><td><input type=\"submit\" name=\"submit\" value=\"Go\"></td></tr>\n"
            "</table></form>\n");

    printf("<hr>\n");
    
    printf("blah blah p1 = %d, p2 = %d", p1, p2);

    putenv("LD_LIBRARY_PATH=/usr/local/pgplot");
    
    while (NULL != fopen(LOCK_FILE, "r")) {
        fprintf(stderr, "lock file exists. waiting 1 sec\n");
        sleep(1);
        if (i++ == 5) {
            printf("<p>ERROR - contact core programmers</p></body></html>");
            exit(-1);
        }
    }
    unlink(GIF_FILE); // ???
    
    // what if prog bombs out before removing lock?
    // need to check date of lock file, if older than certain period (a longer period than
    // the program should take to execute, remove it
        
    fp = fopen(LOCK_FILE, "w");
    if (NULL == fp) 
        fprintf(stderr, "error - couldn't open lock file: %s", sys_errlist[errno]);
        
    fclose(fp);
    
    //sleep(4);

    if (fork() == 0)
        execlp("/user/cp/cjb/jim/gifplot/trial_chisq_exec_f_out", "trial_chisq", (char *)0);
    wait(&status);
    
    //plot_();
    
    unlink(LOCK_FILE);

    if (NULL == fopen(LOCK_FILE, "r")) {
        fprintf(stderr, "error2 - ");
        fprintf(stderr, sys_errlist[errno]);
        fprintf(stderr, "<p>lock file removed.</p>");
    }

    
    printf("<img src=\"p.gif\" alt=\"Image.\"></img>\n");
    printf("<hr>\n");
    
    printf("</body></html>");
    
    if (USE_CGI) xcgi_exit();

    return(0);
}
