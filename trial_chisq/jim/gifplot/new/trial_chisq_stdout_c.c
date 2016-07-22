#include "xcgi.c"

extern void plot_(const int *p1, const int *p2);

int main(int argc, char **argv) {
    char temp[100];
    int p1, p2;

    xcgi_init(argc, argv);
    //xcgi_header("html"); //image/gif???
    printf("content-type: image/gif\n\n");

    p1 = atoi(xcgi_param_value_named("p1"));
    p2 = atoi(xcgi_param_value_named("p2"));

  //  putenv("LD_LIBRARY_PATH=/usr/local/pgplot");

    //strcpy(temp, getenv("LD_LIBRARY_PATH"));
    
    //printf("LD_LIBRARY_PATH: %s", temp);

    plot_(&p1, &p2);

    xcgi_exit();

    return(0);
}
