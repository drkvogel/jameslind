#!/bin/bash

gcc -c $1_c.c
g77 -c $1_f.f 
#gcc wrapper.o $1.o -L/usr/local/pgplot -lpgplot -L. -lchisq -L. -lcgi77 -o trial_chisq_sub

#g77 $1_c.o $1_f.o -L/usr/local/pgplot -lpgplot -L. -lchisq -o $1_out

g77 $1_c.o $1_f.o -L. -lpgplot -L. -lchisq -o $1_out
#g77 $1_c.o $1_f.o -L. -lchisq -o $1_out
