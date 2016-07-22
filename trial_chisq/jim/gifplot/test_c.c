#include <stdio.h>
#include "xcgi.c"

#define USE_CGI 1

extern void test_();

int main(int argc, char **argv)
{
    if (USE_CGI) {
        xcgi_init(argc, argv);
        xcgi_header("html");
    }

    printf("test.c main\n");
    
    putenv("LD_LIBRARY_PATH=/usr/local/pgplot");
    
    test_();

    /*(FILE *fp;
    
    printf("ok.%s\n", __TIME__);
    
    fp = fopen("/tmp/test.txt", "w");
    
    fprintf(fp, "blah blah blah: %s\n", __TIME__);
    
    fclose(fp);*/

    if (USE_CGI) xcgi_exit();
    
    return(0);
}
