#!/bin/bash

gcc -c $1_c.c
g77 -c $1_f.f 

#g77 trial_chisq.o $1.o -L/usr/local/pgplot -lpgplot -L. -lchisq -o trial_chisq_s
g77 $1_c.o $1_f.o -L. -lpgplot -L. -lchisq -o $1_out

cp $1_out ~/public_html/cjb/trial_chisq.cgi
