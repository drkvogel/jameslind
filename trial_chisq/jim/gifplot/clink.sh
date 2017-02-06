#!/bin/bash

g77 -c $1.f 
g77 $1.o -L/usr/local/pgplot -lpgplot -L. -lchisq  -o trial_chisq
