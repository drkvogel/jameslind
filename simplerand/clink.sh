#!/bin/bash

#WEB_ROOT="/user/halsey/public_html/jameslind/trial_chisq"
WEB_ROOT="/user/cp/public_html/cjb/jameslind"

if [$1 == '']
then
    FILE_BASE="simplerand"
    echo no filename base supplied, using $FILE_BASE
else
    FILE_BASE=$1
    echo using $FILE_BASE as filename base
fi

gcc -c "$FILE_BASE"_c.c
g77 -c "$FILE_BASE"_f.f

g77 "$FILE_BASE"_c.o "$FILE_BASE"_f.o -o "$FILE_BASE"_out

cp "$FILE_BASE"_out "$WEB_ROOT/simplerand.cgi"
chmod 755 "$WEB_ROOT/simplerand.cgi"
