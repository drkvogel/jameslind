#include <stdio.h>

extern void test_()

int main(int argc, char **argv)
{
    printf("test.c main\n");
    
    putenv("LD_LIBRARY_PATH=/usr/local/pgplot");
    
    test_();

    /*(FILE *fp;
    
    printf("ok.%s\n", __TIME__);
    
    fp = fopen("/tmp/test.txt", "w");
    
    fprintf(fp, "blah blah blah: %s\n", __TIME__);
    
    fclose(fp);*/
    
    return(0);
}
