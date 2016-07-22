#include "xcgi.c"
#include "trial_chisq.h"

/* for testing */
#define USE_CGI 1

extern int errno, sys_nerr;
extern __const char *__const sys_errlist[];

char* CGI_TARGET = "trial_chisq.cgi";

int main(int argc, char **argv, char *envp[])
{
    int p1, p2;

    if (USE_CGI) {
        xcgi_init(argc, argv);
        xcgi_header("html");
    }
    
    fprintf(stderr, "test\n");

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
			" div.small { font-size: 80%%; }\n"
			" .nowrap {white-space: nowrap}\n"
			"-->\n"
			"</style>\n"
            "</head>\n");

    printf("<body bgcolor=\"#fffff0\">\n");

    printf("<h3>Chi squared</h3>");

    printf( "<form name=\"plotform\" method=\"post\" action=\"%s\">\n", CGI_TARGET);
    
    printf( "<table>\n"
            "<tr><td>p1</td><td width=\"100%\"><input name=\"p1\" type=\"text\" size=\"4\" maxlength=\"4\" value=\"%d\"></td></tr>\n"
            "<tr><td>p2</td><td width=\"100%\"><input name=\"p2\" type=\"text\" size=\"4\" maxlength=\"4\" value=\"%d\"></td></tr>\n"
            "<tr><td>&nbsp;</td><td><input type=\"submit\" name=\"submit\" value=\"Plot\"></td></tr>\n"
            "</table></form>\n", p1, p2);

    printf("<hr>\n");
    
    if (    0 == strlen(xcgi_param_value_named("p1")) 
        &&  0 == strlen(xcgi_param_value_named("p2"))) {
        printf("<p>Please enter values for parameters P1 and P2 (between %d and %d)</p>",
            P1MIN, P1MAX);
    } else if (
        p1 < P1MIN ||
        p1 > P1MAX ||
        p2 < P2MIN ||
        p2 > P2MAX ||
        (0 == p1 && 0 == p2)) {
        printf("<p>P1 must be between %d and %d, and P2 must be between %d and %d. Both parameters cannot be zero.</p>",
            P1MIN, P1MAX, P2MIN, P2MAX);
    } else {
        printf("Graph for p1 = %d, p2 = %d:", p1, p2);

        printf("<img src=\"trial_chisq_plot.cgi?p1=%d&p2=%d\" alt=\"Image.\">\n", p1, p2);
    }

    printf("<hr>\n");
    printf("</body></html>");
    
    if (USE_CGI) xcgi_exit();

    return(0);
}
