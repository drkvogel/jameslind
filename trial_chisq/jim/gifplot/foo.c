#include <unistd.h>
#include <signal.h>
//#include <stdio.h>
//#include <stdlib.h>

#include "xcgi.c"

extern char *environ[];

typedef	struct
{
    int p1;
    int p2;
} FPARAMS;

char this_file[100];
FPARAMS *formParams;

#define USE_CGI 1

int main(int argc, char **argv, char *envp[])
{
    int i=0;
    int status;
    pid_t pid;
    char *envstring, **eval;
    
	strncpy(this_file, __FILE__, (strlen(__FILE__) - 2));
	this_file[(strlen(__FILE__) - 2)] = '\0';
	strcat(this_file, ".cgi");
    
    //formParams = (FPARAMS *) xmalloc(sizeof(FPARAMS));
    if (USE_CGI) {
        xcgi_init(argc, argv);
        xcgi_header("html");
    }
    
    printf("<body>\n");
    
    printf("<h3>GIFPLOT!</h3>");
    printf("<p>");
    printf(__TIME__);
    printf("</p>");
    
/*     envstring = getenv("REQUEST_METHOD");
    //envstring = getenv("PATH_INFO");
    //envstring = getenv("PWD");
    
    printf("<p>");
    if (NULL != envstring) {
        printf("envstring: %s", envstring);
    } else {
        printf("var not found");
    }
    printf("</p>");
    
    //eval = environ;
    eval = envp; */
    
    /*printf("boo!");
    while(*eval) printf("%s\n", *eval++);
    printf("boo!"); */
    
//    while (environ[i++])
  //      printf(environ[i]);
  
    putenv("LD_LIBRARY_PATH=/usr/local/pgplot");

    //printf("<p>exec test...\n<br>");
    //system("/user/cp/cjb/jim/gifplot/test");
    
    //printf("<p>exec trial_chisq_sub\n<br>");
    //system("/user/cp/cjb/jim/gifplot/trial_chisq_sub");
  
    printf("<p>exec trial_chisq...\n<br>");  
    if (fork() == 0)
        execlp("/user/cp/cjb/jim/gifplot/trial_chisq", "trial_chisq", (char *)0);
    wait(&status);
    
    printf("<h4>and now the gif...</h4>");
    
    printf("<img src=\"p.gif\">the gif</img>");
    printf("<p>ok?</p>");

    //printf("<p>exec test2...\n<br>");
    //system("/user/cp/cjb/jim/gifplot/test2");
    
    printf("<p>finished.\n");

    //return;
    //execlp("/user/cp/cjb/jim/gifplot/trial_chisq", "trial_chisq", (char *)0);
    execlp("/user/cp/cjb/jim/gifplot/test", "test", (char *)0);
    
    printf("<p>BOO!</p>");
    
    //execl("/user/cp/cjb/jim/gifplot/prog", "prog", 0);
    
    //export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/pgplot

    
    //execv("/user/cp/cjb/jim/gifplot/prog", NULL);
    
    printf("</body>");
    
    //free(formParams); //??? 
    if (USE_CGI) xcgi_exit();

    return(0);
}
