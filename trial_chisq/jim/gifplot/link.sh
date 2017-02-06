#!/bin/bash

g77 $1 -L/usr/local/pgplot -lpgplot -L. -lchisq  -o trial_chisq
