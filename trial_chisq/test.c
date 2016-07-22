#include "xcgi.c"

#define USE_CGI 1

int main(int argc, char **argv, char *envp[])
{
    if (USE_CGI) {
        xcgi_init(argc, argv);
        xcgi_header("html");
    }
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

    printf("<h3>Chi squared-</h3>");

    printf("<hr>\n");

    printf("</body></html>");
    
    if (USE_CGI) xcgi_exit();

    return(0);
}
