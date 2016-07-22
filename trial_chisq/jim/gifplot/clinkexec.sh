#!/bin/bash

gcc $1_c.c
g77 -c $1_f.f 

#g77 trial_chisq.o $1.o -L/usr/local/pgplot -lpgplot -L. -lchisq -o trial_chisq_s
g77 $1_f.o -L. -lpgplot -L. -lchisq -o $1_f_out

mv a.out $1_c_out
cp trial_chisq_exec_c_out ~/public_html/cjb/trial_chisq_exec.cgi
