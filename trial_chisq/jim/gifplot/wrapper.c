#include <stdio.h>

extern int plot_();

main()
{
    FILE *fp;
    
    printf("wrapper.%s\n", __TIME__);

    fp = fopen("/tmp/wrapper.txt", "w");
    fprintf(fp, "from the wrapper: %s\n", __TIME__);
    fclose(fp);
    
    plot_();
    
    printf("finished");
}
