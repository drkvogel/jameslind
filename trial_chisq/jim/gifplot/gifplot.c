#include <unistd.h>
#include <signal.h>

#include "xcgi.c"

typedef	struct
{
    int p1;
    int p2;
} FPARAMS;

char this_file[100];
FPARAMS *formParams;

int main(int argc, char **argv)
{
    pid_t pid;
    char *envstring;
    
	strncpy(this_file, __FILE__, (strlen(__FILE__) - 2));
	this_file[(strlen(__FILE__) - 2)] = '\0';
	strcat(this_file, ".cgi");
    
    formParams = (FPARAMS *) xmalloc(sizeof(FPARAMS));
    xcgi_init(argc, argv);
    xcgi_header("html");
    
    printf("<body>\n");
    
    printf("<h3>GIFPLOT!</h3>");
    printf("<p>");
    printf(__TIME__);
    printf("</p>");
    
    envstring = getenv("pwd");   
    
    printf("<p>");
    if (NULL != envstring) {
        printf("envstring: %s", envstring);
    } else {
        printf("PWD not found");
    }
    printf("</p>");
    
    execl("./trial_chisq", NULL, 0);
    
    printf("<p>OK.</p>");
    printf("</body>");
    
    free(formParams); //??? 
    xcgi_exit();
    
    
    
    return(0);
}
