#!/bin/bash

if [$1 == '']
then
    set $1 "trial_chisq_stdout"
    echo no filename base supplied, using $1
    
#    echo '$1 =' $1; exit
fi

gcc $1_main.c
cp a.out ~/public_html/cjb/trial_chisq.cgi
gcc -c $1_c.c
g77 -c $1_f.f 

#g77 trial_chisq.o $1.o -L/usr/local/pgplot -lpgplot -L. -lchisq -o trial_chisq_s
g77 $1_c.o $1_f.o -L. -lpgplot -L. -lchisq -o $1_out

cp $1_out ~/public_html/cjb/trial_chisq_plot.cgi
