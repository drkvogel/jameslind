#include <stdio.h>

int main(int argc, char **argv)
{
    FILE *fp;
    
    printf("test2.%s\n", __TIME__);
    
    fp = fopen("/tmp/test2.txt", "w");
    
    fprintf(fp, "blah blah blah: %s\n", __TIME__);
    
    fclose(fp);
    
    return(0);
}
