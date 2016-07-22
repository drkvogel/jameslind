#!/bin/bash

# use #!/bin/bash -v to echo lines

WEB_ROOT="/user/cp/public_html/cjb"

if [$1 == '']
then
    FILE_BASE="forktest"
    echo no filename base supplied, using "$FILE_BASE"
else
    FILE_BASE=$1
    echo using $FILE_BASE as filename base
fi

gcc "$FILE_BASE"_main.c
cp a.out "$WEB_ROOT/forktest.cgi"
chmod 755 "$WEB_ROOT/forktest.cgi"

gcc -c "$FILE_BASE"_c.c
g77 -c "$FILE_BASE"_f.f 

g77 "$FILE_BASE"_c.o "$FILE_BASE"_f.o -L. -lpgplot -L. -lchisq -o "$FILE_BASE"_out
#g77 "$FILE_BASE"_c.o "$FILE_BASE"_f.o -o "$FILE_BASE"_out

cp "$FILE_BASE"_out "$WEB_ROOT/forktest_plot.cgi"
chmod 755 "$WEB_ROOT/forktest_plot.cgi"
