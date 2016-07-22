#include <stdio.h>
#include <stdlib.h>

int main ()
{
  char * buffer;
  buffer = getenv ("PWD");
  if (buffer!=NULL)
    printf ("Current path is: %s",buffer);
  return 0;
}
