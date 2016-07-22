#include <time.h>
#include <sys/wait.h>

#include "xcgi.c"
#include "trial_chisq.h"

#define USE_CGI 1

extern void plot_(const int *p1, const int *p2);

char procname[20];

void pmsg(const char* msg, ...) {
    va_list args;
    time_t t;
    struct tm* lt;
    char tbuf[50];
    
    t = time(NULL);
    lt = localtime(&t);
    strftime(tbuf, 49, "%Y-%m-%d %H:%M:%S", lt);
    
    fprintf(stderr, "%s, %s", tbuf, procname);
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);
    fprintf(stderr, "\n");
    fflush(stderr);
}

int main(int argc, char **argv) {
    char temp[100];
    int p1, p2;
    pid_t pid, parent_pid;
    int status=0, options=0;

    if (USE_CGI) xcgi_init(argc, argv);
    
    fprintf(stderr, "starting %s\n", __FILE__);
    pmsg("starting %s\n", __FILE__);
    strcpy(procname, "parent");
    
    printf("content-type: image/gif\n\n");

    p1 = atoi(xcgi_param_value_named("p1"));
    p2 = atoi(xcgi_param_value_named("p2"));
    
    //plot_(&p1, &p2);
    
    //return(0);
    
    if (
        p1 < P1MIN ||
        p1 > P1MAX ||
        p2 < P2MIN ||
        p2 > P2MAX ||
        (0 == p1 && 0 == p2)) {
        
        pmsg("%s: values %d and %d out of range\n",
            __FILE__, p1, p2);

        return(-1);
    }

    //putenv("LD_LIBRARY_PATH=/usr/local/pgplot");
    //strcpy(temp, getenv("LD_LIBRARY_PATH"));
    //printf("LD_LIBRARY_PATH: %s", temp);
    
    parent_pid = getpid();
    
    pid = fork();
    
    if (0 > pid) {
        pmsg("%s: error forking\n", __FILE__);
    } else if (0 == pid) {
        strcpy(procname, "child");
        
        pmsg("starting");
        
        //alarm(PLOT_TIMEOUT);
        waitpid(parent_pid, &status, options);
        
        //plot_(&p1, &p2);
        
        if (!WIFEXITED(status)) {
            pmsg("parent process %d (%s) had to be killed after %d sec\n",
                parent_pid, __FILE__, PLOT_TIMEOUT);
        } else {
            pmsg("parent process %d (%s) exited normally\n",
                parent_pid, __FILE__);
        } // */
        
    } else {
        pmsg("starting");
        
        alarm(PLOT_TIMEOUT);
        
        //waitpid(pid, &status, options);

        //plot_(&p1, &p2);
        
/*        if (!WIFEXITED(status)) {
            pmsg("child process %d (%s) terminated after %d sec\n",
                pid, __FILE__, PLOT_TIMEOUT);
        } else {
            pmsg("child process %d (%s) exited normally\n",
                pid, __FILE__);
        } // */
    }

    if (USE_CGI) xcgi_exit();

    return(0);
}
