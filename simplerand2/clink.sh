#!/bin/bash

#WEB_ROOT="/user/halsey/public_html/jameslind/trial_chisq"
WEB_ROOT="/user/cp/public_html/cjb/jameslind"

if [$1 == '']
then
    FILE_BASE="simplerand2"
    echo no filename base supplied, using $FILE_BASE
else
    FILE_BASE=$1
    echo using $FILE_BASE as filename base
fi

gcc -c -g "$FILE_BASE"_c.c
g77 -c "$FILE_BASE"_f.f
g77 -c getseed.f

#g77 -g "$FILE_BASE"_c.o "$FILE_BASE"_f.o getseed.o -o "$FILE_BASE"_out
g77 -g libg2c.so.0 "$FILE_BASE"_c.o "$FILE_BASE"_f.o getseed.o -o "$FILE_BASE"_out

#cp "$FILE_BASE"_out "$WEB_ROOT/$FILE_BASE.cgi"
cp "$FILE_BASE"_out "$WEB_ROOT/simplerand3.cgi"
chmod 755 "$WEB_ROOT/$FILE_BASE.cgi"
