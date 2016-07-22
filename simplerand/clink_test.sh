#!/bin/bash

g77 -c simple_rand_main.f 
g77 -c simplerand_f.f 
g77 -o simple_rand_main.o simplerand_f.o -o a.out
